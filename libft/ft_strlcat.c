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

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	len_dst;
	size_t	len_src;
	size_t	res;

	len_dst = 0;
	while (dst[len_dst] && len_dst < dstsize)
		len_dst++;
	len_src = 0;
	while (src[len_src] && len_dst + len_src + 1 < dstsize)
	{
		dst[len_dst + len_src] = src[len_src];
		len_src++;
	}
	if (len_dst < dstsize)
		dst[len_dst + len_src] = '\0';
	res = len_dst + len_src;
	while (src[len_src])
	{
		res++;
		len_src++;
	}
	return (res);
}
