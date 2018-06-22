
default: 
	gcc -o hilbert3D -std=c99 hilbert3D.c -lm

clean:
	rm hilbert3D