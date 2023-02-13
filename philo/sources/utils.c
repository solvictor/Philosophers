/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 00:31:57 by vegret            #+#    #+#             */
/*   Updated: 2023/02/13 01:17:11 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	reset_params(t_params *params)
{
	params->philosophers = 0;
	params->time_to_die = 0;
	params->time_to_eat = 0;
	params->time_to_sleep = 0;
	params->time_must_eat = 0;
}

void	clear_nodes(t_table *table)
{
	t_node	*tmp;
	t_node	*list;
	t_node	*first;

	if (!table->head)
		return ;
	first = table->head;
	list = first->next;
	while (list && list != first)
	{
		tmp = list->next;
		free(list);
		list = tmp;
	}
	free(first);
	table->head = NULL;
}
