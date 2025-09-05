/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurakoto <aurakoto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 20:37:21 by aurakoto          #+#    #+#             */
/*   Updated: 2023/04/04 06:01:23 by aurakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	c;
	int		nbdiv;

	if (n < 0)
	{
		write(fd, "-", 1);
		if (n == -2147483648)
		{
			write(fd, "2", 1);
			n = -147483648;
		}
		n = -n;
	}
	nbdiv = n / 10;
	if (nbdiv < 1)
	{
		c = n + '0';
		write(fd, &c, 1);
	}
	else
	{
		ft_putnbr_fd(nbdiv, fd);
		c = (n % 10) + '0';
		write(fd, &c, 1);
	}
}
