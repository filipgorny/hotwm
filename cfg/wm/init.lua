function hello()
		print('Hello World!')
end

wm.map_key("c", hello);

wm.map_key("S-d", wm.next_client);
wm.map_key("S-s", wm.prev_client);
