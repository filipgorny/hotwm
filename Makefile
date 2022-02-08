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
	cp -f hotwm $(DESTDIR)$(BINDIR)
	cp -f hotwm.1 $(DESTDIR)$(MANDIR)/man1
	chmod 755 $(DESTDIR)$(BINDIR)/hotwm
	chmod 644 $(DESTDIR)$(MANDIR)/man1/hotwm.1
hotwm: hotwm.o
	$(CC) $(ALL_LDFLAGS) -o hotwm hotwm.o $(LDLIBS)
hotwm.o: hotwm.c manager.h keyboard.h
clean:
	rm -f hotwm *.o
uninstall:
	rm -f $(DESTDIR)$(BINDIR)/hotwm
	rm -f $(DESTDIR)$(MANDIR)/man1/hotwm.1
.PHONY: all install uninstall clean


