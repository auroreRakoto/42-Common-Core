/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurakoto <aurakoto@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 20:37:43 by aurakoto          #+#    #+#             */
/*   Updated: 2023/03/23 21:53:58 by aurakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;

	if (!*little)
		return ((char *)big);
	while (len > 0 && *big != '\0')
	{
		i = 0;
		while (big[i] == little[i] && little[i] != '\0' && len - i > 0)
			i++;
		if (little[i] == '\0')
			return ((char *)big);
		big++;
		len--;
	}
	return (NULL);
}
