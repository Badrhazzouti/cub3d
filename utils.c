/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 00:01:54 by bhazzout          #+#    #+#             */
/*   Updated: 2023/08/08 01:19:42 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	**map_getter(void)
{
	int 	fd;
	char	*line;
	int		counter;
	char	*map_line;
	// char	*line2;
	char	**map;

	fd = open("map.cub", O_RDONLY);
	if (fd < 0)
	{
		printf("failed opening map.\n");
		exit (1);
	}
	counter = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break;
		map_line = ft_strjoin(map_line, line);
		free(line);
		counter++;
	}
	map = ft_split(map_line, '\n');
	free(map_line);
	// map_printer(map);
	return (map);
}
