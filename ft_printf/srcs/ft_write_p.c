/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_p.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurakoto <aurakoto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 12:01:43 by aurakoto          #+#    #+#             */
/*   Updated: 2023/05/12 08:57:01 by aurakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

int	ft_print_hexa(uintptr_t ptr_val, int len)
{
	int		temp;
	char	c;

	ptr_val >>= 4;
	if (!ptr_val)
	{
		write(1, "0x", 2);
		return (len);
	}
	len = ft_print_hexa(ptr_val, len) + 1;
	temp = ptr_val & 0xF;
	if (temp < 10)
		c = temp + '0';
	else
		c = temp - 10 + 'a';
	write(1, &c, 1);
	return (len);
}

int	ft_write_p(va_list args)
{
	void		*to_delete;
	uintptr_t	ptr_val;
	char		c;
	int			len;
	int			temp;

	to_delete = (char *)va_arg(args, void *);
	ptr_val = (uintptr_t)to_delete;
	len = 2;
	len = ft_print_hexa(ptr_val, len);
	temp = ptr_val & 0xF;
	if (temp < 10)
		c = temp + '0';
	else
		c = temp - 10 + 'a';
	return (len + write(1, &c, 1));
}
