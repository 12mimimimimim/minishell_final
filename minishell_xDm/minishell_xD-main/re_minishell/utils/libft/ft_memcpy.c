/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 16:47:09 by vmorena           #+#    #+#             */
/*   Updated: 2025/01/02 18:49:33 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	const unsigned char	*tmp_src;
	unsigned char		*tmp_dst;

	tmp_dst = (unsigned char *)dst;
	tmp_src = (const unsigned char *)src;
	if (!tmp_dst && !tmp_src)
	{
		return (NULL);
	}
	while (n--)
	{
		*tmp_dst++ = *tmp_src++;
	}
	return (dst);
}

// int main(){
//     char arr[] = "lollipop";

//     printf("Before ft_memcpy:[%s]",arr);

//    ft_memcpy(arr,arr,sizeof(arr));

//    printf("After ft_memcpy(arr1): [%s],(arr2):[%s]",arr1,arr2);

// }