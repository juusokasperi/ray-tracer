/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_left_right.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:53:26 by jrinta-           #+#    #+#             */
/*   Updated: 2025/04/27 16:30:05 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

uint16_t	get_left_child(uint32_t left_right)
{
	return (left_right & 0xFFFF);
}

uint16_t	get_right_child(uint32_t left_right)
{
	return (left_right >> 16);
}

uint32_t	pack_children(uint16_t left, uint16_t right)
{
	return (left | ((uint32_t)right << 16));
}
