/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 17:10:23 by vmorena           #+#    #+#             */
/*   Updated: 2025/01/02 20:28:34 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ss_counter(char const *s, char c)
{
	int	i;
	int	word_count;
	int	in_word;

	i = 0;
	word_count = 0;
	in_word = 0;
	while (s[i])
	{
		if (s[i] == c)
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			word_count++;
		}
		i++;
	}
	return (word_count);
}

char	*get_c(char const *s, int *i, char c)
{
	int		j;
	char	*word;

	j = *i;
	while (s[j] && s[j] != c)
		j++;
	word = malloc(j - *i + 1);
	if (!word)
		return (NULL);
	ft_strlcpy(word, s + *i, j - *i + 1);
	*i = j;
	return (word);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		k;
	int		word_count;
	char	**result;

	i = 0;
	k = 0;
	word_count = ss_counter(s, c);
	result = malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			result[k] = get_c(s, &i, c);
			if (!result[k])
				return (NULL);
			k++;
		}
	}
	result[k] = NULL;
	return (result);
}

void	free_split_result(char **result)
{
	int	i;

	i = 0;
	while (result[i])
	{
		free(result[i]);
		i++;
	}
	free(result);
}

// Example Usage
// int	main(void)
// {
// 	char	str[] = "   x abc  x   ";
// 	char	delimiter = ' ';
// 	char	**result;

// 	result = ft_split(str, delimiter);
// 	if (result)
// 	{
// 		for (int i = 0; result[i] != NULL; i++)
// 			printf("Substring %d: '%s'\n", i, result[i]);
// 		free_split_result(result);
// 	}
// 	return (0);
// }
