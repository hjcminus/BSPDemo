/*==============================================================================
  BSPDemo
  Menu.cpp
================================================================================
*/

#include "Precompiled.h"
#pragma hdrstop

#include <commdlg.h>

const dxVec4 FILL_COLOR_FOCUS  = {0,0.9f,0,0.5f};
const dxVec4 FILL_COLOR_NORMAL = {0,0.5f,0,0.5f};
const dxVec4 FRAME_COLOR = {0,0.5f,0,1.0f};
const dxVec4 TEXT_COLOR  = {1,1,1,0.5f};

#define DX_CHAR_WIDTH   8
#define DX_CHAR_HEIGHT 16

int TextWidth(const char * str)
{
    return strlen(str) * DX_CHAR_WIDTH;
}

/*
========================================
  dxWidget
========================================
*/
dxWidget::dxWidget()
{
    left = bottom = right = top = 0;

    caption[0] = 0;
    text_width = text_x = text_y = 0;
}

dxWidget::~dxWidget()
{
    //blank
}

void dxWidget::SetFocus(bool focus)
{
    focused = focus;
}

bool dxWidget::PointIn(int x, int y) const
{
    return x >= left && x <= right && y >= bottom && y <= top;
}

void dxWidget::SetPos(int lf, int bt, int rt, int tp)
{
    left   = lf;
    bottom = bt;
    right  = rt;
    top    = tp;
    UpldateTextPos();
}

void dxWidget::SetCaption(const char * str)
{
    strcpy_s(caption, DX_ID_LEN, str);
    text_width = TextWidth(str);
    UpldateTextPos();
}

void dxWidget::UpldateTextPos()
{
    text_x = left + ((right - left - text_width) >> 1);
    text_y = bottom + ((top - bottom - DX_CHAR_HEIGHT) >> 1);
}

void dxWidget::OnClick(int x, int y)
{
    //to override
}

void dxWidget::OnKeyDown(int key)
{
    //to override
}

int dxWidget::GetLeft() const
{
    return left;
}

int dxWidget::GetBottom() const
{
    return bottom;
}

int dxWidget::GetRight() const
{
    return right;
}

int dxWidget::GetTop() const
{
    return top;
}

int dxWidget::GetWidth() const
{
    return right - left;
}

int dxWidget::GetHeight() const
{
    return top - bottom;
}

/*
========================================
  dxLabel
========================================
*/
dxLabel::dxLabel()
{
    //blank
}

dxLabel::~dxLabel()
{
    //blank
}

void dxLabel::Draw()
{
    renderer.FrameRect(FRAME_COLOR, left, bottom, right, top);

    if (caption[0]) //draw caption
    {
        renderer.Print(TEXT_COLOR, text_x, text_y, caption);
    }
}

/*
========================================
  dxButton
========================================
*/
dxButton::dxButton()
{
    //blank
}

dxButton::~dxButton()
{
    //blank
}

void dxButton::Draw()
{
    renderer.FillRect(focused ? FILL_COLOR_FOCUS : FILL_COLOR_NORMAL, left, bottom, right, top);
    renderer.FrameRect(FRAME_COLOR, left, bottom, right, top);

    if (caption[0]) //draw caption
    {
        renderer.Print(TEXT_COLOR, text_x, text_y, caption);
    }
}

/*
========================================
  dxButtonOk
========================================
*/
dxButtonOk::dxButtonOk()
{
    lstmap = NULL;
}

void dxButtonOk::LinkListControl(class dxStaticListBox * lstmap_)
{
    lstmap = lstmap_;
}

void dxButtonOk::OnClick(int x, int y)
{
    if (lstmap)
    {
        const char * mapname = lstmap->GetSelected();
        if (mapname)
        {
            worldModel.Load(mapname, false);
        }
    }
}

/*
========================================
  dxButtonQuit
========================================
*/
void dxButtonQuit::OnClick(int x, int y)
{
    PostQuitMessage(0);
}

