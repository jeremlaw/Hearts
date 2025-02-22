# Source files
SRCS = playHearts.c card.c deck.c hand.c hearts.c animations.c

# build rule
play: $(SRCS)
	gcc -std=c11 $(SRCS) -o play

# clean rule
clean:
	rm -f play