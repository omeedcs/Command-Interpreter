# Definitions

CC = gcc
CC_FLAGS = -fomit-frame-pointer -fno-asynchronous-unwind-tables -ggdb -Wall -Werror
CC_OPTIONS = -c
CC_SO_OPTIONS = -shared -fpic
CC_DL_OPTIONS = -rdynamic
OD = objdump
OD_FLAGS = -d -h -r -s -S -t 
RM = /bin/rm -f
LD = gcc
LIBS = -ldl

SRCS := ci.c handle_args.c interface.c lex.c parse.c eval.c print.c err_handler.c variable.c
OBJS := $(SRCS:%.c=%.o)

HDRS := ci.h node.h
TESTS := tests/test_simple.txt

# Generic rules

%.i: %.c
	${CC} ${CC_OPTIONS} -E ${CC_FLAGS} $<

%.s: %.c
	${CC} ${CC_OPTIONS} -S ${CC_FLAGS} $<

%.o: %.c
	${CC} ${CC_OPTIONS} ${CC_FLAGS} $<

# Targets

all: ci test clean

ci: ${OBJS} ${HDRS}
	${CC} ${CC_FLAGS} -o $@ ${OBJS}

test: ci
	chmod +x driver.sh
	./driver.sh ${TESTS}

clean:
	${RM} *.o *.so
