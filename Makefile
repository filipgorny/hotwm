.POSIX:
ALL_WARNING = -Wall -Wextra -pedantic
ALL_LDFLAGS = -lxcb -lxcb-keysyms $(LDFLAGS)
ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS) -std=c99 $(ALL_WARNING)
PREFIX = /usr/local
LDLIBS = -lm
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man

BIN = bin
OBJ = obj

SRC_WM = wm
SRC_SHARED = shared


DEPS_WM = $(wildcard $(SRC_WM)/*.h) $(wildcard $(SRC_SHARED)/*.h)
SRCS_WM = $(wildcard $(SRC_WM)/*.c) $(wildcard $(SRC_SHARED)/*.c)

OBJS_WM = $(patsubst $(SRCS_WM)/*.h, $(OBJ)/.o, $(SRCS_WM)) \
			$(patsubst $(SRCS_SHARED)/*.h, $(OBJ)/.o, $(SRCS_SHARED))

INC = $(DEP)

all: $(BIN)/hotwm 

install: all
	mkdir -p $(DESTDIR)$(BINDIR)
	mkdir -p $(DESTDIR)$(MANDIR)/man1
	cp -f main $(DESTDIR)$(BINDIR)
	cp -f main.1 $(DESTDIR)$(MANDIR)/man1
	chmod 755 $(DESTDIR)$(BINDIR)/main
	chmod 644 $(DESTDIR)$(MANDIR)/man1/main.1

$(OBJ)/*.o: $(SRC_WM)/*.c $(SRC_SHARED)/*.c $(DEPS_WM)/*.h $(DEPS_SHARED)/*.h
	$(CC) $(ALL_CFLAGS) -c $< -o $@
	
$(BIN)/hotwm: $(OBJS_WM)
	$(CC) $(ALL_LDFLAGS) $(OBJS_WM) $(LDLIBS) -I$(SRC_SHARED) -I$(SRC_WM) -o $(BIN)/hotwm


clean:
	rm -f main *.o

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/main
	rm -f $(DESTDIR)$(MANDIR)/man1/main.1

.PHONY: all install uninstall clean


