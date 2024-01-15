/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 18:02:17 by yrigny            #+#    #+#             */
/*   Updated: 2024/01/15 15:24:24 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <mlx.h>
#include <math.h>
#include <stdlib.h>
#include <X11/keysym.h>

#define LENGTH 900
#define HEIGHT 600
#define ZOOM 2
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
	double	zoom;
	int		focus_x;
	int		focus_y;
	int		offset_x;
	int		offset_y;
}			t_env;

typedef struct 	s_pos {
	int	px;
	int	py; // p for pixel
	double	cx;
	double	cy; // c for coordinate
	int	i; // iterations cost to diverge
	float	shade;
}		t_pos;

void	my_pixel_put(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

int	adjust_i(int i, int zn)
{
	float	mu;

	mu = i + 1 - log(log(sqrt(zn))) / log(2);
	//printf("shade %f", mu);
	return (mu);
}

void	mandel(t_pos *p, double zoom)
{
	double 	zx = 0.0;
	double	zy = 0.0;
	double	zx_tmp;

	p->cx = (double)p->px / ((LENGTH / 4.8) * sqrt(zoom)) - 2.4 / sqrt(zoom);
	p->cy = 1.35 / sqrt(zoom) - (double)p->py / ((HEIGHT / 2.7) * sqrt(zoom));
	p->i = 0;
	while (p->i < MAX_ITER * sqrt(zoom))
	{
		zx_tmp = zx;
		zx = zx * zx - zy * zy + p->cx;
		zy = 2.0 * zx_tmp * zy + p->cy;
		if (zx * zx + zy * zy > 4.0)
			break ;
		p->i++;
	}
}

void	julia(t_pos *p, t_env e)
{
	double 	zx;
	double	zy;
	double	zx_tmp;
	double	c_length;
	double	c_height;

	c_length = 3.6 / sqrt(e.zoom);
	c_height = 2.4 / sqrt(e.zoom);
	p->cx = (double)p->px / (LENGTH / c_length);
	p->cy = (double)p->py / (HEIGHT / c_height);
	zx = p->cx;
	zy = p->cy;
	p->i = 0;
	while (p->i < MAX_ITER * sqrt(e.zoom))
	{
		zx_tmp = zx;
		zx = zx * zx - zy * zy - 0.4;
		zy = 2.0 * zx_tmp * zy + 0.6;
		if (zx * zx + zy * zy > 4.0)
		{
			p->shade = adjust_i(p->i, zx * zx + zy * zy);
			break ;
		}
		p->i++;
	}
}

void	color_img(t_img *img, t_env e, int color)
{
	t_pos	p;
	int		x = 0;
	int		y = 0;

	while (y <= HEIGHT)
	{
		p.py = HEIGHT / 2 - y;
		while (x <= LENGTH)
		{
			p.px = x - LENGTH / 2;
			//mandel(&p, zoom, length, height);
			julia(&p, e);
			//printf("p(%d, %d) c(%f, %f) i %d\n", p.px, p.py, p.cx, p.cy, p.i);
			if (p.i == MAX_ITER * sqrt(e.zoom))
				my_pixel_put(img, x, y, BLACK);
			else
				my_pixel_put(img, x, y, color / (2 * p.i + 2));
			x++;
		}
		y++;
		x = 0;
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
	else
		printf("The %d key has been pressed\n", keysym);
	return (0);
}

int	mouse_scroll(int button, int x, int y, t_env *e)
{
	if (button)
		printf("action %d at %d, %d\n", button, x, y);
	if (button == 5)
	{
		e->zoom *= ZOOM;
		printf("zoom in 1.6x\n");
		// e->offset_x = x - LENGTH / 2;
		// e->offset_y = HEIGHT / 2 - y;
		// e->focus_x = x;
		// e->focus_y = y;
		// printf("offset %d, %d\n", e->offset_x, e->offset_y);
		// printf("new focus at %d, %d\n", e->focus_x, e->focus_y);
	}
	if (button == 4 && e->zoom > 1)
	{
		e->zoom /= ZOOM;
		printf("zoom out 1.6x\n");
	}
	color_img(&e->img, *e, RED);
	mlx_put_image_to_window(e->mlx, e->mlx_win, e->img.img, 0, 0);
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
	e.mlx_win = mlx_new_window(e.mlx, LENGTH, HEIGHT, "fractol"); // malloc called
	e.img.img = mlx_new_image(e.mlx, LENGTH, HEIGHT);
	e.img.addr = mlx_get_data_addr(e.img.img, &e.img.bits_per_pixel, &e.img.line_length, &e.img.endian);
	e.zoom = 1;
	e.focus_x = LENGTH / 2;
	e.focus_y = HEIGHT / 2;
	e.offset_x = 0;
	e.offset_y = 0;
	color_img(&e.img, e, RED);
	mlx_put_image_to_window(e.mlx, e.mlx_win, e.img.img, 0, 0);
	mlx_hook(e.mlx_win, 17, 1L<<5, win_close, &e);
	mlx_hook(e.mlx_win, 4, 1L<<2, mouse_scroll, &e);
	mlx_hook(e.mlx_win, 2, 1L<<0, key_event, &e);
	//mlx_key_hook(e.mlx_win, key_event, &e);
	mlx_loop(e.mlx); // keeps the process alive
}

