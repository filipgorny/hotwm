typedef struct {
    int stack_width_percent;
    int gap_width;
    int border_width;
    int border_color_active;
    int border_color_inactive;
} Setting;

Setting* setting_create() {
    Setting* setting = malloc(sizeof(Setting));
    setting->stack_width_percent = STACK_WIDTH_PERCENT;
    setting->gap_width = GAP_WIDTH;
    setting->border_width = BORDER_WIDTH;
    setting->border_color_active = BORDER_COLOR_ACTIVE;
    setting->border_color_inactive = BORDER_COLOR_INACTIVE;

    return setting;
}
