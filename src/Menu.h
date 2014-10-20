/*==============================================================================
  BSPDemo
  Menu.h
================================================================================
*/

#ifndef __MENU_H__
#define __MENU_H__

/*
========================================
  dxWidget
========================================
*/
class dxWidget
{
public:

    dxWidget();
    ~dxWidget();

    void          SetFocus(bool focus);
    bool          PointIn(int x, int y) const;

    virtual void  SetPos(int lf, int bt, int rt, int tp);
    virtual void  Draw() = 0;
    void          SetCaption(const char * str);

    virtual void  OnClick(int x, int y);
    virtual void  OnKeyDown(int key);

    int           GetLeft() const;
    int           GetBottom() const;
    int           GetRight() const;
    int           GetTop() const;

    int           GetWidth() const;
    int           GetHeight() const;

protected:

    int           left;
    int           bottom;
    int           right;
    int           top;

    char          caption[DX_ID_LEN];
    int           text_x;
    int           text_y;
    int           text_width;

    bool          focused;

    void          UpldateTextPos();
};

/*
========================================
  dxLabel
========================================
*/
class dxLabel : public dxWidget
{
public:

    dxLabel();
    ~dxLabel();

    virtual void  Draw();

};

/*
========================================
  dxButton
========================================
*/
class dxButton : public dxWidget
{
public:

    dxButton();
    ~dxButton();

    virtual void  Draw();

};

/*
========================================
  dxButtonOk
========================================
*/
class dxButtonOk : public dxButton
{
public:

    dxButtonOk();

    void          LinkListControl(class dxStaticListBox * lstmap_);
    virtual void  OnClick(int x, int y);

private:

    class dxStaticListBox * lstmap;
};

/*
========================================
  dxButtonQuit
========================================
*/
class dxButtonQuit : public dxButton
{
public:

    virtual void  OnClick(int x, int y);
};

/*
========================================
  dxButtonSel
========================================
*/
class dxButtonSel : public dxButton
{
public:

	virtual void  OnClick(int x, int y);
};


/*
========================================
  dxImage
========================================
*/
class dxImage : public dxWidget
{
public:

    dxImage();
    ~dxImage();

	void          Clearup();
    void          LoadImage(const char * filename);
    virtual void  Draw();

private:

    GLuint        image;
	char          oldfilename[MAX_PATH];

};

/*
========================================
  dxStaticListBox
========================================
*/

#define DX_MAX_LIST_ITEM 16

class dxStaticListBox : public dxWidget
{
public:

    dxStaticListBox();
    ~dxStaticListBox();

	void          LinkImageControl(dxImage * image_);

    void          AddItem(const char * item);
	void          Select(int index);
    const char *  GetSelected() const;

    virtual void  Draw();
	virtual void  OnClick(int x, int y);
    virtual void  OnKeyDown(int key);

private:

    struct listitem_s
    {
        char      text[DX_ID_LEN];
    };

    listitem_s    items[DX_MAX_LIST_ITEM];
	int           count;
	int           selected;

	dxImage *     image;

};

/*
========================================
  dxMenu
========================================
*/

class dxMenu
{
public:

    dxMenu();
    ~dxMenu();

	void          Init();
	void          Shutdown();

    void          AddMapItem(const char * item);
    void          SelectDefault();

    void          Draw();

    void          OnSize(int cx, int cy);
    void          OnLButtonDown(int x, int y);
    void          OnLButtonUp(int x, int y);
    void          OnMouseMove(int x, int y);
    void          OnKeyDown(int key);

private:

    int           menu_left;
    int           menu_bottom;
    int           menu_right;
    int           menu_top;

    dxLabel       lbl_title;
    dxButtonOk    btn_ok;
    dxButtonQuit  btn_quit;
	dxButtonSel   btn_sel;
    dxStaticListBox lst_map;
    dxImage       img_map;

    dxWidget *    prev_focused;
    dxWidget *    controls[6];

    int           mouse_down_x;
    int           mouse_down_y;

    void          InitControls();
};

extern dxMenu menu;

#endif /* !__MENU_H__ */
