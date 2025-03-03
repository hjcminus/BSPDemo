/*==============================================================================
  BSPDemo
  BSPDemo.cpp - main entrance
================================================================================
*/

#include "Precompiled.h"
#pragma hdrstop

#include <MMSystem.h>
#include <commdlg.h>
#include <Strsafe.h>

#pragma comment(lib, "winmm.lib")

LRESULT CALLBACK Sys_WindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
    RECT  clientrect;
    POINT pt;

    switch (uiMsg)
    {
    case WM_ACTIVATE:
        sysvar.appactive = !(LOWORD(wParam) == WA_INACTIVE);
        break;
    case WM_LBUTTONDOWN:
        GetCursorPos(&pt);
        Event_LButttonDown(pt.x, pt.y);
        //Event_LButttonDown(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_LBUTTONUP:
        GetCursorPos(&pt);
        Event_LButttonUp(pt.x, pt.y);
        //Event_LButttonUp(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_MOUSEMOVE:
        GetCursorPos(&pt);
        Event_MouseMove(pt.x, pt.y);
        //Event_MouseMove(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_KEYDOWN:
        Event_KeyDown((int)wParam);
        break;
    case WM_KEYUP:
        Event_KeyUp((int)wParam);
        break;
	case WM_MOVE:
    case WM_SIZE:
        GetClientRect(hWnd, &clientrect);
        sysvar.viewwidth = clientrect.right - clientrect.left;
        sysvar.viewheight = clientrect.bottom - clientrect.top;
        Event_Size(sysvar.viewwidth, sysvar.viewheight);
        break;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

static DWORD timebase;

float Sys_Seconds()
{
    return (timeGetTime() - timebase) * 0.001f;
}

void Sys_GetOpenFileName(char * filename)
{
    OPENFILENAMEA ofn;

    memset(&ofn, 0, sizeof(ofn));
    filename[0] = 0;

    ofn.lStructSize     = sizeof(OPENFILENAME);
    ofn.hwndOwner       = NULL;
    ofn.lpstrFile       = filename;
    ofn.nMaxFile        = MAX_PATH;
    ofn.lpstrFilter     = "BSP Files (*.bsp)\0*.bsp\0\0";
    ofn.nFilterIndex    = 1;
    ofn.lpstrFileTitle  = " ";
    ofn.nMaxFileTitle   = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (!GetOpenFileNameA(&ofn))
    {
        void Sys_Quit();
        Sys_Quit();
    }
}

char default_basedir[MAX_PATH];

bool Sys_FolderExists(const char* folder) {
    DWORD code = GetFileAttributesA(folder);
    if (code != INVALID_FILE_ATTRIBUTES) {
        return code != 0;
    }
    else {
        DWORD err = GetLastError();
        return err != ERROR_FILE_NOT_FOUND && err != ERROR_PATH_NOT_FOUND && err != ERROR_INVALID_NAME && err != ERROR_BAD_NETPATH;
    }
}

void Sys_LoadConfig()
{
    char exefilename[MAX_PATH];
    GetModuleFileNameA(NULL, exefilename, MAX_PATH);

    char exedirname[MAX_PATH];
    Str_ExtractDirName(exefilename, exedirname);

    //init basedir
	sprintf_s(default_basedir, MAX_PATH, "%s\\..\\..\\..\\res", exedirname);
    if (!Sys_FolderExists(default_basedir)) {
        sprintf_s(default_basedir, MAX_PATH, "%s\\..\\..\\res", exedirname);
    }

	char inifilename[MAX_PATH];
	sprintf_s(inifilename, MAX_PATH, "%s\\demo.ini", default_basedir);

    //init map item
    char value[1024];
    value[0] = 0;
    Cfg_ReadStr("BSPDEMO", "MAPCOUNT", "", value, inifilename);

    int mapcount = atoi(value);

    if (mapcount > DX_MAX_LIST_ITEM)
    {
        Sys_Error("too many demo map");
    }

    for (int i = 0; i < mapcount; i++)
    {
        char ident[DX_ID_LEN];
        sprintf_s(ident, DX_ID_LEN, "MAP%d", i+1);

        value[0] = 0;
        Cfg_ReadStr("BSPDEMO", ident, "", value, inifilename);
        if (value[0])
        {
            if (strlen(value) >= DX_ID_LEN)
            {
                Sys_Error("map name too long");
            }
            menu.AddMapItem(value);
        }
    }

    menu.SelectDefault();

    //init key event
    value[0] = 0;
    Cfg_ReadStr("BSPDEMO", "MOVEFORWARD", "", value, inifilename);
    keys.moveforward = Key_InitMoveForwardKey(value);

    value[0] = 0;
    Cfg_ReadStr("BSPDEMO", "MOVEBACKWARD", "", value, inifilename);
    keys.movebackward = Key_InitMoveBackwardKey(value);

    value[0] = 0;
    Cfg_ReadStr("BSPDEMO", "MOVELEFT", "", value, inifilename);
    keys.moveleft = Key_InitMoveLeftKey(value);

    value[0] = 0;
    Cfg_ReadStr("BSPDEMO", "MOVERIGHT", "", value, inifilename);
    keys.moveright = Key_InitMoveRightKey(value);
}

void Sys_Init()
{
    void Swap_Init();
    Swap_Init();

    SetThreadAffinityMask(GetCurrentThread(), 1);

    sysvar.fs = FS_MENU;
    debugvar.fillmode = true;

    //init time base
    timeBeginPeriod(1);
    timebase = timeGetTime();

    //init gl
    sys_wnd_define_s def;

    def.windowcx = 640;
    def.windowcy = 480;
    def.wndproc  = Sys_WindowProc;

    GL_Init(def);
    renderer.Init();

	menu.Init();

    Key_Init();
    Sys_LoadConfig();
}

void Sys_Quit()
{
    worldModel.Free();
	menu.Shutdown();

    renderer.Shutdown();
    GL_Shutdown();

    timeEndPeriod(1);

    Mem_Dump();

    ExitProcess(0);
}

void Sys_Debug(const char *fmt, ...)
{
    char buffer[1024];

    va_list argptr;
    va_start(argptr, fmt);
    StringCbVPrintfA(buffer, sizeof(buffer), fmt, argptr);
    va_end(argptr);

    OutputDebugStringA(buffer);
}

void Sys_Error(const char *fmt, ...)
{
    char buffer[1024];

    va_list argptr;
    va_start(argptr, fmt);
    StringCbVPrintfA(buffer, sizeof(buffer), fmt, argptr);
    va_end(argptr);

    MessageBoxA(NULL, buffer, "Error", 0);

    Sys_Quit();
}

bool Sys_ProcessMessage()
{
    MSG sMSG;

    if (PeekMessage(&sMSG, NULL, 0, 0, PM_NOREMOVE))
    {
        if (GetMessage(&sMSG, NULL, 0, 0))
        {
            TranslateMessage(&sMSG);
            DispatchMessage(&sMSG);
        } 
        else
        {
            return false; //WM_QUIT
        }
    }

    return true;
}

static float oldtime;

void Sys_Frame()
{
    if (!sysvar.appactive)
    {
        return;
    }

    float curtime = Sys_Seconds();

    float delta = curtime - oldtime;

	if (delta < 0.016f)
	{
		return;
	}

    oldtime = curtime;

    frame.deltatime = delta;
    frame.totaltime += delta;

    renderer.BeginDraw();

    if (FS_MENU == sysvar.fs)
    {
        menu.Draw();
    }
    else if (FS_LOADING == sysvar.fs)
    {
        worldModel.DrawLoading();
    }
    else //FS_DEMO
    {
        viewer.Update();
        worldModel.Draw(viewer.vieworigin, viewer.viewforward, viewer.viewright, viewer.viewup);
    }

    renderer.EndDraw();
}

void Sys_UpdateScreen()
{
    if (Sys_ProcessMessage())
    {
        Sys_Frame();
    }
}

frame_s    frame;
sysvar_s   sysvar;
debugvar_s debugvar;

int	WINAPI WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst,
    LPSTR lpszCmdLine, int nCmdShow)
{
    Sys_Init();

    oldtime = Sys_Seconds();

    while(1)
    {
        if (!Sys_ProcessMessage())
        {
            break;
        }
        Sys_Frame();
    }

    Sys_Quit();

    return 0;
}
