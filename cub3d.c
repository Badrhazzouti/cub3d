/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 22:12:33 by bhazzout          #+#    #+#             */
/*   Updated: 2023/07/14 01:29:21 by bhazzout         ###   ########.fr       */
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

void	line_draw(t_win win, void *win_ptr, void *mlx_ptr, int x, int y, int j)
{
	
	int i = 0;
	int pixel_x = win.cell_size * x;
	int pixel_y = win.cell_size * y;
	while (i < win.cell_size)
	{
		mlx_pixel_put(mlx_ptr, win_ptr, pixel_x + i, pixel_y + j, 0xFF0000);
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
	float	move_speed;
	// float	y_move;
	// float	x_move;

	// y_move = 0;
	// x_move = 0;
	move_speed = 1.0;
	if (move_up == 0)
	{
		win->playerX += win->playerDX;
		win->playerY += win->playerDY;
		move_up = 1;
	}
	if (move_down == 0)
	{
		win->playerX -= win->playerDX;
		win->playerY -= win->playerDY;
		move_down = 1;
	}
	if (rotate_left == 0)
	{
		win->playerA -= 0.1;
		// if (win->playerA < 0)
		// 	win->playerA += M_PI * 2;
		win->playerDX = cos(win->playerA) * 1;
		win->playerDY = sin(win->playerA) * 1;
		rotate_left = 1;
	}
	if (rotate_right == 0)
	{
		win->playerA += 0.1;
		// if (win->playerA > M_PI * 2)
		// 	win->playerA -= M_PI * 2;
		win->playerDX = cos(win->playerA) * 1;
		win->playerDY = sin(win->playerA) * 1;
		rotate_right = 1;
	}
	// win->playerX += x_move;
	// win->playerY += y_move;
	
}

void mlx_draw_line(void *mlx_ptr, void *win_ptr, int x1, int y1, int x2, int y2, int color, t_win *win)
{
	(void)win;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
		if (win->map[(int)(y1 / win->cell_size)][(int)(x1 / win->cell_size)] == '1')
			break;
        mlx_pixel_put(mlx_ptr, win_ptr, x1, y1, color);
		// printf("(%d)====(%d)\n", (y1 / win->cell_size), (x1 / win->cell_size));
        if ((x1 == x2 && y1 == y2))
            break;

        int e2 = err * 2;

        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void	player_render(t_win *win, void *win_ptr, void *mlx_ptr)
{
	mlx_pixel_put(mlx_ptr, win_ptr, win->playerX, win->playerY, 0xFF0000);
    float lineLength = 10000.0f;  // Length of the line to be drawn
    float endX = win->playerX + lineLength * cos(win->playerA);
    float endY = win->playerY + lineLength * sin(win->playerA);
    // mlx_draw_line(mlx_ptr, win_ptr, win->playerX, win->playerY, endX, endY, 0x0000FF, win);
	// float ray_a = win->fov_A / win->num_rays;
	for (int col = 0; col < win->num_rays; col++)
	{
		
		mlx_draw_line(mlx_ptr, win_ptr, win->playerX, win->playerY, endX, endY, 0x0000FF, win);
		float rayX = win->playerX;
		float rayY = win->playerY;
		float distancetowall = 0;
		int hitwall = 0;
		while (hitwall == 0 && distancetowall < WIN_WIDTH)
		{
			rayX += cos(win->playerA);
			rayY += sin(win->playerA);
			// int mapx = (int)(rayX / (win->cell_size));
			// int mapy = (int)(rayY / (win->cell_size));
			// printf("(%d)(%d)\n", mapx, mapy);
			// if (mapx >= 0 && mapx < win->map_width && mapy >= 0 && mapy < win->map_height && win->map[mapy][mapx] == 1)
            // {
            // 	printf("Column: %d, Distance: %.2f\n", col, distancetowall);
                distancetowall = sqrt(pow(win->playerX - rayX, 2) + pow(win->playerY- rayY, 2));
                // hitwall = 1;

                // Draw the wall column based on distanceToWall
				mlx_draw_line(mlx_ptr, win_ptr, win->playerX, win->playerY, endX, endY, 0x0000FF, win);
            // }

			distancetowall += 1.0f;
		}
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
	map_draw(*win, win->win_ptr, win->mlx_ptr, win->map);
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
	win.cell_size = 20;
	win.map_height = 11;
	win.map_width = 25;
	win.playerX = 3.0 * win.cell_size;
	win.playerY = 4.0 * win.cell_size;
	win.playerA = 0;
	win.fov_A = 60;
	win.rays_A = WIN_WIDTH;
	win.num_rays = 1200;
	win.playerDX = cos(win.playerA) * 1;
	win.playerDY = sin(win.playerA) * 1;
	
	win.mlx_ptr = mlx_init();
	win.f_map = one_map();
	win.map = map_getter();
	// map_printer(map);
	win.win_ptr = mlx_new_window(win.mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "CUB3D");
	map_draw(win, win.win_ptr, win.mlx_ptr, win.map);
	player_render(&win, win.win_ptr, win.mlx_ptr);
	// mlx_key_hook(win.win_ptr, &key_handler, &win);
	mlx_hook(win.win_ptr, 2, 0, &key_handler, &win);
	// mlx_key_hook(win.win_ptr, &key_release_handler, &win);
	mlx_hook(win.win_ptr, 17, 0L, &windows_close, NULL);
	// while (1)
	// {
	// 	mlx_clear_window(mlx_ptr, win_ptr);
	// 	p_update(win);
	// 	player_render(win, win_ptr, mlx_ptr);
	// 	usleep(10000);
	// }
	mlx_loop(win.mlx_ptr);
}