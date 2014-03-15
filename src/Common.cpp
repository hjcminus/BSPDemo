/*==============================================================================
  BSPDemo
  Common.cpp
================================================================================
*/

#include "Precompiled.h"
#pragma hdrstop

/*
================================================================================
  heap
    not thread safe
================================================================================
*/

static const int HEAP_ID = 0x5E0E52AB;

struct heap_chain_s
{
    int            id;
    heap_chain_s * prior;
    heap_chain_s * next;
    size_t         datasize;
    size_t         allocsize;
    const char *   fliename;
    int            line;
};

static heap_chain_s * heap_allocated;

void * Mem_Alloc(const size_t size, const char * fliename, int line)
{
    if (!size)
    {
        return NULL;
    }

    size_t s = size + sizeof(heap_chain_s);
    size_t alloc_size = (s + 15) & ~15;

    heap_chain_s * heapchain = (heap_chain_s*)_aligned_malloc(alloc_size, 16);

    if (!heapchain)
    {
        return NULL;
    }

    heapchain->id = HEAP_ID;
    heapchain->datasize = size;
    heapchain->allocsize = alloc_size;
    heapchain->fliename = fliename;
    heapchain->line = line;
    heapchain->prior = heapchain->next = heapchain;

    if (!heap_allocated)
    {
        heap_allocated = heapchain;
    }
    else
    {
        heap_allocated->prior->next = heapchain;
        heapchain->prior = heap_allocated->prior;
        heapchain->next = heap_allocated;
        heap_allocated->prior = heapchain;
    }

    return heapchain + 1;
}

void * Mem_Realloc(void *ptr, const size_t size, const char * fliename, int line)
{
    if (!ptr)
    {
        return Mem_Alloc(size, fliename, line);
    }

    heap_chain_s * heapchain = (heap_chain_s*)((char*)ptr - sizeof(heap_chain_s));

    if (HEAP_ID != heapchain->id)
    {
        Sys_Error("heap crash");
        return NULL;
    }

    size_t datasize = heapchain->datasize;
    void *newptr = Mem_Alloc(size, fliename, line);

    size_t copysize = min(datasize, size);
    memcpy(newptr, ptr, copysize);

    Mem_Free(ptr);

    return newptr;
}

void Mem_Free(void *ptr)
{
    if (!ptr)
    {
        return;
    }

    heap_chain_s * heapchain = (heap_chain_s*)((char*)ptr - sizeof(heap_chain_s));

    if (HEAP_ID != heapchain->id)
    {
        Sys_Error("heap crash");
        return;
    }

    heapchain->prior->next = heapchain->next;
    heapchain->next->prior = heapchain->prior;

    if (heap_allocated == heapchain)
    {
        if (heap_allocated->next == heap_allocated)
        {
            heap_allocated = NULL;
        }
        else
        {
            heap_allocated = heap_allocated->next;
        }
    }

    _aligned_free(heapchain);
}

void Mem_Dump()
{
    heap_chain_s * heapchain = heap_allocated;
    while (heapchain)
    {
        Sys_Debug("alloc %d at file: %s, line: %d\n", heapchain->datasize, heapchain->fliename, heapchain->line);

        heapchain = heapchain->next;
        if (heapchain == heap_allocated)
        {
            break;
        }
    }
}

/*
================================================================================
  static string pool
================================================================================
*/

#define STRPOOL_SIZE (4 * 1024 * 1024)

static char strpool[STRPOOL_SIZE];
static int  strpool_allocated;

void StrPool_Clear()
{
    strpool_allocated = 0;
}

char * StrPool_Alloc(size_t s)
{
    if (s + strpool_allocated > STRPOOL_SIZE)
    {
        Sys_Error("string pool overflow");
    }

    char * str = strpool + strpool_allocated;
    strpool_allocated += (int)s;

    return str;
}

char * StrPool_Copy(const char *str)
{
    if (!str)
    {
        return NULL;
    }

    int l = (int)strlen(str);
    char * dst = StrPool_Alloc(l+1);
    memcpy(dst, str, l);
    dst[l] = 0;

    return dst;
}

/*
================================================================================
  string
================================================================================
*/
void Str_ExtractDirName(const char * fullfilename, char buffer[1024])
{
    buffer[0] = 0;

    char temp[MAX_PATH];
    strcpy_s(temp, MAX_PATH, fullfilename);

    int l = (int)strlen(temp);

    for (int i = l-1; i >= 0; i--)
    {
        if (temp[i] == '/' || temp[i] == '\\')
        {
            char c = temp[i];
            temp[i] = 0;
            strcpy_s(buffer, MAX_PATH, temp);
            break;
        }
    }
}

