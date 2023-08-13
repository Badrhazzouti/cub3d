/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 01:06:13 by bhazzout          #+#    #+#             */
/*   Updated: 2023/08/12 20:35:56 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	ray_correct(double ray_angle)
{
	if (ray_angle < 0)
		ray_angle = ray_angle + (2 * M_PI);
	if (ray_angle >= (2 * M_PI))
		ray_angle = ray_angle - (2 * M_PI);
	return (ray_angle);
}

void	my_pixel_to_img(t_win *win)
{
	int				bits_per_pixel;
	int				size_line;
	int				endian;
	t_img			img;

	bits_per_pixel = 0;
	size_line = 0;
	endian = 0;
	img.x = (int)win->ray.wall_screen_x;
	img.y = (int)win->ray.wall_top;
	img.texture = (unsigned int *)mlx_get_data_addr(win->wall.text, \
		&bits_per_pixel, &size_line, &endian);
	img.img_index = (win->wall.y * (size_line / 4) + win->wall.x);
	img.img = (unsigned int *)mlx_get_data_addr(win->img_ptr, \
		&bits_per_pixel, &size_line, &endian);
	img.index = (img.y * (size_line / 4) + img.x);
	if (img.x < 0 || img.x >= WIN_WIDTH || img.y < 0 || img.y >= WIN_HEIGHT)
		return ;
	if (win->wall.x < 0 || win->wall.x >= CELL_SIZE || \
		win->wall.y < 0 || win->wall.y >= CELL_SIZE * 10)
		return ;
	img.img[img.index] = img.texture[img.img_index];
}

void	mini_pixel_to_img(t_win *win, int x, int y, int color)
{
	unsigned int	*img;
	int				bits_per_pixel;
	int				size_line;
	int				endian;
	int				index;

	bits_per_pixel = 0;
	size_line = 0;
	endian = 0;
	if (x < 0 || x >= WIN_WIDTH || y < 0 || y >= WIN_HEIGHT)
		return ;
	img = (unsigned int *)mlx_get_data_addr(win->img_ptr,
			&bits_per_pixel, &size_line, &endian);
	index = ((int)y * (size_line / 4) + (int)x);
	img[index] = color;
}

void	pixel_to_img(t_win *win, int x, int y)
{
	int				bits_per_pixel;
	unsigned int	*img;
	int				size_line;
	int				endian;
	int				index;

	size_line = 0;
	bits_per_pixel = 0;
	endian = 0;
	if (x < 0 || x >= WIN_WIDTH || y < 0 || y >= WIN_HEIGHT)
		return ;
	img = (unsigned int *)mlx_get_data_addr(win->img_ptr,
			&bits_per_pixel, &size_line, &endian);
	index = ((int)y * (size_line / 4) + (int)x);
	img[index] = win->color;
}

void	my_mlx_draw_line(t_win *win, t_wallhit *wallhit)
{
	double	virt_hight;
	double	repeat_pixel;
	double	i;
	double	j;

	i = 0;
	virt_hight = ((double)(CELL_SIZE) / wallhit->dist) * 450;
	repeat_pixel = (virt_hight) / ((double)CELL_SIZE);
	j = 1 / (repeat_pixel);
	if (wallhit->ver_hor == 1)
		win->wall.x = ((int)(wallhit->x) % (int)CELL_SIZE);
	else
		win->wall.x = (int)(wallhit->y) % (int)CELL_SIZE;
	win->wall.y = 0;
	while (win->ray.wall_top < win->ray.wall_bottom)
	{
		my_pixel_to_img(win);
		win->ray.wall_top += 1;
		i += j * 10;
		win->wall.y = i;
	}
}
