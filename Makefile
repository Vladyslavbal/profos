# Makefile для обох завдань: інтеграл та rwlock

CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lpthread -lm

# Імена виконуваних файлів
INTEGRAL = integral
RWLOCK = task2_rwlock

all: $(INTEGRAL) $(RWLOCK)

$(INTEGRAL): integral.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(RWLOCK): task2_rwlock.c
	$(CC) $(CFLAGS) -o $@ $< -lpthread

clean:
	rm -f $(INTEGRAL) $(RWLOCK)

