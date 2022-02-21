static const int BORDER_COLOR_INACTIVE = 0x00aaaaaa;
static const int BORDER_COLOR_ACTIVE = 0x00ffffff;

static const int BORDER_WIDTH = 3;

static const int GAP_WIDTH = 15;

static const int STACK_WIDTH_PERCENT = 35;

#define MODKEY Mod1Mask

static const Key KEYS[] = {
 { MODKEY,          XK_Return,          spawn,            {.v = "/bin/st"} }
};

