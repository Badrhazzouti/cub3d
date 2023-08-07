/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 22:12:33 by bhazzout          #+#    #+#             */
/*   Updated: 2023/08/07 05:20:54 by bhazzout         ###   ########.fr       */
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
	int	i = 0;
	while (map[i])
	{
		printf("%s\n", map[i]);
		i++;
	}
}

void	my_pixel_to_img(void *img_ptr, void *mlx_ptr, int x, int y, void *win_ptr, t_win *win)
{
	(void)mlx_ptr, (void)win_ptr, (void)win;
	unsigned int	*img;
	unsigned int	*texture;
	int				bits_per_pixel = 0;
	int				size_line = 0;
	int				endian = 0;

	texture = (unsigned int *)mlx_get_data_addr(win->wall.text, &bits_per_pixel, &size_line, &endian);
	int	img_index = (win->wall.y * (size_line / 4) + win->wall.x);
	img = (unsigned int *)mlx_get_data_addr(img_ptr, &bits_per_pixel, &size_line, &endian);
	 int  index = (y * (size_line / 4) + x);
	if (x < 0 || x >= WIN_WIDTH || y < 0 || y >= WIN_HEIGHT)
		return ;
	if (win->wall.x < 0 || win->wall.x >= CELL_SIZE || win->wall.y < 0 || win->wall.y >= CELL_SIZE * 10)
		return ;
	img[index] = texture[img_index];
}

void	mini_pixel_to_img(t_win *win, int x, int y, int color)
{
	int				bits_per_pixel = 0;
	unsigned int			*img;
	int				size_line = 0;
	int				endian = 0;

	if (x < 0 || x >= WIN_WIDTH || y < 0 || y >= WIN_HEIGHT)
		return ;
	img = (unsigned int *)mlx_get_data_addr(win->img_ptr, &bits_per_pixel, &size_line, &endian);
	int index = ((int)y * (size_line / 4) + (int)x);
	img[index] = color;
}

void	pixel_to_img(t_win *win, int x, int y)
{
	int				bits_per_pixel = 0;
	unsigned int			*img;
	int				size_line = 0;
	int				endian = 0;

	if (x < 0 || x >= WIN_WIDTH || y < 0 || y >= WIN_HEIGHT)
		return ;
	img = (unsigned int *)mlx_get_data_addr(win->img_ptr, &bits_per_pixel, &size_line, &endian);
	int index = ((int)y * (size_line / 4) + (int)x);
	img[index] = win->color;
}

void my_mlx_draw_line(void *mlx_ptr, void *win_ptr, double x1, double y1, double y2,void *img_ptr, t_win *win , t_wallhit *wallhit)
{
	(void)mlx_ptr, (void)win_ptr, (void)x1, (void)y1, (void)y2, (void)img_ptr, (void)wallhit, (void)win;
	double	virt_hight;
	// double	start;
	double	repeat_pixel;
	double	i;
	double	j;

	i = 0;
	virt_hight = ((double)(CELL_SIZE) / wallhit->dist) * 450;
	repeat_pixel = (virt_hight) / ((double)CELL_SIZE);
	j = 1 / (repeat_pixel);
	// start = ((double)WIN_HEIGHT / 2) - (virt_hight / 2);
	if (wallhit->ver_hor == 1)
		win->wall.x = ((int)(wallhit->x) % (int)CELL_SIZE);
	else
		win->wall.x = (int)(wallhit->y) % (int)CELL_SIZE;
	win->wall.y = 0;
	while (y1 < y2)
	{
		my_pixel_to_img(img_ptr, mlx_ptr, x1, y1, win_ptr, win);
		y1 += 1;
		i += j * 10;
		win->wall.y = i;
	}
}

t_wallhit	*vertical_check(t_win *win, double ray_angle, t_wallhit *ver)
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
		return (ver);
	}
	if (M_PI_2 < ray_angle && ray_angle < M_PI)
	{
		ver->x = ((win->playerX / win->cell_size) * win->cell_size) - 0.0001;
		ver->y = win->playerY - ((win->playerX - ver->x) * tan(M_PI - ray_angle));
		offset_x = -win->cell_size;
		offset_y = -(win->cell_size * tan(M_PI - ray_angle));
	}
	if (ray_angle == M_PI)
	{
		ver->x = ((win->playerX / win->cell_size) * win->cell_size) - 0.0001;
		ver->y = win->playerY;
		offset_x = -(win->cell_size);
		offset_y = 0;
	}
	if (M_PI < ray_angle && ray_angle < (3 * M_PI) / 2)
	{
		ver->x = ((win->playerX / win->cell_size) * win->cell_size) - 0.0001;
		ver->y = win->playerY + ((win->playerX - ver->x) * tan(ray_angle - M_PI));
		offset_x = -win->cell_size;
		offset_y = win->cell_size * tan(ray_angle - M_PI);
	}
	if (ray_angle == (3 * M_PI) / 2)
	{
		ver->dist = -1;
		return (ver);
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
		return (ver);
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
	ver->ver_hor = 0;
	return (ver);
}

