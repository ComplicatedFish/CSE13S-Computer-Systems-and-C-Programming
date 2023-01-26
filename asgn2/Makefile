CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
TARGET = mathlib-test
SOURCE = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SOURCE))

#NOTE: This make file is almost entirely taken from TA Omar Ahmadyar's lesson
#during lab section

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

clean:
	rm -f *.o $(TARGET)


