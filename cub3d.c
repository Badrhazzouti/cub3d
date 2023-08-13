/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 22:12:33 by bhazzout          #+#    #+#             */
/*   Updated: 2023/08/09 22:05:27 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	windows_close(int keycode, void *param)
{
	(void)keycode, (void)param;
	exit(0);
}

void	m_h_calcul(t_win *win)
{
	int	i;
	int	length;

	i = 0;
	length = 0;
	while (win->map[i])
	{
		length = (int)ft_strlen(win->map[i]);
		i++;
		if (win->map[i] && (int)ft_strlen(win->map[i]) > length)
			length = (int)ft_strlen(win->map[i]);
	}
	win->map_height = i;
	win->map_width = length;
}

void	main_init(t_win *win, t_texture texture, char **mapp)
{
	win->fov_a = (M_PI / 3);
	win->rays_a = WIN_WIDTH;
	win->num_rays = WIN_WIDTH;
	win->distance_towall = -1;
	win->mv.move_up = 1;
	win->mv.move_down = 1;
	win->mv.move_left = 1;
	win->mv.move_right = 1;
	win->mv.rotate_left = 1;
	win->mv.rotate_right = 1;
	win->mlx_ptr = mlx_init();
	win->cell_size = 32;
	win->floor = texture.f;
	win->ceiling = texture.c;
	win->map = mapp + 6;
	win->img.ea = mlx_xpm_file_to_image(win->mlx_ptr, texture.ea, &win->w,
			&win->h);
	win->img.no = mlx_xpm_file_to_image(win->mlx_ptr, texture.no, &win->w,
			&win->h);
	win->img.so = mlx_xpm_file_to_image(win->mlx_ptr, texture.so, &win->w,
			&win->h);
	win->img.we = mlx_xpm_file_to_image(win->mlx_ptr, texture.we, &win->w,
			&win->h);
	win->wall.x = 0;
	win->wall.y = 0;
}

int	main(int argc, char **argv)
{
	t_win		win;
	int			fd;
	char		*map;
	char		**mapp;
	t_texture	texture;

	if (argc != 2)
		print_error();
	if (ft_strncmp(argv[1] + (ft_strlenn(argv[1] - 4)), ".cub", 4))
		print_error();
	fd = open(argv[1], O_RDONLY, 0677);
	map = read_map(fd);
	mapp = ft_split(map, '\n');
	check_map(mapp, &texture, &win);
	main_init(&win, texture, mapp);
	win.win_ptr = mlx_new_window(win.mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "CUB3D");
	m_h_calcul(&win);
	player_render(&win);
	mlx_hook(win.win_ptr, 2, 0, key_handler, &win);
	mlx_hook(win.win_ptr, 3, 0, key_release_handler, &win);
	BONUS && mlx_hook(win.win_ptr, 6, 0, mouse_handler, &win);
	mlx_loop_hook(win.mlx_ptr, p_update, &win);
	mlx_hook(win.win_ptr, 17, 0L, windows_close, NULL);
	mlx_loop(win.mlx_ptr);
}
