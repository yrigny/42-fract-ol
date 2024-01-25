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

#ifndef FRACTOL_BONUS_H

# define FRACTOL_BONUS_H
# include "../libft/libft.h"
# include "../minilibx/mlx.h"
# include <math.h>
# include <stdlib.h>
# include <X11/keysym.h>

# define LENGTH 900
# define HEIGHT 900
# define ZOOM 1.1
# define JULIA_A_R -0.4
# define JULIA_A_I 0.6
# define JULIA_B_R 0.285
# define JULIA_B_I 0.01
# define JULIA_C_R 0.4
# define JULIA_C_I 0.4

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_img;

typedef struct s_env
{
	unsigned int	pattern;
	unsigned int	precision;
	unsigned int	colorbase;
	unsigned int	rgb_range;
	int				view_x;
	int				view_y;
	void			*mlx;
	void			*mlx_win;
	t_img			img;
	double			zoom;
	double			offset_cx;
	double			offset_cy;
	double			c_length;
	double			c_height;
	int				dyno;
	double			c_real;
	double			c_imagine;
}	t_env;

typedef struct s_pos
{
	int				px;
	int				py;
	unsigned int	i;
	double			cx;
	double			cy;
	unsigned int	color;
}	t_pos;

typedef struct s_rgb
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}	t_rgb;

/* parser */
void	error_msg(char *error_arg);
void	more_options(int ac, char **av, int i, t_env *e);
void	which_pattern(int ac, char **av, t_env *e);

/* fractol */
void	choose_julia(t_env *e, double *c_real, double *c_imagine);
void	julia(t_pos *p, t_env e);
void	mandel(t_pos *p, t_env e);
void	burning(t_pos *p, t_env e);
void	apply_fractol(t_pos *p, t_env e);

/* event */
void	offset_helper(int x, int y, int button, t_env *e);
int		mouse_event(int button, int x, int y, t_env *e);
int		mouse_pos(int x, int y, t_env *e);
int		key_event(int key, t_env *e);
int		win_close(t_env *e);

/* utils */
void	e_init(t_env *e);
void	rainbow_helper(t_rgb *color, int pos);
void	get_color(t_pos *p, t_env e);
int		color_img(t_env *e);

#endif
