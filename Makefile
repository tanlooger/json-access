all: jsmn.c test.c
	gcc jsmn.c test.c

clean:
	rm -rf a.out *.gch
