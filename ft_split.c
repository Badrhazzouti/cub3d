/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 23:21:57 by bhazzout          #+#    #+#             */
/*   Updated: 2023/07/25 23:22:46 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	calc_words(const char *s, char c)
{
	int		i;
	int		word;

	i = 0;
	word = 0;
	while (s[i] == c && s[i])
	i++;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
			word++;
		while (s[i] != c && s[i])
			i++;
	}
	return (word);
}

int	ft_wordlen(const char *s, int i, char c)
{
	int	len;

	len = 0;
	while (s[i] && s[i] != c)
	{
		i++;
		len++;
	}
	return (len);
}

char	*ft_charge(const char *s, int i, char c)
{
	char	*str;
	int		len;
	int		j;

	len = ft_wordlen(s, i, c) + 1;
	str = (char *)malloc(sizeof(char) * len);
	if (!str)
		return (NULL);
	j = 0;
	while (s[i] && s[i] != c)
	{
		str[j] = s[i];
		i++;
		j++;
	}
	str[j] = '\0';
	return (str);
}

void	*freeini(char **string, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		free(string[i]);
		i++;
	}
	free(string);
	return (NULL);
}

char	**ft_split(const char *s, char sep)
{
	char	**strs;
	int		i;
	int		j;

	if (!s)
		return (0);
	strs = (char **)ft_calloc(sizeof(char *), (calc_words(s, sep) + 1));
	if (!strs || !s)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i] && j < calc_words(s, sep))
	{
		while (s[i] && s[i] == sep)
			i++;
		if (s[i])
		{
			strs[j++] = ft_charge(s, i++, sep);
			if (!strs[j - 1])
				return (freeini(strs, --j));
		}
		while (s[i] && s[i] != sep)
			i++;
	}
	return (strs);
}