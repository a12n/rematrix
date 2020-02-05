PROG = rematrix

CXX = clang++

CXXFLAGS += -O2
CXXFLAGS += -std=c++17

LDLIBS += -lGL
LDLIBS += -lGLEW
LDLIBS += -lGLX
LDLIBS += -lX11

.PHONY: clean

$(PROG): $(PROG).cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDLIBS)

clean:
	$(RM) $(PROG)
