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

# include <stdio.h>
# include <math.h>
# include <mlx.h>
# include <stdlib.h>
# include <X11/keysym.h>

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
}			t_env;

int	handle_input(int keysym, t_env *e);
void	color_img(t_img *img, int length, int height, int color);
void	my_pixel_put(t_img *img, int x, int y, int color);

#endif
