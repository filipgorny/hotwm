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

DEPS_WM = $(wildcard $(SRC_WM)/*.h) 
SRCS_WM = $(wildcard $(SRC_WM)/*.c) 

OBJS_WM = $(patsubst $(SRCS_WM)/%.h, $(OBJ)/%.o, $(SRCS_WM)) 


DEPS_PANEL = $(wildcard $(SRC_PANEL)/*.h) 
SRCS_PANEL = $(wildcard $(SRC_PANEL)/*.c) 

OBJS_PANEL = $(patsubst $(SRCS_PANEL)/%.h, $(OBJ)/%.o, $(SRCS_PANEL))


DEPS_SV = $(wildcard $(SRC_SV)/*.h) 
SRCS_SV = $(wildcard $(SRC_SV)/*.c) 

OBJS_SV = $(patsubst $(SRCS_SV)/%.h, $(OBJ)/%.o, $(SRCS_SV))

DEPS_TOOLKIT = $(wildcard $(SRC_TOOLKIT)/*.hpp)
SRCS_TOOLKIT = $(wildcard $(SRC_TOOLKIT)/*.cpp) 

OBJS_TOOLKIT = $(patsubst $(SRCS_TOOLKIT)/%.hpp, $(OBJ)/%.o, $(SRCS_TOOLKIT))


DEPS_SHARED = $(wildcard $(SRC_SHARED)/*.h)
SRCS_SHARED = $(wildcard $(SRC_SHARED)/*.c)

OBJS_SHARED = $(patsubst $(SRCS_SHARED)/%.h, $(OBJ)/%.o, $(SRCS_SHARED))


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

$(OBJ)/*.o: $(DEPS_WM) $(DEPS_PANEL) $(DEPS_SV) $(DEPS_SHARED) $(DEPS_TOOLKIT)
	$(GCC) $(ALL_CFLAGS) -c $< -o $@

wm: $(OBJS_WM)
	$(CCC) $(ALL_LDFLAGS) $(OBJS_WM) $(OBJS_SHARED) $(LDLIBS) -I$(SRC_SHARED) -I$(SRC_WM) -o $(BIN)/wm

panel: $(OBJS_PANEL) $(OBJS_TOOLKIT) $(OBJS_SHARED)
	$(GCC) $(ALL_LDFLAGS) $(OBJS_PANEL) $(OBJS_SHARED) $(OBJS_TOOLKIT) $(LDLIBS) -I$(SRC_SHARED) -I$(SRC_TOOLKIT) -I$(SRC_PANEL) -o $(BIN)/panel

session: $(OBJS_SV)
	$(CC) $(ALL_LDFLAGS) $(OBJS_SV) $(LDLIBS) -I$(SRC_SHARED) -I$(SRC_SV) -o $(BIN)/session

clean:
	rm -f bin/*
	rm -f obj/*

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/main
	rm -f $(DESTDIR)$(MANDIR)/man1/main.1

.PHONY: all install uninstall clean


