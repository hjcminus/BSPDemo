/*==============================================================================
  BSPDemo
  Renderer.h
================================================================================
*/

#ifndef __RENDERER_H__
#define __RENDERER_H__

/*
========================================
  dxRenderer
========================================
*/

#define	MAX_GLTEXTURES 1024

struct gltexture_s
{
    GLuint   texnum;
    char	 identifier[DX_ID_LEN];
    int		 width;
    int      height;
    bool     mipmap;
};

#define	MIPLEVELS 4

struct miptex_s
{
    char      name[16];
    unsigned  width;
    unsigned  height;
    unsigned  offsets[MIPLEVELS]; // four mip maps stored
};

#define	BLOCK_WIDTH    128
#define	BLOCK_HEIGHT   128
#define	MAX_LIGHTMAPS   64

struct lightmap_s
{
    bool    used;
    GLuint  texnum;
    int     allocated[BLOCK_WIDTH];
    byte    buffer[BLOCK_HEIGHT][BLOCK_WIDTH][4];
};

class dxRenderer
{
public:

    dxRenderer();
    ~dxRenderer();

    void         Init();
    void         Shutdown();

    void         SetViewport(int viewwidth_, int viewheight_);

    //texture
    GLuint       LoadMipTexture(miptex_s * mt);
    GLuint       LoadBMPTexture(char *identifier, const char * filename);
    lightmap_s * AllocLightmapBlock(int extendx, int extendy, int &light_s, int &light_t);
    void         LoadLightmaps();
    void         ResetLightmaps();

    void         UnloadTextures(); //clear all

	GLuint       LoadTempBMPTexture(const char * filename);
	void         UnloadTempBMPTexture(GLuint texnum);

    void         BindTexture(GLuint texture);

    //vertex buffer
    GLuint       CreateVertexBufferVec2(const void * data, GLsizei count);
    GLuint       CreateVertexBufferVec3(const void * data, GLsizei count);
    void         DestroyVertexBuffer(GLuint &buffer);

    void         BeginDraw();
    void         EndDraw();

    void         SetupTextMode();
    void         EndTextMode();

    void         FrameRect(const dxVec4 &color, int left, int bottom, int right, int top);
    void         FillRect(const dxVec4 &color, int left, int bottom, int right, int top);
    void         Print(const dxVec4 &color, int x, int y, const char * str);
	void         DrawLine(const dxVec4 &color, int x0, int y0, int x1, int y1);
    void         DrawImage(GLuint image, int left, int bottom, int right, int top);

    void         SetupFrame(const dxVec3 &vieworigin_, const dxVec3 &viewforward_,
                     const dxVec3 &viewright_, const dxVec3 &viewup_);

    bool         CullBox(float *mins, float *maxs);

    void         BeginDrawSky();
    void         EndDrawSky();

    void         BeginDrawOpaque(GLuint vertbuf_model, GLuint texcoord1_model, GLuint texcoord2_model, bool alphatest);
    void         EndDrawOpaque();

    void         BeginDrawAlpha(GLuint vertbuf_model, GLuint texcoord_model);
    void         EndDrawAlpha();

    void         DrawQuad(GLuint polytex, GLint offset);
    void         DrawPolygonOpaque(GLuint polytex, GLuint lighttex, GLint offset, GLsizei count);
    void         DrawPolygonAlpha(GLuint polytex, float alpha, float texoffset, GLint offset, GLsizei count);

private:

    bool         inited;

    dxVec3       vieworigin;

    int          viewwidth;
    int          viewheight;

    gltexture_s  textures[MAX_GLTEXTURES];
    int	         numtextures;

    lightmap_s   lightmaps[MAX_LIGHTMAPS];

    //programs
    glprogram_s  program_sky, program_opaque, program_alpha;

    GLuint       vertbuf_sky, texcoordbuf_sky;
    GLint        uniform_skyviewpos;
    GLint        uniform_skytex;


    GLint        uniform_surftex_opaque;
    GLint        uniform_lightmap_opaque;

    GLint        uniform_surftex_alpha;
    GLint        uniform_alpha;
    GLint        uniform_offset;

    dxPlane      frustum[4];
    double       viewratio;

    bool         enable_alphatest;

    struct chartexcoord_s
    {
        float    s1;
        float    t1;
        float    s2;
        float    t2;
    };

    GLuint       texture_text;
    chartexcoord_s  chartexcoords[95];

    void         SetColorMappings(float gamma);
    void         LoadTextTexture();
    void         LoadPrograms();

	const byte * LoadBMPImage(const char * filename, int &image_cx, int &image_cy);
    
};

extern dxRenderer renderer;

#endif /* !__RENDERER_H__ */
