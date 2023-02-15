CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
LFLAGS = `pkg-config --lib gmp`
TARGET = keygen decrypt encrypt
SOURCE = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SOURCE))
KEYGEN_OBJS = numtheory.o ss.o randstate.o
ENCRYPT_OBJS = numtheory.o ss.o randstate.o
DECRYPT_OBJS = numtheory.o ss.o randstate.o

all: $(TARGET)

#$(TARGET): $(OBJS)
#	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) `pkg-config --cflags --libs gmp`

keygen: $(KEYGEN_OBJS)
	$(CC) $(LFLAGS) -o keygen $(KEYGEN_OBJS)

encrypt: $(ENCRYPT_OBJS)
	$(CC) $(LFLAGS) -o encrypt $(ENCRYPT_OBJS)

decrypt: $(DECRYPT_OBJS)
	$(CC) $(LFLAGS) -o decrypt $(DECRYPT_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)

format:
	clang-format -i -style=file *.[ch]

