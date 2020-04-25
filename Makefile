# Makefile for aded editor, updated 2020-04-24 22:45:53
PROGNAME = editor

CC = gcc
WARNINGS = -Wall -Wextra
CFLAGS = $(WARNINGS) -lncursesw -ggdb

BINDIR = /usr/local/bin
SRCDIR = src

OBJ+=$(SRCDIR)/main.o
OBJ+=$(SRCDIR)/init.o
OBJ+=$(SRCDIR)/line.o
OBJ+=$(SRCDIR)/curses.o
OBJ+=$(SRCDIR)/print.o
OBJ+=$(SRCDIR)/display.o
OBJ+=$(SRCDIR)/buffer.o
OBJ+=$(SRCDIR)/input.o
OBJ+=$(SRCDIR)/file.o

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
