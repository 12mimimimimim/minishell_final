/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 17:02:06 by vmorena           #+#    #+#             */
/*   Updated: 2025/01/02 19:12:37 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char			*src;
	unsigned char	tmp;
	char			*last_found;

	tmp = (unsigned char)c;
	src = (char *)s;
	last_found = NULL;
	while (*src)
	{
		if ((unsigned char)*src == tmp)
			last_found = src;
		src++;
	}
	if (tmp == 0)
		return (src);
	return (last_found);
}