t_wallhit	*horizontal_check(t_win *win, double ray_angle, t_wallhit *hor)
{
	double	offset_x = -1;
	double	offset_y = -1;
	hor->x = -1;
	hor->y = -1;
	
	double	adjacent = 0;
	double	opposite = 0;
	if (ray_angle == 0)
	{
		hor->dist = -1;
		return (hor);
	}
	if (0 < ray_angle && ray_angle < M_PI_2)
	{
		hor->y = ((win->playerY / win->cell_size) * win->cell_size) - 0.0001;
		hor->x = win->playerX + ((win->playerY - hor->y)/ tan(ray_angle));
		offset_x = win->cell_size / tan(ray_angle);
		offset_y = -win->cell_size;
	}
	if (ray_angle == M_PI_2)
	{
		hor->x = win->playerX;
		hor->y = ((win->playerY / win->cell_size) * win->cell_size) - 0.0001;///?
		offset_x = 0;
		offset_y = -win->cell_size;
	}
	if (M_PI_2 < ray_angle && ray_angle < M_PI)
	{
		hor->y = ((win->playerY / win->cell_size) * win->cell_size) - 0.0001;
		hor->x = win->playerX - ((win->playerY - hor->y) / tan(M_PI - ray_angle));
		offset_x = -(win->cell_size / tan(M_PI - ray_angle));
		offset_y = -win->cell_size;
	}
	if (ray_angle == M_PI)
	{
		hor->dist = -1;
		return (hor);
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
		return (hor);
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
	hor->ver_hor = 1;
	return (hor);
}

t_wallhit	*distance_getter(t_win *win, double ray_angle)
{
	t_wallhit *ver = (t_wallhit *)malloc(sizeof (t_wallhit));
	t_wallhit *hor = (t_wallhit *)malloc(sizeof (t_wallhit));

	ver = vertical_check(win, ray_angle, ver);
	hor = horizontal_check(win, ray_angle, hor);
	if (ver->dist < 0)
	{
		free(ver);
		return (hor);
	}
	else if (hor->dist < 0)
	{
		free(hor);
		return (ver);
	}
	else if (ver->dist < hor->dist)
	{
		free(hor);
		return (ver);
	}
	else if (ver->dist >= hor->dist)
	{
		free(ver);
		return (hor);
	}
	else
	{
		free(hor);
		ver->dist = -1;
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
	if (y < win->playerY / CELL_SIZE && wall_hit->ver_hor == 1)
		win->wall.text = win->img.EA;
	if (y > win->playerY / CELL_SIZE && wall_hit->ver_hor == 1)
		win->wall.text = win->img.NO;
	if (x > win->playerX / CELL_SIZE && wall_hit->ver_hor == 0)
		win->wall.text = win->img.SO;
	if (x < win->playerX / CELL_SIZE && wall_hit->ver_hor == 0)
		win->wall.text = win->img.WE;
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
	win->ray.ray_start = win->playerA - (win->fov_A / 2);
	win->ray.ray_end = win->playerA + (win->fov_A / 2);
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
	win->playerA = ray_correct(win->playerA);
}

void	player_render(t_win *win)
{
	t_wallhit *wall_hit;
	int	counter = 0;
	
	rays_init(win);
	while (win->ray.ray_temp < win->ray.ray_end)
	{
		win->ray.ray_angle = win->ray.ray_temp;
		win->ray.ray_angle = ray_correct(win->ray.ray_angle);
		wall_hit = distance_getter(win, win->ray.ray_angle);
		wall_hit->dist *= cos(win->playerA - win->ray.ray_angle);
		win->ray.wall_height = (win->cell_size / wall_hit->dist) * 450;
		win->ray.wall_top = ((double)WIN_HEIGHT / 2) - (win->ray.wall_height / 2);
    	win->ray.wall_bottom = ((double)WIN_HEIGHT / 2) + (win->ray.wall_height / 2);
		win->ray.wall_screen_x = win->ray.col;
		wall_put(win, wall_hit);
		floor_ceil(win, 1);
		floor_ceil(win, 0);
		my_mlx_draw_line(win->mlx_ptr, win->win_ptr, win->ray.wall_screen_x, win->ray.wall_top, win->ray.wall_bottom, win->img_ptr, win, wall_hit);
		win->ray.ray_temp += win->ray.ray_i;
		win->ray.col++;
		wall_hit->dist = -1;
		counter++;
	}
	map_draw(win);
	mlx_put_image_to_window(win->mlx_ptr, win->win_ptr, win->img_ptr, 0, 0);
}

void	ray_draw(t_win *win, double cell_x, double cell_y)
{
	double x = (win->mini_x / win->cell_size) * cell_x;
	double y = (win->mini_y / win->cell_size) * cell_y;
	double co = cos(-win->playerA);
	double si = sin(-win->playerA);
	int i = 0;
	int	j = 1;
	int	x_i = 1;
	int	m_y = 0;
	int	m_x = 0;
	while (i < 20)
	{
		if (i == 0)
		{
			m_y = y - 3;
			while (m_y < y + 3)
			{
				m_x = x - 3;
				while (m_x < x + 3)
				{
					mini_pixel_to_img(win, m_x, m_y, 0xFFF0FF);
					m_x += x_i;
				}
				m_y += j;
			}
		}
		mini_pixel_to_img(win,x, y , 0x00FF00);
		x += co;
		y += si;
		i += 1;
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
				mini_pixel_to_img(win, m_map->pixel_x + i, m_map->pixel_y + j, color);
				
			if (i + 1 == m_map->cell_x)
				mini_pixel_to_img(win, m_map->pixel_x + i, m_map->pixel_y + j, 0X000000);
			if (j + 1 == m_map->cell_y)
				mini_pixel_to_img(win, m_map->pixel_x + i, m_map->pixel_y + j, 0X000000);
			i++;
		}
		j++;
	}
}

void	map_draw(t_win *win)
{
	int	x;
	int	y;
	int	color;
	// int	i;
	// int	j;
	t_mini	m_map;
	// int	mini_height = 200;
	// int	mini_width = 300;

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
			// j = 0;
			// while (j < cell_y)
			// {
			// 	i = 0;
			// 	while (i < cell_x)
			// 	{
			// 		if (color != 0xFFF0FF)
			// 			mini_pixel_to_img(win, pixel_x + i, pixel_y + j, color);
						
			// 		if (i + 1 == cell_x)
			// 			mini_pixel_to_img(win, pixel_x + i, pixel_y + j, 0X000000);
			// 		if (j + 1 == cell_y)
			// 			mini_pixel_to_img(win, pixel_x + i, pixel_y + j, 0X000000);
			// 		i++;
			// 	}
			// 	j++;
			// }
			x++;
		}
		y++;
	}
	ray_draw(win, m_map.cell_x, m_map.cell_y);
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

