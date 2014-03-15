/*==============================================================================
  BSPDemo
  Wad.h
================================================================================
*/

#ifndef __WAD_H__
#define __WAD_H__

/*
========================================
  dxWad
========================================
*/
class dxWad
{
public:

    dxWad();
    ~dxWad();

    bool         Load(const char * filename);
    void         Free();
    miptex_s *   GetLumpByName(char *name);

private:

    struct lumpinfo_s
    {
        int      filepos;
        int      disksize;
        int      size;     // uncompressed
        char     type;
        char     compression;
        char     pad1, pad2;
        char     name[16]; // must be null terminated
    };

    char *       wad_base;

    int          wad_numlumps;
    lumpinfo_s * wad_lumps;

};

#endif /* !__WAD_H__ */
