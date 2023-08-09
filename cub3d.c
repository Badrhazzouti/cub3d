/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 22:12:33 by bhazzout          #+#    #+#             */
/*   Updated: 2023/08/09 01:05:48 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	windows_close(int keycode, void *param)
{
	(void) keycode, (void) param;
	exit (0);
}

int main (int ac, char **av)
{
	(void) av, (void) ac;
	t_win	win;

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
	win.mv.move_up = 1;
	win.mv.move_down = 1;
	win.mv.move_left = 1;
	win.mv.move_right = 1;
	win.mv.rotate_left = 1;
	win.mv.rotate_right = 1;
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