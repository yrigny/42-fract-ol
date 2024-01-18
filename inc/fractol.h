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
# define HEIGHT 600
# define ZOOM 1.1
# define RED 0x00FF0000
# define GREEN 0x0000FF00
# define BLUE 0x000000FF
# define WHITE 0x00FFFFFF
# define BLACK 0x00000000
# define MAX_ITER 50

typedef struct	s_img {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_img;

typedef struct	s_env {
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
	int	py; // p for pixel
	double	cx;
	double	cy; // c for coordinate
	int	i; // iterations cost to diverge
	float	shade;
}		t_pos;

void	color_img(t_img *img, t_env e, int color);
void	julia(t_pos *p, t_env e);
void	mandel(t_pos *p, t_env e);
int	adjust_i(int i, int zn);
void	my_pixel_put(t_img *img, int x, int y, int color);
void    e_init(t_env *e);
int	mouse_event(int button, int x, int y, t_env *e);
int	key_event(int keysym, t_env *e);
int	win_close(t_env *e);

#endif
