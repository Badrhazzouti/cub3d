/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 22:56:45 by ikhabour          #+#    #+#             */
/*   Updated: 2023/08/09 01:50:59 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static	int	ft_len(const char *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i] == c && s[i])
		i++;
	while (s[i] != c && s[i])
	{
		i++;
		j++;
	}
	return (j);
}

static	char	*my_strdup(const char *s, char c)
{
	char	*pointer;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	pointer = malloc(ft_len(s, c) * sizeof(char) + 1);
	if (!pointer)
		return (NULL);
	while (s[i] == c && s[i])
		i++;
	while (s[i] != c && s[i])
	{
		pointer[j] = s[i];
		i++;
		j++;
	}
	pointer[j] = '\0';
	return (pointer);
}

static	int	stringcount(const char *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i])
			j++;
		while (s[i] != c && s[i])
			i++;
	}
	return (j);
}

static	void	ft_free(char **str, int j)
{
	while (str[j])
	{
		free (str[j]);
		j--;
	}
	free (str);
	return ;
}

char	**ft_split(const char *s, char c)
{
	int		i;
	int		j;
	char	**str;

	if (!s)
		return (0);
	i = 0;
	j = 0;
	str = malloc((stringcount(s, c) + 1) * sizeof(char *));
	if (!str)
		return (0);
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i] == 0)
			break ;
		str[j] = my_strdup(s + i, c);
		if (!str[j])
			ft_free(str, j);
		j++;
		while (s[i] != c && s[i])
			i++;
	}
	return (str[j] = 0, str);
}
