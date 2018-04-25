CFLAGS = -Wall -g -lm
CC = clang
OUT = dist
EXE = comporg-final

sources = src/base.c src/coder1.c src/coder2.c src/coder3.c

out:
	$(CC) $(CFLAGS) -o $(OUT)/$(EXE) $(sources)

err:
	$(CC) $(CFLAGS) -o $(OUT)/$(EXE) $(sources) -Wno-everything

configure:
	mkdir -p $(OUT)

clean:
	find $(OUT)
