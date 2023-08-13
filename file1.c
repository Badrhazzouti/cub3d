/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 01:39:31 by ikhabour          #+#    #+#             */
/*   Updated: 2023/08/09 01:51:09 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	print_error(void)
{
	write(2, "Error\n", 6);
	exit(123);
}

char	*read_map(int fd)
{
	char	*str;
	char	*str1;
	char	*temp;

	temp = get_next_line(fd);
	str = malloc(1);
	str[0] = '\0';
	if (!temp)
		print_error();
	while (temp)
	{
		str1 = str;
		str = ft_strjoinn(str1, temp);
		free(str1);
		free(temp);
		temp = get_next_line(fd);
	}
	return (str);
}

void	save_texture(char *map, t_texture *texture, char op)
{
	if (op == 'N')
		texture->no = ft_strdup(map + 3);
	else if (op == 'S')
		texture->so = ft_strdup(map + 3);
	else if (op == 'E')
		texture->ea = ft_strdup(map + 3);
	else if (op == 'W')
		texture->we = ft_strdup(map + 3);
}

int	is_elm_helper(char *map, t_texture *texture)
{
	if (!ft_strncmp(map, "NO ", 3))
	{
		save_texture(map, texture, 'N');
		return (1);
	}
	else if (!ft_strncmp(map, "SO ", 3))
	{
		save_texture(map, texture, 'S');
		return (1);
	}
	else if (!ft_strncmp(map, "EA ", 3))
	{
		save_texture(map, texture, 'E');
		return (1);
	}
	else if (!ft_strncmp(map, "WE ", 3))
	{
		save_texture(map, texture, 'W');
		return (1);
	}
	return (0);
}

void	check_values(t_rgb *rgb)
{
	if (rgb->r > 255 || rgb->r < 0)
		print_error();
	if (rgb->g > 255 || rgb->g < 0)
		print_error();
	if (rgb->b > 255 || rgb->b < 0)
		print_error();
}
