/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 00:28:47 by bhazzout          #+#    #+#             */
/*   Updated: 2023/08/09 00:29:50 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	x_y_update(t_win *win, double playerdx, double playerdy)
{
	int	new_x;
	int	new_y;

	new_x = ((win->playerx + (int)(playerdx * 5)));
	new_y = ((win->playery + (int)(playerdy * 5)));
	if (win->map[(int)win->playery / win->cell_size][(int)(new_x
		+ (playerdx * 10)) / win->cell_size] != '1')
	{
		win->playerx = new_x;
		win->mini_x = new_x;
	}
	if (win->map[(int)(new_y + (playerdy * 10)) / win->cell_size]
		[(int)win->playerx / win->cell_size] != '1')
	{
		win->playery = new_y;
		win->mini_y = new_y;
	}
}

void	move_update(t_win *win, double playerdx, double playerdy)
{
	if (win->mv.move_up == 0)
	{
		playerdx = cos(-win->player_a);
		playerdy = sin(-win->player_a);
		x_y_update(win, playerdx, playerdy);
	}
	if (win->mv.move_down == 0)
	{
		playerdx = cos(-win->player_a + M_PI);
		playerdy = sin(-win->player_a + M_PI);
		x_y_update(win, playerdx, playerdy);
	}
	if (win->mv.move_left == 0)
	{
		playerdx = cos(-win->player_a + M_PI_2);
		playerdy = sin(-win->player_a + M_PI_2);
		x_y_update(win, playerdx, playerdy);
	}
	if (win->mv.move_right == 0)
	{
		playerdx = cos(-win->player_a - M_PI_2);
		playerdy = sin(-win->player_a - M_PI_2);
		x_y_update(win, playerdx, playerdy);
	}
}

int	p_update(t_win *win)
{
	double	playerdx;
	double	playerdy;

	playerdx = 0;
	playerdy = 0;
	if (win->mv.rotate_left == 0)
	{
		win->player_a -= 0.1;
		ray_correct(win->player_a);
	}
	if (win->mv.rotate_right == 0)
	{
		win->player_a += 0.1;
		ray_correct(win->player_a);
	}
	move_update(win, playerdx, playerdy);
	player_render(win);
	return (1);
}