/*==============================================================================
  BSPDemo
  Keys.h
================================================================================
*/

#ifndef __KEYS_H__
#define __KEYS_H__

/*
================================================================================
  key defines
================================================================================
*/

#define KEY_ESCAPE           VK_ESCAPE

#define KEY_F1               VK_F1
#define KEY_F2               VK_F2
#define KEY_F3               VK_F3
#define KEY_F4               VK_F4
#define KEY_F5               VK_F5
#define KEY_F6               VK_F6
#define KEY_F7               VK_F7
#define KEY_F8               VK_F8
#define KEY_F9               VK_F9
#define KEY_F10              VK_F10
#define KEY_F11              VK_F11
#define KEY_F12              VK_F12

#define KEY_NUMLOCK          VK_NUMLOCK

#define KEY_TILDE            VK_OEM_3      // `

#define KEY_0                0x30
#define KEY_1                0x31
#define KEY_2                0x32
#define KEY_3                0x33
#define KEY_4                0x34
#define KEY_5                0x35
#define KEY_6                0x36
#define KEY_7                0x37
#define KEY_8                0x38
#define KEY_9                0x39

#define KEY_A                0x41
#define KEY_B                0x42
#define KEY_C                0x43
#define KEY_D                0x44
#define KEY_E                0x45
#define KEY_F                0x46
#define KEY_G                0x47
#define KEY_H                0x48
#define KEY_I                0x49
#define KEY_J                0x4A
#define KEY_K                0x4B
#define KEY_L                0x4C
#define KEY_M                0x4D
#define KEY_N                0x4E
#define KEY_O                0x4F
#define KEY_P                0x50
#define KEY_Q                0x51
#define KEY_R                0x52
#define KEY_S                0x53
#define KEY_T                0x54
#define KEY_U                0x55
#define KEY_V                0x56
#define KEY_W                0x57
#define KEY_X                0x58
#define KEY_Y                0x59
#define KEY_Z                0x5A

#define KEY_NUMPAD_0         VK_NUMPAD0
#define KEY_NUMPAD_1         VK_NUMPAD1
#define KEY_NUMPAD_2         VK_NUMPAD2
#define KEY_NUMPAD_3         VK_NUMPAD3
#define KEY_NUMPAD_4         VK_NUMPAD4
#define KEY_NUMPAD_5         VK_NUMPAD5
#define KEY_NUMPAD_6         VK_NUMPAD6
#define KEY_NUMPAD_7         VK_NUMPAD7
#define KEY_NUMPAD_8         VK_NUMPAD8
#define KEY_NUMPAD_9         VK_NUMPAD9

#define KEY_NUMPAD_ADD       VK_ADD
#define KEY_NUMPAD_SUBTRACT  VK_SUBTRACT
#define KEY_NUMPAD_MULTIPLY  VK_MULTIPLY
#define KEY_NUMPAD_DIVIDE    VK_DIVIDE
#define KEY_NUMPAD_DECIMAL   VK_DECIMAL
#define KEY_NUMPAD_SEPARATOR VK_SEPARATOR

#define KEY_TAB              VK_TAB
#define KEY_CAPSLOCK         VK_CAPITAL
#define KEY_SHIFT            VK_SHIFT
#define KEY_CTRL             VK_CONTROL
#define KEY_ALT              VK_MENU
#define KEY_LWIN             VK_LWIN
#define KEY_RWIN             VK_RWIN

#define KEY_BACKSPACE        VK_BACK
#define KEY_SPACE            VK_SPACE
#define KEY_ENTER            VK_RETURN

#define KEY_MINUS            VK_OEM_MINUS
#define KEY_PLUS             VK_OEM_PLUS

#define KEY_SEMICOLON        VK_OEM_1      // ;
#define KEY_COMMA            VK_OEM_COMMA  // ,
#define KEY_PERIOD           VK_OEM_PERIOD // .

#define KEY_FORWARDSLASH     VK_OEM_2      // |
#define KEY_BACKSLASH        VK_OEM_5      // /

#define KEY_QUOTE            VK_OEM_7      // '

#define KEY_OPENBRACKET      VK_OEM_4      // [
#define KEY_CLOSEBRACKET     VK_OEM_6      // ]

#define KEY_INSERT           VK_INSERT
#define KEY_DELETE           VK_DELETE
#define KEY_HOME             VK_HOME
#define KEY_END              VK_END
#define KEY_PAGEUP           VK_PRIOR
#define KEY_PAGEDOWN         VK_NEXT

#define KEY_UP               VK_UP
#define KEY_DOWN             VK_DOWN
#define KEY_LEFT             VK_LEFT
#define KEY_RIGHT            VK_RIGHT

/*
========================================
  keys_s
========================================
*/
struct keys_s
{
    int moveforward;
    int movebackward;
    int moveleft;
    int moveright;
};

extern keys_s keys;

void Key_Init();

int  Key_InitMoveForwardKey(const char * keystr);
int  Key_InitMoveBackwardKey(const char * keystr);
int  Key_InitMoveLeftKey(const char * keystr);
int  Key_InitMoveRightKey(const char * keystr);

#endif /* !__KEYS_H__ */
