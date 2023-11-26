# Makefile for a simple web server using cpp-httplib

CXX = g++             # The compiler being used
CXXFLAGS = -std=c++11 -Wall -O2  # Compiler flags
LDFLAGS =             # Any specific linking flags
LDLIBS = -lpthread    # Libraries to link against

# Define the source files and the resulting executable
SRCS = server.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = webserver

# Rule for building the final executable
$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(EXEC) $(OBJS) $(LDLIBS)

# Rule for building object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Phony targets for cleaning and running
.PHONY: clean run

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC)

# Include dependencies
-include $(OBJS:.o=.d)

# Rule for building dependency files
%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM -MT $@ -o $*.d $<
