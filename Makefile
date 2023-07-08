# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/08 22:22:17 by bhazzout          #+#    #+#              #
#    Updated: 2023/07/08 22:24:21 by bhazzout         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	= cub3D

CFLAGS	= -Wall -Wextra -Werror

RM	= rm -rf

SOURCES	= cub3d.c

OBJECTS	= $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	cc $(CFLAGS) $(OBJECTS) -lmlx -framework OpenGL -framework AppKit -o $(NAME)

clean:
	$(RM) $(OBJECTS)

fclean:
	$(RM) $(NAME)

re:	fclean all