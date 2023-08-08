/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 22:12:33 by bhazzout          #+#    #+#             */
/*   Updated: 2023/08/08 06:08:48 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	move_up = 1;
int	move_down = 1;
int	move_left = 1;
int	move_right = 1;
int	rotate_left = 1;
int	rotate_right = 1;
int	forward_speed = 5;
int	backward_speed = -5;
int	right_speed = 5;
int	left_speed = -5;
int	MAX_LOD_DISTANCE = 5000;
double	LOD_FACTOR = 0.5;
double	wall_flag = 0;
int	mini_height = 200;
int	mini_width = 300;

int	windows_close(int keycode, void *param)
{
	(void) keycode, (void) param;
	exit (0);
}

double	ray_correct(double ray_angle)
{
	if (ray_angle < 0)
		ray_angle = ray_angle + (2 * M_PI);
	if (ray_angle >= (2 * M_PI))
		ray_angle = ray_angle - (2 * M_PI);
	return (ray_angle);
}

void	map_printer(char **map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		printf("%s\n", map[i]);
		i++;
	}
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

void my_mlx_draw_line(t_win *win , t_wallhit *wallhit)
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

void	left_down(t_hit *hit_const, t_win *win, double ray_angle, t_wallhit *ver)
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

void	distance_calcul(t_hit *hit_const, t_win *win, double ray_angle, t_wallhit *ver)
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
	// if (ray_angle == 0)
	// {
	// 	ver->x = ((win->playerx / win->cell_size) * win->cell_size)
	// 		+ win->cell_size;
	// 	ver->y = win->playery;
	// 	hit_const.offset_x = win->cell_size;
	// 	offset_y = 0;
	// }
	// if (ray_angle > 0 && ray_angle < (M_PI / 2))
	// {
	// 	ver->x = ((win->playerx / win->cell_size) * win->cell_size)
	// 		+ win->cell_size;
	// 	ver->y = win->playery - ((ver->x - win->playerx) * tan(ray_angle));
	// 	hit_const.offset_x = win->cell_size;
	// 	offset_y = -(win->cell_size * tan(ray_angle));
	// }
	if (left_up(&hit_const, win, ray_angle, ver) == 1)
		return (*ver);
	// if (ray_angle == M_PI_2)
	// {
	// 	ver->dist = -1;
	// 	return (*ver);
	// }
	// if (M_PI_2 < ray_angle && ray_angle < M_PI)
	// {
	// 	ver->x = ((win->playerx / win->cell_size) * win->cell_size) - 0.0001;
	// 	ver->y = win->playery - ((win->playerx - ver->x)
	// 			* tan(M_PI - ray_angle));
	// 	hit_const.offset_x = -win->cell_size;
	// 	hit_const.offset_y = -(win->cell_size * tan(M_PI - ray_angle));
	// }
	left_down(&hit_const, win, ray_angle, ver);
	// if (ray_angle == M_PI)
	// {
	// 	ver->x = ((win->playerx / win->cell_size) * win->cell_size) - 0.0001;
	// 	ver->y = win->playery;
	// 	hit_const.offset_x = -(win->cell_size);
	// 	hit_const.offset_y = 0;
	// }
	// if (M_PI < ray_angle && ray_angle < (3 * M_PI) / 2)
	// {
	// 	ver->x = ((win->playerx / win->cell_size) * win->cell_size) - 0.0001;
	// 	ver->y = win->playery + ((win->playerx - ver->x)
	// 			* tan(ray_angle - M_PI));
	// 	hit_const.offset_x = -win->cell_size;
	// 	hit_const.offset_y = win->cell_size * tan(ray_angle - M_PI);
	// }
	if (right_down(&hit_const, win, ray_angle, ver) == 1)
		return (*ver);
	// if (ray_angle == (3 * M_PI) / 2)
	// {
	// 	ver->dist = -1;
	// 	return (*ver);
	// }
	// if ((3 * M_PI) / 2 < ray_angle && ray_angle < (2 * M_PI))
	// {
	// 	ver->x = ((win->playerx / win->cell_size)
	// 			* win->cell_size) + win->cell_size;
	// 	ver->y = win->playery + ((ver->x - win->playerx)
	// 			* tan((2 * M_PI) - ray_angle));
	// 	hit_const.offset_x = win->cell_size;
	// 	hit_const.offset_y = win->cell_size * tan((2 * M_PI) - ray_angle);
	// }
	if (ver->x < 0 || ver->y < 0)
	{
		ver->dist = -1;
		return (*ver);
	}
	distance_calcul(&hit_const, win, ray_angle, ver);
	// while (ver->x != -1 && (int)ver->y / win->cell_size < win->map_height
	// 	&& (int)ver->y / win->cell_size >= 0 && (size_t)(ver->x
	// 	/ win->cell_size) < ft_strlen((win->map)[(int)(ver->y
	// 			/ win->cell_size)]))
	// {
	// 	map_value = (win->map)[(int)(ver->y
	// 			/ win->cell_size)][(int)(ver->x / win->cell_size)];
	// 	if (map_value == '1' || map_value == ' ')
	// 		break ;
	// 	ver->x += hit_const.offset_x;
	// 	ver->y += hit_const.offset_y;
	// }	
	hit_const.opposite = win->playery - ver->y;
	hit_const.adjacent = win->playerx - ver->x;
	ver->dist = sqrt(pow(hit_const.adjacent, 2) + pow(hit_const.opposite, 2));
	ver->ver_hor = 0;
	return (*ver);
}

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

void	hor_left_up(t_hit *hit_const, t_win *win, double ray_angle, t_wallhit *hor)
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

int	hor_left_down(t_hit *hit_const, t_win *win, double ray_angle, t_wallhit *hor)
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

void	hor_right_down(t_hit *hit_const, t_win *win, double ray_angle, t_wallhit *hor)
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

void	hor_dist_calcul(t_hit *hit_const, t_win *win, double ray_angle, t_wallhit *hor)
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
	// if (ray_angle == 0)
	// {
	// 	hor->dist = -1;
	// 	return (*hor);
	// }
	// if (0 < ray_angle && ray_angle < M_PI_2)
	// {
	// 	hor->y = ((win->playery / win->cell_size) * win->cell_size) - 0.0001;
	// 	hor->x = win->playerx + ((win->playery - hor->y) / tan(ray_angle));
	// 	hit_const.offset_x = win->cell_size / tan(ray_angle);
	// 	hit_const.offset_y = -win->cell_size;
	// }
	hor_left_up(&hit_const, win, ray_angle, hor);
	// if (ray_angle == M_PI_2)
	// {
	// 	hor->x = win->playerx;
	// 	hor->y = ((win->playery / win->cell_size) * win->cell_size) - 0.0001;
	// 	hit_const.offset_x = 0;
	// 	hit_const.offset_y = -win->cell_size;
	// }
	// if (ray_angle > M_PI_2 && ray_angle < M_PI)
	// {
	// 	hor->y = ((win->playery / win->cell_size) * win->cell_size) - 0.0001;
	// 	hor->x = win->playerx - ((win->playery - hor->y) / tan(M_PI - ray_angle));
	// 	hit_const.offset_x = -(win->cell_size / tan(M_PI - ray_angle));
	// 	hit_const.offset_y = -win->cell_size;
	// }
	if (hor_left_down(&hit_const, win, ray_angle, hor) == 1)
		return (*hor);
	// if (ray_angle == M_PI)
	// {
	// 	hor->dist = -1;
	// 	return (*hor);
	// }
	// if (ray_angle > M_PI && ray_angle < (3 * M_PI) / 2)
	// {
	// 	hor->y = ((win->playery / win->cell_size) * win->cell_size) + win->cell_size;
	// 	hor->x = win->playerx - ((hor->y - win->playery) / tan(ray_angle - M_PI));
	// 	hit_const.offset_x = -(win->cell_size / tan(ray_angle - M_PI));
	// 	hit_const.offset_y = win->cell_size;
	// }
	hor_right_down(&hit_const, win, ray_angle, hor);
	// if (ray_angle == (3 * M_PI) / 2)
	// {
	// 	hor->x = win->playerx;
	// 	hor->y = ((win->playery / win->cell_size) * win->cell_size) + win->cell_size;
	// 	hit_const.offset_x = 0;
	// 	hit_const.offset_y =  win->cell_size;
	// }
	// if ((3 * M_PI) / 2 < ray_angle && ray_angle < (2 * M_PI))
	// {
	// 	hor->y = ((win->playery / win->cell_size) * win->cell_size) + win->cell_size;
	// 	hor->x = win->playerx + ((hor->y - win->playery) / tan((2 * M_PI) - ray_angle));
	// 	hit_const.offset_x = win->cell_size / tan((2 * M_PI) - ray_angle);
	// 	hit_const.offset_y = win->cell_size;
	// }
	if (hor->x == -1 || hor->y == -1)
	{
		hor->dist = -1;
		return (*hor);
	}
	hor_dist_calcul(&hit_const, win, ray_angle, hor);
	// char map_value;
	// while (hor->x != -1 && (size_t)(hor->x)
	// 	/ win->cell_size < ft_strlen((win->map)[(int)(hor->y / win->cell_size)]))
	// {
	// 	map_value = (win->map)[(int)(hor->y
	// 			/ win->cell_size)][(int)(hor->x / win->cell_size)];
	// 	if (map_value == '1' || map_value == ' ')
	// 		break ;
	// 	hor->x += hit_const.offset_x;
	// 	hor->y += hit_const.offset_y;
	// }
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
	{
		return (hor);
	}
	else if (hor.dist < 0)
	{
		return (ver);
	}
	else if (ver.dist < hor.dist)
	{
		return (ver);
	}
	else if (ver.dist >= hor.dist)
	{
		return (hor);
	}
	else
	{
		ver.dist = -1;
		return (ver);
	}
}

void draw_ray(void	*mlx_ptr, void *win_ptr, double dist, double ray)
{
	double	x;
	double	y;
	double	co;
	double	si;
	double	i;

	x = 700;
	y = 400;
	co = cos(ray);
	i = 0;
	si = sin(ray);
	while (i < dist)
	{
		mlx_pixel_put(mlx_ptr, win_ptr, x, y , 0xF0F0F0);
		x += co;
		y += si;
		i++;
	}
}

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

void	player_render(t_win *win)
{
	t_wallhit	wall_hit;

	rays_init(win);
	while (win->ray.ray_temp < win->ray.ray_end)
	{
		// win->ray.ray_angle = win->ray.ray_temp;
		// win->ray.ray_angle = ray_correct(win->ray.ray_angle);
		// wall_hit = distance_getter(win, win->ray.ray_angle);
		// wall_hit.dist *= cos(win->player_a - win->ray.ray_angle);
		// win->ray.wall_height = (win->cell_size / wall_hit.dist) * 450;
		// win->ray.wall_top = ((double)WIN_HEIGHT / 2)
		// 	- (win->ray.wall_height / 2);
		// win->ray.wall_bottom = ((double)WIN_HEIGHT / 2)
		// 	+ (win->ray.wall_height / 2);
		// win->ray.wall_screen_x = win->ray.col;
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
		mini_pixel_to_img(win,ray_sqr.x, ray_sqr.y , 0x00FF00);
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

	m_map.cell_x = mini_width / win->map_width;
	m_map.cell_y = mini_height / win->map_height;
	y = 0;
	while (y < win->map_height)
	{
		x = 0;
		while (x < win->map_width)
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

int	key_release_handler(int keycode, t_win *win)
{
	(void) win;
	if (keycode == 13)//W
		move_up = 1;
	if (keycode == 0)//A
		move_left = 1;
	if (keycode == 2)//D
		move_right = 1;
	if (keycode == 1)//S
		move_down = 1;
	if (keycode == 123)//cursor left
		rotate_left = 1;
	if (keycode == 124)//cursor right
		rotate_right = 1;
	return 0;
}

int	key_handler(int keycode, t_win *win)
{
	// (void)win;
	if (keycode == 53)//esc
		windows_close(keycode, win);
	if (keycode == 13)//W
		move_up = 0;
	if (keycode == 1)//S
		move_down = 0;
	if (keycode == 0)//A
		move_left = 0;
	if (keycode == 2)//D
		move_right = 0;
	if (keycode == 123)//cursor left
		rotate_left = 0;
	if (keycode == 124)//cursor right
		rotate_right = 0;
	return (0);
}

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
	if (move_up == 0)
	{
		playerdx = cos(-win->player_a);
		playerdy = sin(-win->player_a);
		x_y_update(win, playerdx, playerdy);
	}
	if (move_down == 0)
	{
		playerdx = cos(-win->player_a + M_PI);
		playerdy = sin(-win->player_a + M_PI);
		x_y_update(win, playerdx, playerdy);
	}
	if (move_left == 0)
	{
		playerdx = cos(-win->player_a + M_PI_2);
		playerdy = sin(-win->player_a + M_PI_2);
		x_y_update(win, playerdx, playerdy);
	}
	if (move_right == 0)
	{
		playerdx = cos(-win->player_a - M_PI_2);
		playerdy = sin(-win->player_a - M_PI_2);
		x_y_update(win, playerdx, playerdy);
	}
}

int	 p_update(t_win *win)
{
	double	playerdx;
	double	playerdy;

	playerdx = 0;
	playerdy = 0;
	if (rotate_left == 0)
	{
		win->player_a -= 0.1;
		ray_correct(win->player_a);
	}
	if (rotate_right == 0)
	{
		win->player_a += 0.1;
		ray_correct(win->player_a);
	}
	move_update(win, playerdx, playerdy);
	player_render(win);
	return (1);
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

int main (int ac, char **av)
{
	(void) av, (void) ac;
	t_win win;
	// win = (t_win *)malloc(sizeof(t_win));

	//initialize map and player's constants
	int	w;
	int	h;
	win.cell_size = 32;
	win.map_height = 11;
	win.map_width = 25;
	win.playerx = 4.5 * 32;
	win.playery = 4.5 * 32;
	win.mini_x = 4.5 * 32;
	win.mini_y = 4.5 * 32;
	win.player_a = M_PI / 2;
	win.fov_a = (M_PI / 3);
	win.rays_a = WIN_WIDTH;
	win.num_rays = WIN_WIDTH;
	win.distance_towall = -1;
	
	win.mlx_ptr = mlx_init();
	// win.f_map = one_map();
	win.map = map_getter();
	// map_printer(win.map);
	win.win_ptr = mlx_new_window(win.mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "CUB3D");
	// map_draw(win);
	// int	h ;
	// int	w ;
	// win.wall = NULL;
	// win.wall = (t_wall *)malloc(sizeof(t_wall));
	win.wall.x = 0;
	win.wall.y = 0;
	// win.img = (t_image *)malloc(sizeof(t_image));
	win.img.ea = mlx_xpm_file_to_image(win.mlx_ptr, "./textures/EA.xpm", &w, &h);
	win.img.no = mlx_xpm_file_to_image(win.mlx_ptr, "./textures/NO.xpm", &w, &h);
	win.img.so = mlx_xpm_file_to_image(win.mlx_ptr, "./textures/SO.xpm", &w, &h);
	win.img.we = mlx_xpm_file_to_image(win.mlx_ptr, "./textures/WE.xpm", &w, &h);
	// win.wall->text = mlx_xpm_file_to_image(win.mlx_ptr, "./textures/EA.xpm", &w, &h);
	// win.ray = (t_ray *)malloc (sizeof(t_ray));
	player_render(&win);
	mlx_hook(win.win_ptr, 2, 0, key_handler, &win);
	mlx_hook(win.win_ptr, 3, 0, key_release_handler, &win);
	mlx_hook(win.win_ptr, 6, 0, mouse_handler, &win);
	mlx_loop_hook(win.mlx_ptr, p_update, &win);
	mlx_hook(win.win_ptr, 17, 0L, windows_close, NULL);
	mlx_loop(win.mlx_ptr);
}