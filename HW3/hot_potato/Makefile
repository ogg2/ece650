TARGETS=ringmaster player

all: $(TARGETS)
clean:
	rm -f $(TARGETS)

ringmaster: ringmaster.c
	g++ -g -o $@ $<

player: player.c
	g++ -g -o $@ $<