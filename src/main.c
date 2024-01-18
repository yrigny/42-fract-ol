/* ************************************************************************** */
/*									      */
/*							  :::	   ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*						      +:+ +:+	      +:+     */
/*   By: yrigny <marvin@42.fr>			    +#+  +:+	   +#+	      */
/*						  +#+#+#+#+#+	+#+	      */
/*   Created: 2024/01/09 18:02:17 by yrigny	       #+#    #+#	      */
/*   Updated: 2024/01/18 12:01:14 by yrigny           ###   ########.fr       */
/*									      */
/* ************************************************************************** */

#include "../inc/fractol.h"

void	e_init(t_env *e)
{
	e->mlx = mlx_init();
	e->mlx_win = mlx_new_window(e->mlx, LENGTH, HEIGHT, "fractol");
	e->img.img = mlx_new_image(e->mlx, LENGTH, HEIGHT);
	e->img.addr = mlx_get_data_addr(e->img.img, &e->img.bits_per_pixel,
			&e->img.line_length, &e->img.endian);
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
	*(unsigned int *)dst = color;
}

int	adjust_i(int i, int zn)
{
	float	mu;

	mu = i + 1 - log(log(sqrt(zn))) / log(2);
	return (mu);
}

void	mandel(t_pos *p, t_env e)
{
	double	zx;
	double	zy;
	double	zx_tmp;

	zx = 0.0;
	zy = 0.0;
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
	double 	zx;
	double	zy;
	double	zx_tmp;

    p->cx = (double)p->px / (LENGTH / e.c_length) + e.offset_cx;
	p->cy = (double)p->py / (HEIGHT / e.c_height) + e.offset_cy;
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
	int		x;
	int		y;

	x = 0;
	y = 0;
	while (y <= HEIGHT)
	{
		p.py = HEIGHT / 2 - y;
		while (x <= LENGTH)
		{
			p.px = x - LENGTH / 2;
			// mandel(&p, e);
			julia(&p, e);
			if (x == 0 && y == 0)
                printf("p(%d,%d) c(%f,%f) plan c=%f*%f\n", p.px, p.py, p.cx, p.cy, e.c_length, e.c_height);
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

int	main(void)
{
	t_env	e;

	e_init(&e);
	color_img(&e.img, e, RED);
	mlx_put_image_to_window(e.mlx, e.mlx_win, e.img.img, 0, 0);
	mlx_hook(e.mlx_win, 17, 1L << 5, win_close, &e);
	mlx_hook(e.mlx_win, 4, 1L << 2, mouse_event, &e);
	mlx_hook(e.mlx_win, 2, 1L << 0, key_event, &e);
	mlx_loop(e.mlx);
}
