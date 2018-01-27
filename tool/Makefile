all: bin2c bin2js

bin2c: bin2c.c
	gcc -o bin2c bin2c.c

bin2js: bin2js.c
	gcc -o bin2js bin2js.c

.PHONY: clean

clean:
	rm bin2c bin2js
