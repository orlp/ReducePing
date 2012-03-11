CC=gcc
CFLAGS=-Os -Wall -Wno-strict-aliasing -Wno-unused-but-set-variable
LIBS=-lwinmm -luser32 -lkernel32 -ladvapi32 -lcomctl32 -lcomdlg32 -lgdi32 -s -Wl,--subsystem,windows

.PHONY: reduceping.exe

all: reduceping.exe

ReducePing.exe: reduceping.c reduceping.res
	${CC} ${CFLAGS} -c -o reduceping.o reduceping.c
	${CC} -o ReducePing.exe reduceping.o reduceping.res ${LIBS}

reduceping.res: reduceping.rc
	windres reduceping.rc -O coff -o reduceping.res

test: ReducePing.exe
	ReducePing.exe

clean:
	del /S /Q *.o
	del /S /Q *.exe
	del /S /Q *.res
