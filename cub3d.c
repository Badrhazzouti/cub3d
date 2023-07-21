/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 22:12:33 by bhazzout          #+#    #+#             */
/*   Updated: 2023/07/21 22:54:46 by bhazzout         ###   ########.fr       */
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
		printf("%s", map[i]);
		i++;
	}
}

// void	line_draw(t_win win, void *win_ptr, void *mlx_ptr, int x, int y, int j)
// {
	
// 	int i = 0;
// 	int pixel_x = win.cell_size * x;
// 	int pixel_y = win.cell_size * y;
// 	while (i < win.cell_size)
// 	{
// 		mlx_pixel_put(mlx_ptr, win_ptr, pixel_x + i, pixel_y + j, 0xFF0000);
// 		i++;
// 	}
// }

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

void	p_update(t_win *win)
{
	int hitwall = 0;
	// if (win->map[(int)(win->playerY / win->cell_size)][(int)(win->playerX / win->cell_size)] == '1')
	// 	hitwall = 1;
	if (move_up == 0)
	{
		move_up = 1;
		win->playerX += win->playerDX;
		win->playerY += win->playerDY;
		if (win->map[(int)(win->playerY / win->cell_size)][(int)(win->playerX / win->cell_size)] == '1')
		{
			hitwall = 1;
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
			hitwall = 1;
			win->playerX += win->playerDX;
			win->playerY += win->playerDY;
		}
	}
	if (rotate_left == 0 && hitwall == 0)
	{
		rotate_left = 1;
		win->playerA -= 0.1;
		if (win->playerA < 0)
			win->playerA += M_PI * 2;
		win->playerDX = cos(win->playerA);
		win->playerDY = sin(win->playerA);
	}
	if (rotate_right == 0 && hitwall == 0)
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

void mlx_draw_line(void *mlx_ptr, void *win_ptr, double x1, double y1, double x2, double y2, int color, t_win *win, void *img_ptr)
{
	(void)win, (void)x2, (void)img_ptr;
	while (y1 < y2)
	{
		pixel_to_img(img_ptr, mlx_ptr, x1, y1, color, win_ptr, win);
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

void distance_getter_old(double x1, double y1, double x2, double y2, t_win *win, double ray_angle)
{
	(void)ray_angle;
	// (void)x1, (void)y1,(void)x2, (void)y2;
	// ray_angle = ray_correct(win->playerA);
	// double	ray_dx = cos(ray_angle);
	// double	ray_dy = sin(ray_angle);

	// double	ray_x = win->playerX;
	// double	ray_y = win->playerY;

	// double	deltax = fabs(win->cell_size / ray_dx);
	// double	deltay = fabs(win->cell_size / ray_dy);

	// double stepx = (ray_dx > 0) ? 1 : -1;
	// double stepy = (ray_dy > 0) ? 1 : -1;

	// while (ray_x >= 0 && ray_x < WIN_WIDTH && ray_y >= 0 && ray_y < WIN_HEIGHT)
	// {
	// 	int mapx = (int)(ray_x / win->cell_size);
	// 	int mapy = (int)(ray_y / win->cell_size);
	// 	if (win->map[mapy][mapx] == '1')
	// 	{
	// 		win->distance_towall = sqrt(pow(ray_x - win->playerX, 2) + pow(ray_y - win->playerY, 2));
	// 		break;
	// 	}
	// 	// Move to the next grid cell
    //     double nextX = ray_x + stepx * deltax;
    //     double nextY = ray_y + stepy * deltay;

    //     // Choose the smaller delta to step along the ray
    //     if (fabs(nextX - ray_x) < fabs(nextY - ray_y))
    //         ray_x = nextX;
    //     else
    //         ray_y = nextY;
	// }

	// double stepX;
	// double stepY;

	// double	raydx = cos(ray_angle);
	// double	raydy = sin(ray_angle);
	// if (raydx < 0)
	// 	stepX = -1;
	// else
	// 	stepX = 1;
	// if (raydy < 0)
	// 	stepY = -1;
	// else
	// 	stepY = 1;
	// // double deltaX = win->cell_size / fabs(raydx);
	// // double deltaY = win->cell_size / fabs(raydy);

	// double nextHorizontalHitX;
    // double nextHorizontalHitY;
    // double nextVerticalHitX;
    // double nextVerticalHitY;

    // if (stepX == -1) {
    //     nextHorizontalHitX = floor(win->playerX / win->cell_size) * win->cell_size;
    // } else {
    //     nextHorizontalHitX = (floor(win->playerX / win->cell_size) + 1) * win->cell_size;
    // }

    // if (stepY == -1) {
    //     nextHorizontalHitY = floor(win->playerY / win->cell_size) * win->cell_size;
    // } else {
    //     nextHorizontalHitY = (floor(win->playerY / win->cell_size) + 1) * win->cell_size;
    // }

    // if (stepX == -1) {
    //     nextVerticalHitX = floor(win->playerX / win->cell_size) * win->cell_size;
    // } else {
    //     nextVerticalHitX = (floor(win->playerX / win->cell_size) + 1) * win->cell_size;
    // }

    // if (stepY == -1) {
    //     nextVerticalHitY = floor(win->playerY / win->cell_size) * win->cell_size;
    // } else {
    //     nextVerticalHitY = (floor(win->playerY / win->cell_size) + 1) * win->cell_size;
    // }
	
	// while (1) {
    //     // Find the distance to the next horizontal and vertical intersections
    //     double distToHorizontalHitX = (nextHorizontalHitX - win->playerX) / raydx;
    //     double distToHorizontalHitY = (nextHorizontalHitY - win->playerY) / raydy;

    //     double distToVerticalHitX = (nextVerticalHitX - win->playerX) / raydx;
    //     double distToVerticalHitY = (nextVerticalHitY - win->playerY) / raydy;

    //     // Choose the smallest distance (next intersection) between horizontal and vertical
    //     double distToNextIntersection;
    //     int hitVerticalWall = 0;

    //     if (distToHorizontalHitX < distToVerticalHitX) {
    //         distToNextIntersection = distToHorizontalHitX;
    //         hitVerticalWall = 0;
    //     } else {
    //         distToNextIntersection = distToVerticalHitX;
    //         hitVerticalWall = 1;
    //     }

    //     if (distToHorizontalHitY < distToVerticalHitY) {
    //         if (distToHorizontalHitY < distToNextIntersection) {
    //             distToNextIntersection = distToHorizontalHitY;
    //             hitVerticalWall = 0;
    //         }
    //     } else {
    //         if (distToVerticalHitY < distToNextIntersection) {
    //             distToNextIntersection = distToVerticalHitY;
    //             hitVerticalWall = 1;
    //         }
    //     }

    //     // Calculate the coordinates of the intersection point
    //     double intersectionX = win->playerX + raydx * distToNextIntersection;
    //     double intersectionY = win->playerY + raydy * distToNextIntersection;

    //     // Now you can use the intersection coordinates to check for wall hits,
    //     // calculate wall heights, and render the walls.

    //     // ...

    //     // Move to the next intersection
    //     if (hitVerticalWall) {
    //         nextVerticalHitX += stepX * win->cell_size;
    //         nextVerticalHitY += stepY * distToNextIntersection;
    //     } else {
    //         nextHorizontalHitX += stepX * distToNextIntersection;
    //         nextHorizontalHitY += stepY * win->cell_size;
    //     }
	// 	if (intersectionX < 0 || intersectionX >= WIN_WIDTH || intersectionY < 0 || intersectionY >= WIN_HEIGHT || win->map[(int)(intersectionY / win->cell_size)][(int)(intersectionX / win->cell_size) == '1'])
	// 	{
	// 		win->distance_towall = sqrt(pow(win->playerX - intersectionX, 2) + pow(win->playerY - intersectionY, 2));
	// 		printf("distance is (%f)\n", win->distance_towall);
	// 		break ;
	// 	}
    // }
	
    double deltaX = x2 - x1;
    double deltaY = y2 - y1;
	// double stepx;
	// double stepy;
	
	// if (deltaX < 0)
		// stepx = deltaX / fabs(deltaX);
	// else
		// stepx = 0.008;
	// if (deltaY < 0)
		// stepy = deltaY / fabs(deltaY);
	// else
		// stepy = -0.008;
	
    double stepX = fabs(deltaX);
	double stepY = fabs(deltaY);
	double step;
	
	if (stepX > stepY)
		step = stepX;
	if (stepX < stepY)
		step = stepY;

    double xIncrement = deltaX / step;
    double yIncrement = deltaY / step;
	printf("x increment (%f) ===== y increment (%f)\n", xIncrement, yIncrement);
    double currentX = x1;
    double currentY = y1;
	while((currentY) >= 0 && (currentY) < WIN_HEIGHT && (currentX) >= 0 && (currentX) < WIN_WIDTH)
	{
		if (win->map[(int)(currentY / win->cell_size)][(int)(currentX / win->cell_size)] == '1')
		{
			double	determinant_x;
			double	determinant_y;
			if (win->playerX > currentX)
				determinant_x = win->playerX - currentX;
			else
				determinant_x = currentX - win->playerX;
			if (win->playerY > currentY)
				determinant_y = win->playerY - currentY;
			else
				determinant_y = currentY - win->playerY;
			win->distance_towall = sqrt(pow(determinant_x, 2) + pow(determinant_y, 2));
			// win->distance_towall = sqrt((win->playerX - currentX) * (win->playerX - currentX) + (win->playerY - currentY) * (win->playerX - currentY));
			break ;
		}
		currentX += xIncrement;
		currentY += yIncrement;
	}
}

void	u_r_horizontal_check(t_win *win, double ray_angle)
{
	double	x_n = 0;
	double	y_n = 0;
	double	hypothenuse;
	double	opposite;
	double	adjacent;
	double	offset_x;
	double	offset_y;
	double	new_ray_angle;

	if (ray_angle == 0 || ray_angle == M_PI)
	{
		// win->distance_towall = 0;
		return ;
	}
	if (ray_angle > 0 && ray_angle < M_PI) // the player is facing up
	{
		y_n = ((int)(win->playerY / win->cell_size) * win->cell_size) - 1;
		if (ray_angle > 0 && ray_angle < M_PI / 2)//the player is facing right
		{
			x_n = (win->playerY - y_n) / tan(ray_angle) + win->playerX;
			offset_x = win->cell_size / tan(ray_angle);
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
		else if (ray_angle > (3/2 )* M_PI && ray_angle < 2 * M_PI)//the player is facing right
		{
			new_ray_angle = ray_angle - (3/2 )* M_PI;
			x_n = tan(new_ray_angle) * (y_n - win->playerY) + win->playerX;
			offset_x = tan(new_ray_angle) * win->cell_size;
		}
		offset_y = win->cell_size;
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
		win->distance_towall = hypothenuse;
		return ;
	}
}


void	 u_r_vertical_check(t_win *win, double ray_angle)
{
	double	x_n = 0;
	double	y_n = 0;
	double	hypothenuse;
	double	opposite;
	double	adjacent;
	double	offset_x;
	double	offset_y;
	double	new_angle;

	if (ray_angle == M_PI / 2 || ray_angle == (3/2 )* M_PI)
	{
		// win->distance_towall = 0;
		return ;
	}
	if (ray_angle < M_PI / 2 || ray_angle > (3/2 )* M_PI)//the player is facing right
	{
		x_n = ((int)(win->playerX / win->cell_size) * win->cell_size) + win->cell_size;
		if (ray_angle > 0 && ray_angle < M_PI / 2)//the player is facing up
		{
			y_n = win->playerY - tan(x_n - win->playerX);
			offset_y = -(tan(ray_angle) * win->cell_size);
		}
		else if (ray_angle > (3/2 )* M_PI && ray_angle < 2 * M_PI)//the player is facing down
		{
			new_angle = ray_angle - (3/2 )* M_PI;
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
			y_n = win->playerY - tan(win->playerX - x_n);
			offset_y = -(tan(new_angle) * win->cell_size);
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
		return ;
	}
}

void	distance_getter(t_win *win, double ray_angle)
{
	u_r_horizontal_check(win, ray_angle);
	u_r_vertical_check(win, ray_angle);
}

void	player_render(t_win *win, void *win_ptr, void *mlx_ptr)
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
		distance_getter_old(win->playerX, win->playerY, endX, endY, win, ray_angle);
		// distance_getter(win, ray_angle);
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
	win.cell_size = 50.0f;
	win.map_height = 11;
	win.map_width = 25;
	win.playerX = 3.5 * win.cell_size;
	win.playerY = 2.5 * win.cell_size;
	win.playerA = M_PI / 2;
	win.fov_A = 60 * (M_PI / 180);
	win.rays_A = WIN_WIDTH;
	win.num_rays = 1200;
	win.playerDX = cos(win.playerA);
	win.playerDY = sin(win.playerA);
	win.distance_towall = 0;
	
	win.mlx_ptr = mlx_init();
	win.f_map = one_map();
	win.map = map_getter();
	win.win_ptr = mlx_new_window(win.mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "CUB3D");
	// map_draw(win, win.win_ptr, win.mlx_ptr, win.map);
	player_render(&win, win.win_ptr, win.mlx_ptr);
	mlx_hook(win.win_ptr, 2, 0, &key_handler, &win);
	// mlx_key_hook(win.win_ptr, &key_release_handler, &win);
	mlx_hook(win.win_ptr, 17, 0L, &windows_close, NULL);
	mlx_loop(win.mlx_ptr);
}