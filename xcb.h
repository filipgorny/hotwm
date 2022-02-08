#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>

xcb_connection_t *conn;

xcb_keycode_t *xcb_get_keycodes(xcb_keysym_t keysym) {
	xcb_key_symbols_t *keysyms = xcb_key_symbols_alloc(conn);
	xcb_keycode_t *keycode;
	keycode = (!(keysyms) ? NULL : xcb_key_symbols_get_keycode(keysyms, keysym));
	xcb_key_symbols_free(keysyms);
	return keycode;
}

xcb_keysym_t xcb_get_keysym(xcb_keycode_t keycode) {
	xcb_key_symbols_t *keysyms = xcb_key_symbols_alloc(conn);
	xcb_keysym_t keysym;
	keysym = (!(keysyms) ? 0 : xcb_key_symbols_get_keysym(keysyms, keycode, 0));
	xcb_key_symbols_free(keysyms);
	return keysym;
}
