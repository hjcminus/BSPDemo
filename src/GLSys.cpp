/*==============================================================================
  BSPDemo
  GLSys.cpp
================================================================================
*/

#include "Precompiled.h"
#pragma hdrstop

#define DX_INFO_LOG_LEN 4096

GLuint GL_LoadShader(GLenum type, const GLchar **src)
{
    GLuint s = dxglCreateShader(type);

    dxglShaderSource(s, 1, src, NULL);
    dxglCompileShader(s);

    GLint compile_status = 0;
    GLint info_log_len = 0;

    dxglGetShaderiv(s, GL_COMPILE_STATUS,  &compile_status);
    dxglGetShaderiv(s, GL_INFO_LOG_LENGTH, &info_log_len);  //including null terminate char

    if (!compile_status)
    {
        char info_log[DX_INFO_LOG_LEN];
        dxglGetShaderInfoLog(s, DX_INFO_LOG_LEN, &info_log_len, info_log);

        Sys_Error("shader compile error: %s", info_log);
    }

    return s;
}

void GL_CreateProg(const GLchar **vs_src, const GLchar **fs_src, glprogram_s &prg)
{
    prg.prgnum = dxglCreateProgram();

    prg.vsnum = GL_LoadShader(GL_VERTEX_SHADER,   vs_src);
    prg.fsnum = GL_LoadShader(GL_FRAGMENT_SHADER, fs_src);

    dxglAttachShader(prg.prgnum, prg.vsnum);
    dxglAttachShader(prg.prgnum, prg.fsnum);
}

void GL_LinkProg(glprogram_s &prg)
{
    dxglLinkProgram(prg.prgnum);

    GLint link_status = 0;
    dxglGetProgramiv(prg.prgnum, GL_LINK_STATUS, &link_status);

    if (!link_status)
    {
        GLint info_log_len = 0;
        dxglGetProgramiv(prg.prgnum, GL_INFO_LOG_LENGTH, &info_log_len);

        char info_log[DX_INFO_LOG_LEN];
        dxglGetProgramInfoLog(prg.prgnum, DX_INFO_LOG_LEN, &info_log_len, info_log);

        Sys_Error("program link error: %s", info_log);
    }

    dxglValidateProgram(prg.prgnum);

    GLint validate_status = 0;
    dxglGetProgramiv(prg.prgnum, GL_VALIDATE_STATUS, &validate_status);

    if (!validate_status)
    {
        GLint info_log_len = 0;
        dxglGetProgramiv(prg.prgnum, GL_INFO_LOG_LENGTH, &info_log_len);

        char info_log[DX_INFO_LOG_LEN];
        dxglGetProgramInfoLog(prg.prgnum, DX_INFO_LOG_LEN, &info_log_len, info_log);

        Sys_Error("program validate error: %s", info_log);
    }
}

void GL_DestroyProg(glprogram_s &prg)
{
    if (prg.prgnum > 0)
    {
        if (prg.fsnum > 0)
        {
            dxglDetachShader(prg.prgnum, prg.fsnum);
            dxglDeleteShader(prg.fsnum);
            prg.fsnum = 0;
        }

        if (prg.vsnum > 0)
        {
            dxglDetachShader(prg.prgnum, prg.vsnum);
            dxglDeleteShader(prg.vsnum);
            prg.vsnum = 0;
        }

        dxglDeleteShader(prg.prgnum);
        prg.prgnum = 0;
    }
}

void GL_BindAttribLocation(glprogram_s &prg, GLuint index, const GLchar *name)
{
    dxglBindAttribLocation(prg.prgnum, index, name);
    GL_CheckError();
}

GLint GL_GetUniformLocation(glprogram_s &prg, const GLchar *name)
{
    if (prg.prgnum <= 0)
    {
        Sys_Error("bad gl program");
    }

    GLint location = dxglGetUniformLocation(prg.prgnum, name);
    if (location < 0)
    {
        GL_CheckError();
        Sys_Error("bad location");
    }

    return location;
}

//vertex buffer
GLuint GL_CreateVertexBuffer(const void * data, size_t size)
{
    GLuint buffer = 0;

    dxglGenBuffers(1, &buffer);

    if (buffer == 0)
    {
        Sys_Error("glGenBuffers failed");
    }

    dxglBindBuffer(GL_ARRAY_BUFFER, buffer);
    dxglBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    dxglBindBuffer(GL_ARRAY_BUFFER, 0);

    return buffer;
}

void GL_DestroyVertexBuffer(GLuint &buffer)
{
    if (buffer != 0)
    {
        dxglDeleteBuffers(1, &buffer);
        buffer = 0;
    }
}

void GL_BindVertexBuffer(GLuint vb)
{
    static GLuint currentvb = 0;

    if (currentvb == vb)
    {
        return;
    }
    currentvb = vb;
    dxglBindBuffer(GL_ARRAY_BUFFER, vb);
}

//texture

GLuint GL_GenTexture()
{
    GLuint texnum = 0;
    dxglGenTextures(1, &texnum);
    return texnum;

    /*
    static GLuint texnum = 1;
    return texnum++;
     */
}

gltexture_s	gltextures[MAX_GLTEXTURES];
int	numgltextures;

GLuint GL_FindTexture(char *identifier, int width, int height)
{
    gltexture_s	*glt = gltextures;
    for (int i = 0; i < numgltextures; i++, glt++)
    {
        if (!_stricmp(identifier, glt->identifier)) //to lowercase
        {
            if (width != glt->width || height != glt->height)
            {
                Sys_Error("GL_FindTexture: %s cache mismatch", identifier);
            }
            return glt->texnum;
        }
    }
    return 0;
}

