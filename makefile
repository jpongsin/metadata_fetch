CC=gcc
CFLAGS= -g -Wall -o

TARGET= metadata_fetch

${TARGET}: main.c
	${CC} ${CFLAGS} ${TARGET} main.c -lm `pkg-config --cflags --libs libavformat libavcodec libswscale libavutil`

.PHONY: clean
clean:
	-${RM} ${TARGET} *~
	
