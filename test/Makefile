NAME			:= tester
CC				:= c++
CFLAGS			:= -Wall -Wextra -g3 -std=c++98 -pedantic -fsanitize=address -DLOG_ENABLED=$(LOG_ENABLED)
################################################################################
OBJ_DIR			:= ../obj
INCL_DIR		:= ../include
SRCS			+= $(wildcard *.cpp)
OBJS			:= $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))
################################################################################

all: $(NAME)

$(NAME): $(OBJS)
	@printf "$(LIGHT_CYAN)$(BOLD)make$(RESET)   [$(LIGHT_GREEN)$(NAME)$(RESET)] : "
	$(CC) $(OBJS) $(CFLAGS) -I$(INCL_DIR) -o $(NAME)

$(OBJ_DIR)/%.o: $(notdir %.cpp)
	@$(CC) $(CFLAGS) -I$(INCL_DIR) -c $< -o $@
	@printf "$(LIGHT_CYAN)$(BOLD)make$(RESET)   [$(LIGHT_GREEN)$(NAME)$(RESET)] : "
	@printf "$(notdir $(basename $@)) created\n"

run: $(NAME)
	@./$(NAME)

clean:
	@rm -f $(NAME)

echo:
	@echo $(LOG_ENABLED)
	