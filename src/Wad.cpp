/*==============================================================================
  BSPDemo
  Wad.cpp
================================================================================
*/

#include "Precompiled.h"
#pragma hdrstop

/*
========================================
  dxWad
========================================
*/
dxWad::dxWad()
{
    wad_base = NULL;
}

dxWad::~dxWad()
{
    Free();
}

bool dxWad::Load(const char * filename)
{
    struct wadinfo_s
    {
        char  identification[4];
        int   numlumps;
        int   infotableofs;
    };

    FILE * f = NULL;
    fopen_s(&f, filename, "rb");

    if (!f)
    {
        return false;
    }

    fseek(f, 0, SEEK_END);
    long filesize = ftell(f);
    fseek(f, 0, SEEK_SET);

    wad_base = (char*)DX_MEM_ALLOC(filesize);
    if (!wad_base)
    {
        fclose(f);
        Sys_Error("could not alloc memory");
    }

    fread(wad_base, 1, filesize, f);
    fclose(f);

    wadinfo_s * header = (wadinfo_s*)wad_base;

    if ((strncmp(header->identification, "WAD2", 4) != 0)
    &&  (strncmp(header->identification, "WAD3", 4) != 0))
    {
        Sys_Error("bad wad file");
    }

    wad_numlumps = Swap_LittleLong(header->numlumps);
    int infotableofs = Swap_LittleLong(header->infotableofs);
    wad_lumps = (lumpinfo_s*)(wad_base + infotableofs);

    lumpinfo_s * lump_p = wad_lumps;
    for (int i = 0; i < wad_numlumps; i++,lump_p++)
    {
        lump_p->filepos = Swap_LittleLong(lump_p->filepos);
        lump_p->size    = Swap_LittleLong(lump_p->size);

        //if (lump_p->type == TYP_QPIC)
        //{
        //    SwapPic((qpic_s*)(g_wad_base + lump_p->filepos));
        //}
    }

    return true;
}

void dxWad::Free()
{
    DX_SAFE_FREE(wad_base);
}

miptex_s * dxWad::GetLumpByName(char *name)
{
    int i;
    lumpinfo_s * lump_p;

    for (lump_p = wad_lumps, i = 0; i < wad_numlumps; i++, lump_p++)
    {
        if (!_stricmp(name, lump_p->name))
        {
            return (miptex_s*)(wad_base + lump_p->filepos);
        }
    }
    return NULL;
}
