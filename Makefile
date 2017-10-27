PROGNAME = abook
SRCS = $(PROGNAME).c
CFLAGS = -g -O2 -Wall
BINOWN	= root
BINGRP  = root
BINMODE	= 755
INSDIR = /usr/bin

all: $(SRCS)
	$(CC) $(CFLAGS) -o $(PROGNAME) $(PROGNAME).c

install: all
	if [ -n "$(PROGNAME)" ]; then \
		strip $(PROGNAME) ; \
		install -m $(BINMODE) -o $(BINOWN) -g $(BINGRP) $(PROGNAME) $(INSDIR) ; \
	fi

remove:
	cd $(INSDIR) ; rm -f $(PROGNAME)

clean:
	rm -f *.o *~ $(PROGNAME)
