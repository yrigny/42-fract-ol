/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 18:02:17 by yrigny            #+#    #+#             */
/*   Updated: 2024/01/10 10:47:07 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <mlx.h>
#include <stdlib.h>
#include <X11/keysym.h>

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

void	my_pixel_put(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	color_img(t_img *img, int length, int height, int color)
{
	int x = 0;
	int y = 0;

	while (y <= height)
	{
		while (x <= length)
		{
			if (x % 10 == 0)
				my_pixel_put(img, x, y, color);
			x++;
		}
		y++;
		x = 0;
	}
}

int	handle_input(int keysym, t_env *e)
{
	if (keysym == XK_Escape)
	{
		mlx_destroy_image(e->mlx, e->img.img);
		mlx_destroy_window(e->mlx, e->mlx_win);
		mlx_destroy_display(e->mlx);
		free(e->mlx);
		exit(1);
	}
	// printf("The %d key has been pressed\n", keysym);
	return (0);
}

int	main(void)
{
	void	*mlx;
	t_img	img;
	void	*mlx_win;
	t_env	e;

	e.mlx = mlx_init(); // malloc called
	e.mlx_win = mlx_new_window(e.mlx, 1920, 1080, "Hello world!"); // malloc called
	e.img.img = mlx_new_image(e.mlx, 1920, 1080);
	e.img.addr = mlx_get_data_addr(e.img.img, &e.img.bits_per_pixel, &e.img.line_length, &e.img.endian);
	color_img(&e.img, 1920, 1080, 0x00FF0000);
//	my_pixel_put(&e.img, 150, 150, 0x00FF0000);
	mlx_put_image_to_window(e.mlx, e.mlx_win, e.img.img, 0, 0);
	mlx_key_hook(e.mlx_win, handle_input, &e);
	mlx_loop(e.mlx); // keeps the process alive
}

