DEPS= common.h
CFLAGS = -Wall -c
all: broker suscriber publisher
broker: broker.o common.o $(DEPS)
	gcc -o $@ broker.o common.o -lpthread $(DFLAGS)
publisher: publisher.o common.o $(DEPS)
	gcc -o $@  publisher.o common.o -lpthread $(DFLAGS)
	
suscriber: suscriber.o common.o $(DEPS)
	gcc -o $@  suscriber.o common.o   $(DFLAGS)

%.o: %.c $(DEPS)
	gcc $(CFLAGS) -o $@ $< -I. $(DFLAGS)





.PHONY: debug
debug: DFLAGS= -g
debug: clean shell
.PHONY: clean


clean:
	rm -rf broker suscriber publisher *.o
