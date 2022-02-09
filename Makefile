.POSIX:
ALL_WARNING = -Wall -Wextra -pedantic
ALL_LDFLAGS = -lxcb -lxcb-keysyms $(LDFLAGS)
ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS) -std=c99 $(ALL_WARNING)
PREFIX = /usr/local
LDLIBS = -lm
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man

all: hotwm
install: all
	mkdir -p $(DESTDIR)$(BINDIR)
	mkdir -p $(DESTDIR)$(MANDIR)/man1
	cp -f main $(DESTDIR)$(BINDIR)
	cp -f main.1 $(DESTDIR)$(MANDIR)/man1
	chmod 755 $(DESTDIR)$(BINDIR)/main
	chmod 644 $(DESTDIR)$(MANDIR)/man1/main.1
hotwm: main.o
	$(CC) $(ALL_LDFLAGS) -o hotwm main.o $(LDLIBS)
main.o: main.c manager.h keyboard.h config.h hot.h
clean:
	rm -f main *.o
uninstall:
	rm -f $(DESTDIR)$(BINDIR)/main
	rm -f $(DESTDIR)$(MANDIR)/man1/main.1
.PHONY: all install uninstall clean


