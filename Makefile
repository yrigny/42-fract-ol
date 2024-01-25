CC		= cc

CFLAGS		= -Wall -Wextra -Werror

FT_DIR		= ./libft

MLX_DIR		= minilibx

SRC_DIR		= src/

INC_DIR		= ./inc

X11_DIR		= /usr/lib

X11_INC		= /usr/include

CFILES		= main.c event.c fractol.c parser.c

CFILES_B	= main_bonus.c event_bonus.c fractol_bonus.c parser_bonus.c

SRC		= $(addprefix $(SRC_DIR), $(CFILES))

SRC_B		= $(addprefix $(SRC_DIR), $(CFILES_B))

OBJ		= $(SRC:.c=.o)

OBJ_B		= $(SRC_B:.c=.o)

NAME		= fractol

NAME_B		= fractol_bonus

LFLAGS 		= -L$(FT_DIR) -lft -L$(MLX_DIR) -lmlx_Linux -L$(X11_DIR) -lX11 -lXext -lm

all: libs $(NAME)

bonus: libs $(NAME_B)

libs:
	make -C $(FT_DIR)
	make -C $(MLX_DIR)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LFLAGS)

$(NAME_B): $(OBJ_B)
	$(CC) $(OBJ_B) -o $(NAME_B) $(LFLAGS)

$(OBJ): $(SRC_DIR)%.o: $(SRC_DIR)%.c $(INC_DIR)/fractol.h
	$(CC) $(CFLAGS) -I$(X11_INC) -I$(MLX_DIR) -c $< -o $@

$(OBJ_B): $(SRC_DIR)%.o: $(SRC_DIR)%.c $(INC_DIR)/fractol_bonus.h
	$(CC) $(CFLAGS) -I$(X11_INC) -I$(MLX_DIR) -c $< -o $@

clean:
	make clean -C $(FT_DIR)
	make clean -C $(MLX_DIR)
	rm -f $(OBJ) $(OBJ_B)

fclean: clean
	make fclean -C $(FT_DIR)
	rm -f $(NAME) $(NAME_B)

re: fclean all

.PHONY : all libs bonus clean fclean re
