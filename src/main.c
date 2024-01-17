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
#define ZOOM 1.1
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

void    e_init(t_env *e)
{
    e->zoom = 1.0;
    e->offset_cx = 0.0;
    e->offset_cy = 0.0;
    e->c_length = 4.8;
    e->c_height = 3.2;
}

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

void	mandel(t_pos *p, t_env e)
{
	double 	zx = 0.0;
	double	zy = 0.0;
	double	zx_tmp;

	// p->cx = (double)p->px / ((LENGTH / 4.8) * sqrt(zoom)) - 2.4 / sqrt(zoom);
	// p->cy = 1.35 / sqrt(zoom) - (double)p->py / ((HEIGHT / 2.7) * sqrt(zoom));
	p->cx = (double)p->px / (LENGTH / e.c_length) - 0.5 / e.zoom - e.offset_cx;
	p->cy = (double)p->py / (HEIGHT / e.c_height) - e.offset_cy;
	p->i = 0;
	while (p->i < MAX_ITER * sqrt(e.zoom))
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
	double	zx_tmp;

	p->cx = (double)p->px / (LENGTH / e.c_length) - e.offset_cx;
	p->cy = (double)p->py / (HEIGHT / e.c_height) - e.offset_cy;
	p->i = 0;
	while (p->i < MAX_ITER * sqrt(e.zoom))
	{
		zx_tmp = p->cx;
		p->cx = p->cx * p->cx - p->cy * p->cy - 0.4;
		p->cy = 2.0 * zx_tmp * p->cy + 0.6;
		if (p->cx * p->cx + p->cy * p->cy > 4.0)
		{
			p->shade = adjust_i(p->i, p->cx * p->cx + p->cy * p->cy);
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
			mandel(&p, e);
			// julia(&p, e);
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

int	win_close(t_env *e)
{
	mlx_destroy_image(e->mlx, e->img.img);
	mlx_destroy_window(e->mlx, e->mlx_win);
	mlx_destroy_display(e->mlx);
	free(e->mlx);
	exit(1);
	return (0);
}

int	key_event(int keysym, t_env *e)
{
	if (keysym == XK_Escape)
		win_close(e);
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
		e->zoom *= (double)ZOOM;
		e->c_length /= ZOOM;
		e->c_height /= ZOOM;
		e->offset_cx += e->c_length * (ZOOM - 1) * ((double)x / LENGTH - 0.5);
		e->offset_cy += e->c_height * (ZOOM - 1) * (0.5 - (double)y / HEIGHT);
		printf("zoom in %fx\n", ZOOM);
	}
	if (button == 4 && e->zoom >= ZOOM)
	{
		e->zoom /= (double)ZOOM;
		e->c_length *= ZOOM;
		e->c_height *= ZOOM;
		e->offset_cx -= e->c_length * ((ZOOM - 1) / ZOOM) * ((double)x / LENGTH - 0.5);
		e->offset_cy -= e->c_height * ((ZOOM - 1) / ZOOM) * (0.5 - (double)y / HEIGHT);
		printf("zoom out %fx\n", 1/ZOOM);
	}
	color_img(&e->img, *e, RED);
	mlx_put_image_to_window(e->mlx, e->mlx_win, e->img.img, 0, 0);
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
	e_init(&e);
	color_img(&e.img, e, RED);
	mlx_put_image_to_window(e.mlx, e.mlx_win, e.img.img, 0, 0);
	mlx_hook(e.mlx_win, 17, 1L<<5, win_close, &e);
	mlx_hook(e.mlx_win, 4, 1L<<2, mouse_scroll, &e);
	mlx_hook(e.mlx_win, 2, 1L<<0, key_event, &e);
	mlx_loop(e.mlx); // keeps the process alive
}

