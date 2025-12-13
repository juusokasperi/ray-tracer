/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_first_count.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:29:48 by jrinta-           #+#    #+#             */
/*   Updated: 2025/04/27 16:30:09 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

uint16_t	get_first(uint32_t first_count)
{
	return (first_count & 0xFFFF);
}

uint16_t	get_count(uint32_t first_count)
{
	return (first_count >> 16);
}

uint32_t	pack_first_count(uint16_t first, uint16_t count)
{
	return (first | ((uint32_t)count << 16));
}
