NAME			:= webserv
CC				:= c++
CFLAGS			:= -Wall -Wextra -Werror -g -std=c++98 -pedantic

################################################################################

ifdef ADDRESS
CFLAGS			+= -fsanitize=address -g3
endif
ifdef NO_FLAGS
CFLAGS			:= 
endif

################################################################################

HEADERS 		:= 	include/Socket.hpp \
					include/HttpMessage.hpp \
					include/HttpRequest.hpp \
					include/HttpResponse.hpp

INCLUDE_FLAGS 	:= $(addprefix -I, $(sort $(dir $(HEADERS))))

SOURCES			:= 	src/Socket.cpp \
					src/HttpMessage.cpp \
					src/HttpRequest.cpp \
					src/HttpResponse.cpp

################################################################################

ifdef TESTS
CFLAGS			+= 
HEADERS			+= 	tests/catch2/catch.hpp
SOURCES			+= 	tests/catch2/catch-runner.cpp \
					tests/example_test.cpp \
					tests/another_example_test.cpp
else
SOURCES			+= 	src/main.cpp
endif

################################################################################

BOLD 			:= \e[1m
RESET 			:= \e[0m
LIGHT_GREEN 	:= \e[92m
LIGHT_CYAN 		:= \e[96m

OBJECTS_DIR		:= obj
OBJECTS			:= $(SOURCES:src/%.c=$(OBJECTS_DIR)/%.o)

################################################################################

all: $(NAME)

obj/%.o: src/%.c $(HEADERS)
	@mkdir -p $(@D)
	@printf "$(LIGHT_CYAN)$(BOLD)make$(RESET)   [$(LIGHT_GREEN)$(NAME)$(RESET)] : "
	@$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

$(NAME): $(OBJECTS)
	@$(CC) $(CFLAGS) $(OBJECTS) $(INCLUDE_FLAGS) -o $(NAME)
	@printf "$(LIGHT_CYAN)$(BOLD)make$(RESET)   [$(LIGHT_GREEN)$(NAME)$(RESET)] : "
	@printf "$(NAME) created\n"

test: $(NAME)
	./$(NAME)

clean:
	@printf "$(LIGHT_CYAN)$(BOLD)clean$(RESET)  [$(LIGHT_GREEN)$(NAME)$(RESET)] : "
	/bin/rm -rf obj

fclean: clean
	@printf "$(LIGHT_CYAN)$(BOLD)fclean$(RESET) [$(LIGHT_GREEN)$(NAME)$(RESET)] : "
	/bin/rm -rf $(NAME) $(NAME).dSYM

re: fclean all

.PHONY: all clean fclean re

################################################################################