/*==============================================================================
  BSPDemo
  GLFuncs.h
================================================================================
*/

#ifndef __GLFUNCS_H__
#define __GLFUNCS_H__

typedef char GLchar;

//GL_VERSION_1_1

extern void      (APIENTRY * dxglAlphaFunc) (GLenum func, GLclampf ref);
extern void      (APIENTRY * dxglBegin) (GLenum mode);
extern void      (APIENTRY * dxglBindTexture) (GLenum target, GLuint texture);
extern void      (APIENTRY * dxglBlendFunc) (GLenum sfactor, GLenum dfactor);
extern void      (APIENTRY * dxglClear) (GLbitfield mask);
extern void      (APIENTRY * dxglClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
extern void      (APIENTRY * dxglClearDepth) (GLclampd depth);
extern void      (APIENTRY * dxglColor4fv) (const GLfloat *v);
extern void      (APIENTRY * dxglCullFace) (GLenum mode);
extern void      (APIENTRY * dxglDeleteTextures) (GLsizei n, const GLuint *textures);
extern void      (APIENTRY * dxglDepthFunc) (GLenum func);
extern void      (APIENTRY * dxglDepthMask) (GLboolean flag);
extern void      (APIENTRY * dxglDisable) (GLenum cap);
extern void      (APIENTRY * dxglDrawArrays) (GLenum mode, GLint first, GLsizei count);
extern void      (APIENTRY * dxglDrawBuffer) (GLenum mode);
extern void      (APIENTRY * dxglEnable) (GLenum cap);
extern void      (APIENTRY * dxglEnableClientState) (GLenum array);
extern void      (APIENTRY * dxglEnd) (void);
extern void      (APIENTRY * dxglFinish) (void);
extern void      (APIENTRY * dxglFrontFace) (GLenum mode);
extern void      (APIENTRY * dxglGenTextures) (GLsizei n, GLuint *textures);
extern GLenum    (APIENTRY * dxglGetError) (void);
extern void      (APIENTRY * dxglGetFloatv) (GLenum pname, GLfloat *params);
extern void      (APIENTRY * dxglLoadIdentity) (void);
extern void      (APIENTRY * dxglLoadMatrixf) (const GLfloat *m); //TO REMOVE
extern void      (APIENTRY * dxglMatrixMode) (GLenum mode);
extern void      (APIENTRY * dxglOrtho) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern void      (APIENTRY * dxglPolygonMode) (GLenum face, GLenum mode);
extern void      (APIENTRY * dxglPolygonOffset) (GLfloat factor, GLfloat units);
extern void      (APIENTRY * dxglRotatef) (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
extern void      (APIENTRY * dxglTexCoord2f) (GLfloat s, GLfloat t);
extern void      (APIENTRY * dxglTexCoord2fv) (const GLfloat *v);
extern void      (APIENTRY * dxglTexEnvf) (GLenum target, GLenum pname, GLfloat param);
extern void      (APIENTRY * dxglTexImage2D) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern void      (APIENTRY * dxglTexParameterf) (GLenum target, GLenum pname, GLfloat param);
extern void      (APIENTRY * dxglTexParameteri) (GLenum target, GLenum pname, GLint param);
extern void      (APIENTRY * dxglTranslatef) (GLfloat x, GLfloat y, GLfloat z);
extern void      (APIENTRY * dxglVertex2f) (GLfloat x, GLfloat y);
extern void      (APIENTRY * dxglVertex2i) (GLint x, GLint y);
extern void      (APIENTRY * dxglVertex3f) (GLfloat x, GLfloat y, GLfloat z);
extern void      (APIENTRY * dxglVertex3fv) (const GLfloat *v);
extern void      (APIENTRY * dxglViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

//GL_VERSION_1_2

#define GL_CLAMP_TO_EDGE                  0x812F

//GL_VERSION_1_3

#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1

extern void      (APIENTRY * dxglActiveTexture) (GLenum texture);
extern void      (APIENTRY * dxglMultiTexCoord2fv) (GLenum target, const GLfloat *v);

//GL_VERSION_1_5

typedef ptrdiff_t GLsizeiptr;

#define GL_ARRAY_BUFFER                   0x8892
#define GL_WRITE_ONLY                     0x88B9
#define GL_DYNAMIC_DRAW                   0x88E8

extern void      (APIENTRY * dxglBindBuffer) (GLenum target, GLuint buffer);
extern void      (APIENTRY * dxglDeleteBuffers) (GLsizei n, const GLuint *buffers);
extern void      (APIENTRY * dxglGenBuffers) (GLsizei n, GLuint *buffers);
extern void      (APIENTRY * dxglBufferData) (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
extern GLvoid *  (APIENTRY * dxglMapBuffer) (GLenum target, GLenum access);
extern GLboolean (APIENTRY * dxglUnmapBuffer) (GLenum target);

//GL_VERSION_2_0

#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83
#define GL_INFO_LOG_LENGTH                0x8B84

extern void      (APIENTRY * dxglAttachShader) (GLuint program, GLuint shader);
extern void      (APIENTRY * dxglBindAttribLocation) (GLuint program, GLuint index, const GLchar *name);
extern void      (APIENTRY * dxglCompileShader) (GLuint shader);
extern GLuint    (APIENTRY * dxglCreateProgram) (void);
extern GLuint    (APIENTRY * dxglCreateShader) (GLenum type);
extern void      (APIENTRY * dxglDeleteProgram) (GLuint program);
extern void      (APIENTRY * dxglDeleteShader) (GLuint shader);
extern void      (APIENTRY * dxglDetachShader) (GLuint program, GLuint shader);
extern void      (APIENTRY * dxglDisableVertexAttribArray) (GLuint index);
extern void      (APIENTRY * dxglEnableVertexAttribArray) (GLuint index);
extern void      (APIENTRY * dxglGetProgramiv) (GLuint program, GLenum pname, GLint *params);
extern void      (APIENTRY * dxglGetProgramInfoLog) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern void      (APIENTRY * dxglGetShaderiv) (GLuint shader, GLenum pname, GLint *params);
extern void      (APIENTRY * dxglGetShaderInfoLog) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern GLint     (APIENTRY * dxglGetUniformLocation) (GLuint program, const GLchar *name);
extern void      (APIENTRY * dxglLinkProgram) (GLuint program);
extern void      (APIENTRY * dxglShaderSource) (GLuint shader, GLsizei count, const GLchar* const *string, const GLint *length);
extern void      (APIENTRY * dxglUseProgram) (GLuint program);
extern void      (APIENTRY * dxglUniform1f) (GLint location, GLfloat v0);
extern void      (APIENTRY * dxglUniform3f) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern void      (APIENTRY * dxglUniform4f) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern void      (APIENTRY * dxglUniform1i) (GLint location, GLint v0);
extern void      (APIENTRY * dxglUniformMatrix4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void      (APIENTRY * dxglValidateProgram) (GLuint program);
extern void      (APIENTRY * dxglVertexAttribPointer) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

//glu

extern void      (APIENTRY * dxgluPerspective) (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
extern void      (APIENTRY * dxgluLookAt) (GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx, GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy, GLdouble upz);
extern int       (APIENTRY * dxgluBuild2DMipmaps) (GLenum target, GLint components, GLint width, GLint height, GLenum format, GLenum type, const void *data);

//window callback
typedef LRESULT (CALLBACK * GL_WindowProc)(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

struct sys_wnd_define_s
{
    int           windowcx;
    int           windowcy;
    GL_WindowProc wndproc;
};

void GL_Init(const sys_wnd_define_s &wnddefine);
void GL_Shutdown();
void GL_SwapBuffers();

#endif /* !__GLFUNCS_H__ */
