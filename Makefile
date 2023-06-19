CC:=$(CC)
CFLAGS+=-O2 -g
LDFLAGS+=-lm

OS:=$(shell uname)

ifeq ($(OS),Linux)
   CFLAGS+=-std=c99 -march=native -pedantic -Wall -Werror -D_POSIX_C_SOURCE=200809L -Wnewline-eof
   LDFLAGS+=-lrt
endif

#OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
OBJS=\
     heap.o\
     graph.o\
     dijkstra.o

TARGET=main

OBJS_ALL=$(OBJS)
OBJS_ALL+=$(TARGET).o 

bin: $(TARGET)

time: $(TARGET_TIME)

$(OBJS_ALL): %.o: %.c
	$(CC) -c $< $(CFLAGS) $(CPPFLAGS) -o $@

$(TARGET): $(OBJS_ALL)
	$(CC) $(OBJS) $@.o $(LDFLAGS) -o $@ 

$(TARGET_TIME): $(OBJS_ALL)
	$(CC) $(OBJS) $@.o $(LDFLAGS) -o $@ 

clean:
	$(RM) $(OBJS_ALL) $(TARGET) $(TARGET_TIME)