void	x_y_update(t_win *win, double playerDX, double playerDY)
{
	int	new_x = ((win->playerX + (int)(playerDX * 5)));
	int	new_y = ((win->playerY + (int)(playerDY * 5)));
	if (win->map[(int)win->playerY / win->cell_size][(int)(new_x + (playerDX * 10)) / win->cell_size] != '1')
	{
		win->playerX = new_x;
		win->mini_x = new_x;
	}
	if (win->map[(int)(new_y + (playerDY * 10)) / win->cell_size][(int)win->playerX / win->cell_size] != '1')
	{
		win->playerY = new_y;
		win->mini_y = new_y;
	}
}

void	move_update(t_win *win, double playerDX, double playerDY)
{
	if (move_up == 0)
	{
		playerDX = cos(-win->playerA);
		playerDY = sin(-win->playerA);
		x_y_update(win, playerDX, playerDY);
	}
	if (move_down == 0)
	{
		playerDX = cos(-win->playerA + M_PI);
		playerDY = sin(-win->playerA + M_PI);
		x_y_update(win, playerDX, playerDY);
	}
	if (move_left == 0)
	{
		playerDX = cos(-win->playerA + M_PI_2);
		playerDY = sin(-win->playerA + M_PI_2);
		x_y_update(win, playerDX, playerDY);
	}
	if (move_right == 0)
	{
		playerDX = cos(-win->playerA - M_PI_2);
		playerDY = sin(-win->playerA - M_PI_2);
		x_y_update(win, playerDX, playerDY);
	}
}

int	 p_update(t_win *win)
{
	double	playerDX;
	double	playerDY;

	playerDX = 0;
	playerDY = 0;
	if (rotate_left == 0)
	{
		win->playerA -= 0.1;
		ray_correct(win->playerA);
	}
	if (rotate_right == 0)
	{
		win->playerA += 0.1;
		ray_correct(win->playerA);
	}
	move_update(win, playerDX, playerDY);
	player_render(win);
	return (1);
}

int	mouse_handler(int x, int y, t_win *win)
{
	double dx = x - WIN_WIDTH / 2;
    double dy = y - WIN_HEIGHT / 2;
    double angle = atan2(dx, dy);

    // Update the player's point of view (angle)
    win->playerA = angle;
	win->playerA = ray_correct(win->playerA);
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
	win.playerX = 4.5 * 32;
	win.playerY = 4.5 * 32;
	win.mini_x = 4.5 * 32;
	win.mini_y = 4.5 * 32;
	win.playerA = M_PI / 2;
	win.fov_A = (M_PI / 3);
	win.rays_A = WIN_WIDTH;
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
	win.img.EA = mlx_xpm_file_to_image(win.mlx_ptr, "./textures/EA.xpm", &w, &h);
	win.img.NO = mlx_xpm_file_to_image(win.mlx_ptr, "./textures/NO.xpm", &w, &h);
	win.img.SO = mlx_xpm_file_to_image(win.mlx_ptr, "./textures/SO.xpm", &w, &h);
	win.img.WE = mlx_xpm_file_to_image(win.mlx_ptr, "./textures/WE.xpm", &w, &h);
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