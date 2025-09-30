/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 16:46:43 by vmorena           #+#    #+#             */
/*   Updated: 2024/12/29 14:55:13 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	if (n == 0)
		return ;
	while (n--)
	{
		*ptr = 0;
		ptr++;
	}
}

// int main() {

//     char arr[] = "hello";

//     printf("Before bzero: %s\n", arr);

//     bzero(arr, 3);

//     printf("After bzero: %s\n", arr);

//     int data[4] = {123, 456, 789, 101112};
//     printf("\nBefore bzero on integer array:\n");
//     for (int i = 0; i < 4; i++) {
//         printf("data[%d] = %d\n", i, data[i]);
//     }

//     bzero(data, sizeof(data));

//     printf("\nAfter bzero on integer array:\n");
//     for (int i = 0; i < 4; i++) {
//         printf("data[%d] = %d\n", i, data[i]);
//     }

//     return (0);
// }