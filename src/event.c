/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 14:19:35 by yrigny            #+#    #+#             */
/*   Updated: 2024/01/24 14:30:35 by yrigny           ###   ########.fr       */
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
	if (key == XK_KP_Subtract && e->precision >= 30)
		e->precision -= 10;
	if (key == XK_KP_Enter)
		e->rgb_range = (e->rgb_range + 16) % 256;
	e->render = 1;
	return (0);
}

void	offset_helper(int x, int y, int button, t_env *e)
{
	if (button == 5)
	{
		e->offset_cx += e->c_length * (ZOOM - 1)
			* ((double)(x - e->view_x) / LENGTH - 0.5);
		e->offset_cy += e->c_height * (ZOOM - 1)
			* (0.5 - (double)(y - e->view_y) / HEIGHT);
	}
	if (button == 4)
	{
		e->offset_cx -= e->c_length * ((ZOOM - 1) / ZOOM)
			* ((double)x / LENGTH - 0.5);
		e->offset_cy -= e->c_height * ((ZOOM - 1) / ZOOM)
			* (0.5 - (double)y / HEIGHT);
	}
}

int	mouse_event(int button, int x, int y, t_env *e)
{
	if (button && (e->pattern == 1 || e->pattern == 6))
		x -= 0.15 * LENGTH;
	if (button == 5)
	{
		e->zoom *= (double)ZOOM;
		e->precision += 1;
		e->c_length /= ZOOM;
		e->c_height /= ZOOM;
		offset_helper(x, y, button, e);
	}
	if (button == 4 && e->zoom >= ZOOM && e->precision > 20)
	{
		e->zoom /= (double)ZOOM;
		e->precision -= 1;
		e->c_length *= ZOOM;
		e->c_height *= ZOOM;
		offset_helper(x, y, button, e);
	}
	e->render = 1;
	return (0);
}
