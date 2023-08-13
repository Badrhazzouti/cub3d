/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 01:54:21 by ikhabour          #+#    #+#             */
/*   Updated: 2023/08/09 22:07:02 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	checking_map(char **map, t_win *win)
{
	int	i;

	i = 0;
	while (map[i] && !is_a_wall(map[i]))
		i++;
	if (i > 0)
		print_error();
	if (!map[0])
		print_error();
	check_map_content(map, win);
}

void	check_map(char **map, t_texture *texture, t_win *win)
{
	int	i;
	int	elm;

	i = 0;
	elm = 0;
	while (map[i] && elm < 6)
	{
		if (is_elm(map[i], texture))
			elm++;
		else
			print_error();
		i++;
	}
	if (elm < 6)
		print_error();
	checking_map(map + i, win);
}

int	is_a_wall(char *str)
{
	int	i;

	i = 0;
	while (str[i] == '1' || str[i] == ' ')
		i++;
	if (!str[i])
		return (1);
	return (0);
}
