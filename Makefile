# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/08 22:22:17 by bhazzout          #+#    #+#              #
#    Updated: 2023/07/25 23:25:24 by bhazzout         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	= cub3D

CFLAGS	= -Wall -Wextra -Werror -fsanitize=address -g3

RM	= rm -rf

SOURCES	= cub3d.c utils.c ft_split.c ft_split_utils.c gnl/get_next_line_utils.c gnl/get_next_line.c 

OBJECTS	= $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	cc $(CFLAGS) $(OBJECTS) -lmlx -framework OpenGL -framework AppKit -o $(NAME)

clean:
	$(RM) $(OBJECTS)

fclean:
	$(RM) $(NAME)

re:	fclean all