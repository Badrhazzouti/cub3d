/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 01:41:10 by ikhabour          #+#    #+#             */
/*   Updated: 2023/08/09 01:51:11 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	store_and_free(char *map, t_rgb *rgb, t_colors col)
{
	char	*color;

	if (col.i == 0)
	{
		color = ft_substr(map, 0, col.j);
		rgb->r = ft_atoi(color);
		free(color);
	}
	else if (col.i == 1)
	{
		color = ft_substr(map, col.last_value, col.j);
		rgb->g = ft_atoi(color);
		free(color);
	}
	else
	{
		color = ft_substr(map, col.last_value, col.j);
		rgb->b = ft_atoi(color);
		free(color);
	}
}

void	store_colors(char *map, t_rgb *rgb)
{
	t_colors	col;

	col.j = 0;
	col.i = 0;
	while (col.i < 3)
	{
		while (map[col.j] && map[col.j] != ',')
			col.j++;
		store_and_free(map, rgb, col);
		col.i++;
		col.j++;
		col.last_value = col.j;
	}
	check_values(rgb);
}

int	is_elm_helper2(char *map, t_texture *texture)
{
	int		i;
	t_rgb	colors;

	i = 2;
	if (!ft_strncmp(map, "F ", 2))
	{
		while (map[i] != '\n' && ((map[i] >= '0' && map[i] <= '9')
				|| (map[i] == ' ' || map[i] == ',')))
			i++;
		store_colors(map + 2, &colors);
		texture->f = (colors.r << 16) + (colors.g << 8) + (colors.b);
		if (!map[i])
			return (1);
	}
	else if (!ft_strncmp(map, "C ", 2))
	{
		while (map[i] != '\n' && ((map[i] >= '0' && map[i] <= '9')
				|| (map[i] == ' ' || map[i] == ',')))
			i++;
		store_colors(map + 2, &colors);
		texture->c = (colors.r << 16) + (colors.g << 8) + (colors.b);
		if (!map[i])
			return (1);
	}
	return (0);
}

int	is_elm(char *map, t_texture *texture)
{
	int	i;

	i = 0;
	if (*map == 'N' || *map == 'S' || *map == 'E' || *map == 'W' || *map == 'F'
		|| *map == 'C')
	{
		if (is_elm_helper(map, texture))
			return (1);
		else if (is_elm_helper2(map, texture))
			return (1);
		else
			return (0);
	}
	return (0);
}

int	check_first_index(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ')
		i++;
	if (str[i] != '1')
		print_error();
	return (i);
}
