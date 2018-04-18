CFLAGS = -Wall -g
CC = clang
OUT = dist

out:
	$(MAKE) -B impl1 impl2 impl3

impl%:
	$(CC) $(CFLAGS) -o $(OUT)/$@ $@/*.c

configure:
	mkdir -p $(OUT)

clean:
	rm $(OUT)/*
