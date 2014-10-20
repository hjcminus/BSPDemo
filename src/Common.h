/*==============================================================================
  BSPDemo
  Common.h
================================================================================
*/

#ifndef __COMMON_H__
#define __COMMON_H__

/*
================================================================================
  type definition
================================================================================
*/
typedef unsigned char byte;

/*
================================================================================
  macros
================================================================================
*/

#define DX_ID_LEN       64

#define DX_NO_RETURN    __declspec(noreturn)
#define DX_SAFE_FREE(p) do { if (p) { DX_MEM_FREE(p); p = NULL; } } while (0)

/*
================================================================================
  output
================================================================================
*/

void Sys_Debug(const char *fmt, ...);
void DX_NO_RETURN Sys_Error(const char *fmt, ...);

/*
================================================================================
  heap
================================================================================
*/

void * Mem_Alloc(const size_t size, const char * fliename, int line);
void * Mem_Realloc(void *ptr, const size_t size, const char * fliename, int line);
void   Mem_Free(void *ptr);
void   Mem_Dump();

#define DX_MEM_ALLOC(size)        Mem_Alloc(size, __FILE__, __LINE__)
#define DX_MEM_REALLOC(ptr, size) Mem_Realloc(ptr, size, __FILE__, __LINE__)
#define DX_MEM_FREE(ptr)          Mem_Free(ptr)

/*
================================================================================
  static string pool
================================================================================
*/
void   StrPool_Clear();
char * StrPool_Alloc(size_t s);
char * StrPool_Copy(const char *str);

/*
================================================================================
  string
================================================================================
*/
void Str_ExtractDirName(const char * fullfilename, char buffer[1024]);
void Str_ExtractFileName(char * fullfilename, char buffer[1024]);

/*
================================================================================
  swap
================================================================================
*/
extern short (*Swap_BigShort)(short l);
extern short (*Swap_LittleShort)(short l);
extern int   (*Swap_BigLong)(int l);
extern int   (*Swap_LittleLong)(int l);
extern float (*Swap_BigFloat)(float l);
extern float (*Swap_LittleFloat)(float l);

/*
================================================================================
  config
================================================================================
*/
void Cfg_ReadStr(const char * section, const char * ident, 
    const char * default_, char buffer[1024], const char * filename);

/*
================================================================================
  demo system
================================================================================
*/

/*
========================================
  sysvar_s
========================================
*/

enum FrameState
{
    FS_MENU,
    FS_LOADING,
    FS_DEMO,
};

struct sysvar_s
{
    FrameState fs;
    bool       appactive;
    HWND       hMainWnd;
    HDC        hMainDC;
    int        viewwidth;
    int        viewheight;
	int        ctrx;
	int        ctry;
	char *     basedir;
};

extern char     default_basedir[MAX_PATH];

extern sysvar_s sysvar;

void FS_Change(FrameState fs);

/*
========================================
  dxViewer
========================================
*/

class dxViewer
{
public:

    dxVec3 vieworigin;
    dxVec3 viewangles;
    dxVec3 viewforward;
    dxVec3 viewright;
    dxVec3 viewup;

    bool   anglechanged;

    dxViewer();

    void   Update();
};

extern dxViewer viewer;

/*
========================================
  frame
========================================
*/
struct frame_s
{
    float totaltime;
    float deltatime;

    frame_s()
    {
        totaltime = deltatime = 0;
    }
};

extern frame_s frame;

void Sys_WindowCoordToSysCoord(int &x, int &y);
void Sys_UpdateScreen();

/*
========================================
  debugvar_s
========================================
*/
struct debugvar_s
{
    bool       fillmode;
};

extern debugvar_s debugvar;

/*
================================================================================
  dxItemBuffer
================================================================================
*/
template<class T>
class dxItemBuffer
{
public:

    dxItemBuffer(int initsize);
    ~dxItemBuffer();

    void         Add(const T &item);

    const void * GetBuffer() const;
    int          GetCount() const;

private:

    T *          buffer;
    int          count;
    int          capacity;
};

template<class T>
dxItemBuffer<T>::dxItemBuffer(int initsize)
{
    buffer = (T*)DX_MEM_ALLOC(initsize * sizeof(T));
    capacity = initsize;
    count = 0;
}

template<class T>
dxItemBuffer<T>::~dxItemBuffer()
{
    DX_SAFE_FREE(buffer);
}

template<class T>
void dxItemBuffer<T>::Add(const T &item)
{
    if (count == capacity)
    {
        if (capacity > 1024)
        {
            capacity += (capacity >> 1);
        }
        else
        {
            capacity += 1024;
        }

        buffer = (T*)DX_MEM_REALLOC(buffer, capacity);
    }

    buffer[count++] = item;
}

template<class T>
const void * dxItemBuffer<T>::GetBuffer() const
{
    return buffer;
}

template<class T>
int dxItemBuffer<T>::GetCount() const
{
    return count;
}

#endif /* !__COMMON_H__ */
