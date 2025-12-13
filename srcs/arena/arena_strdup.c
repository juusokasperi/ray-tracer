#include "mini_rt.h"

char	*arena_strdup(Arena *a, const char *s)
{
	char	*dest;
	size_t	len;

	if (!s)
		return (NULL);

	len = strlen(s);
	dest = arena_alloc(a, len + 1);
	if (!dest)
		return (NULL);
	memcpy(dest, s, len + 1);	
	return (dest);
}
