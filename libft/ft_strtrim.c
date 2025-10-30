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
	size_t	len;

	start = 0;
	while (s1[start] && ft_strchr(set, (int)s1[start]))
		start++;
	end = ft_strlen(s1);
	while (end > start && ft_strchr(set, (int)s1[end - 1]))
		end--;
	len = end - start;
	res = (char *)malloc(len + 1);
	if (!res)
		return (NULL);
	ft_strlcpy(res, s1 + start, len + 1);
	return (res);
}
