#CONFIGURATION

progname = gf
CC = clang
CFLAGS = -Wall -ansi -pedantic -g -DMATHLIB_STANDALONE
LDLIBS = -lm -lRmath

#TEMPLATE

objects := $(patsubst %.c,%.o,$(wildcard *.c))
test_objects := $(patsubst %.c,%.o,$(wildcard *_test.c))
prog_objects := $(filter-out $(test_objects),$(objects))

$(progname): $(prog_objects)


#Source file dependencies

%.d: %.c
	$(CC) $(CFLAGS) -MM $< | sed 's/$*\.o/$*.o $@/' > $@

-include $(objects:.o=.d)


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
	rm -f *.d *.o *_test
