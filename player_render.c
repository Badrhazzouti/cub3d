/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 00:34:17 by bhazzout          #+#    #+#             */
/*   Updated: 2023/08/09 03:46:35 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	player_render(t_win *win)
{
	t_wallhit	wall_hit;

	rays_init(win);
	while (win->ray.ray_temp < win->ray.ray_end)
	{
		wall_hit = ray_const_init(win);
		wall_put(win, &wall_hit);
		floor_ceil(win, 1);
		floor_ceil(win, 0);
		my_mlx_draw_line(win, &wall_hit);
		win->ray.ray_temp += win->ray.ray_i;
		win->ray.col++;
		wall_hit.dist = -1;
	}
	map_draw(win);
	mlx_put_image_to_window(win->mlx_ptr, win->win_ptr, win->img_ptr, 0, 0);
}

void	sqr_init(t_sqr *ray_sqr, t_win *win, t_mini *m_map)
{
	ray_sqr->x = (win->mini_x / win->cell_size) * m_map->cell_x;
	ray_sqr->y = (win->mini_y / win->cell_size) * m_map->cell_y;
	ray_sqr->co = cos(-win->player_a);
	ray_sqr->si = sin(-win->player_a);
	ray_sqr->i = 0;
	ray_sqr->j = 1;
	ray_sqr->x_i = 1;
	ray_sqr->m_y = 0;
	ray_sqr->m_x = 0;
}

void	ray_draw(t_win *win, t_mini	*m_map)
{
	t_sqr	ray_sqr;

	sqr_init(&ray_sqr, win, m_map);
	while (ray_sqr.i < 20)
	{
		if (ray_sqr.i == 0)
		{
			ray_sqr.m_y = ray_sqr.y - 3;
			while (ray_sqr.m_y < ray_sqr.y + 3)
			{
				ray_sqr.m_x = ray_sqr.x - 3;
				while (ray_sqr.m_x < ray_sqr.x + 3)
				{
					mini_pixel_to_img(win, ray_sqr.m_x, ray_sqr.m_y, 0xFFF0FF);
					ray_sqr.m_x += ray_sqr.x_i;
				}
				ray_sqr.m_y += ray_sqr.j;
			}
		}
		mini_pixel_to_img(win, ray_sqr.x, ray_sqr.y, 0x00FF00);
		ray_sqr.x += ray_sqr.co;
		ray_sqr.y += ray_sqr.si;
		ray_sqr.i += 1;
	}
}

void	cells_draw(t_win *win, t_mini *m_map, int color)
{
	int	i;
	int	j;

	j = 0;
	while (j < m_map->cell_y)
	{
		i = 0;
		while (i < m_map->cell_x)
		{
			if (color != 0xFFF0FF)
				mini_pixel_to_img(win, m_map->pixel_x + i,
					m_map->pixel_y + j, color);
			if (i + 1 == m_map->cell_x)
				mini_pixel_to_img(win, m_map->pixel_x + i,
					m_map->pixel_y + j, 0X000000);
			if (j + 1 == m_map->cell_y)
				mini_pixel_to_img(win, m_map->pixel_x + i,
					m_map->pixel_y + j, 0X000000);
			i++;
		}
		j++;
	}
}

void	map_draw(t_win *win)
{
	int		x;
	int		y;
	int		color;
	t_mini	m_map;

	m_map.cell_x = MINI_WIDTH / win->map_width;
	m_map.cell_y = MINI_HEIGHT / win->map_height;
	y = 0;
	while (y < win->map_height)
	{
		x = 0;
		while (x < (int)ft_strlen(win->map[y]))
		{
			if (win->map[y][x] == '1')
				color = 0x000000;
			else if (win->map[y][x] == '0')
				color = 0xFFF0FF;
			m_map.pixel_x = m_map.cell_x * x;
			m_map.pixel_y = m_map.cell_y * y;
			cells_draw(win, &m_map, color);
			x++;
		}
		y++;
	}
	ray_draw(win, &m_map);
}
