/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 22:12:33 by bhazzout          #+#    #+#             */
/*   Updated: 2023/07/09 18:24:20 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

	// printf("this is the win cell size %d\n", );

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
				// mlx_pixel_put(mlx_ptr, win_ptr, pixel_x + i, pixel_y + j, 0xFF0000);
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

int main (int ac, char **av)
{
	void	*mlx_ptr;
	void	*win_ptr;
	char	**map;
	(void) av, (void) ac;
	t_win win;

	//initialize map's constants
	win.cell_size = 32;
	win.map_height = 11;
	win.map_width = 25;
	
	mlx_ptr = mlx_init();
	map = map_getter();
	// map_printer(map);
	win_ptr = mlx_new_window(mlx_ptr, win.map_width * win.cell_size, win.map_height * win.cell_size, "CUB3D");
	map_draw(win, win_ptr, mlx_ptr, map);
	mlx_pixel_put(mlx_ptr, win_ptr, 100, 100, 0xFF0000);
	mlx_hook(win_ptr, 17, 0L, &windows_close, NULL);

    // int x, y;
    // for (x = 200; x <= 400; x++) 
	// {
    //     y = x;
    //     mlx_pixel_put(mlx_ptr, win_ptr, x, y, 0x0000FF);
    // }
	mlx_loop(mlx_ptr);
}