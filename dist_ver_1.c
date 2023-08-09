/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dist_ver_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 01:04:02 by bhazzout          #+#    #+#             */
/*   Updated: 2023/08/09 01:04:10 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	right_up(t_hit *hit_const, t_win *win, double ray_angle, t_wallhit *ver)
{
	if (ray_angle == 0)
	{
		ver->x = ((win->playerx / win->cell_size) * win->cell_size)
			+ win->cell_size;
		ver->y = win->playery;
		hit_const->offset_x = win->cell_size;
		hit_const->offset_y = 0;
	}
	if (ray_angle > 0 && ray_angle < (M_PI / 2))
	{
		ver->x = ((win->playerx / win->cell_size) * win->cell_size)
			+ win->cell_size;
		ver->y = win->playery - ((ver->x - win->playerx) * tan(ray_angle));
		hit_const->offset_x = win->cell_size;
		hit_const->offset_y = -(win->cell_size * tan(ray_angle));
	}
}

int	left_up(t_hit *hit_const, t_win *win, double ray_angle, t_wallhit *ver)
{
	if (ray_angle == M_PI_2)
	{
		ver->dist = -1;
		return (1);
	}
	if (M_PI_2 < ray_angle && ray_angle < M_PI)
	{
		ver->x = ((win->playerx / win->cell_size) * win->cell_size) - 0.0001;
		ver->y = win->playery - ((win->playerx - ver->x)
				* tan(M_PI - ray_angle));
		hit_const->offset_x = -win->cell_size;
		hit_const->offset_y = -(win->cell_size * tan(M_PI - ray_angle));
	}
	return (0);
}