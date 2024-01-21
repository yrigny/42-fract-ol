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

void	color_img(t_img *img, t_env e)
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
			apply_fractol(&p, e);
			if (x == 0 && y == 0)
                printf("p(%d,%d) c(%f,%f) plan c=%f*%f\n", p.px, p.py, p.cx, p.cy, e.c_length, e.c_height);
			if (p.i == e.precision * sqrt(e.zoom))
				my_pixel_put(img, x, y, BLACK);
			else
				my_pixel_put(img, x, y, e.colorbase / (2 * p.i + 2));
			x++;
		}
		y++;
		x = 0;
	}
}

int	main(int ac, char **av)
{
	t_env	e;

	which_pattern(ac, av, &e);
	e_init(&e);
	color_img(&e.img, e);
	mlx_put_image_to_window(e.mlx, e.mlx_win, e.img.img, 0, 0);
	mlx_hook(e.mlx_win, 17, 1L << 5, win_close, &e);
	mlx_hook(e.mlx_win, 4, 1L << 2, mouse_event, &e);
	mlx_hook(e.mlx_win, 2, 1L << 0, key_event, &e);
	mlx_loop(e.mlx);
}
