# Project name
NAME = mybot

# Directories
SRC_DIR = srcs
INC_DIR = include
OUT_DIR = build
BIN_DIR = bin
LIB_DIR = libs/json/include

# Source files by directory
SRC_FILES = bot jenkins_utils dpp_utils config main

# Compile and Linker Flags
CXX = clang++
CXXFLAGS = -std=c++17 -g3 -I$(INC_DIR) -I$(LIB_DIR) -I/opt/homebrew/include -I/opt/homebrew/opt/llvm/include
LDFLAGS = -L/opt/homebrew/lib -ldpp -lcurl

# Source and Object files
SRCS = $(addprefix $(SRC_DIR)/, $(addsuffix .cpp, $(SRC_FILES)))
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OUT_DIR)/%.o)

# Rules
all: $(NAME)

clean:
	$(RM) -r $(OUT_DIR)

fclean: clean
	$(RM) -r $(BIN_DIR)
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

# Build rules
$(NAME): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(BIN_DIR)/$(NAME)

$(OBJS): $(OUT_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@