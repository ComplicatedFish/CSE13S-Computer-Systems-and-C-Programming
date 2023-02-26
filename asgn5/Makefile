CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
LFLAGS = `pkg-config --cflags --libs gmp`
TARGET = keygen decrypt encrypt
SOURCE = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SOURCE))
KEYGEN_OBJS = numtheory.o ss.o randstate.o keygen.o
ENCRYPT_OBJS = numtheory.o ss.o randstate.o encrypt.o
DECRYPT_OBJS = numtheory.o ss.o randstate.o decrypt.o

all: $(TARGET)

#$(TARGET): $(OBJS)
#	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) `pkg-config --cflags --libs gmp`

keygen: $(KEYGEN_OBJS)
	$(CC) -o keygen $(KEYGEN_OBJS) $(LFLAGS)
#`pkg-config --cflags gmp`

encrypt: $(ENCRYPT_OBJS)
	$(CC) -o encrypt $(ENCRYPT_OBJS) $(LFLAGS)
#`pkg-config --cflags gmp`

decrypt: $(DECRYPT_OBJS)
	$(CC) -o decrypt $(DECRYPT_OBJS) $(LFLAGS)
#`pkg-config --cflags gmp`


%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)

format:
	clang-format -i -style=file *.[ch]

