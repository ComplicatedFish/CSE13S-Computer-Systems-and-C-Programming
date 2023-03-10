CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
#LFLAGS = `pkg-config --cflags --libs gmp`
TARGET = encode decode
SOURCE = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SOURCE))
ENCODE_OBJS = trie.o word.o io.o endian.o
DECODE_OBJS = trie.o word.o io.o endian.o

all: $(TARGET)

#$(TARGET): $(OBJS)
#	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) `pkg-config --cflags --libs gmp`

encode: $(ENCODE_OBJS)
	$(CC) -o encode $(ENCODE_OBJS)

decode: $(DECODE_OBJS)
	$(CC) -o decode $(DECODE_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)

format:
	clang-format -i -style=file *.[ch]

