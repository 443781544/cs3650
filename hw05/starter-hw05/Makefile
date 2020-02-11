SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
HDRS := $(wildcard *.h)

tokens: $(OBJS)
	gcc -g -o tokens $(SRCS) $(HDRS)

$(OBJS): $(SRCS) $(HDRS)
	gcc -c -g -o $@ $<

clean:
	rm -rf tmp tokens *.o *.plist

.PHONY: all clean