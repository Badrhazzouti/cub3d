/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 22:12:33 by bhazzout          #+#    #+#             */
/*   Updated: 2023/07/15 06:38:43 by bhazzout         ###   ########.fr       */
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
				color = 0xFFFFFF;
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
		// if (win->playerA < 0)
		// 	win->playerA += M_PI * 2;
		win->playerDX = cos(win->playerA);
		win->playerDY = sin(win->playerA);
	}
	if (rotate_right == 0)
	{
		rotate_right = 1;
		win->playerA += 0.1;
		// if (win->playerA > M_PI * 2)
		// 	win->playerA -= M_PI * 2;
		win->playerDX = cos(win->playerA);
		win->playerDY = sin(win->playerA);
	}
}

// void mlx_draw_line(void *mlx_ptr, void *win_ptr, int x1, int y1, int x2, int y2, int color, t_win *win)
// {
//     int dx = abs(x2 - x1);
//     int dy = abs(y2 - y1);
//     int sx = x1 < x2 ? 1 : -1;
//     int sy = y1 < y2 ? 1 : -1;
//     int err = dx - dy;

//     while (1)
//     {
// 		if ((int)(y1 / win->cell_size) >= 0 && (int)(y1 / win->cell_size) < win->map_height && (int)(x1 / win->cell_size) >= 0 && (int)(x1 / win->cell_size) < win->map_width && win->map[(int)(y1 / win->cell_size)][(int)(x1 / win->cell_size)] == '1')
// 		{
// 			// printf("haaaa\n");
// 			// distancetowall = sqrt(pow(win->playerX - x1, 2) + pow(win->playerY - y1, 2));
// 			printf("(%d)====(%d)\n", (int)(y1 / win->cell_size), (int)(x1 / win->cell_size));
// 			break;
// 		}
//         mlx_pixel_put(mlx_ptr, win_ptr, x1, y1, color);
// 		// sleep(1);
//         if ((x1 == x2 && y1 == y2))
//             break;

//        	int e2 = err * 2;

//         if (e2 > -dy)
//         {
//             err -= dy;
//             x1 += sx;
//         }

//         if (e2 < dx)
//         {
//             err += dx;
//             y1 += sy;
//         }
//     }
// 	// return (0);
// }

void mlx_draw_line(void *mlx_ptr, void *win_ptr, double x1, double y1, double x2, double y2, int color, t_win *win)
{
    double deltaX = x2 - x1;
    double deltaY = y2 - y1;

    double step = fabs(deltaX) > fabs(deltaY) ? fabs(deltaX) : fabs(deltaY);

    double xIncrement = deltaX / step;
    double yIncrement = deltaY / step;

    double currentX = x1;
    double currentY = y1;


    for (int i = 0; i <= step; i++)
    {
        // Draw pixel at (currentX, currentY)
		if ((int)(currentY / win->cell_size) >= 0 && (int)(currentY / win->cell_size) < win->map_height && (int)(currentX / win->cell_size) >= 0 && (int)(currentX / win->cell_size) < win->map_width && win->map[(int)(currentY / win->cell_size)][(int)(currentX / win->cell_size)] == '1')
		{
			break;
		}
		mlx_pixel_put(mlx_ptr, win_ptr, currentX, currentY, color);
        currentX += xIncrement;
        currentY += yIncrement;
    }
}

double distance_getter(double x1, double y1, double x2, double y2, t_win *win)
{
    double deltaX = x2 - x1;
    double deltaY = y2 - y1;
	double	distancetowall = 0;

    double step = fabs(deltaX) > fabs(deltaY) ? fabs(deltaX) : fabs(deltaY);

    double xIncrement = deltaX / step;
    double yIncrement = deltaY / step;

    double currentX = x1;
    double currentY = y1;

    for (int i = 0; i <= step; i++)
    {
        // Draw pixel at (currentX, currentY)
		if ((currentY / win->cell_size) >= 0 && (currentY / win->cell_size) < win->map_height && (currentX / win->cell_size) >= 0 && (currentX / win->cell_size) < win->map_width && win->map[(int)(currentY / win->cell_size)][(int)(currentX / win->cell_size)] == '1')
		{
			// printf("haaaa\n");
			distancetowall = sqrt(pow(win->playerX - currentX, 2) + pow(win->playerY - currentY, 2));
			break ;
		}
        currentX += xIncrement;
        currentY += yIncrement;
    }
	return (distancetowall);
}

