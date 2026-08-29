CC = cc
CFLAGS = -Wall -Wextra -std=c11 -I./src
TARGET = rlp
SRC = src/main.c \
      src/lexer.c \
      src/ast.c \
      src/parser.c \
      src/police.c \
      src/crulisp.c
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
	rm -f output.c

.PHONY: clean
