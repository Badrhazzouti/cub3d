/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 00:36:31 by bhazzout          #+#    #+#             */
/*   Updated: 2023/08/09 01:34:25 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	wall_put(t_win *win, t_wallhit *wall_hit)
{
	int	x;
	int	y;

	x = wall_hit->x / CELL_SIZE;
	y = wall_hit->y / CELL_SIZE;
	if (wall_hit->ver_hor == 1)
		win->wall.x = (int)wall_hit->x % CELL_SIZE;
	else
		win->wall.x = (int)wall_hit->y % CELL_SIZE;
	if (y < win->playery / CELL_SIZE && wall_hit->ver_hor == 1)
		win->wall.text = win->img.ea;
	if (y > win->playery / CELL_SIZE && wall_hit->ver_hor == 1)
		win->wall.text = win->img.no;
	if (x > win->playerx / CELL_SIZE && wall_hit->ver_hor == 0)
		win->wall.text = win->img.so;
	if (x < win->playerx / CELL_SIZE && wall_hit->ver_hor == 0)
		win->wall.text = win->img.we;
}

void	floor_ceil(t_win *win, int flag)
{
	int	y1;
	int	x1;
	int	y2;

	x1 = win->ray.wall_screen_x;
	if (flag == 0)
	{	
		y1 = 0;
		y2 = win->ray.wall_top;
		win->color = 0x0F00FF;
	}
	else
	{
		y1 = win->ray.wall_bottom;
		y2 = WIN_HEIGHT;
		win->color = 0xFF00F0;
	}
	while (y1 < y2)
	{
		pixel_to_img(win, x1, y1);
		y1++;
	}
}

void	rays_init(t_win *win)
{
	win->ray.ray_i = 0.00087266462;
	win->ray.ray_start = win->player_a - (win->fov_a / 2);
	win->ray.ray_end = win->player_a + (win->fov_a / 2);
	win->ray.ray_angle = win->ray.ray_start;
	win->ray.ray_temp = win->ray.ray_start;
	win->img_ptr = NULL;
	win->ray.wall_height = 0;
	win->ray.col = 0;
	win->ray.wall_screen_x = 0;
	win->img_ptr && mlx_destroy_image(win->mlx_ptr, win->img_ptr);
	win->img_ptr = mlx_new_image(win->mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	win->wall.x = 0;
	win->wall.y = 0;
	win->player_a = ray_correct(win->player_a);
}

t_wallhit	ray_const_init(t_win *win)
{
	t_wallhit	wall_hit;

	win->ray.ray_angle = win->ray.ray_temp;
	win->ray.ray_angle = ray_correct(win->ray.ray_angle);
	wall_hit = distance_getter(win, win->ray.ray_angle);
	wall_hit.dist *= cos(win->player_a - win->ray.ray_angle);
	win->ray.wall_height = (win->cell_size / wall_hit.dist) * 450;
	win->ray.wall_top = ((double)WIN_HEIGHT / 2)
		- (win->ray.wall_height / 2);
	win->ray.wall_bottom = ((double)WIN_HEIGHT / 2)
		+ (win->ray.wall_height / 2);
	win->ray.wall_screen_x = win->ray.col;
	return (wall_hit);
}
