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

#define RED 0x00FF0000
#define GREEN 0x0000FF00
#define BLUE 0x000000FF
#define WHITE 0x00FFFFFF

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

typedef struct 	s_pos {
	int	px;
	int	py; // p for pixel
	double	cx;
	double	cy; // c for coordinate
	int	div; // time cost to diverge
}		t_pos;

void	my_pixel_put(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	mandel(t_pos *p)
{
	double 	zx = 0;
	double	zy = 0;
	double	zx_tmp;

	while (p->div < 100)
	{
		zx_tmp = zx;
		zx = zx * zx - zy * zy + p->cx;
		zy = 2.0 * zx_tmp * zy + p->cy;
		if (zx * zx + zy * zy > 4)
			break;
		p->div++;
	}
}

void	color_img(t_img *img, int length, int height, int color)
{
	t_pos	p;

	p.px = 0;
	p.py = 0;
	p.div = 0;

	while (p.py < height)
	{
		while (p.px < length)
		{
			p.cx = (double)p.px / (length / 3) - 2;
			p.cy = 1 - (double)p.py / (height / 2);
			mandel(&p);
			//printf("p(%d, %d) c(%f, %f) div %d\n", p.px, p.py, p.cx, p.cy, p.div);
			if (p.div == 100)
				my_pixel_put(img, p.px, p.py, 0x000000);
			else
				my_pixel_put(img, p.px, p.py, color * p.div);
			p.px++;
		}
		p.py++;
		p.px = 0;
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
	color_img(&e.img, 1920, 1080, BLUE);
//	my_pixel_put(&e.img, 150, 150, RED);
	mlx_put_image_to_window(e.mlx, e.mlx_win, e.img.img, 0, 0);
	mlx_key_hook(e.mlx_win, handle_input, &e);
	mlx_loop(e.mlx); // keeps the process alive
}

