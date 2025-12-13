#include "mini_rt.h"

static int	istrim(char const c, char const *set)
{
	while (*set)
		if (c == *set++)
			return (1);
	return (0);
}

char	*arena_strtrim(Arena *a, char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	size_t	len;
	char	*new_str;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && istrim(s1[start], set))
		start++;
	end = strlen(s1);
	while (end > start && istrim(s1[end - 1], set))
		end--;
	len = end - start;
	new_str = (char *)arena_alloc(a, len + 1 * sizeof (char));
	memcpy(new_str, s1 + start, len);
	new_str[len] = '\0';
	return (new_str);
}
