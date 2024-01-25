/* ************************************************************************** */
/*									      */
/*							  :::	   ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*						      +:+ +:+	      +:+     */
/*   By: yrigny <marvin@42.fr>			    +#+  +:+	   +#+	      */
/*						  +#+#+#+#+#+	+#+	      */
/*   Created: 2024/01/21 16:52:02 by yrigny	       #+#    #+#	      */
/*   Updated: 2024/01/24 14:24:33 by yrigny           ###   ########.fr       */
/*									      */
/* ************************************************************************** */

#include "../inc/fractol.h"

void	error_msg(char *error_arg)
{
	ft_printf("Invalid option \"%s\"\n", error_arg);
	ft_printf("- Julia set options: 1      | 2      | 3\n");
	ft_printf("- Precision options: Low    | Medium | High\n");
	ft_printf("- Color options    : A      | B      | C\n");
	exit(1);
}

void	more_options(int ac, char **av, int i, t_env *e)
{
	while (i <= ac - 1)
	{
		if (e->pattern == 2 && ft_strncmp(av[i], "1", 2) == 0)
			e->pattern = 3;
		else if (e->pattern == 2 && ft_strncmp(av[i], "2", 2) == 0)
			e->pattern = 4;
		else if (e->pattern == 2 && ft_strncmp(av[i], "3", 2) == 0)
			e->pattern = 5;
		else if (e->precision == 0 && ft_strncmp(av[i], "Low", 4) == 0)
			e->precision = 20;
		else if (e->precision == 0 && ft_strncmp(av[i], "Medium", 7) == 0)
			e->precision = 60;
		else if (e->precision == 0 && ft_strncmp(av[i], "High", 5) == 0)
			e->precision = 100;
		else if (e->colorbase == 0 && ft_strncmp(av[i], "A", 2) == 0)
			e->colorbase = 1;
		else if (e->colorbase == 0 && ft_strncmp(av[i], "B", 2) == 0)
			e->colorbase = 2;
		else if (e->colorbase == 0 && ft_strncmp(av[i], "C", 2) == 0)
			e->colorbase = 3;
		else
			error_msg(av[i]);
		i++;
	}
}

void	which_pattern(int ac, char **av, t_env *e)
{
	e->pattern = 0;
	e->precision = 0;
	e->colorbase = 0;
	if (ac >= 2 && ft_strncmp(av[1], "Mandelbrot", 11) == 0)
		e->pattern = 1;
	else if (ac >= 2 && ft_strncmp(av[1], "Julia", 6) == 0)
		e->pattern = 2;
	else if (ac >= 2 && ft_strncmp(av[1], "Bonus", 6) == 0)
		e->pattern = 6;
	else
	{
		ft_printf("Available fractals:\nMandelbrot\nJulia\n");
		exit(1);
	}
	if (e->pattern != 0 && ac > 2)
		more_options(ac, av, 2, e);
	if (e->pattern != 0 && e->precision == 0)
		e->precision = 60;
	if (e->pattern != 0 && e->colorbase == 0)
		e->colorbase = 1;
}
