/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurakoto <aurakoto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 05:56:52 by aurakoto          #+#    #+#             */
/*   Updated: 2025/10/04 09:27:50 by aurakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(const char *s, char c)
{
	int	count;
	int	is_word;

	count = 0;
	is_word = 0;
	while (*s)
	{
		if (*s != c && !is_word)
		{
			is_word = 1;
			count++;
		}
		else if (*s == c)
			is_word = 0;
		s++;
	}
	return (count);
}

static char	*ft_word_start(const char *s, char c)
{
	while (*s && *s == c)
		s++;
	return ((char *)s);
}

static char	*ft_word_end(const char *s, char c)
{
	while (*s && *s != c)
		s++;
	return ((char *)s);
}

static char	**ft_malloc(char const *s, char c)
{
	char	**res;
	int		i;
	int		num_words;

	num_words = ft_count_words(s, c);
	res = (char **)malloc((num_words + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (i < num_words)
	{
		s = ft_word_start(s, c);
		res[i] = ft_substr(s, 0, ft_word_end(s, c) - s);
		if (!res[i])
		{
			while (i > 0)
				free(res[--i]);
			free(res);
			return (NULL);
		}
		s = ft_word_end(s, c);
		i++;
	}
	res[i] = NULL;
	return (res);
}

char	**ft_split(char const *s, char c)
{
	char	**res;

	if (!s)
		return (NULL);
	res = ft_malloc(s, c);
	return (res);
}
