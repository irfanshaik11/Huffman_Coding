CC = gcc
CFLAGS = -g
RM = rm -f
RMF = rm -rf


default: all

all: main

main: main.c
	$(CC) $(CFLAGS) -o fileCompressor main.c

clean veryclean:
	$(RM) fileCompressor
	$(RM) HuffmanCodebook
	$(RM) dir_reader_filepaths.txt
	$(RMF) fileCompressor.dSYM
