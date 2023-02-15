/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 00:31:57 by vegret            #+#    #+#             */
/*   Updated: 2023/02/15 22:44:41 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	current_time_millis(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (0);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	reset_params(t_params *params)
{
	params->philosophers = 0;
	params->time_to_die = 0;
	params->time_to_eat = 0;
	params->time_to_sleep = 0;
	params->time_must_eat = 0;
	params->one_died = false;
}

void	clear_nodes(t_philo **philos)
{
	t_philo	*tmp;
	t_philo	*list;

	if (!philos || !*philos)
		return ;
	list = (*philos)->next;
	while (list && list != (*philos))
	{
		tmp = list->next;
		free(list);
		list = tmp;
	}
	free(*philos);
	*philos = NULL;
}
