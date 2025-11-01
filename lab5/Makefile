CC = gcc
CFLAGS = -Wall

all: task1 task2 task3

task1: task1_pipe_sync.c
	$(CC) $(CFLAGS) task1_pipe_sync.c -o task1

task2: task2_pipe_chat.c
	$(CC) $(CFLAGS) task2_pipe_chat.c -o task2

task3: task3_fifo_chat.c
	$(CC) $(CFLAGS) task3_fifo_chat.c -o task3

run1:
	./task1

run2:
	./task2

run3:
	./task3

clean:
	rm -f task1 task2 task3 /tmp/fifo1 /tmp/fifo2

