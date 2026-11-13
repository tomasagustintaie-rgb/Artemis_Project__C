CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm
TARGET = artemis_sim
SRCS = main.c artemis.c validaciones.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe

.PHONY: all clean