/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_d.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurakoto <aurakoto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 12:02:11 by aurakoto          #+#    #+#             */
/*   Updated: 2023/05/12 08:52:58 by aurakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

int	ft_write_d(va_list args)
{
	int			var;
	int			len;
	long int	div;

	len = 1;
	div = 10;
	var = va_arg(args, int);
	if (var < 0)
	{
		if (var == INT_MIN)
			return (write(1, "-2147483648", 11));
		var = -var;
		len += write(1, "-", 1);
		if (len == 0)
			return (-1);
	}
	ft_putnbr_fd(var, 1);
	while (var / div > 0)
	{
		len++;
		div *= 10;
	}
	return (len);
}
