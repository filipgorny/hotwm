.POSIX:
ALL_WARNING = -Wall -Wextra -pedantic
ALL_LDFLAGS = -lxcb -lxcb-keysyms $(LDFLAGS)
ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS) -std=c99 $(ALL_WARNING)
PREFIX = /usr/local
LDLIBS = -lm
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man
DEPS = config.h draw.h bar.h wm.h gui.h window.h layout.h arg.h input.h client.h mouse.h notify.h
OBJS_WM = wm.o draw.o gui.o window.o client.o mouse.o notify.o
OBJS_BAR = bar.o draw.o gui.o window.o client.o mouse.o notify.o


all: wm bar

install: all
	mkdir -p $(DESTDIR)$(BINDIR)
	mkdir -p $(DESTDIR)$(MANDIR)/man1
	cp -f main $(DESTDIR)$(BINDIR)
	cp -f main.1 $(DESTDIR)$(MANDIR)/man1
	chmod 755 $(DESTDIR)$(BINDIR)/main
	chmod 644 $(DESTDIR)$(MANDIR)/man1/main.1

%.o: %.c $(DEPS)
	$(CC) $(ALL_CFLAGS) -c $< -o $@
	
wm: $(OBJS_WM)
	$(CC) $(ALL_LDFLAGS) $(OBJS_WM) $(LDLIBS) -o hotwm

bar: $(OBJS_BAR)
	$(CC) $(ALL_LDFLAGS) $(OBJS_BAR) $(LDLIBS) -o hotbar


clean:
	rm -f main *.o

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/main
	rm -f $(DESTDIR)$(MANDIR)/man1/main.1

.PHONY: all install uninstall clean


