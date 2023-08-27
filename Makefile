CXX := clang++
CXXFLAGS := -std=c++17 -F/Library/Frameworks -Wall -Wextra -Werror
LDFLAGS := -F/Library/Frameworks/ -framework SDL2

srcdir := ./src
objdir := ./obj
src := $(wildcard $(srcdir)/*.cpp)
headers := $(wildcard $(srcdir)/*.h)
obj := $(patsubst $(srcdir)/%.cpp, $(objdir)/%.o, $(src))
dep := $(addsuffix .d, $(basename $(obj)))
bin := draw2d

.PHONY: all clean print

all: $(bin)

$(bin): $(obj)
	$(CXX) $(LDFLAGS) $^ -o $@

$(objdir)/%.o: $(srcdir)/%.cpp
	$(CXX) -c $(CXXFLAGS) -MMD $< -o $@

clean:
	rm -f $(obj) $(dep) $(bin)

print:
	@echo "src: $(src)"
	@echo "headers: $(headers)"
	@echo "obj: $(obj)"
	@echo "dep: $(dep)"
