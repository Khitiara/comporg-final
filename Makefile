CFLAGS = -Wall -g -lm
CC = clang
OUT = dist
EXE = comporg-final

sources = src/base.c src/coder1.c src/coder2.c src/coder3.c

out:
	$(CC) $(CFLAGS) -o $(OUT)/$(EXE) $(sources)

# Silence all warnings and redirect stderr to stdout to make it easier to analyze errors
err:
	$(CC) $(CFLAGS) -o $(OUT)/$(EXE) $(sources) -Wno-everything 2>&1

configure:
	mkdir -p $(OUT)

clean:
	find $(OUT)
