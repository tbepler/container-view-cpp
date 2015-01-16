
########################
# Initialize directory constants
########################

INCLUDE=.

TEST_DIR=test
GTEST_DIR=$(TEST_DIR)/gtest
GTEST_INCLUDE = $(GTEST_DIR) $(GTEST_DIR)/include

BIN=bin

########################
# Find source files
########################

#find the header files not in the test directory
HEADERS := $(shell find . -type f -name "*.h" )


########################
# Test settings
########################

#find the header files in the test directory
TEST_HEADERS := $(shell find $(TEST_DIR) -type f -name "*.h" )

#find the src files in the test directory
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS=$(addprefix $(BIN)/,$(TEST_SRCS:.cpp=.o))

TEST_MAIN=$(TEST_DIR)run_tests.cpp
TEST_EXE=$(BIN)/run_test.out

GTEST_SRCS=$(GTEST_DIR)/src/gtest-all.cc
GTEST_OBJS=$(addprefix $(BIN)/,$(GTEST_SRCS:.cc=.o))

########################
# Compiler settings
########################
CXX=g++
CXXFLAGS= -Wall -Wextra -pthread -std=c++11
INCL= $(foreach incl,$(INCLUDE),-I$(incl))
INCL_GTEST= $(foreach incl,$(GTEST_INCLUDE),-I$(incl))

########################
# Build targets
########################

.PHONY: test runtest clean depend

test: $(TEST_EXE)

runtest: $(TEST_EXE)
	./$(TEST_EXE)

clean:
	rm -f ./.depend
	rm -r $(BIN)

$(TEST_EXE): $(TEST_OBJS) $(GTEST_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $(TEST_EXE)

$(BIN)/$(GTEST_DIR)/src/%.o: $(GTEST_DIR)/src/%.cc
	mkdir -p $(@D)
	$(CXX) $(INCL_GTEST) $(CXXFLAGS) -c $< -o $@

$(BIN)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp %.o
	mkdir -p $(@D)
	$(CXX) $(INCL) $(INCL_GTEST) $(CXXFLAGS) -c $< -o $@

depend: .depend

.depend: $(TEST_SRCS)
	rm -f ./.depend
	$(CXX) $(INCL) $(INCL_GTEST) -MM $^ > ./.depend

sinclude .depend
