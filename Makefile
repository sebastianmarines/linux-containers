OBJS	=
SOURCE	= src/main.c
OUT	= container
CC	 = gcc
FLAGS	 =
LFLAGS	 =

all: $(SOURCE)
	$(CC) $(SOURCE) -o $(OUT) $(LFLAGS)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OBJS) $(OUT)
