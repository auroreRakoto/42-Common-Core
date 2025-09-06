/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_u.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurakoto <aurakoto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 12:02:44 by aurakoto          #+#    #+#             */
/*   Updated: 2023/05/12 08:47:58 by aurakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

void	ft_unsigned_int(unsigned int n)
{
	char	c;
	int		nbdiv;

	nbdiv = n / 10;
	if (nbdiv < 1)
	{
		c = n + '0';
		write(1, &c, 1);
	}
	else
	{
		ft_unsigned_int(nbdiv);
		c = (n % 10) + '0';
		write(1, &c, 1);
	}
}

int	ft_write_u(va_list args)
{
	unsigned int	var;
	int				len;
	long int		div;

	len = 1;
	div = 10;
	var = va_arg(args, unsigned int);
	ft_unsigned_int(var);
	while (var / div > 0)
	{
		len++;
		div *= 10;
	}
	return (len);
}
