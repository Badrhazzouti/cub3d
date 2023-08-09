/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 00:31:30 by bhazzout          #+#    #+#             */
/*   Updated: 2023/08/09 01:34:15 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	key_release_handler(int keycode, t_win *win)
{
	(void) win;
	if (keycode == 13)
		win->mv.move_up = 1;
	if (keycode == 0)
		win->mv.move_left = 1;
	if (keycode == 2)
		win->mv.move_right = 1;
	if (keycode == 1)
		win->mv.move_down = 1;
	if (keycode == 123)
		win->mv.rotate_left = 1;
	if (keycode == 124)
		win->mv.rotate_right = 1;
	return (0);
}

int	key_handler(int keycode, t_win *win)
{
	if (keycode == 53)
		windows_close(keycode, win);
	if (keycode == 13)
		win->mv.move_up = 0;
	if (keycode == 1)
		win->mv.move_down = 0;
	if (keycode == 0)
		win->mv.move_left = 0;
	if (keycode == 2)
		win->mv.move_right = 0;
	if (keycode == 123)
		win->mv.rotate_left = 0;
	if (keycode == 124)
		win->mv.rotate_right = 0;
	return (0);
}

int	mouse_handler(int x, int y, t_win *win)
{
	double	dx;
	double	dy;
	double	angle;

	dx = x - WIN_WIDTH / 2;
	dy = y - WIN_HEIGHT / 2;
	angle = atan2(dx, dy);
	win->player_a = angle;
	win->player_a = ray_correct(win->player_a);
	return (0);
}
