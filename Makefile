CC = cc
#CC = mips-openwrt-linux-gcc

ROOTPATH=.
INCLUDE = -I.

OBJ += $(patsubst %.c, %.o, $(wildcard *.c))
OBJ += $(patsubst %.c, %.o, $(wildcard $(ROOTPATH)/*.c))

CFLAGS = -O0 -g3 -Wall
target = example

all:$(OBJ)
	$(CC) out/*.o -o $(target) $(LIB)
	mv $(target) out
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)
	mv $@ out
clean:
	rm -rf out/*
