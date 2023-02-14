/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 00:31:57 by vegret            #+#    #+#             */
/*   Updated: 2023/02/14 23:19:35 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	has_dead(t_philo *start)
{
	t_philo	*tmp;

	if (!start)
		return (false);
	if (start->state == DEAD)
		return (true);
	tmp = start->next;
	while (tmp != start)
	{
		if (tmp->state == DEAD)
			return (true);
		tmp = tmp->next;
	}
	return (false);
}

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
	t_philo	*tmp;
	t_philo	*list;
	t_philo	*first;

	if (!table->first)
		return ;
	first = table->first;
	list = first->next;
	while (list && list != first)
	{
		tmp = list->next;
		free(list);
		list = tmp;
	}
	free(first);
	table->first = NULL;
}
