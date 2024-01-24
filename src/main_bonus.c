/* ************************************************************************** */
/*									      */
/*							  :::	   ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*						      +:+ +:+	      +:+     */
/*   By: yrigny <marvin@42.fr>			    +#+  +:+	   +#+	      */
/*						  +#+#+#+#+#+	+#+	      */
/*   Created: 2024/01/09 18:02:17 by yrigny	       #+#    #+#	      */
/*   Updated: 2024/01/24 14:19:45 by yrigny           ###   ########.fr       */
/*									      */
/* ************************************************************************** */

#include "../inc/fractol_bonus.h"

void	e_init(t_env *e)
{
	e->mlx = mlx_init();
	e->mlx_win = mlx_new_window(e->mlx, LENGTH, HEIGHT, "fractol");
	e->img.img = mlx_new_image(e->mlx, LENGTH, HEIGHT);
	e->img.addr = mlx_get_data_addr(e->img.img, &e->img.bits_per_pixel,
			&e->img.line_length, &e->img.endian);
	e->rgb_range = 256;
	e->view_x = 0;
	e->view_y = 0;
	e->zoom = 1.0;
	e->offset_cx = 0.0;
	e->offset_cy = 0.0;
	e->c_length = 4.0;
	e->c_height = (e->c_length / LENGTH) * HEIGHT;
	e->dyno = -1;
	e->c_real = 0;
	e->c_imagine = 0;
}

void	rainbow_helper(t_rgb *color, int pos)
{
	if (pos <= 255)
		color->r = 255;
	if (pos <= 255)
		color->g = pos;
	if (pos > 255 && pos <= 510)
		color->r = 255 - (pos - 255);
	if (pos > 255 && pos <= 510)
		color->g = 255;
	if (pos > 510 && pos <= 765)
		color->g = 255;
	if (pos > 510 && pos <= 765)
		color->b = pos - 510;
	if (pos > 765 && pos <= 1020)
		color->g = 255 - (pos - 765);
	if (pos > 765 && pos <= 1020)
		color->b = 255;
	if (pos > 1020 && pos <= 1275)
		color->r = pos - 1020;
	if (pos > 1020 && pos <= 1275)
		color->b = 255;
	if (pos > 1275)
		color->r = 255;
	if (pos > 1275)
		color->b = 255 - (pos - 1275);
}

void	get_color(t_pos *p, t_env e)
{
	t_rgb	color;
	int		pos;

	color.r = 0;
	color.g = 0;
	color.b = 0;
	pos = ((double)p->i / e.precision) * 1530;
	if (e.pattern == 5)
		p->i *= 3;
	if (e.colorbase == 1)
	{
		color.r = 255 - p->i;
		color.g = 214 - p->i;
		color.b = 10 + p->i;
	}
	if (e.colorbase == 2)
	{
		color.r = 255 - p->i;
		color.g = 230 - p->i;
		color.b = 255;
	}
	if (e.colorbase == 3)
		rainbow_helper(&color, pos);
	p->color = color.r * e.rgb_range * e.rgb_range + color.g * e.rgb_range
		+ color.b;
}

void	color_img(t_img *img, t_env e)
{
	t_pos	p;
	int		x;
	int		y;
	char	*dst;

	x = 0;
	y = 0;
	dst = NULL;
	while (y <= HEIGHT)
	{
		p.py = HEIGHT / 2 - (y - e.view_y);
		while (x <= LENGTH)
		{
			p.px = (x - e.view_x) - LENGTH / 2;
			apply_fractol(&p, e);
			get_color(&p, e);
			dst = img->addr + (y * img->line_length + x
					* (img->bits_per_pixel / 8));
			*(unsigned int *)dst = p.color;
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
	mlx_hook(e.mlx_win, 6, 1L << 6, mouse_pos, &e);
	mlx_hook(e.mlx_win, 4, 1L << 2, mouse_event, &e);
	mlx_hook(e.mlx_win, 2, 1L << 0, key_event, &e);
	mlx_loop(e.mlx);
}
