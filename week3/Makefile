NAME     = graph

SRCDIR   = src
HEADIR   = include
OBJDIR   = .obj

CXX      = g++
CPPFLAGS = -Wall -Wextra -Wpedantic -I$(HEADIR) -std=c++11

SRCS     := $(shell find $(SRCDIR) -name '*.cpp' -type f)
HEADS    := $(shell find $(HEADIR) -name '*.hpp' -type f)
OBJS     := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CPPFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADS)
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

re: clean all

.PHONY: all clean
