/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurakoto <aurakoto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 21:16:19 by aurakoto          #+#    #+#             */
/*   Updated: 2023/04/04 07:17:35 by aurakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	size_t	size;
	size_t	start;
	size_t	end;

	start = 0;
	size = ft_strlen(s1);
	if (size == 0)
	{
		res = (char *)malloc(sizeof(char) * 1);
		res[0] = '\0';
		return (res);
	}
	end = size - 1;
	while (s1[start] && ft_strchr(set, (int)s1[start]))
		start++;
	while (end > start && ft_strrchr(set, (int)s1[end]))
		end--;
	res = (char *)malloc(sizeof(char) * (end - start + 2));
	if (!res)
		return (NULL);
	ft_strlcpy(res, (s1 + start), end - start + 2);
	return (res);
}

/*
int	main(void)
{
	ft_strtrim("", "");
}
*/
