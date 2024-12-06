NAME		=	minishell
EXECUTABLE	=	$PWD/$(NAME)

RED			=	$(shell tput bold setaf 1)
GREEN		=	$(shell tput setaf 2)
PURPLE		=	$(shell tput setaf 5)
BLUE		=	$(shell tput setaf 6)
LIGHTBLUE	=	$(shell tput -Txterm setaf 4)
BOLD		=	$(shell tput bold)
RESET		=	$(shell tput -Txterm sgr0)

CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -lreadline

include Files.mk

SRCS		= 	$(addsuffix .c,$(addprefix $(SRC_DIR)/,$(FILES)))
OBJS		= 	$(addsuffix .o,$(addprefix $(OBJ_DIR)/,$(FILES)))
OBJ_DIR		= 	obj

NB			=	$(shell echo $(SRCS) | wc -w)
NUMB2		=	0
NUMB3		=	0
PERCENT		=	0

all: $(NAME)
	@if [ $(shell echo $(NUMB3)) -eq 0 ]; then echo "$(BOLD)$(RED)Nothing to be made."; fi

$(NAME): $(OBJS)
	@make --no-print-directory -s -C libft
	@echo "$(BOLD)$(RED)Libft compiled$(RESET)"
	@if [ -f ./$(NAME) ]; then echo "$(BOLD)$(BLUE)Executable already exists.$(RESET)"; else echo "$(BOLD)$(BLUE)Created the executable : $(NAME)$(RESET)"; fi
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) libft/libft.a
	@echo "$(BOLD)$(PURPLE)Finished the compilation of the Makefile$(RESET)"
	@$(eval NUMB3=$(shell echo $$(($(NUMB3)+1))))

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(eval NUMB2=$(shell echo $$(($(NUMB2)+1))))
	$(eval PERCENT=$(shell awk "BEGIN { printf(\"%.1f\", $(NUMB2) * 100 / $(NB)) }"))
	@$(CC) $(CFLAGS) -c $< -o $@
	@if [ $(shell uname -a | grep arch | wc -l) -gt 0 ]; then echo -e "$(BOLD)$(PURPLE)[Percent : "$(PERCENT)%"] $(BOLD)$(GREEN) \t~Compiling $< : $(shell echo $@ | cut -d'/' -f 2)$(RESET)"; else echo "$(BOLD)$(PURPLE)[Percent : "$(PERCENT)%"] $(BOLD)$(GREEN) \t~Compiling $< : $(shell echo $@ | cut -d'/' -f 2)$(RESET)"; fi

clean:
	@make --no-print-directory -s clean -C libft
	@echo "$(BOLD)$(BLUE)Finished cleaning libft$(RESET)"
	@if [ $(shell find -name "*.o" | wc -l) -gt 0 ]; then echo "$(BOLD)$(RED)Removed all objs$(RESET)"; else echo "$(BOLD)$(PURPLE)All objs were already removed.$(RESET)"; fi
	@rm -rf $(OBJ_DIR)
	@echo "$(BOLD)$(BLUE)Finished cleaning all$(RESET)"

fclean: clean
	@make --no-print-directory -s fclean -C libft
	@echo "$(BOLD)$(LIGHTBLUE)Finished fcleaning libft$(RESET)"
	@if [ -f ./$(NAME) ]; then echo "$(BOLD)$(RED)Removed the executable : $(NAME).$(RESET)"; else echo "$(BOLD)$(PURPLE)The executable $(NAME) was already removed.$(RESET)"; fi
	@rm -f $(NAME)
	@echo "$(BOLD)$(LIGHTBLUE)Finished fcleaning all$(RESET)"

re: fclean all

.PHONY: all clean fclean re
