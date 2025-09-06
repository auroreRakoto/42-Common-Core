/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurakoto <aurakoto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 18:29:03 by aurakoto          #+#    #+#             */
/*   Updated: 2023/05/12 09:27:52 by aurakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// renvoie le char apres %
#include "ft_printf.h"

int	ft_check_char(char str, va_list args)
{
	int	flag;

	if (str == 'c')
		flag = ft_write_c(args);
	else if (str == 's')
		flag = ft_write_str(args);
	else if (str == 'p')
		flag = ft_write_p(args);
	else if (str == 'd')
		flag = ft_write_d(args);
	else if (str == 'i')
		flag = ft_write_d(args);
	else if (str == 'u')
		flag = ft_write_u(args);
	else if (str == 'x')
		flag = ft_write_x(args, 0);
	else if (str == 'X')
		flag = ft_write_x(args, 1);
	else if (str == '%')
		flag = write(1, "%", 1);
	else
		flag = -1;
	return (flag);
}

int	ft_write_args(va_list args, char *str, int len, int temp_len)
{
	while (*str)
	{
		if (*str == '%')
		{
			if (!str++)
				return (-1);
			temp_len = ft_check_char(*str, args);
			if (temp_len == -1)
			{
				if (write(1, "(null)", 6) == -1)
					return (-1);
				len += 6;
			}
			else
				len += temp_len;
		}
		else
		{
			if (write(1, str, 1) == -1)
				return (-1);
			len += 1;
		}
		str++;
	}
	return (len);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		len;

	len = -1;
	va_start(args, str);
	len = ft_write_args(args, (char *)str, 0, 1);
	va_end(args);
	return (len);
}
