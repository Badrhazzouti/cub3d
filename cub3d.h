/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 22:12:42 by bhazzout          #+#    #+#             */
/*   Updated: 2023/08/09 03:17:31 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <mlx.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <math.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# define WIN_WIDTH 900
# define WIN_HEIGHT 600
# define CELL_SIZE 32
# define MINI_HEIGHT 200
# define MINI_WIDTH 300

typedef struct s_move
{
	int	move_up;
	int	move_down;
	int	move_left;
	int	move_right;
	int	rotate_left;
	int	rotate_right;
}	t_move;

typedef struct s_sqr
{
	double	x;
	double	y;
	double	co;
	double	si;
	int		i;
	int		j;
	int		x_i;
	int		m_y;
	int		m_x;
}	t_sqr;

typedef struct s_hit
{
	double	offset_x;
	double	offset_y;
	double	adjacent;
	double	opposite;
}	t_hit;

typedef struct s_img
{
	int				x;
	int				y;
	unsigned int	*img;
	unsigned int	*texture;
	int				img_index;
	int				index;
}	t_img;

typedef struct s_wallhit
{
	double	dist;
	double	x;
	double	y;
	int		ver_hor;
}	t_wallhit;

typedef struct s_mini
{
	int		i;
	int		j;
	int		pixel_x;
	int		pixel_y;
	double	cell_x;
	double	cell_y;
}	t_mini;

typedef struct s_wall
{
	int		x;
	int		y;
	void	*text;
}	t_wall;

typedef struct s_image
{
	void	*no;
	void	*so;
	void	*ea;
	void	*we;
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
	int			playerx;
	int			playery;
	double		mini_x;
	double		mini_y;
	double		playerdx;
	double		playerdy;
	double		player_a;
	double		fov_a;
	double		rays_a;
	int			num_rays;
	int			cell_size;
	int			map_width;
	int			map_height;
	double		distance_towall;
	int			color;
	t_wall		wall;
	t_image		img;
	t_ray		ray;
	t_move		mv;
}	t_win;

char		*get_next_line(int fd);
char		*ft_strjoin(const char *s1, const char *s2);
size_t		ft_strlen(const char *str);
char		*ft_strchr(const char *s, int c);
char		*ft_strdup(char *s1);
char		**map_getter(void);
void		map_printer(char **map);
char		**map2d_getter(int fd, int counter);
int			calc_words(const char *s, char c);
int			ft_wordlen(const char *s, int i, char c);
char		*ft_charge(const char *s, int i, char c);
void		*freeini(char **string, int len);
char		**ft_split(const char *s, char sep);
void		ft_bzero(void *s, size_t n);
void		*ft_calloc(size_t count, size_t size);
void		map_draw(t_win *win);
void		x_y_update(t_win *win, double playerdx, double playerdy);
void		move_update(t_win *win, double playerdx, double playerdy);
int			p_update(t_win *win);
int			key_release_handler(int keycode, t_win *win);
int			key_handler(int keycode, t_win *win);
int			mouse_handler(int x, int y, t_win *win);
void		player_render(t_win *win);
void		sqr_init(t_sqr *ray_sqr, t_win *win, t_mini *m_map);
void		ray_draw(t_win *win, t_mini	*m_map);
void		cells_draw(t_win *win, t_mini *m_map, int color);
void		map_draw(t_win *win);
void		wall_put(t_win *win, t_wallhit *wall_hit);
void		floor_ceil(t_win *win, int flag);
void		rays_init(t_win *win);
t_wallhit	ray_const_init(t_win *win);
void		hor_right_down(t_hit *hit_const, t_win *win,
				double ray_angle, t_wallhit *hor);
void		hor_dist_calcul(t_hit *hit_const, t_win *win,
				double ray_angle, t_wallhit *hor);
t_wallhit	horizontal_check(t_win *win, double ray_angle, t_wallhit *hor);
t_wallhit	distance_getter(t_win *win, double ray_angle);
void		hor_hit_const_init(t_hit *hit_const, t_wallhit *hor);
int			hor_right_up(t_hit *hit_const, t_win *win,
				double ray_angle, t_wallhit *hor);
void		hor_left_up(t_hit *hit_const, t_win *win,
				double ray_angle, t_wallhit *hor);
int			hor_left_down(t_hit *hit_const, t_win *win,
				double ray_angle, t_wallhit *hor);
void		left_down(t_hit *hit_const, t_win *win, double ray_angle,
				t_wallhit *ver);
int			right_down(t_hit *hit_const, t_win *win, double ray_angle,
				t_wallhit *ver);
void		distance_calcul(t_hit *hit_const, t_win *win, double ray_angle,
				t_wallhit *ver);
void		ver_hit_const_init(t_hit *hit_const, t_wallhit *ver);
t_wallhit	vertical_check(t_win *win, double ray_angle, t_wallhit *ver);
void		right_up(t_hit *hit_const, t_win *win, double ray_angle,
				t_wallhit *ver);
int			left_up(t_hit *hit_const, t_win *win, double ray_angle,
				t_wallhit *ver);
double		ray_correct(double ray_angle);
void		my_pixel_to_img(t_win *win);
void		mini_pixel_to_img(t_win *win, int x, int y, int color);
void		pixel_to_img(t_win *win, int x, int y);
void		my_mlx_draw_line(t_win *win, t_wallhit *wallhit);
int			windows_close(int keycode, void *param);

#endif
