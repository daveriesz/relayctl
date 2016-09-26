
PROGRAM = relayctl

CSOURCES = relayctl.c
COBJECTS = $(CSOURCES:.c=.o)

CC       = arm-linux-gnueabihf-gcc
CFLAGS   = $(COPT) $(CDEF) $(CINC)
COPT     = -g
CDEF     = 
CINC     = 

LDFLAGS  = $(LDOPTS) $(LDDIRS) $(LDLIBS)
LDOPTS   = 
LDDIRS   = 
LDLIBS   = -lrt

all: $(PROGRAM)

$(PROGRAM): $(COBJECTS)
	$(CC) -o $@ $(COBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean: execlean objclean

execlean:
	$(RM) $(PROGRAM)

objclean:
	$(RM) $(COBJECTS)

run: $(PROGRAM)
	./$<
	./$< all-off
