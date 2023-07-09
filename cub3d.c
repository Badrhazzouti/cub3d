/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 22:12:33 by bhazzout          #+#    #+#             */
/*   Updated: 2023/07/09 01:14:18 by bhazzout         ###   ########.fr       */
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

int main (int ac, char **av)
{
	void	*mlx_ptr;
	void	*win_ptr;
	char	**map;
	(void) av, (void) ac;

	mlx_ptr = mlx_init();
	map = map_getter();
	map_printer(map);
	// printf("%s\n", map);
	win_ptr = mlx_new_window(mlx_ptr, 800, 600, "CUB3D");
	mlx_hook(win_ptr, 17, 0L, &windows_close, NULL);
	mlx_loop(mlx_ptr);
}