/*
========================================
  dxButtonSel
========================================
*/
void dxButtonSel::OnClick(int x, int y)
{
	OPENFILENAMEA ofn;

	char mapname[MAX_PATH];
	mapname[0] = 0;

	memset(&ofn, 0, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = mapname;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = "BSP Files (*.bsp)\0*.bsp\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = " ";
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn))
	{
		worldModel.Load(mapname, true);
	}
}

/*
========================================
  dxImage
========================================
*/
dxImage::dxImage()
{
    image = 0;
	oldfilename[0] = 0;
}

dxImage::~dxImage()
{
    //
}

void dxImage::Clearup()
{
	if (image != 0)
	{
		renderer.UnloadTempBMPTexture(image);
		image = 0;
	}
}

void dxImage::LoadImage(const char * filename)
{
	if (!_stricmp(filename, oldfilename))
	{
		return;
	}

	Clearup();

	strcpy_s(oldfilename, MAX_PATH, filename);

	char fullfilename[MAX_PATH];
	sprintf_s(fullfilename, MAX_PATH, "%s\\maps\\%s.bmp", default_basedir, oldfilename);
	image = renderer.LoadTempBMPTexture(fullfilename);
}

void dxImage::Draw()
{
    if (image != 0)
	{
		renderer.DrawImage(image, left, bottom, right, top);
	}
    renderer.FrameRect(FRAME_COLOR, left, bottom, right, top);
}

/*
========================================
  dxStaticListBox
========================================
*/
dxStaticListBox::dxStaticListBox()
{
    count = 0;
	selected = -1;
	image = NULL;
}

dxStaticListBox::~dxStaticListBox()
{
    //blank
}

void dxStaticListBox::LinkImageControl(dxImage * image_)
{
	image = image_;
}

void dxStaticListBox::AddItem(const char * item)
{
	if (count == DX_MAX_LIST_ITEM)
	{
		Sys_Error("too many list items");
	}

	strcpy_s(items[count++].text, DX_ID_LEN, item);
}

void dxStaticListBox::Select(int index)
{
	if (index < 0)
	{
		index = 0;
	}

	if (index >= count)
	{
		index = count - 1;
	}

	selected = index;

	if (image && selected != -1)
	{
		image->LoadImage(items[selected].text);
	}
}

const char * dxStaticListBox::GetSelected() const
{
    if (selected == -1)
    {
        return NULL;
    }
    return items[selected].text;
}

void dxStaticListBox::Draw()
{
	for (int i = 0; i < count; i++)
	{
		int item_lf = left;
		int item_bt = top - 16 * (i+1) - 5;
		int item_tp = item_bt + 16;
		int item_rt = right;

		if (i == selected)
		{
			renderer.FillRect(FILL_COLOR_NORMAL, item_lf, item_bt, item_rt, item_tp);
		}

		renderer.Print(TEXT_COLOR, item_lf + 10, item_bt, items[i].text);
	}

	renderer.FrameRect(FRAME_COLOR, left, bottom, right, top);
}

void dxStaticListBox::OnClick(int x, int y)
{
	int index = (top - y - 5) / 16;
	if (index >= count)
	{
		return;
	}
	Select(index);
}

void dxStaticListBox::OnKeyDown(int key)
{
    if (KEY_UP == key)
    {
        Select(selected-1);
    }
    else if (KEY_DOWN == key)
    {
        Select(selected+1);
    }
}

/*
========================================
  dxMenu
========================================
*/

#define MENU_WIDTH   480
#define MENU_HEIGHT  360

#define IMAGE_WIDTH  256
#define IMAGE_HEIGHT 256

dxMenu menu;

dxMenu::dxMenu()
{
    menu_left = menu_bottom = menu_right = menu_top = 0;
    mouse_down_x = mouse_down_y = 0;

    prev_focused = NULL;

    controls[0] = &lbl_title;
    controls[1] = &btn_ok;
    controls[2] = &btn_quit;
	controls[3] = &btn_sel;
    controls[4] = &img_map;
    controls[5] = &lst_map;

    InitControls();
}

dxMenu::~dxMenu()
{
    //blank
}

