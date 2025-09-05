/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurakoto <aurakoto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 20:58:46 by aurakoto          #+#    #+#             */
/*   Updated: 2023/04/04 06:00:02 by aurakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_getsize(int n)
{
	int	size;

	size = 1;
	while (n > 9)
	{
		size++;
		n = n / 10;
	}
	return (size);
}

static void	ft_convert(char *res, int n, int i, int size)
{
	if (i == 1)
		res[0] = '-';
	res[size + i] = '\0';
	while (size != 0)
	{
		res[size + i - 1] = n % 10 + 48;
		n = n / 10;
		size--;
	}
}

char	*ft_itoa(int n)
{
	char	*res;
	int		size;
	int		i;

	i = 0;
	if (n == -2147483648)
	{
		res = (char *)malloc(sizeof(char) * 12);
		if (!res)
			return (0);
		ft_strlcpy(res, "-2147483648", 12);
		return (res);
	}
	if (n < 0)
	{
		n = -n;
		i = 1;
	}
	size = ft_getsize(n);
	res = (char *)malloc(sizeof(char) * (size + 1 + i));
	if (!res)
		return (0);
	ft_convert(res, n, i, size);
	return (res);
}
