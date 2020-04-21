# Makefile for aded editor, updated 2020-04-21 20:11:59
PROGNAME = editor

CC = gcc
WARNINGS = -Wall -Wextra
CFLAGS = $(WARNINGS) -lncursesw -g

BINDIR = /usr/bin
SRCDIR = src

OBJ+=$(SRCDIR)/main.o
OBJ+=$(SRCDIR)/init.o

all: $(PROGNAME)

$(PROGNAME): $(OBJ)
	$(CC) -o $(PROGNAME) $(OBJ) $(CFLAGS)

clean:
	rm -f $(PROGNAME) $(SRCDIR)/*.o
	@echo "CLEAN COMPLITED!"

install: $(PROGNAME)
	cp $(PROGNAME) $(BINDIR)
	chmod 755 $(BINDIR)/$(PROGNAME)
	@echo "INSTALL COMPLITED!"

remove: $(PROGNAME)
	rm $(BINDIR)/$(PROGNAME)
	@echo "REMOVE COMPLITED!"
