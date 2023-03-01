/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 00:31:57 by vegret            #+#    #+#             */
/*   Updated: 2023/03/02 00:10:31 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_ullong	current_time_micros(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (0);
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}

bool	check_stop(t_params *params)
{
	bool	stop;

	pthread_mutex_lock(&params->died_mutex);
	stop = params->one_died;
	pthread_mutex_unlock(&params->died_mutex);
	pthread_mutex_lock(&params->eat_mutex);
	stop |= params->eat_enough >= params->philosophers;
	pthread_mutex_unlock(&params->eat_mutex);
	return (stop);
}

void	ft_usleep(unsigned int micros)
{
	t_ullong	start;

	start = current_time_micros();
	while (current_time_micros() - start < micros)
		usleep(5);
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

bool	destroy_mutexes(t_philo *philos, t_params *params)
{
	unsigned int	i;
	bool			failed;

	if (!philos || !params)
		return (EXIT_FAILURE);
	failed = false;
	failed |= pthread_mutex_destroy(&params->sync) != 0;
	failed |= pthread_mutex_destroy(&params->print_mutex) != 0;
	failed |= pthread_mutex_destroy(&params->died_mutex) != 0;
	failed |= pthread_mutex_destroy(&params->eat_mutex) != 0;
	i = 0;
	while (i < params->philosophers)
	{
		failed |= pthread_mutex_destroy(&philos->prev_eat) != 0;
		failed |= pthread_mutex_destroy(&philos->fork) != 0;
		failed |= pthread_mutex_destroy(&philos->forks_mutex) != 0;
		philos = philos->next;
		i++;
	}
	return (failed);
}
