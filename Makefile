CC=gcc
CFLAGS=-lusb

sixpair: sixpair.c
	$(CC) sixpair.c -o sixpair $(CFLAGS)
