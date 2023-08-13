/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 01:44:08 by ikhabour          #+#    #+#             */
/*   Updated: 2023/08/09 22:08:10 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_last_index(char *str)
{
	int	i;

	i = ft_strlenn(str) - 1;
	while (str[i] == ' ')
		i--;
	if (str[i] != '1')
		print_error();
	return (i);
}

void	handle_space(char **map, int y, int x, int *flag)
{
	char	c;

	if (y < 0 || x < 0 || !map[y] || !map[y][x])
		return ;
	if (map[y][x] != ' ')
	{
		if (map[y][x] == '0')
			*flag = 1;
		return ;
	}
	c = map[y][x];
	map[y][x] = '2';
	handle_space(map, y + 1, x, flag);
	handle_space(map, y - 1, x, flag);
	handle_space(map, y, x + 1, flag);
	handle_space(map, y, x - 1, flag);
	map[y][x] = c;
}

void	define_player_pov(char c, t_win *win)
{
	if (c == 'N')
		win->player_a = M_PI_2;
	else if (c == 'S')
		win->player_a = 3 * (M_PI) / 2;
	else if (c == 'W')
		win->player_a = M_PI;
	else
		win->player_a = 0;
}

void	check_map_helper(char **map, t_helper *s, t_win *win)
{
	while (s->j <= s->last && (map[s->i][s->j] == '1' || map[s->i][s->j] == '0'
			|| map[s->i][s->j] == 'N' || map[s->i][s->j] == 'S'
			|| map[s->i][s->j] == 'E' || map[s->i][s->j] == 'W'
			|| map[s->i][s->j] == ' '))
	{
		if (map[s->i][s->j] == ' ')
			handle_space(map, s->i, s->j, &s->flag);
		if (s->flag == 1)
			print_error();
		if (map[s->i][s->j] == 'N' || map[s->i][s->j] == 'S'
			|| map[s->i][s->j] == 'E' || map[s->i][s->j] == 'W')
		{
			win->playerx = (s->j + 0.5) * 32;
			win->playery = (s->i + 0.5) * 32;
			win->mini_x = (s->j + 0.5) * 32;
			win->mini_y = (s->i + 0.5) * 32;
			define_player_pov(map[s->i][s->j], win);
			s->p++;
		}
		if (s->p > 1)
			print_error();
		s->j++;
	}
	if (s->j < s->last)
		print_error();
}

void	check_map_content(char **map, t_win *win)
{
	t_helper	s;

	s.i = 0;
	s.flag = 0;
	s.p = 0;
	while (map[s.i])
	{
		s.j = check_first_index(map[s.i]);
		s.last = check_last_index(map[s.i]);
		if (s.i == 0 || !map[s.i + 1])
		{
			while (s.j++ <= s.last && (map[s.i][s.j] == ' '
				|| map[s.i][s.j] == '1'))
			{
				if (map[s.i][s.j] == ' ')
					handle_space(map, s.i, s.j, &s.flag);
				if (s.flag == 1)
					print_error();
			}
		}
		else
			check_map_helper(map, &s, win);
		s.i++;
	}
}
