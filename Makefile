CC=gcc
CFLAGS= -std=c99 -Wall -lcurl

rhat: rhat.c
	$(CC) $(CFLAGS) rhat.c -o rhat
