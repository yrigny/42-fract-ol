CC		= cc

CFLAGS	= -Wall -Wextra -Werror

FT_DIR	= ./src/libft

MLX_DIR	= minilibx

SRC_DIR	= src/

INC_DIR	= ./inc

X11_DIR	= /usr/lib

X11_INC	= /usr/include

CFILES	= main.c event.c fractol.c parser.c

SRC		= $(addprefix $(SRC_DIR), $(CFILES))

OBJ		= $(SRC:.c=.o)

NAME	= fractol

LFLAGS 	= -L$(FT_DIR) -lft -L$(MLX_DIR) -lmlx_Linux -L$(X11_DIR) -lX11 -lXext -lm

$(NAME): libs $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LFLAGS)

all: $(NAME)

libs:
	make -C $(FT_DIR)
	make -C $(MLX_DIR)

%.o: $(SRC_DIR)%.c $(INC_DIR)/fractol.h
	$(CC) $(CFLAGS) -I$(X11_INC) -I$(MLX_DIR) -c $< -o $@

clean:
	make clean -C $(FT_DIR)
	make clean -C $(MLX_DIR)
	rm $(OBJ)

fclean: clean
	make fclean -C $(FT_DIR)
	rm $(NAME)

re: fclean all

.PHONY : all clean fclean re
