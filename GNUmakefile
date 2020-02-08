PROG = rematrix

CXX = clang++

CXXFLAGS += -O2
CXXFLAGS += -std=c++17

LDLIBS += -lGL
LDLIBS += -lGLEW
LDLIBS += -lGLX
LDLIBS += -lX11

OBJS =	\
	main.o	\
	program.o	\
	rendering_context.o	\
	shader.o

.PHONY: clean

$(PROG): $(OBJS)
	$(CXX) $^ -o $@ $(LDLIBS)

clean:
	$(RM) $(PROG) $(OBJS)
