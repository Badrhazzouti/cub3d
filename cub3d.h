/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 22:12:42 by bhazzout          #+#    #+#             */
/*   Updated: 2023/07/14 05:58:11 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D
#define CUB3D

#include <mlx.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

#define WIN_WIDTH 1200
#define WIN_HEIGHT 800

typedef struct s_win
{
	char		*f_map;
	char		**map;
	void		*mlx_ptr;
	void		*win_ptr;
	double		playerX;
	double		playerY;
	double		playerDX;
	double		playerDY;
	double		playerA;
	double		fov_A;
	double		rays_A;
	int			num_rays;
	int			cell_size;
	int			map_width;
	int			map_height;
} t_win;

char	*get_next_line(int fd);

char	*ft_strjoin(const char *s1, const char *s2);
size_t	ft_strlen(const char *str);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(char *s1);
char	**map_getter(void);
void	map_printer(char **map);
char	*one_map(void);

#endif
