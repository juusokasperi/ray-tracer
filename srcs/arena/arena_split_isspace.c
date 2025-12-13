/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:26:18 by jrinta-           #+#    #+#             */
/*   Updated: 2025/12/12 20:21:58 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static bool	is_delim(char c, const char *delims)
{
	while (*delims)
	{
		if ((unsigned char)c == (unsigned char)*delims)
			return (true);
		delims++;
	}
	return (false);
}

static char	**count_words(Arena *a, char const *s, char const *delims)
{
	char	**result;
	int		i;
	int		word_count;

	word_count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && is_delim(s[i], delims))
			i++;
		if (s[i])
			word_count++;
		while (s[i] && !is_delim(s[i], delims))
			i++;
	}
	result = arena_alloc(a, sizeof(char *) * (word_count + 1));
	return (result);
}

static void	fill_result(Arena *a, char const *s, 
			char **result, char const *delims)
{
	int		i;
	int		start;
	int		index;

	index = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && is_delim(s[i], delims))
			i++;
		if (s[i])
		{
			start = i;
			while (s[i] && !is_delim(s[i], delims))
				i++;
			result[index] = arena_alloc(a, sizeof(char) * (i - start + 1));
			memcpy(result[index], s + start, i - start);
			result[index][i - start] = '\0';
			index++;
		}
	}
	result[index] = NULL;
}

char **arena_split(Arena *a, const char *s, const char *delims)
{
	char	**words;

	if (!s)
		return (NULL);
	words = count_words(a, s, delims);
	fill_result(a, s, words, delims);
	return (words);
}

char **arena_split_isspace(Arena *a, const char *s)
{
	return (arena_split(a, s, " \t\n\r"));
}
