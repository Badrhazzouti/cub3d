/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dist_hor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 00:54:38 by bhazzout          #+#    #+#             */
/*   Updated: 2023/08/09 00:55:17 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	hor_hit_const_init(t_hit *hit_const, t_wallhit *hor)
{
	hit_const->offset_x = -1;
	hit_const->offset_y = -1;
	hit_const->adjacent = 0;
	hit_const->opposite = 0;
	hor->x = -1;
	hor->y = -1;
}

int	hor_right_up(t_hit *hit_const, t_win *win, double ray_angle, t_wallhit *hor)
{
	if (ray_angle == 0)
	{
		hor->dist = -1;
		return (1);
	}
	if (0 < ray_angle && ray_angle < M_PI_2)
	{
		hor->y = ((win->playery / win->cell_size) * win->cell_size) - 0.0001;
		hor->x = win->playerx + ((win->playery - hor->y) / tan(ray_angle));
		hit_const->offset_x = win->cell_size / tan(ray_angle);
		hit_const->offset_y = -win->cell_size;
	}
	return (0);
}

void	hor_left_up(t_hit *hit_const, t_win *win,
			double ray_angle, t_wallhit *hor)
{
	if (ray_angle == M_PI_2)
	{
		hor->x = win->playerx;
		hor->y = ((win->playery / win->cell_size) * win->cell_size) - 0.0001;
		hit_const->offset_x = 0;
		hit_const->offset_y = -win->cell_size;
	}
	if (ray_angle > M_PI_2 && ray_angle < M_PI)
	{
		hor->y = ((win->playery / win->cell_size) * win->cell_size) - 0.0001;
		hor->x = win->playerx - ((win->playery - hor->y)
				/ tan(M_PI - ray_angle));
		hit_const->offset_x = -(win->cell_size / tan(M_PI - ray_angle));
		hit_const->offset_y = -win->cell_size;
	}
}

int	hor_left_down(t_hit *hit_const, t_win *win,
			double ray_angle, t_wallhit *hor)
{
	if (ray_angle == M_PI)
	{
		hor->dist = -1;
		return (1);
	}
	if (ray_angle > M_PI && ray_angle < (3 * M_PI) / 2)
	{
		hor->y = ((win->playery / win->cell_size) * win->cell_size)
			+ win->cell_size;
		hor->x = win->playerx - ((hor->y - win->playery)
				/ tan(ray_angle - M_PI));
		hit_const->offset_x = -(win->cell_size / tan(ray_angle - M_PI));
		hit_const->offset_y = win->cell_size;
	}
	return (0);
}