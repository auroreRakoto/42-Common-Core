/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurakoto <aurakoto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 20:23:44 by ut                #+#    #+#             */
/*   Updated: 2025/10/04 07:09:30 by aurakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	size1;
	size_t	size2;
	size_t	size;
	char	*res;

	if (!s1 || !s2)
		return (NULL);
	size1 = ft_strlen(s1);
	size2 = ft_strlen(s2);
	size = size1 + size2;
	res = (char *)malloc(sizeof(char) * (size + 1));
	if (!res)
		return (NULL);
	ft_strlcpy(res, s1, size + 1);
	ft_strlcat(res, s2, size + 1);
	res[size] = '\0';
	return (res);
}
