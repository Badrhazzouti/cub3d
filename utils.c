/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 00:01:54 by bhazzout          #+#    #+#             */
/*   Updated: 2023/07/25 23:13:38 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// char	**map2d_getter(int fd, int counter)
// {
	
// 	// map = (char **)malloc(sizeof(char *) * (counter + 1));
// 	// map[counter] = NULL;
// 	// int	i = 0;
// 	// while (1)
// 	// {
// 	// 	line2 = get_next_line(fd);
// 	// 	if (!line2)
// 	// 		break;
// 	// 	map[i] = ft_strdup(line2);
// 	// 	free(line2);
// 	// 	i++;
// 	// }
// 	// return (map);
// }

char	**map_getter(void)
{
	int 	fd;
	char	*line;
	int		counter;
	char	*line2;
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
		// map_line = ft_strjoin(map_line, line);
		counter++;
		free(line);
	}
	printf("counter is (%d)\n", counter);
	close(fd);
	fd = open("map.cub", O_RDONLY);
	map = (char **)malloc(sizeof(char *) * (counter + 1));
	map[counter] = NULL;
	int	i = 0;
	while (1)
	{
		line2 = get_next_line(fd);
		if (!line2)
			break;
		map[i] = ft_strdup(line2);
		printf("%d====%s", i, line2);
		i++;
		free(line2);
	}
	close(fd);
	// map_printer(map);
	return (map);
}
