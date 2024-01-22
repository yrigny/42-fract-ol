/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 18:02:55 by yrigny            #+#    #+#             */
/*   Updated: 2024/01/10 10:46:54 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H

# define FRACTOL_H
# include "../src/libft/libft.h"
# include "../minilibx/mlx.h"
# include <stdio.h>
# include <math.h>
# include <stdlib.h>
# include <X11/keysym.h>

# define LENGTH 900
# define HEIGHT 900
# define ZOOM 1.1
# define RED 0xFF0000
# define GREEN 0x00FF00
# define BLUE 0x0000FF
# define WHITE 0xFFFFFF
# define BLACK 0x0
# define JULIA_A_R -0.4
# define JULIA_A_I 0.6
# define JULIA_B_R 0.285
# define JULIA_B_I 0.01
# define JULIA_C_R 0.4
# define JULIA_C_I 0.4

typedef struct	s_img {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_img;

typedef struct	s_env {
	int		pattern;
	int		precision;
	int		colorbase;
	void	*mlx;
	void	*mlx_win;
	t_img	img;
	double	zoom;
	double	offset_cx;
	double	offset_cy;
	double	c_length;
	double	c_height;
}			t_env;

typedef struct 	s_pos {
	int	px;
	int	py;
	int	i;
	double	cx;
	double	cy;
	double	shade;
}		t_pos;

/* parser */
void    error_msg(char *error_arg);
void	more_options(int ac, char **av, int i, t_env *e);
void	which_pattern(int ac, char **av, t_env *e);

/* fractol */
void    choose_julia(t_env e, double *c_real, double *c_imagine);
void	julia(t_pos *p, t_env e);
void	mandel(t_pos *p, t_env e);
void	apply_fractol(t_pos *p, t_env e);

/* event */
int		mouse_event(int button, int x, int y, t_env *e);
int		key_event(int keysym, t_env *e);
int		win_close(t_env *e);

/* utils */
int		adjust_i(int i, int zn);
void	my_pixel_put(t_img *img, int x, int y, int color);
void    e_init(t_env *e);
void	color_img(t_img *img, t_env e);

#endif