GLuint GL_AllocTexture(char *identifier, int width, int height, bool mipmap)
{
    if (MAX_GLTEXTURES == numgltextures)
    {
        Sys_Error("too many textures");
    }

    gltexture_s * glt = gltextures + numgltextures;
    numgltextures++;

    GLuint t = GL_GenTexture();

    strcpy_s(glt->identifier, DX_ID_LEN, identifier);
    glt->texnum = t;
    glt->width  = width;
    glt->height = height;
    glt->mipmap = mipmap;

    GL_BindTexture(t);

    return t;
}

void GL_BindTexture(GLuint texnum)
{
    static GLuint currenttexture = 0;

    if (currenttexture == texnum)
    {
        return;
    }
    currenttexture = texnum;
    dxglBindTexture(GL_TEXTURE_2D, texnum);
}

void GL_ClearTextures()
{
    if (numgltextures > 0)
    {
        GLuint texnums[MAX_GLTEXTURES];

        gltexture_s	*glt = gltextures;
        for (int i = 0; i < numgltextures; i++, glt++)
        {
            texnums[i] = glt->texnum;
            memset(glt, 0, sizeof(*glt));
        }

        dxglDeleteTextures(numgltextures, texnums);
        numgltextures = 0;
    }

}

void GL_Upldate32(int width, int height, const byte *data, bool mipmap, GLfloat WrapMode)
{
    dxglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapMode);
    dxglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapMode);

    dxglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (mipmap)
    {
        dxglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        dxgluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        dxglTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //The first element corresponds to the lower left corner of the texture image
        dxglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
}

//translate
static unsigned int trans[DX_MAX_TEXTURE_SIZE * DX_MAX_TEXTURE_SIZE];

GLuint GL_LoadTexture8(char *identifier, int width, int height, const byte * pal, const byte *data, bool mipmap, GLfloat WrapMode)
{
    GLuint t = GL_FindTexture(identifier, width, height);
    if (t != 0)
    {
        return t;
    }

    t = GL_AllocTexture(identifier, width, height, mipmap);

    //init palette
    unsigned int _8to24table[256];
    unsigned int * table = _8to24table;

    for (int i = 0; i < 256; i++)
    {
        unsigned int r = pal[0];
        unsigned int g = pal[1];
        unsigned int b = pal[2];

        pal += 3;

        unsigned int v = (255 << 24) + (r << 0) + (g << 8) + (b << 16);

        *table++ = v;
    }

    _8to24table[255] &= 0x00ffffff; // 255 is transparent

    //translate
    if (width > DX_MAX_TEXTURE_SIZE || height > DX_MAX_TEXTURE_SIZE)
    {
        Sys_Error("texture %s is too large", identifier);
    }

    int s = width * height;

    for (int i = 0; i < s; i++)
    {
        trans[i] = _8to24table[data[i]];
    }

    GL_Upldate32(width, height, (byte*)trans, mipmap, WrapMode);

    return t;
}

GLuint GL_LoadTexture24(char *identifier, int width, int height, const byte *data, bool mipmap, GLfloat WrapMode)
{
    GLuint t = GL_FindTexture(identifier, width, height);
    if (t != 0)
    {
        return t;
    }

    t = GL_AllocTexture(identifier, width, height, mipmap);

    //translate
    if (width > DX_MAX_TEXTURE_SIZE || height > DX_MAX_TEXTURE_SIZE)
    {
        Sys_Error("texture %s is too large", identifier);
    }

    int s = width * height;

    const byte * p = data;
    for (int i = 0; i < s; i++)
    {
        unsigned int r = p[0];
        unsigned int g = p[1];
        unsigned int b = p[2];

        unsigned int v = (255 << 24) + (r << 0) + (g << 8) + (b << 16);

        trans[i] = v;

        p += 3;
    }

    GL_Upldate32(width, height, (byte*)trans, mipmap, WrapMode);

    return t;
}

GLuint GL_LoadTexture32(char *identifier, int width, int height, const byte *data, bool mipmap, GLfloat WrapMode)
{
    GLuint t = GL_FindTexture(identifier, width, height);
    if (t != 0)
    {
        return t;
    }

    t = GL_AllocTexture(identifier, width, height, mipmap);

    GL_Upldate32(width, height, data, mipmap, WrapMode);

    return t;
}

GLuint GL_LoadTempTexture32(int width, int height, const byte *data, bool mipmap, GLfloat WrapMode)
{
	GLuint texnum = GL_GenTexture();
	dxglBindTexture(GL_TEXTURE_2D, texnum);
	GL_Upldate32(width, height, data, mipmap, WrapMode);
	dxglBindTexture(GL_TEXTURE_2D, 0);

	return texnum;
}

void GL_UnloadTempTexture(GLuint texnum)
{
	dxglDeleteTextures(1, &texnum);
}

const char * GL_ErrorToString(GLenum err)
{
    switch (err)
    {
    case GL_INVALID_ENUM:      return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:     return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
    case GL_STACK_OVERFLOW:    return "GL_STACK_OVERFLOW";
    case GL_STACK_UNDERFLOW:   return "GL_STACK_UNDERFLOW";
    case GL_OUT_OF_MEMORY:     return "GL_OUT_OF_MEMORY";
    default:
        return "GL_UNKNOWN_ERROR";
    }
}

void GL_CheckError()
{
    GLenum err = dxglGetError();
    if (err != GL_NO_ERROR)
    {
        Sys_Error("gl error: %s", GL_ErrorToString(err));
    }
}
