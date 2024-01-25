/* ************************************************************************** */
/*									      */
/*							  :::	   ::::::::   */
/*   fractol_bonus.c                                    :+:      :+:    :+:   */
/*						      +:+ +:+	      +:+     */
/*   By: yrigny <marvin@42.fr>			    +#+  +:+	   +#+	      */
/*						  +#+#+#+#+#+	+#+	      */
/*   Created: 2024/01/21 14:18:50 by yrigny	       #+#    #+#	      */
/*   Updated: 2024/01/25 11:08:55 by yrigny           ###   ########.fr       */
/*									      */
/* ************************************************************************** */

#include "../inc/fractol_bonus.h"

void	mandel(t_pos *p, t_env e)
{
	double	zx;
	double	zy;
	double	zx_tmp;

	zx = 0.0;
	zy = 0.0;
	p->cx = (double)p->px / (LENGTH / e.c_length) + e.offset_cx;
	p->cy = (double)p->py / (HEIGHT / e.c_height) + e.offset_cy;
	p->i = 0;
	if ((p->cx + 0.25) * (p->cx + 0.25) + p->cy * p->cy <= 0.25)
		p->i = e.precision - 1;
	if ((p->cx + 1) * (p->cx + 1) + p->cy * p->cy <= 0.0625)
		p->i = e.precision - 1;
	while (++p->i < e.precision)
	{
		zx_tmp = zx;
		zx = zx * zx - zy * zy + p->cx;
		zy = 2.0 * zx_tmp * zy + p->cy;
		if (zx * zx + zy * zy >= 4.0)
			break ;
	}
}

void	choose_julia(t_env *e, double *c_real, double *c_imagine)
{
	if (e->pattern == 2 || e->pattern == 3)
	{
		*c_real = JULIA_A_R;
		*c_imagine = JULIA_A_I;
	}
	if (e->pattern == 4)
	{
		*c_real = JULIA_B_R;
		*c_imagine = JULIA_B_I;
	}
	if (e->pattern == 5)
	{
		*c_real = JULIA_C_R;
		*c_imagine = JULIA_C_I;
		e->precision /= 2;
	}
	if (e->dyno == 1)
	{
		*c_real = e->c_real;
		*c_imagine = e->c_imagine;
	}
}

void	julia(t_pos *p, t_env e)
{
	double	c_real;
	double	c_imagine;
	double	zx;
	double	zy;
	double	zx_tmp;

	choose_julia(&e, &c_real, &c_imagine);
	p->cx = (double)p->px / (LENGTH / e.c_length) + e.offset_cx;
	p->cy = - ((double)p->py / (HEIGHT / e.c_height) + e.offset_cy);
	zx = p->cx;
	zy = p->cy;
	p->i = 0;
	while (++p->i < e.precision)
	{
		zx_tmp = zx;
		zx = zx * zx - zy * zy + c_real;
		zy = 2.0 * zx_tmp * zy + c_imagine;
		if (zx * zx + zy * zy >= 4.0)
			break ;
	}
}

void	burning(t_pos *p, t_env e)
{
	double	zx;
	double	zy;
	double	zx_tmp;

	zx = 0.0;
	zy = 0.0;
	p->cx = (double)p->px / (LENGTH / e.c_length) + e.offset_cx;
	p->cy = - ((double)p->py / (HEIGHT / e.c_height) + e.offset_cy);
	p->i = 0;
	while (++p->i < e.precision)
	{
		zx_tmp = zx;
		zx = zx * zx - zy * zy + p->cx;
		zy = 2.0 * fabs(zx_tmp * zy) + p->cy;
		if (zx * zx + zy * zy >= 4.0)
			break ;
	}
}

void	apply_fractol(t_pos *p, t_env e)
{
	if (e.pattern == 1)
	{
		p->px -= 0.15 * LENGTH;
		mandel(p, e);
	}
	if (e.pattern >= 2 && e.pattern <= 5)
		julia(p, e);
	if (e.pattern == 6)
	{
		p->px -= 0.15 * LENGTH;
		burning(p, e);
	}
}
