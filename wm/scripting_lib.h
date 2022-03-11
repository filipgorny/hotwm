int _wm_map_key(lua_State *L) {
	int function = luaL_ref(L, LUA_REGISTRYINDEX);
	char* key = luaL_checkstring(L, 1);
	printf("Function: %d, key: %s\n", function, key);

	KeySymbol keysymbol = scripting_get_keysymbol(key);

	scripting_register_keybind(current_scripting_engine, keysymbol, function);

	printf("registered");
	return 0;
}
