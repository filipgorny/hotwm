wm.map_key("S-j", wm.next_client);
wm.map_key("S-k", wm.prev_client);
wm.map_key("S-s", function () wm.set_layout("stack") end);
wm.map_key("S-m", function () wm.set_layout("mono") end);

wm.spawn("/bin/st")

