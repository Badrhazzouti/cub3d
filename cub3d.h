/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 22:12:42 by bhazzout          #+#    #+#             */
/*   Updated: 2023/08/07 02:20:16 by bhazzout         ###   ########.fr       */
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

#define WIN_WIDTH 900
#define WIN_HEIGHT 600
#define	CELL_SIZE 32

typedef struct	s_wallhit
{
	double	dist;
	double	x;
	double	y;
	int		ver_hor;
}	t_wallhit;

typedef struct s_mini
{
	int	i;
	int	j;
}	t_mini;


typedef struct	s_wall
{
	int		x;
	int		y;
	void	*text;
}	t_wall;

typedef struct s_image
{
	void	*NO;
	void	*SO;
	void	*EA;
	void	*WE;
}	t_image;

typedef struct s_ray
{
	double	ray_i;
	double	ray_start;
	double	ray_end;
	double	ray_angle;
	double	ray_temp;
	double	wall_height;
	double	wall_screen_x;
	double	wall_top;
	double	wall_bottom;
	double	col;
}	t_ray;

typedef struct s_win
{
	void		*img_ptr;
	char		*f_map;
	char		**map;
	void		*mlx_ptr;
	void		*win_ptr;
	int			playerX;
	int			playerY;
	double		mini_x;
	double		mini_y;
	double		playerDX;
	double		playerDY;
	double		playerA;
	double		fov_A;
	double		rays_A;
	int			num_rays;
	int			cell_size;
	int			map_width;
	int			map_height;
	t_wall 		wall;
	t_image		img;
	t_ray		ray;
	double		distance_towall;
	int			color;
} t_win;

char	*get_next_line(int fd);

char	*ft_strjoin(const char *s1, const char *s2);
size_t	ft_strlen(const char *str);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(char *s1);
char	**map_getter(void);
void	map_printer(char **map);
char	**map2d_getter(int fd, int counter);
int		calc_words(const char *s, char c);
int		ft_wordlen(const char *s, int i, char c);
char	*ft_charge(const char *s, int i, char c);
void	*freeini(char **string, int len);
char	**ft_split(const char *s, char sep);
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t count, size_t size);
void	map_draw(t_win *win);

#endif
