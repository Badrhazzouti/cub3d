/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 22:12:33 by bhazzout          #+#    #+#             */
/*   Updated: 2023/07/26 21:12:11 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	move_up = 1;
int	move_down = 1;
int	rotate_left = 1;
int	rotate_right = 1;


int	windows_close(int keycode, void *param)
{
	(void) keycode, (void) param;
	exit (0);
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
	if (move_up == 0)
	{
		move_up = 1;
		win->playerX += win->playerDX;
		win->playerY += win->playerDY;
		if (win->map[(int)(win->playerY / win->cell_size)][(int)(win->playerX / win->cell_size)] == '1')
		{
			win->playerX -= win->playerDX;
			win->playerY -= win->playerDY;
		}
	}
	if (move_down == 0)
	{
		move_down = 1;
		win->playerX -= win->playerDX;
		win->playerY -= win->playerDY;
		if (win->map[(int)(win->playerY / win->cell_size)][(int)(win->playerX / win->cell_size)] == '1')
		{
			win->playerX += win->playerDX;
			win->playerY += win->playerDY;
		}
	}
	if (rotate_left == 0)
	{
		rotate_left = 1;
		win->playerA -= 0.1;
		if (win->playerA < 0)
			win->playerA += M_PI * 2;
		win->playerDX = cos(win->playerA);
		win->playerDY = sin(win->playerA);
	}
	if (rotate_right == 0)
	{
		rotate_right = 1;
		win->playerA += 0.1;
		if (win->playerA >= M_PI * 2)
			win->playerA -= M_PI * 2;
		win->playerDX = cos(win->playerA);
		win->playerDY = sin(win->playerA);
	}
}

void	pixel_to_img(void *img_ptr, void *mlx_ptr, double x, double y, int color, void *win_ptr, t_win *win)
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

void 

mlx_draw_line(void *mlx_ptr, void *win_ptr, double x1, double y1, double x2, double y2, int color, t_win *win, void *img_ptr)
{
	(void)win, (void)x2, (void)img_ptr;
	while (y1 < y2)
	{
		mlx_pixel_put(mlx_ptr, win_ptr, x1, y1, color);
		// pixel_to_img(img_ptr, mlx_ptr, x1, y1, color, win_ptr, win);
		y1 += 1;
	}
}

double	ray_correct(double ray_angle)
{
	if (ray_angle < 0)
		ray_angle += M_PI * 2;
	if (ray_angle >= M_PI * 2)
		ray_angle -= M_PI * 2;
	return (ray_angle);
}

void	u_r_horizontal_checker(t_win *win, double ray_angle)
{
	double	x_n = 0;
	double	y_n = 0;
	// double	hypothenuse;
	// double	opposite;
	// double	adjacent;
	double	offset_x = 0;
	double	offset_y = 0;
	double	new_ray_angle;

	if (ray_angle == 0 || ray_angle == M_PI)
		return ;
	if (ray_angle > 0 && ray_angle < M_PI) // the player is facing up
	{
		y_n = ((int)(win->playerY / win->cell_size) * win->cell_size) - 0.0000001;
		if (ray_angle > 0 && ray_angle < M_PI / 2)//the player is facing right
		{
			x_n = (win->playerY - y_n) / tan(ray_angle) + win->playerX;
			offset_x = win->cell_size / tan(ray_angle);
		}
		else if (ray_angle == M_PI_2)
		{
			x_n = win->playerX;
			offset_x = 0;
		}
		else if (ray_angle > M_PI / 2 && ray_angle < M_PI)//the player is facing left
		{
			new_ray_angle = M_PI - ray_angle;
			x_n = win->playerX - ((win->playerY - y_n)/tan(new_ray_angle));
			offset_x = -(win->cell_size / tan(new_ray_angle));
		}
		offset_y = -win->cell_size;
	}
	else // the player is facing down
	{
		y_n = ((int)(win->playerY / win->cell_size) * win->cell_size) + win->cell_size;
		if (ray_angle > M_PI && ray_angle < (3/2 )* M_PI)//the player is facing left
		{
			new_ray_angle = ray_angle - M_PI;
			x_n = win->playerX - ((y_n - win->playerY) / tan(new_ray_angle));
			offset_x = (win->cell_size / tan(new_ray_angle)) * (-1);
		}
		else if (ray_angle == (3/2)*M_PI)
		{
			x_n = win->playerX;
			offset_x = 0;
		}
		else if (ray_angle > (3/2 )* M_PI && ray_angle < 2 * M_PI)//the player is facing right
		{
			new_ray_angle = ray_angle - (3/2 )* M_PI;
			x_n = tan(new_ray_angle) * (y_n - win->playerY) + win->playerX;
			offset_x = tan(new_ray_angle) * win->cell_size;
		}
		offset_y = win->cell_size;
	}
	// while ((int)(y_n / win->cell_size) >= 0 && (int)(y_n / win->cell_size) < win->map_height && (int)(x_n / win->cell_size) >= 0 
	// 	&& (int)(x_n / win->cell_size) < win->map_width && win->map[(int)(y_n / win->cell_size)][(int)(x_n / win->cell_size)] != '1')
	// {
	// 	x_n += offset_x;
	// 	y_n += offset_y;
	// }
	// if ((int)(y_n / win->cell_size) >= 0 && (int)(y_n / win->cell_size) < win->map_height && (int)(x_n / win->cell_size) >= 0 
	// 	&& (int)(x_n / win->cell_size) < win->map_width && win->map[(int)(y_n / win->cell_size)][(int)(x_n / win->cell_size)] == '1')
	// {
	// 	opposite = win->playerY - y_n;
	// 	adjacent = win->playerX - x_n;
	// 	hypothenuse = sqrt(pow(adjacent, 2) + pow(opposite, 2));//hypothenuse
	// 	win->distance_towall = hypothenuse;
	// }
}
void	 u_r_vertical_checker(t_win *win, double ray_angle)
{
	double	x_n = 0;
	double	y_n = 0;
	double	hypothenuse;
	double	opposite;
	double	adjacent;
	double	offset_x = 0;
	double	offset_y = 0;
	double	new_angle;

	if (ray_angle == M_PI / 2 || ray_angle == (3/2 )* M_PI)
	{
		return ;
	}
	if (ray_angle < M_PI / 2 || ray_angle > (3/2 )* M_PI)//the player is facing right
	{
		x_n = ((int)(win->playerX / win->cell_size) * win->cell_size) + win->cell_size;
		if (ray_angle > 0 && ray_angle < M_PI / 2)//the player is facing up
		{
			y_n = win->playerY - (tan(ray_angle) * (x_n - win->playerX));
			offset_y = -(tan(ray_angle) * win->cell_size);
		}
		else if (ray_angle == 0)
		{
			y_n = win->playerY;
			offset_y = 0;
		}
		else if (ray_angle > (3/2 )* M_PI && ray_angle < 2 * M_PI)//the player is facing down
		{
			new_angle = ray_angle - (3/2 ) * M_PI;
			y_n = (x_n - win->playerX) / tan(new_angle) + win->playerY;
			offset_y = win->cell_size / tan(new_angle);
		}
		offset_x = win->cell_size;
	}
	else// the player is facing left
	{
		x_n = ((int)(win->playerX / win->cell_size) * win->cell_size) - 1;
		if (ray_angle > M_PI / 2 && ray_angle < M_PI)//the player is facing up
		{
			new_angle = M_PI - ray_angle;
			y_n = win->playerY - tan(new_angle) * (win->playerX - x_n);
			offset_y = -(tan(new_angle) * win->cell_size);
		}
		else if (ray_angle == M_PI)
		{
			y_n = win->playerY;
			offset_y = 0;
		}
		else if (ray_angle > M_PI && ray_angle < (3/2 )* M_PI)//the player is facing down
		{
			new_angle = ray_angle - M_PI;
			y_n = tan(new_angle) * (x_n - win->playerX) + win->playerY;
			offset_y = tan(new_angle) * win->cell_size;
		}
		offset_x = -win->cell_size;
	}
	while ((int)(y_n / win->cell_size) >= 0 && (int)(y_n / win->cell_size) < win->map_height && (int)(x_n / win->cell_size) >= 0 
	&& (int)(x_n / win->cell_size) < win->map_width && win->map[(int)(y_n / win->cell_size)][(int)(x_n / win->cell_size)] != '1')
	{
		x_n += offset_x;
		y_n += offset_y;
	}	
	if ((int)(y_n / win->cell_size) >= 0 && (int)(y_n / win->cell_size) < win->map_height && (int)(x_n / win->cell_size) >= 0 
		&& (int)(x_n / win->cell_size) < win->map_width && win->map[(int)(y_n / win->cell_size)][(int)(x_n / win->cell_size)] == '1')
	{
		opposite = win->playerY - y_n;
		adjacent = win->playerX - x_n;
		hypothenuse = sqrt(pow(adjacent, 2) + pow(opposite, 2));//hypothenuse
		if (hypothenuse < win->distance_towall)
			win->distance_towall = hypothenuse;
		if (win->distance_towall < 0)
			win->distance_towall = hypothenuse;
	}
}

double	vertical_check(t_win *win, double ray_angle)
{
	double	offset_x = -1;
	double	offset_y = -1;
	double	first_x = -1;
	double	first_y = -1;
	// double	n_hypothenuse = 0;
	double	hypothenuse = 0;
	double	adjacent = 0;
	double	opposite = 0;

	if (ray_angle == 0)
	{
		first_x = ((win->playerX / win->cell_size) * win->cell_size) + win->cell_size;
		first_y = win->playerY;
		offset_x = win->cell_size;
		offset_y = 0;
	}
	if (0 < ray_angle && ray_angle < (M_PI / 2))
	{
		first_x = ((win->playerX / win->cell_size) * win->cell_size) + win->cell_size;
		first_y = win->playerY - ((first_x - win->playerX) * tan(ray_angle));
		offset_x = win->cell_size;
		offset_y = -(win->cell_size * tan(ray_angle));
	}
	if (ray_angle == M_PI_2)
		return (-1);
	if (M_PI_2 < ray_angle && ray_angle < M_PI)
	{
		first_x = ((win->playerX / win->cell_size) * win->cell_size) - 0.0000001;
		first_y = win->playerY - ((win->playerX - first_x) * tan(M_PI - ray_angle));
		offset_x = -win->cell_size;
		offset_y = -(win->cell_size * tan(M_PI - ray_angle));
	}
	if (ray_angle == M_PI)
	{
		first_x = ((win->playerX / win->cell_size) * win->cell_size) - 0.0000001;
		first_y = win->playerY;
		offset_x = -(win->cell_size);
		offset_y = 0;
	}
	if (M_PI < ray_angle && ray_angle < (3 * M_PI) / 2)
	{
		first_x = ((win->playerX / win->cell_size) * win->cell_size) - 0.0000001;
		first_y = win->playerY + ((win->playerX - first_x) * tan(ray_angle - M_PI));
		offset_x = -win->cell_size;
		offset_y = win->cell_size * tan(ray_angle - M_PI);
	}
	if (ray_angle == (3 * M_PI) / 2)
		return (-1);
	if ((3 * M_PI) / 2 < ray_angle && ray_angle < (2 * M_PI))
	{
		first_x = ((win->playerX / win->cell_size) * win->cell_size) + win->cell_size;
		first_y = win->playerY + ((first_x - win->playerX) * tan((2 * M_PI) - ray_angle));
		offset_x =  win->cell_size;
		offset_y = win->cell_size * tan((2 * M_PI) - ray_angle);
	}
	if (first_x < 0 || first_y < 0)
		return (-1);
	char map_value;
	while (first_x != -1 && (int)first_y / win->cell_size < win->map_height
		&& (int)first_y / win->cell_size >= 0 && (size_t)(first_x
		/ win->cell_size) < ft_strlen((win->map)[(int)(first_y
				/ win->cell_size)]))
	{
		map_value = (win->map)[(int)(first_y
				/ win->cell_size)][(int)(first_x / win->cell_size)];
		if (map_value == '1' || map_value == ' ')
			break ;
		first_x += offset_x;
		first_y += offset_y;
	}

	// while ((int)(first_y / win->cell_size) >= 0 && (int)(first_y / win->cell_size) < win->map_height && (int)(first_x / win->cell_size) >= 0 
	// && (int)(first_x / win->cell_size) < win->map_width && win->map[(int)(first_y / win->cell_size)][(int)(first_x / win->cell_size)] != '1')
	// {
	// 	first_x += offset_x;
	// 	first_y += offset_y;
	// }	
	// if ((int)(first_y / win->cell_size) >= 0 && (int)(first_y / win->cell_size) < win->map_height && (int)(first_x / win->cell_size) >= 0 
	// 	&& (int)(first_x / win->cell_size) < win->map_width && win->map[(int)(first_y / win->cell_size)][(int)(first_x / win->cell_size)] == '1')
	// {
	// 	if (win->playerY >= first_y)
	// 		opposite = win->playerY - first_y;
	// 	if (win->playerY < first_y)
	// 		opposite = first_y - win->playerY;
		
		opposite = win->playerY - first_y;
		adjacent = win->playerX - first_x;
		hypothenuse = sqrt(pow(adjacent, 2) + pow(opposite, 2));//hypothenuse
	return (hypothenuse);
	// }
	
	// while ((int)first_y >= 0 && (int)first_y < win->map_height && (int)first_x >= 0 
	// 	&& (int)first_x < win->map_width && win->map[(int)first_y][(int)first_x] == '0')
	// {
	// 	first_x += offset_x;
	// 	first_y += offset_y;
	// 	n_hypothenuse += hypothenuse;
	// }
	// printf("distance is (%d)\n", (int)first_x);
	// 	printf("distance (y) is (%d)\n", (int)first_y);
	// if ((int)first_y >= 0 && (int)first_y < win->map_height && (int)first_x >= 0 
	// 	&& (int)first_x < win->map_width && win->map[(int)first_y][(int)first_x] == '1')
	// {
	// 	win->distance_towall = n_hypothenuse;
	// }
}

double	horizontal_check(t_win *win, double ray_angle)
{
	double	offset_x = -1;
	double	offset_y = -1;
	double	first_x = -1;
	double	first_y = -1;
	// double	n_hypothenuse = 0;
	double	hypothenuse = 0;
	double	adjacent = 0;
	double	opposite = 0;
	if (ray_angle == 0)
		return (-1);
	if (0 < ray_angle && ray_angle < M_PI_2)
	{
		first_y = ((win->playerY / win->cell_size) * win->cell_size) - 0.0000001;
		first_x = win->playerX + ((win->playerY - first_y)/ tan(ray_angle));
		offset_x = win->cell_size / tan(ray_angle);
		offset_y = -win->cell_size;
	}
	if (ray_angle == M_PI_2)
	{
		first_x = win->playerX;
		first_y = ((win->playerY / win->cell_size) * win->cell_size) - 1;
		offset_x = 0;
		offset_y = -win->cell_size;
	}
	if (M_PI_2 < ray_angle && ray_angle < M_PI)
	{
		first_y = ((win->playerY / win->cell_size) * win->cell_size) - 0.0000001;
		first_x = win->playerX - ((win->playerY - first_y) / tan(M_PI - ray_angle));
		offset_x = -(win->cell_size / tan(M_PI - ray_angle));
		offset_y = -win->cell_size;
	}
	if (ray_angle == M_PI)
		return (-1);
	if (M_PI < ray_angle && ray_angle < (3 * M_PI) / 2)
	{
		first_y = ((win->playerY / win->cell_size) * win->cell_size) + win->cell_size;
		first_x = win->playerX - ((first_y - win->playerY) / tan(ray_angle - M_PI));
		offset_x = -(win->cell_size / tan(ray_angle - M_PI));
		offset_y = win->cell_size;
	}
	if (ray_angle == (3 * M_PI) / 2)
	{
		first_x = win->playerX;
		first_y = ((win->playerY / win->cell_size) * win->cell_size) + win->cell_size;
		offset_x = 0;
		offset_y =  win->cell_size;
	}
	if ((3 * M_PI) / 2 < ray_angle && ray_angle < (2 * M_PI))
	{
		first_y = ((win->playerY / win->cell_size) * win->cell_size) + win->cell_size;
		first_x = win->playerX + ((first_y - win->playerY) / tan((2 * M_PI) - ray_angle));
		offset_x = win->cell_size / tan((2 * M_PI) - ray_angle);
		offset_y = win->cell_size;
	}
	if (first_x == -1 || first_y == -1)
		return (-1);
	char map_value;
	while (first_x != -1 && (size_t)(first_x)
		/ win->cell_size < ft_strlen((win->map)[(int)(first_y / win->cell_size)]))
	{
		map_value = (win->map)[(int)(first_y
				/ win->cell_size)][(int)(first_x / win->cell_size)];
		if (map_value == '1' || map_value == ' ')
			break ;
		first_x += offset_x;
		first_y += offset_y;
	}
	// while ((int)(first_y / win->cell_size) >= 0 && (int)(first_y / win->cell_size) < win->map_height && (int)(first_x / win->cell_size) >= 0 
	// 	&& (int)(first_x / win->cell_size) < win->map_width && win->map[(int)(first_y / win->cell_size)][(int)(first_x / win->cell_size)] != '1')
	// {
	// 	first_x += offset_x;
	// 	first_y += offset_y;
	// }
	// if ((int)(first_y / win->cell_size) >= 0 && (int)(first_y / win->cell_size) < win->map_height && (int)(first_x / win->cell_size) >= 0 
	// 	&& (int)(first_x / win->cell_size) < win->map_width && win->map[(int)(first_y / win->cell_size)][(int)(first_x / win->cell_size)] == '1')
	// {
		opposite = win->playerY - first_y;
		adjacent = win->playerX - first_x;
		hypothenuse = sqrt(pow(adjacent, 2) + pow(opposite, 2));//hypothenuse
		return (hypothenuse);

	// }
	// while ((int)first_y >= 0 && (int)first_y < win->map_height && (int)first_x >= 0 
	// 	&& (int)first_x < win->map_width && win->map[(int)first_y][(int)first_x] != '1')
	// {
	// 	first_x += offset_x;
	// 	first_y += offset_y;

	// 	n_hypothenuse += hypothenuse;
	// }
	// if ((int)first_y >= 0 && (int)first_y < win->map_height && (int)first_x >= 0 && (int)first_x < win->map_width && win->map[(int)first_y][(int)first_x] == '1')
	// {
	// 	if (win->distance_towall > n_hypothenuse)
	// 		win->distance_towall = n_hypothenuse;
	// }
}

double	distance_getter(t_win *win, double ray_angle)
{
	double ver_dist;
	double hor_dist;
	// u_r_horizontal_checker(win, ray_angle);
	// u_r_vertical_checker(win, ray_angle);
	// double	first_x = win->playerX + x_d * cos(ray_angle);
	// double	first_x = win->playerX - y_d * sin(ray_angle);
	ver_dist = vertical_check(win, ray_angle);
	hor_dist = horizontal_check(win, ray_angle);
	if (ver_dist < 0 && hor_dist < 0)
		printf("fuuuuuuuck\n");
	if (ver_dist < 0)
		return (hor_dist);
	if (hor_dist < 0)
		return (ver_dist);
	if (ver_dist < hor_dist)
		return (ver_dist);
	if (ver_dist >= hor_dist)
		return (hor_dist);
	return (-1);
}

void	player_renderer(t_win *win, void *win_ptr, void *mlx_ptr)
{
	// mlx_pixel_put(mlx_ptr, win_ptr, win->playerX, win->playerY, 0x0000FF);
    double lineLength = 10000.0f;  // Length of the line to be drawn
	double	ray_a = win->fov_A / win->num_rays ;
	double	wall_height;
	double	ray_angle = win->playerA - (win->fov_A / 2.0f);
	void	*img_ptr;
	img_ptr = mlx_new_image(mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	for (double col = 0; col < win->num_rays; col += 1)
	{
		double	endX;
		double	endY;
		ray_angle = ray_correct(ray_angle);
		endX = win->playerX + lineLength * cos(ray_angle);
		endY = win->playerY + lineLength * sin(ray_angle);
		// distance_getter_old(win->playerX, win->playerY, endX, endY, win, ray_angle);
		distance_getter(win, ray_angle);
		win->distance_towall *= cos(win->playerA - ray_angle);
		wall_height = (win->cell_size / win->distance_towall) * 554;
		// wall_height = 100;
		double wall_screen_x = col;
		double wallTop = (WIN_HEIGHT - wall_height) / 2;
    	double wallBottom = wallTop + wall_height;
        mlx_draw_line(mlx_ptr, win_ptr, wall_screen_x, 0, wall_screen_x, wallTop, 0xFFFFFF, win, img_ptr);
		mlx_draw_line(mlx_ptr, win_ptr, wall_screen_x, wallTop, wall_screen_x, wallBottom, 0x000000, win, img_ptr);
		mlx_draw_line(mlx_ptr, win_ptr, wall_screen_x, wallBottom, wall_screen_x, WIN_HEIGHT, 0xFF0000, win, img_ptr);
		ray_angle += ray_a;
	}
	mlx_put_image_to_window(mlx_ptr, win_ptr, img_ptr, 0, 0);
	mlx_destroy_image(mlx_ptr, img_ptr);
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
	// double	linelength = 10000.0f;
	double	ray_incrementation = 0.00087266462;
	double	ray_start = win->playerA - (M_PI / 3);
	double	ray_end = win->playerA + (M_PI / 3);
	double	ray_angle = ray_start;
	double	ray_temp = ray_start;
	void	*img_ptr = NULL;
	double	wall_height;
	double	col = 0;
	double	wall_screen_x = 0;
	// img_ptr = mlx_new_image(mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	while (ray_angle < ray_end)
	{
		win->distance_towall = -1;
		ray_angle = ray_temp;
		ray_angle = ray_correct(ray_angle);
		win->distance_towall = distance_getter(win, ray_angle);
		// win->distance_towall *= cos(win->playerA - ray_angle));
		wall_height = (win->cell_size / win->distance_towall) * 554;
		draw_ray(mlx_ptr, win_ptr, win->distance_towall, ray_angle);
		// printf("distance to wall  %f\n", win->distance_towall);
		// wall_height = 100;
		double wallTop = (WIN_HEIGHT / 2) - (wall_height / 2);
		wall_screen_x = col;
    	double wallBottom = wallTop + wall_height;
        // mlx_draw_line(mlx_ptr, win_ptr, wall_screen_x, 0, wall_screen_x, wallTop, 0xFF00FF, win, img_ptr);
		mlx_draw_line(mlx_ptr, win_ptr, wall_screen_x, wallTop, wall_screen_x, wallBottom, 0x00FF00, win, img_ptr);
		// mlx_draw_line(mlx_ptr, win_ptr, wall_screen_x, wallBottom, wall_screen_x, WIN_HEIGHT, 0xFF0000, win, img_ptr);
		ray_temp += ray_incrementation;
		col++;
	}
	
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
		move_up = 0;
	if (keycode == 0)//A
		rotate_left = 0;
	if (keycode == 2)//D
		rotate_right = 0;
	if (keycode == 1)//S
		move_down = 0;
	mlx_clear_window(win->mlx_ptr, win->win_ptr);
	// map_draw(*win, win->win_ptr, win->mlx_ptr, win->map);
	p_update(win);
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
	win.num_rays = 1200;
	win.playerDX = cos(win.playerA);
	win.playerDY = sin(win.playerA);
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