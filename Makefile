CC = mpic++
CFLAGS = -Wall -Wunused -Wextra -g -std=c++11 -O3 -I src
LIBS = `Magick++-config --cxxflags --cppflags --ldflags --libs`

SRCS = sp.cpp general/rectangle.cpp general/imgSaver.cpp algorithms/shelf.cpp general/general.cpp algorithms/bounds.cpp algorithms/bottomLeft.cpp parallel/parallel.cpp parallel/tuner.cpp parallel/runner.cpp algorithms/recordable.cpp parallel/portfolio.cpp
OBJS = $(addprefix obj/,$(SRCS:.cpp=.o))
MAIN = sp
GEN = gen

all: $(MAIN) $(GEN)
$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)
$(GEN) : obj/gen.o
	$(CC) $(CFLAGS) -o gen obj/gen.o -O3
obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f $(OBJS) $(MAIN)
	rm -f obj/gen.o
