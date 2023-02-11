CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
TARGET = life
SOURCE = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SOURCE))

#NOTE: This make file is almost entirely based upon from TA Omar Ahmadyar's 
#section during week 4

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) -lncurses

clean:
	rm -f *.o $(TARGET)

format:
	clang-format -i -style=file *.[ch]
