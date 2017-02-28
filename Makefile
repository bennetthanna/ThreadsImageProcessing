all: threads.c
	gcc -Wall -o a.out threads.c -pthread

clean:
	$(RM) a.out