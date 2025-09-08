/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurakoto <aurakoto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 20:37:37 by aurakoto          #+#    #+#             */
/*   Updated: 2023/04/05 07:43:43 by aurakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	len_dst;
	size_t	len_src;
	size_t	res;

	len_dst = ft_strlen(dst);
	len_src = ft_strlen(src);
	if (size < len_dst + 1)
	{
		return (len_src + size);
	}
	res = 0;
	while (src[res])
	{
		dst[len_dst + res] = src[res];
		res++;
	}
	dst[len_dst + res] = 0;
	return (len_dst + len_src);
}
