.POSIX:
ALL_WARNING = -Wall -Wextra -pedantic
ALL_LDFLAGS = -g -lxcb -lxcb-keysyms -lxcb-ewmh -lxcb-randr -llua $(LDFLAGS)
ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS) -std=c99 $(ALL_WARNING)
PREFIX = /usr/local
LDLIBS = -lm
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man

BIN = bin
OBJ = obj

SRC_WM = wm
SRC_PANEL = panel
SRC_SV = sv
SRC_SHARED = shared
SRC_TOOLKIT = toolkit

DEPS_WM = $(wildcard $(SRC_WM)/*.h) $(wildcard $(SRC_SHARED)/*.h)
SRCS_WM = $(wildcard $(SRC_WM)/*.c) $(wildcard $(SRC_SHARED)/*.c)

OBJS_WM = $(patsubst $(SRCS_WM)/*.h, $(OBJ)/.o, $(SRCS_WM)) \
			$(patsubst $(SRCS_SHARED)/*.h, $(OBJ)/.o, $(SRCS_SHARED))


DEPS_PANEL = $(wildcard $(SRC_PANEL)/*.h) $(wildcard $(SRC_SHARED)/*.h) $(wildcard $(SRC_TOOLKIT)/*.hpp)
SRCS_PANEL = $(wildcard $(SRC_PANEL)/*.c) $(wildcard $(SRC_SHARED)/*.c) $(wildcard $(SRC_TOOLKIT)/*.cpp)

OBJS_PANEL = $(patsubst $(SRCS_PANEL)/*.h, $(OBJ)/.o, $(SRCS_PANEL)) \
			$(patsubst $(SRCS_SHARED)/*.h, $(OBJ)/.o, $(SRCS_SHARED)) \
			$(patsubst $(SRCS_TOOLKIT)/*.hpp, $(OBJ)/.o, $(SRCS_SHARED))


DEPS_SV = $(wildcard $(SRC_SV)/*.h) $(wildcard $(SRC_SHARED)/*.h)
SRCS_SV = $(wildcard $(SRC_SV)/*.c) $(wildcard $(SRC_SHARED)/*.c)

OBJS_SV = $(patsubst $(SRCS_SV)/*.h, $(OBJ)/.o, $(SRCS_SV)) \
			$(patsubst $(SRCS_SHARED)/*.h, $(OBJ)/.o, $(SRCS_SHARED))

INC = $(DEP)

CCC = gcc 
GCC = g++

all: wm panel session

install: all
	mkdir -p $(DESTDIR)$(BINDIR)
	mkdir -p $(DESTDIR)$(MANDIR)/man1
	cp -f main $(DESTDIR)$(BINDIR)
	cp -f main.1 $(DESTDIR)$(MANDIR)/man1
	chmod 755 $(DESTDIR)$(BINDIR)/main
	chmod 644 $(DESTDIR)$(MANDIR)/man1/main.1

$(OBJ)/*.o: $(SRC_WM)/*.c $(SRC_PANEL)/*.c $(SRC_SV)/*.c $(SRC_SHARED)/*.c $(DEPS_WM)/*.h $(DEPS_PANEL)/*.h $(DEPS_SV)/*.h $(DEPS_SHARED)/*.h
	$(CCC) $(ALL_CFLAGS) -c $< -o $@

wm: $(OBJS_WM)
	$(CCC) $(ALL_LDFLAGS) $(OBJS_WM) $(LDLIBS) -I$(SRC_SHARED) -I$(SRC_WM) -o $(BIN)/wm

panel: $(OBJS_PANEL)
	$(GCC) $(ALL_LDFLAGS) $(OBJS_PANEL) $(LDLIBS) -I$(SRC_SHARED) -I$(SRCS_TOOLKIT) -I$(SRC_PANEL) -o $(BIN)/panel

session: $(OBJS_SV)
	$(CC) $(ALL_LDFLAGS) $(OBJS_SV) $(LDLIBS) -I$(SRC_SHARED) -I$(SRC_SV) -o $(BIN)/session

clean:
	rm -f bin/*
	rm -f obj/*

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/main
	rm -f $(DESTDIR)$(MANDIR)/man1/main.1

.PHONY: all install uninstall clean


