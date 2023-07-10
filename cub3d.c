/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 22:12:33 by bhazzout          #+#    #+#             */
/*   Updated: 2023/07/10 03:42:28 by bhazzout         ###   ########.fr       */
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
						mlx_pixel_put(mlx_ptr, win_ptr, pixel_x + i, pixel_y + j, 0X000000);
					if (j + 1 == win.cell_size)
						mlx_pixel_put(mlx_ptr, win_ptr, pixel_x + i, pixel_y + j, 0X000000);
					i++;
				}
				j++;
				if (j == win.cell_size)
					line_draw(win, win_ptr, mlx_ptr, x, y, j);
			}
			x++;
		}
		y++;
	}
}

void	p_update(t_win *win)
{
	// float	move_speed;
	// float	rotate_speed;

	// move_speed = 1.0;
	// rotate_speed = 0.05;
	// if (move_up) {
    //     win->playerX += move_speed * cos(win->playerA);
    //     win->playerY += move_speed * sin(win->playerA);
    // }
    // if (move_down) {
    //     win->playerX -= move_speed * cos(win->playerA);
    //     win->playerY -= move_speed * sin(win->playerA);
    // }
    // if (rotate_left)
    //     win->playerA -= rotate_speed;
    // if (rotate_right)
    //     win->playerA += rotate_speed;
	float	move_speed;
	float	y_move;
	float	x_move;

	y_move = 0;
	x_move = 0;
	move_speed = 1.0;
	if (move_up == 0)
		y_move -= move_speed;
	else if (move_down == 0)
		y_move += move_speed;
	else if (rotate_left == 0)
		x_move -= move_speed;
	else if (rotate_right == 0)
		x_move += move_speed;
	else
	// if ((move_up == 0 || move_down == 0) && (rotate_right == 0 || rotate_left == 0))
	{
		x_move *= cos(M_PI / 4);
		y_move *= sin(M_PI / 4);
	}
	win->playerX += x_move;
	win->playerY += y_move;
	
}

void	player_render(t_win *win, void *win_ptr, void *mlx_ptr)
{
	mlx_pixel_put(mlx_ptr, win_ptr, win->playerX, win->playerY, 0xFF0000);
	int x, y;
    // for (x = win->playerX; x <= win->playerX + 50; x++) {
    //     y = x;  // y = x for a diagonal line
    //     mlx_pixel_put(mlx_ptr, win_ptr, x, y, 0x0000FF);
    // }
	y = win->playerY;
	while (y <= win->playerY + 50)
	{
		x = win->playerX;
		while (x <= win->playerX + 50)
		{
			mlx_pixel_put(mlx_ptr, win_ptr, x, y, 0x0000FF);
			x++;
		}
		
		y++;
	}
}

int	key_release_handler(int keycode, t_win *win)
{
	(void) win;
	if (keycode == 13)//W
		move_up = 1;
	if (keycode == 0)//A
		rotate_left = 1;
	if (keycode == 2)//D
		rotate_right = 1;
	if (keycode == 1)//S
		move_down = 1;
	return 0;
}

int	key_handler(int keycode, t_win *win)
{
	printf("hello%d\n", keycode);
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
	key_release_handler(keycode, win);
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
	win.playerX = 100;
	win.playerY = 100;
	win.playerA = 0.0;
	
	win.mlx_ptr = mlx_init();
	win.map = map_getter();
	// map_printer(map);
	win.win_ptr = mlx_new_window(win.mlx_ptr, win.map_width * win.cell_size, win.map_height * win.cell_size, "CUB3D");
	map_draw(win, win.win_ptr, win.mlx_ptr, win.map);
	player_render(&win, win.win_ptr, win.mlx_ptr);
	mlx_key_hook(win.win_ptr, &key_handler, &win);
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