void dxMenu::Init()
{
	lst_map.LinkImageControl(&img_map);
	lst_map.Select(0);

    btn_ok.LinkListControl(&lst_map);
}

void dxMenu::Shutdown()
{
	img_map.Clearup();
}

void dxMenu::AddMapItem(const char * item)
{
    lst_map.AddItem(item);
}

void dxMenu::SelectDefault()
{
    lst_map.Select(0);
}

#define BTN_CX   65
#define BTN_CY   25
#define BTN_SPAN 10

void dxMenu::Draw()
{
    renderer.SetupTextMode();

    renderer.FillRect(FILL_COLOR_NORMAL, menu_left, menu_bottom, menu_right, menu_top);

    for (int i = 0; i < 6; i++)
    {
        controls[i]->Draw();
    }

	renderer.FrameRect(FRAME_COLOR, menu_left, menu_bottom, menu_right, menu_top);

	renderer.DrawLine(FRAME_COLOR, menu_left + BTN_SPAN, btn_quit.GetTop() + BTN_SPAN, 
		menu_right - BTN_SPAN, btn_quit.GetTop() + BTN_SPAN);

    renderer.EndTextMode();
}

void dxMenu::OnSize(int cx, int cy)
{
    //blank
}

void dxMenu::OnLButtonDown(int x, int y)
{
    mouse_down_x = x;
    mouse_down_y = y;
}

void dxMenu::OnLButtonUp(int x, int y)
{
    for (int i = 0; i < 6; i++)
    {
        if (controls[i]->PointIn(mouse_down_x, mouse_down_y)
        &&  controls[i]->PointIn(x, y))
        {
            controls[i]->OnClick(x, y);
            break;
        }
    }
}

void dxMenu::OnMouseMove(int x, int y)
{
    if (prev_focused)
    {
        prev_focused->SetFocus(false);
    }

    for (int i = 0; i < 6; i++)
    {
        if (controls[i]->PointIn(x, y))
        {
            prev_focused = controls[i];
            prev_focused->SetFocus(true);
            break;
        }
    }

}

void dxMenu::OnKeyDown(int key)
{
	if (KEY_ESCAPE == key)
	{
		if (worldModel.IsLoaded())
		{
			FS_Change(FS_DEMO);
		}
	}
	else
	{
        lst_map.OnKeyDown(key);
	}
}

void dxMenu::InitControls()
{
    int menu_cx = MENU_WIDTH;
    int menu_cy = MENU_HEIGHT;

    menu_left   = -menu_cx >> 1;
    menu_right  =  menu_cx >> 1;
    menu_bottom = -menu_cy >> 1;
    menu_top    =  menu_cy >> 1;

    lbl_title.SetPos(menu_left, menu_top - 25, menu_right, menu_top);
    lbl_title.SetCaption("Test Maps");

    int lf = menu_right - (BTN_CX + BTN_SPAN) * 2;
    int rt = lf + BTN_CX;
    int bt = menu_bottom + BTN_SPAN;
    int tp = bt + BTN_CY;

    btn_ok.SetPos(lf, bt, rt, tp);
    btn_ok.SetCaption("Ok");

    lf = rt + BTN_SPAN;
    rt = lf + BTN_CX;

    btn_quit.SetPos(lf, bt, rt, tp);
    btn_quit.SetCaption("Quit");

	btn_sel.SetPos(menu_left + BTN_SPAN, bt, menu_left + BTN_CX + 25, bt + BTN_CY);
	btn_sel.SetCaption("Select");

    lf = menu_left + BTN_SPAN;
    rt = lf + IMAGE_WIDTH;
    tp = menu_top - 15 - lbl_title.GetHeight();
    bt = tp - IMAGE_HEIGHT;

    img_map.SetPos(lf, bt, rt, tp);

    lf = img_map.GetRight() + BTN_SPAN;
    rt = menu_right - BTN_SPAN;
    tp = img_map.GetTop();
    bt = tp - IMAGE_HEIGHT;

    lst_map.SetPos(lf, bt, rt, tp);
}
