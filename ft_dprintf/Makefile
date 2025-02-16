SRCS_FILE = ft_dprintf.c \
	    ft_ltoa.c \
	    ft_ptoa.c \
	    ft_putstr_fd.c \
	    ft_utoa.c \
	    ft_count_unsigned.c \
	    ft_putchar_fd.c \
	    ft_ultoa.c \
		ft_dtoa.c

SRCS = $(addprefix ./,$(SRCS_FILE))

OBJ = $(SRCS:.c=.o)

NAME = dprintf.a

%.o: %.c ./ft_printf.h
		@gcc -c -o $@ $< -Wall -Wextra -Werror

$(NAME): $(OBJ)
		@ar rc $@ $(OBJ)

clean:
		@rm -f $(OBJ)

fclean: clean
		@rm -f $(NAME)

all: $(NAME)

re: fclean all

.PHONY: all fclean clean re
