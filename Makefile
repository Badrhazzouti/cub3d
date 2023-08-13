# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/08 22:22:17 by bhazzout          #+#    #+#              #
#    Updated: 2023/08/09 21:57:05 by ikhabour         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	= cub3D

CFLAGS	= -Wall -Wextra -Werror # -fsanitize=address

RM	= rm -rf

SOURCES	= cub3d.c gnl/get_next_line_utils.c gnl/get_next_line.c update.c move_handle.c player_render.c \
		rays.c distance.c dist_hor.c dist_ver.c dist_ver_1.c pixel.c ft_split.c file1.c file2.c file3.c file4.c cub3d_utils.c \

OBJECTS	= $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	cc $(CFLAGS) $(OBJECTS) -Ofast -lmlx -framework OpenGL -framework AppKit -o $(NAME)


clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(NAME)

bonus : 
	make CFLAGS="-Wall -Wextra -Werror -D BONUS=1"

re:	fclean all