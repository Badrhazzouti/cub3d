/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 00:01:54 by bhazzout          #+#    #+#             */
/*   Updated: 2023/07/09 01:13:29 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	**map2d_getter(int fd, int counter)
{
	char	*line2;
	char	**map;
	map = (char **)malloc(sizeof(char *) * (counter + 1));
	map[counter] = NULL;
	int	i = 0;
	while (1)
	{
		line2 = get_next_line(fd);
		if (!line2)
			break;
		map[i] = ft_strdup(line2);
		free(line2);
		i++;
	}
	return (map);
}

char	**map_getter(void)
{
	int 	fd;
	char	*line;
	char	*map_line = NULL;
	int		counter;

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
		counter++;
		free(line);
	}
	close(fd);
	fd = open("map.cub", O_RDONLY);
	// map = map2d_getter(fd, counter);
	// map_printer(map);
	return (map2d_getter(fd, counter));
}