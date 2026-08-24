CC = cc
CFLAGS = -Wall -Wextra -std=c11 -I./src
TARGET = rlp
SRC = src/main.c
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean
