/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 22:12:33 by bhazzout          #+#    #+#             */
/*   Updated: 2023/07/29 19:05:00 by bhazzout         ###   ########.fr       */
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
	int	i = 0;
	while (map[i])
	{
		printf("%s\n", map[i]);
		i++;
	}
}

void	map_draw(t_win win, void *win_ptr, void *mlx_ptr, char **map)
{
	int	x;
	int	y;
	int	color;
	int	pixel_x;
	int	pixel_y;
	int	i;
	int	j;

	y = 0;
	while (y < win.map_height)
	{
		x = 0;
		while (x < win.map_width)
		{
			if (map[y][x] == '1')
				color = 0x000000;
			else if (map[y][x] == '0')
				color = 0xFFF0FF;
			pixel_x = win.cell_size * x;
			pixel_y = win.cell_size * y;
			j = 0;
			while (j < win.cell_size)
			{
				i = 0;
				while (i < win.cell_size)
				{
					mlx_pixel_put(mlx_ptr, win_ptr, pixel_x + i, pixel_y + j, color);
					if (i + 1 == win.cell_size)
						mlx_pixel_put(mlx_ptr, win_ptr, pixel_x + i, pixel_y + j, 0XFF0000);
					if (j + 1 == win.cell_size)
						mlx_pixel_put(mlx_ptr, win_ptr, pixel_x + i, pixel_y + j, 0XFF0000);
					i++;
				}
				j++;
			}
			x++;
		}
		y++;
	}
}

void	 p_update(t_win *win)
{
	if (move_up == 0 || move_down == 0 || move_left == 0 || move_right == 0)
	{
		if (move_up == 0)
			move_up = 1;
		if (move_down == 0)
			move_down = 1;
		if (move_left == 0)
			move_left = 1;
		if (move_right == 0)
			move_right = 1;
		int	new_x = (int)((win->playerX + win->playerDX) / win->cell_size);
		int	new_y = (int)((win->playerY + win->playerDY) / win->cell_size);
		if (new_y > 0 && new_y < win->map_height && new_x > 0 && new_x < win->map_width && win->map[new_y][new_x] != '1')
		{
			win->playerX += win->playerDX;
			win->playerY += win->playerDY;
		}
	}
	if (rotate_left == 0)
	{
		rotate_left = 1;
		win->playerA -= 1;
		ray_correct(win->playerA);
		win->playerDX = cos(win->playerA);
		win->playerDY = sin(win->playerA);
	}
	if (rotate_right == 0)
	{
		rotate_right = 1;
		win->playerA += 1;
		ray_correct(win->playerA);
		win->playerDX = cos(win->playerA);
		win->playerDY = sin(win->playerA);
	}
}

void	pixel_to_img(void *img_ptr, void *mlx_ptr, int x, int y, int color, void *win_ptr, t_win *win)
{
	(void)mlx_ptr, (void)win_ptr, (void)win;
	int				bits_per_pixel = 0;
	unsigned int			*img;
	int				size_line = 0;
	int				endian = 0;

	if (x < 0 || x >= WIN_WIDTH || y < 0 || y >= WIN_HEIGHT)
		return ;
	img = (unsigned int *)mlx_get_data_addr(img_ptr, &bits_per_pixel, &size_line, &endian);
	int index = ((int)y * (size_line / 4) + (int)x);
	img[index] = color;
}

void mlx_draw_line(void *mlx_ptr, void *win_ptr, double x1, double y1, double y2, int color, t_win *win, void *img_ptr)
{
	while (y1 < y2)
	{
		pixel_to_img(img_ptr, mlx_ptr, x1, y1, color, win_ptr, win);
		y1 += 1;
	}
}

t_wallhit	vertical_check(t_win *win, double ray_angle, t_wallhit *ver)
{
	// t_wallhit 	ver;
	double	offset_x = -1;
	double	offset_y = -1;
	// double	first_x = -1;
	ver->x = -1;
	// double	first_y = -1;
	ver->y = -1;
	// double	hypothenuse = 0;
	double	adjacent = 0;
	double	opposite = 0;

	if (ray_angle == 0)
	{
		ver->x = ((win->playerX / win->cell_size) * win->cell_size) + win->cell_size;
		ver->y = win->playerY;
		offset_x = win->cell_size;
		offset_y = 0;
	}
	if (0 < ray_angle && ray_angle < (M_PI / 2))
	{
		ver->x = ((win->playerX / win->cell_size) * win->cell_size) + win->cell_size;
		ver->y = win->playerY - ((ver->x - win->playerX) * tan(ray_angle));
		offset_x = win->cell_size;
		offset_y = -(win->cell_size * tan(ray_angle));
	}
	if (ray_angle == M_PI_2)
	{
		ver->dist = -1;
		return (*ver);
	}
	if (M_PI_2 < ray_angle && ray_angle < M_PI)
	{
		ver->x = ((win->playerX / win->cell_size) * win->cell_size) - 0.0000001;
		ver->y = win->playerY - ((win->playerX - ver->x) * tan(M_PI - ray_angle));
		offset_x = -win->cell_size;
		offset_y = -(win->cell_size * tan(M_PI - ray_angle));
	}
	if (ray_angle == M_PI)
	{
		ver->x = ((win->playerX / win->cell_size) * win->cell_size) - 0.0000001;
		ver->y = win->playerY;
		offset_x = -(win->cell_size);
		offset_y = 0;
	}
	if (M_PI < ray_angle && ray_angle < (3 * M_PI) / 2)
	{
		ver->x = ((win->playerX / win->cell_size) * win->cell_size) - 0.0000001;
		ver->y = win->playerY + ((win->playerX - ver->x) * tan(ray_angle - M_PI));
		offset_x = -win->cell_size;
		offset_y = win->cell_size * tan(ray_angle - M_PI);
	}
	if (ray_angle == (3 * M_PI) / 2)
	{
		ver->dist = -1;
		return (*ver);
	}
	if ((3 * M_PI) / 2 < ray_angle && ray_angle < (2 * M_PI))
	{
		ver->x = ((win->playerX / win->cell_size) * win->cell_size) + win->cell_size;
		ver->y = win->playerY + ((ver->x - win->playerX) * tan((2 * M_PI) - ray_angle));
		offset_x =  win->cell_size;
		offset_y = win->cell_size * tan((2 * M_PI) - ray_angle);
	}
	if (ver->x < 0 || ver->y < 0)
	{
		ver->dist = -1;
		return (*ver);
	}
	char map_value;
	while (ver->x != -1 && (int)ver->y / win->cell_size < win->map_height
		&& (int)ver->y / win->cell_size >= 0 && (size_t)(ver->x
		/ win->cell_size) < ft_strlen((win->map)[(int)(ver->y
				/ win->cell_size)]))
	{
		map_value = (win->map)[(int)(ver->y
				/ win->cell_size)][(int)(ver->x / win->cell_size)];
		if (map_value == '1' || map_value == ' ')
			break ;
		ver->x += offset_x;
		ver->y += offset_y;
	}	
	opposite = win->playerY - ver->y;
	adjacent = win->playerX - ver->x;
	ver->dist = sqrt(pow(adjacent, 2) + pow(opposite, 2));//hypothenuse
	return (*ver);
}

t_wallhit	horizontal_check(t_win *win, double ray_angle, t_wallhit *hor)
{
	double	offset_x = -1;
	double	offset_y = -1;
	// double	first_x = -1;
	hor->x = -1;
	// double	first_y = -1;
	hor->y = -1;
	
	// double	hypothenuse = 0;
	double	adjacent = 0;
	double	opposite = 0;
	if (ray_angle == 0)
	{
		hor->dist = -1;
		return (*hor);
	}
	if (0 < ray_angle && ray_angle < M_PI_2)
	{
		hor->y = ((win->playerY / win->cell_size) * win->cell_size) - 0.0000001;
		hor->x = win->playerX + ((win->playerY - hor->y)/ tan(ray_angle));
		offset_x = win->cell_size / tan(ray_angle);
		offset_y = -win->cell_size;
	}
	if (ray_angle == M_PI_2)
	{
		hor->x = win->playerX;
		hor->y = ((win->playerY / win->cell_size) * win->cell_size) - 0.0000001;///?
		offset_x = 0;
		offset_y = -win->cell_size;
	}
	if (M_PI_2 < ray_angle && ray_angle < M_PI)
	{
		hor->y = ((win->playerY / win->cell_size) * win->cell_size) - 0.0000001;
		hor->x = win->playerX - ((win->playerY - hor->y) / tan(M_PI - ray_angle));
		offset_x = -(win->cell_size / tan(M_PI - ray_angle));
		offset_y = -win->cell_size;
	}
	if (ray_angle == M_PI)
	{
		hor->dist = -1;
		return (*hor);
	}
	if (M_PI < ray_angle && ray_angle < (3 * M_PI) / 2)
	{
		hor->y = ((win->playerY / win->cell_size) * win->cell_size) + win->cell_size;
		hor->x = win->playerX - ((hor->y - win->playerY) / tan(ray_angle - M_PI));
		offset_x = -(win->cell_size / tan(ray_angle - M_PI));
		offset_y = win->cell_size;
	}
	if (ray_angle == (3 * M_PI) / 2)
	{
		hor->x = win->playerX;
		hor->y = ((win->playerY / win->cell_size) * win->cell_size) + win->cell_size;
		offset_x = 0;
		offset_y =  win->cell_size;
	}
	if ((3 * M_PI) / 2 < ray_angle && ray_angle < (2 * M_PI))
	{
		hor->y = ((win->playerY / win->cell_size) * win->cell_size) + win->cell_size;
		hor->x = win->playerX + ((hor->y - win->playerY) / tan((2 * M_PI) - ray_angle));
		offset_x = win->cell_size / tan((2 * M_PI) - ray_angle);
		offset_y = win->cell_size;
	}
	if (hor->x == -1 || hor->y == -1)
	{
		hor->dist = -1;
		return (*hor);
	}
	char map_value;
	while (hor->x != -1 && (size_t)(hor->x)
		/ win->cell_size < ft_strlen((win->map)[(int)(hor->y / win->cell_size)]))
	{
		map_value = (win->map)[(int)(hor->y
				/ win->cell_size)][(int)(hor->x / win->cell_size)];
		if (map_value == '1' || map_value == ' ')
			break ;
		hor->x += offset_x;
		hor->y += offset_y;
	}
	opposite = win->playerY - hor->y;
	adjacent = win->playerX - hor->x;
	hor->dist = sqrt(pow(adjacent, 2) + pow(opposite, 2));//hypothenuse
	return (*hor);
}

t_wallhit	distance_getter(t_win *win, double ray_angle)
{
	t_wallhit ver;
	t_wallhit hor;

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
		return(ver);
	}
}

void draw_ray(void	*mlx_ptr, void *win_ptr, double dist, double ray)
{
	double x = 700;
	double y = 400;
	double co = cos(ray);
	double si = sin(ray);
	double i = 0;
	while (i < dist)
	{
		mlx_pixel_put(mlx_ptr, win_ptr, x, y , 0xF0F0F0);
		x += co;
		y += si;
		i++;
	}
}

void	player_render(t_win *win, void *win_ptr, void *mlx_ptr)
{
	win->playerA = ray_correct(win->playerA);

	t_wallhit wall_hit;
	double	ray_incrementation = 0.00087266462;
	double	ray_start = win->playerA - (win->fov_A / 2);
	double	ray_end = win->playerA + (win->fov_A / 2);
	double	ray_angle = ray_start;
	double	ray_temp = ray_start;
	void	*img_ptr = NULL;
	double	wall_height = 0;
	double	col = 0;
	double	wall_screen_x = 0;
	img_ptr && mlx_destroy_image(mlx_ptr,img_ptr);
	img_ptr = mlx_new_image(mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	while (ray_temp < ray_end)
	{
		ray_angle = ray_temp;
		ray_angle = ray_correct(ray_angle);
		wall_hit = distance_getter(win, ray_angle);
		wall_hit.dist *= cos(win->playerA - ray_angle);
		wall_height = (win->cell_size / wall_hit.dist) * 800;
		// draw_ray(mlx_ptr, win_ptr, wall_hit.dist, ray_angle);
		double wallTop = ((double)WIN_HEIGHT / 2) - (wall_height / 2);
		wall_screen_x = col;
    	double wallBottom = ((double)WIN_HEIGHT / 2) + (wall_height / 2);
        // mlx_draw_line(mlx_ptr, win_ptr, wall_screen_x, 0, wallTop, 0xFF00FF, win, img_ptr);
		mlx_draw_line(mlx_ptr, win_ptr, wall_screen_x, wallTop, wallBottom, 0xFF7575, win, img_ptr);
		// mlx_draw_line(mlx_ptr, win_ptr, wall_screen_x, wallBottom, WIN_HEIGHT, 0xFF0000, win, img_ptr);
		ray_temp += ray_incrementation;
		col++;
		wall_hit.dist = -1;
	}
	mlx_put_image_to_window(mlx_ptr, win_ptr, img_ptr, 0, 0);
}

// int	key_release_handler(int keycode, t_win *win)
// {
// 	(void) win;
// 	if (keycode == 13)//W
// 		move_up = 1;
// 	if (keycode == 0)//A
// 		rotate_left = 1;
// 	if (keycode == 2)//D
// 		rotate_right = 1;
// 	if (keycode == 1)//S
// 		move_down = 1;
// 	return 0;
// }

int	key_handler(int keycode, t_win *win)
{
	if (keycode == 13)//W
	{
		// move_up = 0;
		win->playerDX = cos(win->playerA) * 5;
		win->playerDY = -sin(win->playerA) * 5;
		int	new_x = (int)((win->playerX + win->playerDX) / win->cell_size);
		int	new_y = (int)((win->playerY + win->playerDY) / win->cell_size);
		if (new_y > 0 && new_y < win->map_height && new_x > 0 && new_x < win->map_width && win->map[new_y][new_x] != '1')
		{
			win->playerX += win->playerDX;
			win->playerY += win->playerDY;
		}
	}
	if (keycode == 1)//S
	{
		// move_down = 0;
		win->playerDX = cos(win->playerA) * 5;
		win->playerDY = -sin(win->playerA) * 5;
		int	new_x = (int)((win->playerX - win->playerDX) / win->cell_size);
		int	new_y = (int)((win->playerY - win->playerDY) / win->cell_size);
		if (new_y > 0 && new_y < win->map_height && new_x > 0 && new_x < win->map_width && win->map[new_y][new_x] != '1')
		{
			win->playerX -= win->playerDX;
			win->playerY -= win->playerDY;
		}
	}
	if (keycode == 0)//A
	{
		// move_left = 0;
		win->playerDX = cos(win->playerA + M_PI_2) * 5;
		win->playerDY = -sin(win->playerA + M_PI_2) * 5;
		int	new_x = (int)((win->playerX - win->playerDX) / win->cell_size);
		int	new_y = (int)((win->playerY - win->playerDY) / win->cell_size);
		if (new_y > 0 && new_y < win->map_height && new_x > 0 && new_x < win->map_width && win->map[new_y][new_x] != '1')
		{
			win->playerX -= win->playerDX;
			win->playerY -= win->playerDY;
		}
	}
	if (keycode == 2)//D
	{
		// move_right = 0;
		win->playerDX = cos(win->playerA + M_PI_2) * 5;
		win->playerDY = -sin(win->playerA + M_PI_2) * 5;
		int	new_x = (int)((win->playerX + win->playerDX) / win->cell_size);
		int	new_y = (int)((win->playerY + win->playerDY) / win->cell_size);
		if (new_y > 0 && new_y < win->map_height && new_x > 0 && new_x < win->map_width && win->map[new_y][new_x] != '1')
		{
			win->playerX += win->playerDX;
			win->playerY += win->playerDY;
		}
	}
	if (keycode == 123)//cursor left
	{
		// rotate_left = 0;
		win->playerA -= 0.1;
		ray_correct(win->playerA);
		win->playerDX = cos(win->playerA);
		win->playerDY = sin(win->playerA);
	}
	if (keycode == 124)//cursor right
	{
		// rotate_right = 0;
		win->playerA += 0.1;
		ray_correct(win->playerA);
		win->playerDX = cos(win->playerA);
		win->playerDY = sin(win->playerA);
	}
	// map_draw(*win, win->win_ptr, win->mlx_ptr, win->map);
	p_update(win);
	mlx_clear_window(win->mlx_ptr, win->win_ptr);
	player_render(win, win->win_ptr, win->mlx_ptr);
	// key_release_handler(keycode, win);
	return (0);
}

int main (int ac, char **av)
{
	// void	*mlx_ptr;
	// void	*win_ptr;
	// char	**map;
	(void) av, (void) ac;
	t_win win;

	//initialize map and player's constants
	win.cell_size = 32;
	win.map_height = 11;
	win.map_width = 25;
	win.playerX = 4.5 * 32;
	win.playerY = 4.5 * 32;
	win.playerA = M_PI / 2;
	win.fov_A = (M_PI / 3);
	win.rays_A = WIN_WIDTH;
	win.num_rays = WIN_WIDTH;
	// win.playerDX = cos(win.playerA) * 5;
	// win.playerDY = sin(win.playerA) * 5;
	win.distance_towall = -1;
	
	win.mlx_ptr = mlx_init();
	// win.f_map = one_map();
	win.map = map_getter();
	// map_printer(win.map);
	win.win_ptr = mlx_new_window(win.mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "CUB3D");
	// map_draw(win, win.win_ptr, win.mlx_ptr, win.map);
	player_render(&win, win.win_ptr, win.mlx_ptr);
	mlx_hook(win.win_ptr, 2, 0, &key_handler, &win);
	// mlx_key_hook(win.win_ptr, &key_release_handler, &win);
	mlx_hook(win.win_ptr, 17, 0L, &windows_close, NULL);
	mlx_loop(win.mlx_ptr);
}