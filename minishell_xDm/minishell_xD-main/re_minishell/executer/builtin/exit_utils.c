/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: didris <didris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 22:31:20 by didris            #+#    #+#             */
/*   Updated: 2025/09/30 22:31:20 by didris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static int	check_code(char *str, int *i, int *sign, unsigned long long *res)
{
	*res = 0;
	while (str[*i])
	{
		if (!ft_isdigit(str[*i]))
			return (1);
		if (*sign == 1 && *res
			> (unsigned long long)(LLONG_MAX - (str[*i] - '0')) / 10)
			return (1);
		if (*sign == -1 && *res
			> (unsigned long long)(-(LLONG_MIN + (str[*i] - '0'))) / 10)
			return (1);
		*res = (10 * *res) + (str[(*i)++] - '0');
	}
	return (0);
}

int	is_valid_exit_arg(char *str, long long *result)
{
	int					i;
	int					sign;
	unsigned long long	res;

	i = 0;
	if (!str || !*str)
		return (0);
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	sign = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!str[i] || !ft_isdigit(str[i]))
		return (0);
	if (check_code(str, &i, &sign, &res))
		return (0);
	*result = sign * res;
	return (1);
}
