bird: bird.cc
	g++ bird.cc -O2 -Wall -lcurses -o bird

clean:
	rm bird

.PHONY: clean
