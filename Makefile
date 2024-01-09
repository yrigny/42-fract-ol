CC		= cc

CFLAGS	= -Wall -Wextra -Werror

FT_DIR	= ./src/libft

MLX_DIR	= ./minilibx

SRC_DIR	= src/

OBJ_DIR	= obj/

INC_DIR	= ./inc

LIB_DIR	= /usr/lib/x86_64-linux-gnu

CFILES	= main.c

SRC		= $(addprefix $(SRC_DIR), $(CFILES))

OBJ		= $(SRC:.c=.o)

NAME	= fractol

LFLAGS 	= -L$(MLX_DIR) -lmlx -L$(LIB_DIR) -lXext -lX11 -lm -L$(FT_DIR) -lft

libs:
	make -C $(FT_DIR)
	make -C $(MLX_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(INC_DIR)/fractol.h
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC_DIR) -03 -c $< -o $@

$(NAME): libs $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LFLAGS)

all: $(NAME)

clean:
	make clean -C $(FT_DIR)
	make clean -C $(MLX_DIR)
	rm -rf $(OBJ_DIR)

fclean: clean
	make fclean -C $(FT_DIR)
	rm $(NAME)

re: fclean all

.PHONY : all clean fclean re
