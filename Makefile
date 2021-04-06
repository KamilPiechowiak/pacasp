CC = mpic++
CFLAGS = -Wall -Wunused -Wextra -std=c++11 -O3 -I src
LIBS = `Magick++-config --cxxflags --cppflags --ldflags --libs`

SRCS_WITH_PREFIX = $(shell find src | egrep "\.cpp" | egrep -v gen.cpp)
SRCS = $(SRCS_WITH_PREFIX:src/%=%)
OBJS = $(addprefix obj/,$(SRCS:.cpp=.o))
MAIN = sp
GEN = gen

all: $(MAIN) $(GEN)
$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LIBS)
$(GEN) : obj/gen.o
	$(CC) $(CFLAGS) -o gen obj/gen.o -O3
obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $< $(LIBS)
clean:
	rm -f $(OBJS) $(MAIN)
	rm -f obj/gen.o
