Compile = gcc
filename = knockmem

all:
	sudo $(Compile) $(filename).c -o /usr/bin/$(filename)
	sudo cp $(filename).8.gz /usr/share/man/man8

clean:
	sudo rm -f /usr/bin/$(filename) /usr/share/man/man8/$(filename).8.gz