void Str_ExtractFileName(char * fullfilename, char buffer[1024])
{
    buffer[0] = 0;

    int l = (int)strlen(fullfilename);

    for (int i = l-1; i >= 0; i--)
    {
        if (fullfilename[i] == '/' || fullfilename[i] == '\\')
        {
            strcpy_s(buffer, MAX_PATH, fullfilename + i + 1);
            break;
        }
    }
}

/*
================================================================================
  swap
================================================================================
*/
short (*Swap_BigShort)(short l);
short (*Swap_LittleShort)(short l);
int   (*Swap_BigLong)(int l);
int   (*Swap_LittleLong)(int l);
float (*Swap_BigFloat)(float l);
float (*Swap_LittleFloat)(float l);

static short ShortSwap(short l)
{
	char b1,b2;

	b1 = l & 255;
	b2 = (l>>8) & 255;

	return (b1<<8) + b2;
}

static short ShortNoSwap (short l)
{
	return l;
}

static int LongSwap(int l)
{
	char b1, b2, b3, b4;

	b1 = l & 255;
	b2 = (l>>8)  & 255;
	b3 = (l>>16) & 255;
	b4 = (l>>24) & 255;

	return ((int)b1<<24) + ((int)b2<<16) + ((int)b3<<8) + b4;
}

static int LongNoSwap(int l)
{
	return l;
}

static float FloatSwap(float f)
{
	union
	{
		float   f;
		char    b[4];
	} dat1, dat2;
	
	
	dat1.f = f;

	dat2.b[0] = dat1.b[3];
	dat2.b[1] = dat1.b[2];
	dat2.b[2] = dat1.b[1];
	dat2.b[3] = dat1.b[0];

	return dat2.f;
}

static float FloatNoSwap(float f)
{
	return f;
}

void Swap_Init()
{
	char swaptest[2] = {1,0};

	if (*(short*)swaptest == 1) //big-endian
	{
		Swap_BigShort    = ShortSwap;
		Swap_LittleShort = ShortNoSwap;
		Swap_BigLong     = LongSwap;
		Swap_LittleLong  = LongNoSwap;
		Swap_BigFloat    = FloatSwap;
		Swap_LittleFloat = FloatNoSwap;
	}
	else
	{
		Swap_BigShort    = ShortNoSwap;
		Swap_LittleShort = ShortSwap;
		Swap_BigLong     = LongNoSwap;
		Swap_LittleLong  = LongSwap;
		Swap_BigFloat    = FloatNoSwap;
		Swap_LittleFloat = FloatSwap;
	}
}

void FS_Change(FrameState fs)
{
	if (sysvar.fs == fs)
	{
		return;
	}

	sysvar.fs = fs;

	if (FS_MENU == fs)
	{
		//
	}
}

/*
========================================
  dxViewer
========================================
*/
dxViewer viewer;

dxViewer::dxViewer()
{
    memset(this, 0, sizeof(*this));
}

static const float MOVE_SPEED = 200.0f;

#define IS_KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

void dxViewer::Update()
{
    if (anglechanged)
    {
        anglechanged = false;

        AngleToVec3s(viewangles, viewforward, viewright, viewup);
    }

    dxVec3 move_dir;
    bool   moved = false;

    if (IS_KEY_DOWN(keys.moveforward))
    {
        move_dir = viewforward;
        moved = true;
    }

    if (IS_KEY_DOWN(keys.movebackward))
    {
        move_dir += -viewforward;
        moved = true;
    }

    if (IS_KEY_DOWN(keys.moveright))
    {
        move_dir += viewright;
        moved = true;
    }

    if (IS_KEY_DOWN(keys.moveleft))
    {
        move_dir += -viewright;
        moved = true;
    }

    if (moved)
    {
        move_dir.Normalize();
        viewer.vieworigin += move_dir * frame.deltatime * MOVE_SPEED;
    }

}

/*
================================================================================
  config
================================================================================
*/
void Cfg_ReadStr(const char * section, const char * ident, 
    const char * default_, char buffer[1024], const char * filename)
{
    GetPrivateProfileStringA(section, ident, default_, buffer, 1024, filename);
}

/*
================================================================================
  demo system
================================================================================
*/

void Sys_WindowCoordToSysCoord(int &x, int &y)
{
    int halfviewcx = sysvar.viewwidth  >> 1;
    int halfviewcy = sysvar.viewheight >> 1;

    x -= halfviewcx;
    y -= halfviewcy;

    y = -y;
}
