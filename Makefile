#CONFIGURATION

progname = main
CFLAGS = -Wall -DMATHLIB_STANDALONE
LDLIBS = -lm -lRmath

#TEMPLATE

sources := $(wildcard *.c)
objects := $(patsubst %.c,%.o,$(sources))
test_objects := $(patsubst %.c,%.o,$(wildcard *_test.c))
prog_objects := $(filter-out $(test_objects),$(objects))

$(progname): $(prog_objects)


#Source file dependencies

depend: $(sources)
	makedepend -- $(CFLAGS) -- -Y $^ 2> /dev/null


#Unit testing

test_binaries := $(patsubst %.c,%,$(wildcard *_test.c))

.PHONY: test
test: $(test_binaries)
	$(foreach test,$^,./$(test))

%_test: %_test.o $(filter-out $(progname).o,$(prog_objects))
	$(CC) $^ $(LDLIBS) -o $@


#Cleanup

.PHONY: clean
clean:
	rm -f *.d *.o *_test Makefile.bak


#Rules generated by makedepend
# DO NOT DELETE

main.o: simulation.h
pqueue.o: pqueue.h
pqueue_test.o: pqueue.h
simulation.o: pqueue.h simulation.h
