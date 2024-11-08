OBJECTS = linked_list.c markov_chain.c
CC = gcc

CCFLAGS = -Wall -Wextra -Wvla -std=c99
tweets: tweets_generator.c $(OBJECTS)
	$(CC) $(CCFLAGS) $^ -o tweets_generator
snake: snakes_and_ladders.c $(OBJECTS)
	$(CC) $(CCFLAGS) $^ -o snakes_and_ladders
