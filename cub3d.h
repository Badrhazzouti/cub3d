/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbalahce <jbalahce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 22:12:42 by bhazzout          #+#    #+#             */
/*   Updated: 2023/07/27 18:38:31 by jbalahce         ###   ########.fr       */
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

typedef struct s_win
{
	char		*f_map;
	char		**map;
	void		*mlx_ptr;
	void		*win_ptr;
	int		playerX;
	int		playerY;
	double		playerDX;
	double		playerDY;
	double		playerA;
	double		fov_A;
	double		rays_A;
	int			num_rays;
	int			cell_size;
	int			map_width;
	int			map_height;
	double		distance_towall;
} t_win;

// typedef	struct s_end_coor
// {
// 	double	rayX;
// 	double	rayY;
// }	t_coor;


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

#endif
