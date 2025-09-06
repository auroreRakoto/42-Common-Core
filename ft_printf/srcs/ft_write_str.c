/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurakoto <aurakoto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 11:48:12 by aurakoto          #+#    #+#             */
/*   Updated: 2023/05/12 08:39:01 by aurakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

int	ft_write_str(va_list args)
{
	char	*var;
	int		len;

	var = va_arg(args, char *);
	if (!var)
		return (-1);
	len = ft_strlen(var);
	return (write(1, var, len));
}
