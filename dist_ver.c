/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dist_ver.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 00:57:34 by bhazzout          #+#    #+#             */
/*   Updated: 2023/08/09 01:34:03 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	left_down(t_hit *hit_const, t_win *win, double ray_angle,
				t_wallhit *ver)
{
	if (ray_angle == M_PI)
	{
		ver->x = ((win->playerx / win->cell_size) * win->cell_size) - 0.0001;
		ver->y = win->playery;
		hit_const->offset_x = -(win->cell_size);
		hit_const->offset_y = 0;
	}
	if (M_PI < ray_angle && ray_angle < (3 * M_PI) / 2)
	{
		ver->x = ((win->playerx / win->cell_size) * win->cell_size) - 0.0001;
		ver->y = win->playery + ((win->playerx - ver->x)
				* tan(ray_angle - M_PI));
		hit_const->offset_x = -win->cell_size;
		hit_const->offset_y = win->cell_size * tan(ray_angle - M_PI);
	}
}

int	right_down(t_hit *hit_const, t_win *win, double ray_angle, t_wallhit *ver)
{
	if (ray_angle == (3 * M_PI) / 2)
	{
		ver->dist = -1;
		return (1);
	}
	if ((3 * M_PI) / 2 < ray_angle && ray_angle < (2 * M_PI))
	{
		ver->x = ((win->playerx / win->cell_size)
				* win->cell_size) + win->cell_size;
		ver->y = win->playery + ((ver->x - win->playerx)
				* tan((2 * M_PI) - ray_angle));
		hit_const->offset_x = win->cell_size;
		hit_const->offset_y = win->cell_size * tan((2 * M_PI) - ray_angle);
	}
	return (0);
}

void	distance_calcul(t_hit *hit_const, t_win *win, double ray_angle,
			t_wallhit *ver)
{
	char	c;

	(void)ray_angle;
	while (ver->x != -1 && (int)ver->y / win->cell_size < win->map_height
		&& (int)ver->y / win->cell_size >= 0 && (size_t)(ver->x
		/ win->cell_size) < ft_strlen((win->map)[(int)(ver->y
				/ win->cell_size)]))
	{
		c = (win->map)[(int)(ver->y
				/ win->cell_size)][(int)(ver->x / win->cell_size)];
		if (c == '1' || c == ' ')
			break ;
		ver->x += hit_const->offset_x;
		ver->y += hit_const->offset_y;
	}	
}

void	ver_hit_const_init(t_hit *hit_const, t_wallhit *ver)
{
	hit_const->offset_x = -1;
	hit_const->offset_y = -1;
	hit_const->adjacent = 0;
	hit_const->opposite = 0;
	ver->x = -1;
	ver->y = -1;
}

t_wallhit	vertical_check(t_win *win, double ray_angle, t_wallhit *ver)
{
	t_hit	hit_const;

	ver_hit_const_init(&hit_const, ver);
	right_up(&hit_const, win, ray_angle, ver);
	if (left_up(&hit_const, win, ray_angle, ver) == 1)
		return (*ver);
	left_down(&hit_const, win, ray_angle, ver);
	if (right_down(&hit_const, win, ray_angle, ver) == 1)
		return (*ver);
	if (ver->x < 0 || ver->y < 0)
	{
		ver->dist = -1;
		return (*ver);
	}
	distance_calcul(&hit_const, win, ray_angle, ver);
	hit_const.opposite = win->playery - ver->y;
	hit_const.adjacent = win->playerx - ver->x;
	ver->dist = sqrt(pow(hit_const.adjacent, 2) + pow(hit_const.opposite, 2));
	ver->ver_hor = 0;
	return (*ver);
}
