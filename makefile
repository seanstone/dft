all: dft.c
	mkdir -p bin
	gcc dft.c -o bin/dft -lm

clean:
	rm -rf bin/
