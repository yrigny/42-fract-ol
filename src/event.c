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

int	key_event(int keysym, t_env *e)
{
	if (keysym == XK_Escape)
		win_close(e);
	else
		ft_printf("The %d key has been pressed\n", keysym);
	return (0);
}

int	mouse_event(int button, int x, int y, t_env *e)
{
	if (button)
		ft_printf("action %d at %d, %d\n", button, x, y);
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
		printf("zoom out %fx\n", 1 / ZOOM);
	}
	color_img(&e->img, *e, RED);
	mlx_put_image_to_window(e->mlx, e->mlx_win, e->img.img, 0, 0);
	return (0);
}
