/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etaquet <etaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 08:04:03 by etaquet           #+#    #+#             */
/*   Updated: 2025/02/18 08:07:23 by etaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_getdigit(char c)
{
	int	result;

	if (c >= '0' && c <= '9')
	{
		result = c - '0';
		return (result);
	}
	if (c >= 'a' && c <= 'z')
	{
		result = c - 'a' + 10;
		return (result);
	}
	if (c >= 'A' && c <= 'Z')
	{
		result = c - 'A' + 10;
		return (result);
	}
	return (-1);
}

static const char	*handle_sign(const char *p, int *sign)
{
	if (p[0] == '-' || p[0] == '+')
	{
		if (p[0] == '-')
			*sign = -1;
		else
			*sign = 1;
		p++;
	}
	return (p);
}

static const char	*detect_base(const char *p, int *base)
{
	if (*base == 0)
	{
		if (p[0] == '0')
		{
			p++;
			if (p[0] == 'x' || p[0] == 'X')
			{
				*base = 16;
				p++;
			}
			else
				*base = 8;
		}
		else
			*base = 10;
	}
	else if (*base == 16)
	{
		if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X'))
			p += 2;
	}
	return (p);
}

static unsigned long long	convert_number(const char *p, int base,
					unsigned long long cutoff, const char **new_ptr)
{
	unsigned long long	acc;
	int					digit;

	acc = 0;
	digit = ft_getdigit(p[0]);
	while (digit >= 0 && digit < base)
	{
		if (acc > cutoff / (unsigned long long)base
			|| (acc == cutoff / (unsigned long long)base
				&& (unsigned int)digit > cutoff % (unsigned long long)base))
		{
			errno = ERANGE;
			acc = cutoff;
			while (digit >= 0 && digit < base)
				digit = ft_getdigit((++p)[0]);
			break ;
		}
		acc = acc * (unsigned long long)base + (unsigned long long)digit;
		digit = ft_getdigit((++p)[0]);
	}
	*new_ptr = p;
	return (acc);
}

long long	ft_strtoll(const char *nptr, char **endptr, int base)
{
	const char			*p;
	int					sign;
	unsigned long long	cutoff;
	unsigned long long	acc;

	p = nptr;
	sign = 1;
	while (p[0] == ' ' || (p[0] >= 9 && p[0] <= 13))
		p++;
	p = handle_sign(p, &sign);
	p = detect_base(p, &base);
	if (sign == 1)
		cutoff = (unsigned long long)LLONG_MAX;
	else
		cutoff = (unsigned long long)LLONG_MAX + 1;
	acc = convert_number(p, base, cutoff, &p);
	if (endptr != (void *)0)
		*endptr = (char *)p;
	if (sign == -1)
		return (-(long long)acc);
	return ((long long)acc);
}
