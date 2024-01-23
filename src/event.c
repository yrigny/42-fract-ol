/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 14:19:35 by yrigny            #+#    #+#             */
/*   Updated: 2024/01/21 14:19:38 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/fractol.h"

int	win_close(t_env *e)
{
	mlx_destroy_image(e->mlx, e->img.img);
	mlx_destroy_window(e->mlx, e->mlx_win);
	mlx_destroy_display(e->mlx);
	free(e->mlx);
	exit(1);
	return (0);
}

int	key_event(int key, t_env *e)
{
	if (key == XK_Escape)
		win_close(e);
	if (key == XK_Up || key == XK_Down || key == XK_Left || key == XK_Right)
	{
		if (key == XK_Up)
			e->view_y -= 50;
		if (key == XK_Down)
			e->view_y += 50;
		if (key == XK_Left)
			e->view_x -= 50;
		if (key == XK_Right)
			e->view_x += 50;
	}
	if (key == XK_KP_Add && e->precision <= 500)
		e->precision += 10;
	if (key == XK_KP_Subtract && e->precision >= 10)
		e->precision -= 10;
	if (key == XK_KP_Enter)
		e->colorbase /= 10;
	printf("colorbase %x\n", e->colorbase);
	color_img(&e->img, *e);
	mlx_put_image_to_window(e->mlx, e->mlx_win, e->img.img, 0, 0);
	return (0);
}

int	mouse_event(int button, int x, int y, t_env *e)
{
	if (button)
		ft_printf("max iteration: %d\n", e->precision);
	if (button && (e->pattern == 1 || e->pattern == 6))
		x -= 0.15 * LENGTH;
	if (button == 5)
	{
		e->zoom *= (double)ZOOM;
		e->precision += 1;
		e->c_length /= ZOOM;
		e->c_height /= ZOOM;
		e->offset_cx += e->c_length * (ZOOM - 1) * ((double)x / LENGTH - 0.5);
		e->offset_cy += e->c_height * (ZOOM - 1) * (0.5 - (double)y / HEIGHT);
	}
	if (button == 4 && e->zoom >= ZOOM)
	{
		e->zoom /= (double)ZOOM;
		e->precision -= 1;
		e->c_length *= ZOOM;
		e->c_height *= ZOOM;
		e->offset_cx -= e->c_length * ((ZOOM - 1) / ZOOM) * ((double)x / LENGTH - 0.5);
		e->offset_cy -= e->c_height * ((ZOOM - 1) / ZOOM) * (0.5 - (double)y / HEIGHT);
	}
	color_img(&e->img, *e);
	mlx_put_image_to_window(e->mlx, e->mlx_win, e->img.img, 0, 0);
	return (0);
}
