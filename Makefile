# Makefile for aded editor, updated 2020-04-24 22:45:53
PROGNAME = aded

CC = gcc
WARNINGS = -Wall -Wextra
CFLAGS = $(WARNINGS) -lncursesw -ggdb -O2 -g #-DALLOW_DEBUG

BINDIR = /usr/local/bin
SRCDIR = src

OBJ+=$(SRCDIR)/main.o
OBJ+=$(SRCDIR)/line.o
OBJ+=$(SRCDIR)/curses.o
OBJ+=$(SRCDIR)/version.o
OBJ+=$(SRCDIR)/display.o
OBJ+=$(SRCDIR)/buffer.o
OBJ+=$(SRCDIR)/input.o
OBJ+=$(SRCDIR)/file.o
OBJ+=$(SRCDIR)/debug.o
OBJ+=$(SRCDIR)/statusbar.o
OBJ+=$(SRCDIR)/cursor.o

all: $(PROGNAME)

$(PROGNAME): $(OBJ)
	$(CC) -o $(PROGNAME) $(OBJ) $(CFLAGS)


.PHONY: clean
clean:
	rm -f $(PROGNAME) $(SRCDIR)/*.o
	@echo "CLEAN COMPLETED!"

.PHONY: install
install: $(PROGNAME)
	cp $(PROGNAME) $(BINDIR)
	chmod 755 $(BINDIR)/$(PROGNAME)
	@echo "INSTALL COMPLETED!"

.PHONY: uninstall
uninstall: $(PROGNAME)
	rm $(BINDIR)/$(PROGNAME)
	@echo "UNINSTALL COMPLETED!"
