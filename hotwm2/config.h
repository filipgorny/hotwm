#include <X11/X.h>
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
 { MODKEY|ShiftMask,XK_Return,          toggle,           {} }
};

