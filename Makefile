# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/08 22:22:17 by bhazzout          #+#    #+#              #
#    Updated: 2023/08/07 02:27:31 by bhazzout         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	= cub3D

CFLAGS	= -Wall -Wextra -Werror -Wuninitialized -fsanitize=address

RM	= rm -rf

SOURCES	= cub3d.c utils.c ft_split.c ft_split_utils.c gnl/get_next_line_utils.c gnl/get_next_line.c 

OBJECTS	= $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	cc $(CFLAGS) $(OBJECTS) -Ofast -lmlx -framework OpenGL -framework AppKit -o $(NAME)

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(NAME)

re:	fclean all