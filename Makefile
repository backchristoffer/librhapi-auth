CC = gcc
CFLAGS = $(shell pkg-config --cflags libcurl json-c)
LIBS = $(shell pkg-config --libs libcurl json-c)
TARGET = rhapi_test

all: $(TARGET)

$(TARGET): test.o rhapi_auth.o
	$(CC) -o $(TARGET) test.o rhapi_auth.o $(LIBS)

test.o: test.c rhapi_auth.h
	$(CC) $(CFLAGS) -c test.c

rhapi_auth.o: rhapi_auth.c rhapi_auth.h
	$(CC) $(CFLAGS) -c rhapi_auth.c

clean:
	rm -f *.o $(TARGET)
