/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   distance.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 00:50:19 by bhazzout          #+#    #+#             */
/*   Updated: 2023/08/09 03:17:27 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	hor_right_down(t_hit *hit_const, t_win *win,
						double ray_angle, t_wallhit *hor)
{
	if (ray_angle == (3 * M_PI) / 2)
	{
		hor->x = win->playerx;
		hor->y = ((win->playery / win->cell_size) * win->cell_size)
			+ win->cell_size;
		hit_const->offset_x = 0;
		hit_const->offset_y = win->cell_size;
	}
	if ((3 * M_PI) / 2 < ray_angle && ray_angle < (2 * M_PI))
	{
		hor->y = ((win->playery / win->cell_size) * win->cell_size)
			+ win->cell_size;
		hor->x = win->playerx + ((hor->y - win->playery)
				/ tan((2 * M_PI) - ray_angle));
		hit_const->offset_x = win->cell_size / tan((2 * M_PI) - ray_angle);
		hit_const->offset_y = win->cell_size;
	}
}

void	hor_dist_calcul(t_hit *hit_const, t_win *win,
						double ray_angle, t_wallhit *hor)
{
	char	c;

	(void)ray_angle;
	while (hor->x != -1 && (size_t)(hor->x)
		/ win->cell_size < ft_strlen((win->map)[(int)(hor->y
		/ win->cell_size)]))
	{
		c = (win->map)[(int)(hor->y
				/ win->cell_size)][(int)(hor->x / win->cell_size)];
		if (c == '1' || c == ' ')
			break ;
		hor->x += hit_const->offset_x;
		hor->y += hit_const->offset_y;
	}
}

t_wallhit	horizontal_check(t_win *win, double ray_angle, t_wallhit *hor)
{
	t_hit	hit_const;

	hor_hit_const_init(&hit_const, hor);
	if (hor_right_up(&hit_const, win, ray_angle, hor) == 1)
		return (*hor);
	hor_left_up(&hit_const, win, ray_angle, hor);
	if (hor_left_down(&hit_const, win, ray_angle, hor) == 1)
		return (*hor);
	hor_right_down(&hit_const, win, ray_angle, hor);
	if (hor->x == -1 || hor->y == -1)
	{
		hor->dist = -1;
		return (*hor);
	}
	hor_dist_calcul(&hit_const, win, ray_angle, hor);
	hit_const.opposite = win->playery - hor->y;
	hit_const.adjacent = win->playerx - hor->x;
	hor->dist = sqrt(pow(hit_const.adjacent, 2) + pow(hit_const.opposite, 2));
	hor->ver_hor = 1;
	return (*hor);
}

t_wallhit	distance_getter(t_win *win, double ray_angle)
{
	t_wallhit	ver;
	t_wallhit	hor;

	ver = vertical_check(win, ray_angle, &ver);
	hor = horizontal_check(win, ray_angle, &hor);
	if (ver.dist < 0)
		return (hor);
	else if (hor.dist < 0)
		return (ver);
	else if (ver.dist < hor.dist)
		return (ver);
	else if (ver.dist >= hor.dist)
		return (hor);
	else
	{
		ver.dist = -1;
		return (ver);
	}
}
