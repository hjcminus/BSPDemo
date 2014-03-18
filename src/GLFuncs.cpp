/*==============================================================================
  BSPDemo
  GLFuncs.cpp
================================================================================
*/

#include "Precompiled.h"
#pragma hdrstop

//GL_VERSION_1_1

void      (APIENTRY * dxglAlphaFunc) (GLenum func, GLclampf ref);
void      (APIENTRY * dxglBegin) (GLenum mode);
void      (APIENTRY * dxglBindTexture) (GLenum target, GLuint texture);
void      (APIENTRY * dxglBlendFunc) (GLenum sfactor, GLenum dfactor);
void      (APIENTRY * dxglClear) (GLbitfield mask);
void      (APIENTRY * dxglClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void      (APIENTRY * dxglClearDepth) (GLclampd depth);
void      (APIENTRY * dxglColor4fv) (const GLfloat *v);
void      (APIENTRY * dxglCullFace) (GLenum mode);
void      (APIENTRY * dxglDeleteTextures) (GLsizei n, const GLuint *textures);
void      (APIENTRY * dxglDepthFunc) (GLenum func);
void      (APIENTRY * dxglDepthMask) (GLboolean flag);
void      (APIENTRY * dxglDisable) (GLenum cap);
void      (APIENTRY * dxglDisableClientState) (GLenum array);
void      (APIENTRY * dxglDrawArrays) (GLenum mode, GLint first, GLsizei count);
void      (APIENTRY * dxglDrawBuffer) (GLenum mode);
void      (APIENTRY * dxglEnable) (GLenum cap);
void      (APIENTRY * dxglEnableClientState) (GLenum array);
void      (APIENTRY * dxglEnd) (void);
void      (APIENTRY * dxglFinish) (void);
void      (APIENTRY * dxglFrontFace) (GLenum mode);
void      (APIENTRY * dxglGenTextures) (GLsizei n, GLuint *textures);
void      (APIENTRY * dxglGetIntegerv) (GLenum pname, GLint *params);
GLenum    (APIENTRY * dxglGetError) (void);
void      (APIENTRY * dxglGetFloatv) (GLenum pname, GLfloat *params);
void      (APIENTRY * dxglLoadIdentity) (void);
void      (APIENTRY * dxglLoadMatrixf) (const GLfloat *m);
void      (APIENTRY * dxglMatrixMode) (GLenum mode);
void      (APIENTRY * dxglOrtho) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void      (APIENTRY * dxglRotatef) (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY * dxglTexCoord2f) (GLfloat s, GLfloat t);
void      (APIENTRY * dxglTexCoord2fv) (const GLfloat *v);
void      (APIENTRY * dxglTexEnvf) (GLenum target, GLenum pname, GLfloat param);
void      (APIENTRY * dxglPolygonMode) (GLenum face, GLenum mode);
void      (APIENTRY * dxglPolygonOffset) (GLfloat factor, GLfloat units);
void      (APIENTRY * dxglTexImage2D) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void      (APIENTRY * dxglTexParameterf) (GLenum target, GLenum pname, GLfloat param);
void      (APIENTRY * dxglTexParameteri) (GLenum target, GLenum pname, GLint param);
void      (APIENTRY * dxglTranslatef) (GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY * dxglVertex2f) (GLfloat x, GLfloat y);
void      (APIENTRY * dxglVertex2i) (GLint x, GLint y);
void      (APIENTRY * dxglVertex3f) (GLfloat x, GLfloat y, GLfloat z);
void      (APIENTRY * dxglVertex3fv) (const GLfloat *v);
void      (APIENTRY * dxglViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

//GL_VERSION_1_3

void      (APIENTRY * dxglActiveTexture) (GLenum texture);
void      (APIENTRY * dxglMultiTexCoord2fv) (GLenum target, const GLfloat *v);

//GL_VERSION_1_5

void      (APIENTRY * dxglBindBuffer) (GLenum target, GLuint buffer);
void      (APIENTRY * dxglDeleteBuffers) (GLsizei n, const GLuint *buffers);
void      (APIENTRY * dxglGenBuffers) (GLsizei n, GLuint *buffers);
void      (APIENTRY * dxglBufferData) (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
GLvoid *  (APIENTRY * dxglMapBuffer) (GLenum target, GLenum access);
GLboolean (APIENTRY * dxglUnmapBuffer) (GLenum target);

//GL_VERSION_2_0

void      (APIENTRY * dxglAttachShader) (GLuint program, GLuint shader);
void      (APIENTRY * dxglBindAttribLocation) (GLuint program, GLuint index, const GLchar *name);
void      (APIENTRY * dxglCompileShader) (GLuint shader);
GLuint    (APIENTRY * dxglCreateProgram) (void);
GLuint    (APIENTRY * dxglCreateShader) (GLenum type);
void      (APIENTRY * dxglDeleteProgram) (GLuint program);
void      (APIENTRY * dxglDeleteShader) (GLuint shader);
void      (APIENTRY * dxglDetachShader) (GLuint program, GLuint shader);
void      (APIENTRY * dxglDisableVertexAttribArray) (GLuint index);
void      (APIENTRY * dxglEnableVertexAttribArray) (GLuint index);
void      (APIENTRY * dxglGetProgramiv) (GLuint program, GLenum pname, GLint *params);
void      (APIENTRY * dxglGetProgramInfoLog) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void      (APIENTRY * dxglGetShaderiv) (GLuint shader, GLenum pname, GLint *params);
void      (APIENTRY * dxglGetShaderInfoLog) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLint     (APIENTRY * dxglGetUniformLocation) (GLuint program, const GLchar *name);
void      (APIENTRY * dxglLinkProgram) (GLuint program);
void      (APIENTRY * dxglShaderSource) (GLuint shader, GLsizei count, const GLchar* const *string, const GLint *length);
void      (APIENTRY * dxglUseProgram) (GLuint program);
void      (APIENTRY * dxglUniform1f) (GLint location, GLfloat v0);
void      (APIENTRY * dxglUniform3f) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void      (APIENTRY * dxglUniform4f) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void      (APIENTRY * dxglUniform1i) (GLint location, GLint v0);
void      (APIENTRY * dxglUniformMatrix4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void      (APIENTRY * dxglValidateProgram) (GLuint program);
void      (APIENTRY * dxglVertexAttribPointer) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

//glu

void      (APIENTRY * dxgluPerspective) (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
void      (APIENTRY * dxgluLookAt) (GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx, GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy, GLdouble upz);
int       (APIENTRY * dxgluBuild2DMipmaps) (GLenum target, GLint components, GLint width, GLint height, GLenum format, GLenum type, const void *data);

/*
================================================================================
  functions
================================================================================
*/

static HINSTANCE hOpenGL;
static HINSTANCE hGLU;

//wgl
static HGLRC  (WINAPI * dxwglCreateContext) (HDC);
static PROC   (WINAPI * dxwglGetProcAddress) (LPCSTR);
static BOOL   (WINAPI * dxwglMakeCurrent) (HDC, HGLRC);

static void GL_InitWGL()
{
#define GETAPI_WGL(X) if ((*(FARPROC*)&dx##X = GetProcAddress(hOpenGL, #X)) == NULL) { Sys_Error("load \"%s\" failed", #X); }

    GETAPI_WGL(wglCreateContext)
    GETAPI_WGL(wglGetProcAddress)
    GETAPI_WGL(wglMakeCurrent)
}

#define DEMO_STR L"BSPDEMO"

static void GL_CreateWindow(const sys_wnd_define_s &wnddefine)
{
    WNDCLASSEX wc;

    HINSTANCE hInst  = GetModuleHandle(NULL);

    wc.cbSize        = sizeof(wc);
    wc.style		 = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc   = wnddefine.wndproc;								
    wc.cbClsExtra	 = 0;										
    wc.cbWndExtra	 = 0;										
    wc.hInstance	 = hInst;							
    wc.hIcon		 = NULL;
    wc.hCursor	     = LoadCursor(NULL, IDC_ARROW);			
    wc.lpszMenuName  = NULL;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = DEMO_STR;
    wc.hIconSm       = NULL;

    if (!RegisterClassEx(&wc))
    {
        Sys_Error("register class failed");
        return;		
    }

    RECT rc = {0, 0, wnddefine.windowcx, wnddefine.windowcy};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	int screencx = GetSystemMetrics(SM_CXSCREEN);
	int screency = GetSystemMetrics(SM_CYSCREEN);

	int window_cx = rc.right - rc.left;
	int window_cy = rc.bottom - rc.top;

	int window_lf = (screencx - window_cx) >> 1;
	int window_tp = (screency - window_cy) >> 1;

    sysvar.hMainWnd = CreateWindowEx(WS_EX_CLIENTEDGE, DEMO_STR, DEMO_STR,
        WS_OVERLAPPEDWINDOW,
        window_lf, window_tp, window_cx, window_cy, NULL, NULL, hInst, NULL);

    if (NULL == sysvar.hMainWnd)
    {
        Sys_Error("create window failed");
        return;	
    }

    ShowWindow(sysvar.hMainWnd, SW_SHOW);
    UpdateWindow(sysvar.hMainWnd);
    SetForegroundWindow(sysvar.hMainWnd);
    SetFocus(sysvar.hMainWnd);

    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof( PIXELFORMATDESCRIPTOR ),	//Get the size of the structure
        1,									//Version number
        PFD_DRAW_TO_WINDOW |				//Format must support Windows
        PFD_SUPPORT_OPENGL |				//Format must support OpenGL
        PFD_DOUBLEBUFFER,					//Must support Double Buffering
        PFD_TYPE_RGBA,						//Request a RGBA (red,green,blue,alpha) format
        24,								    //Select the bits per pixel
        0, 0, 0, 0, 0, 0,					//Color bits ignored
        0,									//No alpha buffer
        0,									//shift bit ignored
        0,									//No accumulation buffer (advanced)
        0, 0, 0, 0,							//Accumulation bits ignored
        32,									//16 bit Z-Buffer (Depth Buffer)  
        0,									//No Stencil Buffer (advanced)
        0,									//No Auxiliary Buffer (advanced)
        PFD_MAIN_PLANE,						//The main drawing layer
        0,									//Reserved area
        0, 0, 0								//Layer masks ignored
    };

    int uiPixelFormat;

    sysvar.hMainDC = GetDC(sysvar.hMainWnd);

    //find a compatible pixel format
    if (!(uiPixelFormat = ChoosePixelFormat(sysvar.hMainDC, &pfd)))
    {
        Sys_Error("ChoosePixelFormat failed");
        return;
    }

    //set the previously found pixel format
    if (!SetPixelFormat(sysvar.hMainDC, uiPixelFormat, &pfd))
    {
        Sys_Error("SetPixelFormat failed");
        return;
    }

    //get a handle to the application's rendering context
    HGLRC hGLRC = dxwglCreateContext(sysvar.hMainDC);

    //set the device and rendering context to our window (activating OpenGL)
    if (!dxwglMakeCurrent(sysvar.hMainDC, hGLRC))
    {
        Sys_Error("wglMakeCurrent failed");
    }
}

static void GL_InitGL()
{
#define GETAPI_1_1(X) if ((*(FARPROC*)&dx##X = GetProcAddress(hOpenGL, #X)) == NULL) { Sys_Error("load \"%s\" failed", #X); }
#define GETAPI_EXT(X) if ((*(FARPROC*)&dx##X = dxwglGetProcAddress(#X)) == NULL) { Sys_Error("load \"%s\" failed", #X); }

    //GL_VERSION_1_1

    GETAPI_1_1(glAlphaFunc)
    GETAPI_1_1(glBegin)
    GETAPI_1_1(glBindTexture)
    GETAPI_1_1(glBlendFunc)
    GETAPI_1_1(glClear)
    GETAPI_1_1(glClearColor)
    GETAPI_1_1(glClearDepth)
    GETAPI_1_1(glColor4fv)
    GETAPI_1_1(glCullFace)
    GETAPI_1_1(glDeleteTextures)
    GETAPI_1_1(glDepthFunc)
    GETAPI_1_1(glDepthMask)
    GETAPI_1_1(glDisable)
    GETAPI_1_1(glDisableClientState)
    GETAPI_1_1(glDrawArrays)
    GETAPI_1_1(glDrawBuffer)
    GETAPI_1_1(glEnable)
    GETAPI_1_1(glEnableClientState)
    GETAPI_1_1(glEnd)
    GETAPI_1_1(glFinish)
    GETAPI_1_1(glFrontFace)
    GETAPI_1_1(glGenTextures)
	GETAPI_1_1(glGetIntegerv)
    GETAPI_1_1(glGetError)
    GETAPI_1_1(glGetFloatv)
    GETAPI_1_1(glLoadIdentity)
    GETAPI_1_1(glLoadMatrixf)
    GETAPI_1_1(glMatrixMode)
    GETAPI_1_1(glOrtho)
    GETAPI_1_1(glPolygonMode)
    GETAPI_1_1(glPolygonOffset)
    GETAPI_1_1(glRotatef)
    GETAPI_1_1(glTexCoord2f)
    GETAPI_1_1(glTexCoord2fv)
    GETAPI_1_1(glTexEnvf)
    GETAPI_1_1(glTexImage2D)
    GETAPI_1_1(glTexParameterf)
    GETAPI_1_1(glTexParameteri)
    GETAPI_1_1(glTranslatef)
    GETAPI_1_1(glVertex2f)
    GETAPI_1_1(glVertex2i)
    GETAPI_1_1(glVertex3f)
    GETAPI_1_1(glVertex3fv)
    GETAPI_1_1(glViewport)

    //GL_VERSION_1_3

    GETAPI_EXT(glActiveTexture)
    GETAPI_EXT(glMultiTexCoord2fv)

    //GL_VERSION_1_5

    GETAPI_EXT(glBindBuffer)
    GETAPI_EXT(glDeleteBuffers)
    GETAPI_EXT(glGenBuffers)
    GETAPI_EXT(glBufferData)
    GETAPI_EXT(glMapBuffer)
    GETAPI_EXT(glUnmapBuffer)

    //GL_VERSION_2_0

    GETAPI_EXT(glAttachShader)
    GETAPI_EXT(glBindAttribLocation)
    GETAPI_EXT(glCompileShader)
    GETAPI_EXT(glCreateProgram)
    GETAPI_EXT(glCreateShader)
    GETAPI_EXT(glDeleteProgram)
    GETAPI_EXT(glDeleteShader)
    GETAPI_EXT(glDetachShader)
    GETAPI_EXT(glDisableVertexAttribArray)
    GETAPI_EXT(glEnableVertexAttribArray)
    GETAPI_EXT(glGetProgramiv)
    GETAPI_EXT(glGetProgramInfoLog)
    GETAPI_EXT(glGetShaderiv)
    GETAPI_EXT(glGetShaderInfoLog)
    GETAPI_EXT(glGetUniformLocation)
    GETAPI_EXT(glLinkProgram)
    GETAPI_EXT(glShaderSource)
    GETAPI_EXT(glUseProgram)
    GETAPI_EXT(glUniform1f)
    GETAPI_EXT(glUniform3f)
    GETAPI_EXT(glUniform4f)
    GETAPI_EXT(glUniform1i)
    GETAPI_EXT(glUniformMatrix4fv)
    GETAPI_EXT(glValidateProgram)
    GETAPI_EXT(glVertexAttribPointer)

	GLint major, minor;

	dxglGetIntegerv(GL_MAJOR_VERSION, &major);
	dxglGetIntegerv(GL_MINOR_VERSION, &minor);

	if (major < 3 || (major == 3 && minor < 1))
	{
		Sys_Error("gl version must be equal or above to 3.1");
	}
}

static void GL_InitGLU()
{
#define GETAPI_GLU(X) if ((*(FARPROC*)&dx##X = GetProcAddress(hGLU, #X)) == NULL) { Sys_Error("load \"%s\" failed", #X); }

    GETAPI_GLU(gluPerspective)
    GETAPI_GLU(gluLookAt)
    GETAPI_GLU(gluBuild2DMipmaps)
}

void GL_Init(const sys_wnd_define_s &wnddefine)
{
    //load dll
    if (NULL == hOpenGL)
    {
        hOpenGL = LoadLibrary(L"opengl32.dll");
    }

    if (!hOpenGL)
    {
        Sys_Error("could not load opengl32.dll");
    }

    if (NULL == hGLU)
    {
        hGLU = LoadLibrary(L"glu32.dll");
    }

    if (!hGLU)
    {
        Sys_Error("could not load glu32.dll");
    }

    GL_InitWGL();
    GL_CreateWindow(wnddefine);
    GL_InitGL();
    GL_InitGLU();
}

void GL_Shutdown()
{
#define CLEARAPI(X) dx##X = NULL;

    CLEARAPI(wglCreateContext)
    CLEARAPI(wglGetProcAddress)
    CLEARAPI(wglMakeCurrent)

    //GL_VERSION_1_1

    CLEARAPI(glAlphaFunc)
    CLEARAPI(glBegin)
    CLEARAPI(glBindTexture)
    CLEARAPI(glBlendFunc)
    CLEARAPI(glClear)
    CLEARAPI(glClearColor)
    CLEARAPI(glClearDepth)
    CLEARAPI(glColor4fv)
    CLEARAPI(glCullFace)
    CLEARAPI(glDeleteTextures)
    CLEARAPI(glDepthFunc)
    CLEARAPI(glDepthMask)
    CLEARAPI(glDisable)
    CLEARAPI(glDisableClientState)
    CLEARAPI(glDrawArrays)
    CLEARAPI(glDrawBuffer)
    CLEARAPI(glEnable)
    CLEARAPI(glEnableClientState)
    CLEARAPI(glEnd)
    CLEARAPI(glFinish)
    CLEARAPI(glFrontFace)
    CLEARAPI(glGenTextures)
	CLEARAPI(glGetIntegerv)
    CLEARAPI(glGetError)
    CLEARAPI(glGetFloatv)
    CLEARAPI(glLoadIdentity)
    CLEARAPI(glLoadMatrixf)
    CLEARAPI(glMatrixMode)
    CLEARAPI(glOrtho)
    CLEARAPI(glPolygonMode)
    CLEARAPI(glPolygonOffset)
    CLEARAPI(glRotatef)
    CLEARAPI(glTexCoord2f)
    CLEARAPI(glTexCoord2fv)
    CLEARAPI(glTexEnvf)
    CLEARAPI(glTexImage2D)
    CLEARAPI(glTexParameterf)
    CLEARAPI(glTexParameteri)
    CLEARAPI(glTranslatef)
    CLEARAPI(glVertex2f)
    CLEARAPI(glVertex2i)
    CLEARAPI(glVertex3f)
    CLEARAPI(glVertex3fv)
    CLEARAPI(glViewport)

    //GL_VERSION_1_3

    CLEARAPI(glActiveTexture)
    CLEARAPI(glMultiTexCoord2fv)

    //GL_VERSION_1_5

    CLEARAPI(glBindBuffer)
    CLEARAPI(glDeleteBuffers)
    CLEARAPI(glGenBuffers)
    CLEARAPI(glBufferData)
    CLEARAPI(glMapBuffer)
    CLEARAPI(glUnmapBuffer)

    //GL_VERSION_2_0

    CLEARAPI(glAttachShader)
    CLEARAPI(glBindAttribLocation)
    CLEARAPI(glCompileShader)
    CLEARAPI(glCreateProgram)
    CLEARAPI(glCreateShader)
    CLEARAPI(glDeleteProgram)
    CLEARAPI(glDeleteShader)
    CLEARAPI(glDetachShader)
    CLEARAPI(glDisableVertexAttribArray)
    CLEARAPI(glEnableVertexAttribArray)
    CLEARAPI(glGetProgramiv)
    CLEARAPI(glGetProgramInfoLog)
    CLEARAPI(glGetShaderiv)
    CLEARAPI(glGetShaderInfoLog)
    CLEARAPI(glGetUniformLocation)
    CLEARAPI(glLinkProgram)
    CLEARAPI(glShaderSource)
    CLEARAPI(glUseProgram)
    CLEARAPI(glUniform1f)
    CLEARAPI(glUniform3f)
    CLEARAPI(glUniform4f)
    CLEARAPI(glUniform1i)
    CLEARAPI(glUniformMatrix4fv)
    CLEARAPI(glValidateProgram)
    CLEARAPI(glVertexAttribPointer)

    CLEARAPI(gluPerspective)
    CLEARAPI(gluLookAt)
    CLEARAPI(gluBuild2DMipmaps)

    //destroy window
    if (sysvar.hMainWnd != NULL)
    {
        if (sysvar.hMainDC != NULL)
        {
            ReleaseDC(sysvar.hMainWnd, sysvar.hMainDC);
            sysvar.hMainDC = NULL;
        }

        DestroyWindow(sysvar.hMainWnd);
        sysvar.hMainWnd = NULL;

        HINSTANCE hInst  = GetModuleHandle(NULL);
        UnregisterClass(DEMO_STR, hInst);
    }

    //unload library

    if (hGLU != NULL)
    {
        FreeLibrary(hGLU);
        hGLU = NULL;
    }

    if (hOpenGL != NULL)
    {
        FreeLibrary(hOpenGL);
        hOpenGL = NULL;
    }
}

void GL_SwapBuffers()
{
    SwapBuffers(sysvar.hMainDC);
}
