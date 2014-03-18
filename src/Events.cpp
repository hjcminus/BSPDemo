/*==============================================================================
  BSPDemo
  Events.cpp
================================================================================
*/

#include "Precompiled.h"
#pragma hdrstop

void Event_Size(int sizex, int sizey)
{
	RECT r;
	GetClientRect(sysvar.hMainWnd, &r);

	POINT ctr;

	ctr.x = (r.left + r.right)  >> 1;
	ctr.y = (r.top  + r.bottom) >> 1;
	ClientToScreen(sysvar.hMainWnd, &ctr);

	sysvar.ctrx = ctr.x;
	sysvar.ctry = ctr.y;

    menu.OnSize(sizex, sizey);
    renderer.SetViewport(sizex, sizey);
}

static bool changing_angle = false;
static int  prev_x = 0;
static int  prev_y = 0;

void Event_KeyDown(int key)
{
    if (FS_MENU == sysvar.fs)
    {
        menu.OnKeyDown(key);
    }
    else if (FS_DEMO == sysvar.fs)
    {
        if (KEY_F2 == key)
        {
            debugvar.fillmode = !debugvar.fillmode;
        }
        else if (KEY_ESCAPE == key)
        {
			FS_Change(FS_MENU); //back to menu

			if (changing_angle)
			{
				changing_angle = false;
				ReleaseCapture();
			}
			
        }
    }
}

void Event_KeyUp(int key)
{
    //
}

void Event_LButttonDown(int x, int y)
{
    if (FS_MENU == sysvar.fs)
    {
        Sys_WindowCoordToSysCoord(x, y);
        menu.OnLButtonDown(x, y);
    }
	else if (FS_DEMO == sysvar.fs)
	{
		changing_angle = true;
		prev_x = x;
		prev_y = y;
		SetCapture(sysvar.hMainWnd);
	}

}

void Event_LButttonUp(int x, int y)
{
    if (FS_MENU == sysvar.fs)
    {
        Sys_WindowCoordToSysCoord(x, y);
        menu.OnLButtonUp(x, y);
    }
	else if (FS_DEMO == sysvar.fs)
	{
		changing_angle = false;
		ReleaseCapture();
	}
}

void Event_MouseMove(int x, int y)
{
    if (FS_MENU == sysvar.fs)
    {
        Sys_WindowCoordToSysCoord(x, y);
        menu.OnMouseMove(x, y);
    }
    else if (FS_DEMO == sysvar.fs)
    {
		if (!changing_angle)
		{
			return;
		}

        int deltax = -(x - prev_x);
        int deltay =  (y - prev_y);

        //static int i = 0;
        //i++;

        //Sys_Debug("[%d] y: %d, prev_y: %d, deltay: %d\n", i, y, prev_y, deltay);

		prev_x = x;
		prev_y = y;

        //update yaw
        viewer.viewangles[DX_YAW] += deltax * 0.2f;
        viewer.viewangles[DX_YAW]  = AngleNormalize360(viewer.viewangles[DX_YAW]);

        //update pitch
        float pitch = AngleNormalize180(viewer.viewangles[DX_PITCH]);

        //Sys_Debug("[%d] pitch %f\n", i, pitch);

        if (deltay > 0)
        {
            pitch += deltay * 0.2f;
            if (pitch > 89)
            {
                //Sys_Debug("[%d] clamp to 89\n", i);
                pitch = 89;
            }
        }
        else if (deltay < 0)
        {
            pitch += deltay * 0.2f;
            if (pitch < -89)
            {
                //Sys_Debug("[%d] clamp to -89\n", i);
                pitch = -89;
            }
        }

        viewer.viewangles[DX_PITCH] = AngleNormalize360(pitch);

        //Sys_Debug("[%d] DX_PITCH %f\n", i, viewer.viewangles[DX_PITCH]);

        /*

        // -89 <= pitch <= 89
        if (deltay < 0)
        {
            if (viewer.viewangles[DX_PITCH] < -89)
            {
                viewer.viewangles[DX_PITCH] = -89;
            }
        }
        else if (deltay > 0)
        {
            if (viewer.viewangles[DX_PITCH] > 89)
            {
                viewer.viewangles[DX_PITCH] = 89;
            }
        }

        viewer.viewangles[DX_PITCH] = AngleNormalize360(viewer.viewangles[DX_PITCH]);

        */

        viewer.anglechanged = true;
    }
    
}
