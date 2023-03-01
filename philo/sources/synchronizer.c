/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchronizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 21:14:08 by vegret            #+#    #+#             */
/*   Updated: 2023/03/01 23:55:42 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	all_synced(t_params *params)
{
	bool	all_synced;

	pthread_mutex_lock(&params->sync);
	all_synced = params->synced == params->philosophers;
	all_synced &= params->start != 0;
	pthread_mutex_unlock(&params->sync);
	return (all_synced);
}

void	sync_thread(t_params *params)
{
	pthread_mutex_lock(&params->sync);
	params->synced++;
	pthread_mutex_unlock(&params->sync);
	while (!all_synced(params))
		; // proteger
}

void	set_start(t_philo *philos, t_params *params)
{
	unsigned int	i;

	pthread_mutex_lock(&params->sync);
	params->start = current_time_micros();
	i = 0;
	while (i < params->philosophers)
	{
		pthread_mutex_lock(&philos->prev_eat);
		philos->last_eat = params->start;
		pthread_mutex_unlock(&philos->prev_eat);
		philos = philos->next;
		i++;
	}
	pthread_mutex_unlock(&params->sync);
}
