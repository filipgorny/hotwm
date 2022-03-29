#include "tasks.h"

Task*[] tasks_get_all() {
	xcb_get_property_reply_t** reply)
{
	xcb_get_property_cookie_t cookie;
	xcb_window_t* start;
	cookie = xcb_ewmh_get_client_list_stacking(properties->ewmh_conn,
			properties->screen_id);
	xcb_get_property_reply_t* = xcb_get_property_reply(properties->xcb_conn, cookie, NULL);
	start = xcb_get_property_value(*reply);
	*size = xcb_get_property_value_length(*reply) / (sizeof(xcb_window_t));
	return start;
}
