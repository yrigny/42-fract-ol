/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 18:02:17 by yrigny            #+#    #+#             */
/*   Updated: 2024/01/12 11:48:21 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <mlx.h>
#include <math.h>
#include <stdlib.h>
#include <X11/keysym.h>

#define RED 0x00FF0000
#define GREEN 0x0000FF00
#define BLUE 0x000000FF
#define WHITE 0x00FFFFFF
#define BLACK 0x00000000

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
	int		zoom;
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

void	mandel(t_pos *p, int zoom, int length, int height)
{
	double 	zx = 0.0;
	double	zy = 0.0;
	double	zx_tmp;

	p->cx = (double)p->px / (((length / 4.8) - 3.0) / sqrt(zoom));
	p->cy = 1.35 - (double)p->py / ((height / 2.7) / sqrt(zoom));
	p->div = 0;
	while (p->div < MAX_ITER)
	{
		zx_tmp = zx;
		zx = zx * zx - zy * zy + p->cx;
		zy = 2.0 * zx_tmp * zy + p->cy;
		if (zx * zx + zy * zy > 4.0)
			break ;
		p->div++;
	}
}

void	julia(t_pos *p, int length, int height)
{
	double 	zx;
	double	zy;
	double	zx_tmp;

	p->cx = (double)p->px / (length / 4.8) - 2.4;
	p->cy = 1.35 - (double)p->py / (height / 2.7);
	zx = p->cx;
	zy = p->cy;
	p->div = 0;
	while (p->div < MAX_ITER)
	{
		zx_tmp = zx;
		zx = zx * zx - zy * zy -0.8;
		zy = 2.0 * zx_tmp * zy + 0.156;
		if (zx * zx + zy * zy > 4.0)
			break ;
		p->div++;
	}
}

void	color_img(t_img *img, int zoom, int length, int height, int color)
{
	t_pos	p;

	p.px = 0;
	p.py = 0;

	while (p.py < height)
	{
		while (p.px < length)
		{
			mandel(&p, zoom, length, height);
			//julia(&p, length, height);
			//printf("p(%d, %d) c(%f, %f) div %d\n", p.px, p.py, p.cx, p.cy, p.div);
			if (p.div == MAX_ITER)
				my_pixel_put(img, p.px, p.py, BLACK);
			else
				my_pixel_put(img, p.px, p.py, color / (2 * p.div + 2));
			p.px++;
		}
		p.py++;
		p.px = 0;
	}
}

int	key_event(int keysym, t_env *e)
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

int	mouse_event(int button, int x, int y, t_env *e)
{
	if (button == 5)
	{
		e->zoom *= 4;
		printf("zoom in\n");
	}
	return (0);
}

int	win_close(t_env *e)
{
	mlx_destroy_image(e->mlx, e->img.img);
	mlx_destroy_window(e->mlx, e->mlx_win);
	mlx_destroy_display(e->mlx);
	free(e->mlx);
	exit(1);
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
	e.zoom = 0;
	color_img(&e.img, e.zoom, 1920, 1080, GREEN);
	mlx_put_image_to_window(e.mlx, e.mlx_win, e.img.img, 0, 0);
	mlx_hook(e.mlx_win, 17, 1L<<5, win_close, &e);
	mlx_hook(e.mlx_win, 4, 1L<<2, mouse_event, &e);
	mlx_hook(e.mlx_win, 2, 1L<<0, key_event, &e);
	//mlx_key_hook(e.mlx_win, key_event, &e);
	mlx_loop(e.mlx); // keeps the process alive
}

