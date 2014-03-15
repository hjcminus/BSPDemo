/*==============================================================================
  BSPDemo
  GLSys.h
================================================================================
*/

#ifndef __GLSYS_H__
#define __GLSYS_H__

#define DX_MAX_TEXTURE_SIZE 1024

struct glprogram_s
{
    GLuint   prgnum;
    GLuint   vsnum;
    GLuint   fsnum;
};

//program
GLuint  GL_LoadShader(GLenum type, const GLchar **src);
void    GL_CreateProg(const GLchar **vs_src, const GLchar **fs_src, glprogram_s &prg);
void    GL_LinkProg(glprogram_s &prg);
void    GL_DestroyProg(glprogram_s &prg);
void    GL_BindAttribLocation(glprogram_s &prg, GLuint index, const GLchar *name);
GLint   GL_GetUniformLocation(glprogram_s &prg, const GLchar *name);

//vertex buffer
GLuint  GL_CreateVertexBuffer(const void * data, size_t size);
void    GL_DestroyVertexBuffer(GLuint &buffer);
void    GL_BindVertexBuffer(GLuint vb);

//texture
GLuint  GL_GenTexture();
GLuint  GL_LoadTexture8(char *identifier, int width, int height, const byte * pal, const byte *data, bool mipmap, GLfloat WrapMode);
GLuint  GL_LoadTexture24(char *identifier, int width, int height, const byte *data, bool mipmap, GLfloat WrapMode);
GLuint  GL_LoadTexture32(char *identifier, int width, int height, const byte *data, bool mipmap, GLfloat WrapMode);
GLuint  GL_LoadTempTexture32(int width, int height, const byte *data, bool mipmap, GLfloat WrapMode);
void    GL_UnloadTempTexture(GLuint texnum);
void    GL_BindTexture(GLuint texnum);
void    GL_ClearTextures();

void    GL_CheckError();

#endif /* !__GLSYS_H__ */
