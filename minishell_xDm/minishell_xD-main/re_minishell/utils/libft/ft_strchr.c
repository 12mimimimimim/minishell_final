/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 16:47:14 by vmorena           #+#    #+#             */
/*   Updated: 2025/01/02 19:12:15 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char			*src;
	unsigned char	tmp;

	tmp = (unsigned char)c;
	src = (char *)s;
	while (*src)
	{
		if ((unsigned char)*src == tmp)
			return (src);
		src++;
	}
	if (tmp == 0)
		return (src);
	return (NULL);
}
