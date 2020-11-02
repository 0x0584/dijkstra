DEBUG	 ?= 0
TEST	 ?= 0

NAME     = graph

SRCDIR   = src
HEADIR   = include
OBJDIR   = .obj

CXX      = g++
CPPFLAGS = -Wall -Wextra -Wpedantic -I$(HEADIR) -std=c++1z

ifeq ($(DEBUG),1)
	CPPFLAGS += -ggdb -Og
else
	CPPFLAGS += -O3
endif

SRCS     := $(shell find $(SRCDIR) -name '*.cpp' -type f)
HEADS    := $(shell find $(HEADIR) -name '*.hpp' -type f)
OBJS     := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

define test
	unzip -j algs4-data.zip algs4-data/$(1) -d .
	./$(NAME) < $(1)
	rm -f $(1)
endef

all: $(NAME)
ifeq ($(TEST),1)
	wget -c 'https://algs4.cs.princeton.edu/code/algs4-data.zip'
	$(call test,tinyDG.txt)
	$(call test,mediumDG.txt)
#   $(call test,largeDG.txt)
endif

$(NAME): $(OBJS)
	$(CXX) $(CPPFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADS)
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

re: clean all

.PHONY: all clean
