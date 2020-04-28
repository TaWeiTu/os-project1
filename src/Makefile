CC = gcc
CFLAGS = -O3 # -DDEBUG

all: main job

main: main.c *.h
	$(CC) $(CFLAGS) main.c -o main

job: job.c unit.h
	$(CC) $(CFLAGS) job.c -o job

.PHONY: clean
clean:
	$(RM) main job
