CXX := clang++
sdlf := -framework SDL2
sdlh := -I /Library/Frameworks/SDL2.framework/Headers
CXXFLAGS := -std=c++17 -Wall $(sdlh)
LDFLAGS := $(sdlf) -F /Library/Frameworks/

# Declare names that indicate recipes, not files 
.PHONY: all clean vars

srcdir := ./src
objdir := ./obj
src := $(wildcard $(srcdir)/*.cpp)
headers := $(patsubst %.cpp, %.h, $(filter-out $(srcdir)/main.cpp, $(src)))
obj := $(patsubst $(srcdir)/%.cpp, $(objdir)/%.o, $(src))
dep := $(addsuffix .d, $(basename $(obj)))
bin := draw2d

all: $(bin)

$(bin): $(obj)
	$(CXX) $(LDFLAGS) $^ -o $@

# Generic object file creation rule
$(objdir)/%.o: $(srcdir)/%.cpp
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

clean:
	rm -f $(obj) $(dep) $(bin)

vars:
	@echo "src: $(src)"
	@echo "headers: $(headers)"
	@echo "obj: $(obj)"
	@echo "dep: $(dep)"