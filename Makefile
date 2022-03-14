.POSIX:
ALL_WARNING = -Wall -Wextra -pedantic
ALL_LDFLAGS = -lxcb -lxcb-keysyms -llua $(LDFLAGS)
ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS) -std=c99 $(ALL_WARNING)
PREFIX = /usr/local
LDLIBS = -lm
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man

BIN = bin
OBJ = obj

SRC_WM = wm
SRC_BAR = bar
SRC_SHARED = shared
SRC_SV = sv 


DEPS_WM = $(wildcard $(SRC_WM)/*.h) $(wildcard $(SRC_SHARED)/*.h)
SRCS_WM = $(wildcard $(SRC_WM)/*.c) $(wildcard $(SRC_SHARED)/*.c)

OBJS_WM = $(patsubst $(SRCS_WM)/*.h, $(OBJ)/.o, $(SRCS_WM)) \
			$(patsubst $(SRCS_SHARED)/*.h, $(OBJ)/.o, $(SRCS_SHARED))


DEPS_BAR = $(wildcard $(SRC_BAR)/*.h) $(wildcard $(SRC_SHARED)/*.h)
SRCS_BAR = $(wildcard $(SRC_BAR)/*.c) $(wildcard $(SRC_SHARED)/*.c)

OBJS_BAR = $(patsubst $(SRCS_BAR)/*.h, $(OBJ)/.o, $(SRCS_BAR)) \
			$(patsubst $(SRCS_SHARED)/*.h, $(OBJ)/.o, $(SRCS_SHARED))


DEPS_SV = $(wildcard $(SRC_SV)/*.h) $(wildcard $(SRC_SHARED)/*.h)
SRCS_SV = $(wildcard $(SRC_SV)/*.c) $(wildcard $(SRC_SHARED)/*.c)

OBJS_SV = $(patsubst $(SRCS_SV)/*.h, $(OBJ)/.o, $(SRCS_SV)) \
			$(patsubst $(SRCS_SHARED)/*.h, $(OBJ)/.o, $(SRCS_SHARED))

INC = $(DEP)

all: hotwm hotbar hotsv

install: all
	mkdir -p $(DESTDIR)$(BINDIR)
	mkdir -p $(DESTDIR)$(MANDIR)/man1
	cp -f main $(DESTDIR)$(BINDIR)
	cp -f main.1 $(DESTDIR)$(MANDIR)/man1
	chmod 755 $(DESTDIR)$(BINDIR)/main
	chmod 644 $(DESTDIR)$(MANDIR)/man1/main.1

$(OBJ)/*.o: $(SRC_WM)/*.c $(SRC_BAR)/*.c $(SRC_SV)/*.c $(SRC_SHARED)/*.c $(DEPS_WM)/*.h $(DEPS_BAR)/*.h $(DEPS_SV)/*.h $(DEPS_SHARED)/*.h
	$(CC) $(ALL_CFLAGS) -c $< -o $@

hotwm: $(OBJS_WM)
	$(CC) $(ALL_LDFLAGS) $(OBJS_WM) $(LDLIBS) -I$(SRC_SHARED) -I$(SRC_WM) -o $(BIN)/hotwm

hotbar: $(OBJS_BAR)
	$(CC) $(ALL_LDFLAGS) $(OBJS_BAR) $(LDLIBS) -I$(SRC_SHARED) -I$(SRC_BAR) -o $(BIN)/hotbar

hotsv: $(OBJS_BAR)
	$(CC) $(ALL_LDFLAGS) $(OBJS_BAR) $(LDLIBS) -I$(SRC_SHARED) -I$(SRC_SV) -o $(BIN)/hotsv

clean:
	rm -f main *.o

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/main
	rm -f $(DESTDIR)$(MANDIR)/man1/main.1

.PHONY: all install uninstall clean


