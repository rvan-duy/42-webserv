NAME			:= tester
CC				:= c++
CFLAGS			:= -Wall -Wextra -I$(INCL_DIR) -g3 -std=c++98 -pedantic -fsanitize=address
################################################################################
OBJ_DIR			:= ../obj
INCL_DIR		:= ../include
SRCS			+= $(wildcard *.cpp)
OBJS			:= $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))
################################################################################

all: $(NAME)

$(NAME): $(OBJS)
	@printf "$(LIGHT_CYAN)$(BOLD)make$(RESET)   [$(LIGHT_GREEN)$(NAME)$(RESET)] : "
	$(CC) $(OBJS) $(CFLAGS) -o $(NAME)
	@./$(NAME)

$(OBJ_DIR)/%.o: $(notdir %.cpp)
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "$(LIGHT_CYAN)$(BOLD)make$(RESET)   [$(LIGHT_GREEN)$(NAME)$(RESET)] : "
	@printf "$(notdir $(basename $@)) created\n"

clean:
	@rm -f $(NAME)

echo:
	@echo $(OBJS)
	