// double distance_getter(int x1, int y1, int x2, int y2, t_win *win)
// {
//     int dx = abs(x2 - x1);
//     int dy = abs(y2 - y1);
//     int sx = x1 < x2 ? 1 : -1;
//     int sy = y1 < y2 ? 1 : -1;
//     int err = dx - dy;
// 	double	distancetowall;
// 	// int	index = 0;

//     while (1)
//     {
// 		if ((y1 / win->cell_size) >= 0 && (y1 / win->cell_size) < win->map_height && (x1 / win->cell_size) >= 0 && (x1 / win->cell_size) < win->map_width && win->map[(int)(y1 / win->cell_size)][(int)(x1 / win->cell_size)] == '1')
// 		{
// 			distancetowall = sqrt(pow(win->playerX - x1, 2) + pow(win->playerY - y1, 2));
// 			return (distancetowall);
// 		}
//         // mlx_pixel_put(mlx_ptr, win_ptr, x1, y1, color);
// 		// printf("(%d)====(%d)\n", (y1 / win->cell_size), (x1 / win->cell_size));
//         if ((x1 == x2 && y1 == y2))
//             break;

//         int e2 = err * 2;

//         if (e2 > -dy)
//         {
//             err -= dy;
//             x1 += sx;
//         }

//         if (e2 < dx)
//         {
//             err += dx;
//             y1 += sy;
//         }
// 		// index++;
//     }
// 	return (0);
// }

void	player_render(t_win *win, void *win_ptr, void *mlx_ptr)
{
	// mlx_pixel_put(mlx_ptr, win_ptr, win->playerX, win->playerY, 0x0000FF);
    double lineLength = 10000.0f;  // Length of the line to be drawn
	double	ray_a = win->fov_A / win->num_rays ;
	double	ray_angle = win->playerA - (win->fov_A / 2);
	double	distancetowall = 0;
	double	wall_height;
	// int	index = 0;
	for (int col = 0; col < win->num_rays; col++)
	{
		t_coor	end_coor;
		end_coor.endX = win->playerX + lineLength * cos(ray_angle);
		end_coor.endY = win->playerY + lineLength * sin(ray_angle);
		end_coor.rayX = win->playerX;
		end_coor.rayY = win->playerY;
		// distancetowall = distance_getter(&end_coor, win);
		distancetowall = distance_getter(win->playerX, win->playerY, end_coor.endX, end_coor.endY, win);
		printf("this is the distance (%f)\n", distancetowall);
		// mlx_draw_line(mlx_ptr, win_ptr, win->playerX, win->playerY, end_coor.endX, end_coor.endY, 0x0000FF, win);
		distancetowall *= cos(win->playerA - ray_angle);
		wall_height = WIN_HEIGHT * 500/ (win->cell_size * distancetowall);
		double wall_screen_x = col;
        // double wall_screen_y = (WIN_HEIGHT / 2) - (wall_height / 2);
		double wallTop = (WIN_HEIGHT - wall_height) / 2;
    	// Calculate the bottom position of the wall segment
    	double wallBottom = wallTop + wall_height;
		// (void)wall_screen_x, (void)wall_screen_y;
        // Draw the wall segment on the screen
        mlx_draw_line(mlx_ptr, win_ptr, wall_screen_x, wallTop, wall_screen_x, wallBottom, 0xFFFFFF, win);
		ray_angle += ray_a;
		// index++;
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
	win.playerX = 3.0 * win.cell_size;
	win.playerY = 2.0 * win.cell_size;
	win.playerA = M_PI;
	win.fov_A = M_PI / 3;
	win.rays_A = WIN_WIDTH;
	win.num_rays = 1200;
	win.playerDX = cos(win.playerA) * 1;
	win.playerDY = sin(win.playerA) * 1;
	
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