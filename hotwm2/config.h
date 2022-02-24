#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>
#include <X11/Xutil.h>
#include "input.h"
#include "main.h"

static const int BORDER_COLOR_INACTIVE = 0x00aaaaaa;
static const int BORDER_COLOR_ACTIVE = 0x00ff0000;

static const int BORDER_WIDTH = 3;

static const int GAP_WIDTH = 15;

static const int STACK_WIDTH_PERCENT = 35;

static const int BAR_MARGIN = 48;

#define MODKEY Mod1Mask

static const Key KEYS[] = {
 { MODKEY,          XK_Return,          spawn,            {.v = "/bin/st"} },
 { MODKEY,          XK_j,               active_up,        {} },
 { MODKEY,          XK_k,               active_down,      {} },
 { MODKEY|ShiftMask,XK_Return,          toggle,           {} },
 { MODKEY,          XK_m,               set_layout,       {.layout = mono} },
 { MODKEY,          XK_s,               set_layout,       {.layout = stacked} }
};

//static const Button BUTTONS[] = {
 // { ClkClientWin,   MODKEY,              Button1,     toggle_float,          {0}}
//}
//

static const int TITLE_BAR_HEIGHT = 32;
static const int WINDOW_PADDING = 2;

static const int COLOR_TITLE_INACTIVE = 0x00aaaaaa;
static const int COLOR_TITLE_ACTIVE = 0x00ff0000;
static const int COLOR_BORDER_INACTIVE = 0x00222222;
static const int COLOR_BORDER_ACTIVE = 0x00555555;
static const int COLOR_TITLE_TEXT_INACTIVE = 0x00ffffff;
static const int COLOR_TITLE_TEXT_ACTIVE = 0x00ffffff;

