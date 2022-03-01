.POSIX:
ALL_WARNING = -Wall -Wextra -pedantic
ALL_LDFLAGS = -lxcb -lxcb-keysyms $(LDFLAGS)
ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS) -std=c99 $(ALL_WARNING)
PREFIX = /usr/local
LDLIBS = -lm
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man
OBJDIR = obj
SRCDIR = src
DISTDIR = dist
DEPS = config.h draw.h main.h gui.h window.h layout.h arg.h input.h client.h mouse.h
SRCS = main.c draw.c gui.c window.c client.c mouse.c
OBJS = main.o draw.o gui.o window.o client.o mouse.o

all: hotwm

install: all
	mkdir -p $(DESTDIR)$(BINDIR)
	mkdir -p $(DESTDIR)$(MANDIR)/man1
	cp -f main $(DESTDIR)$(BINDIR)
	cp -f main.1 $(DESTDIR)$(MANDIR)/man1
	chmod 755 $(DESTDIR)$(BINDIR)/main
	chmod 644 $(DESTDIR)$(MANDIR)/man1/main.1

$(addprefix $(OBJDIR)/,$(OBJS)): $(addprefix $(SRCDIR)/,$(SRCS) $(DEPS))
	$(CC) $(ALL_CFLAGS) -c $< -o $@
	
hotwm: $(OBJDIR)/$(OBJS)
	$(CC) $(ALL_LDFLAGS) $(OBJDIR)/$(OBJS) $(LDLIBS) -o $(DISTDIR)/hotwm

clean:
	rm -f dist/hotwm obj/*.o

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/hotwm
	rm -f $(DESTDIR)$(MANDIR)/man1/hotwm.1

.PHONY: all install uninstall clean


