/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_x.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurakoto <aurakoto@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 12:01:20 by aurakoto          #+#    #+#             */
/*   Updated: 2023/05/10 12:10:22 by aurakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

int	ft_to_hexa(unsigned int val, int len, int maj)
{
	char	c;

	if (val / 16 >= 1)
		len = ft_to_hexa(val / 16, len, maj) + 1;
	else
	{
		if (val < 10)
			c = val + '0';
		else if (val < 16)
			c = val - 10 + 'a';
		if (maj)
			c = ft_toupper(c);
		write(1, &c, 1);
		return (len);
	}
	if (val % 16 < 10)
		c = val % 16 + '0';
	else if (val % 16 < 16)
		c = val % 16 - 10 + 'a';
	if (maj)
		c = ft_toupper(c);
	write(1, &c, 1);
	return (len);
}

int	ft_write_x(va_list args, int maj)
{
	unsigned int	to_delete;
	int				len;

	to_delete = va_arg(args, unsigned int);
	len = 1;
	len = ft_to_hexa(to_delete, len, maj);
	return (len);
}
