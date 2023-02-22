/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 00:31:57 by vegret            #+#    #+#             */
/*   Updated: 2023/02/22 01:05:45 by vegret           ###   ########.fr       */
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

bool	destroy_mutexes(t_philo *philos, t_params *params)
{
	unsigned int	i;
	bool			failed;

	if (!philos || !params)
		return (EXIT_FAILURE);
	failed = false;
	failed |= pthread_mutex_destroy(&params->print_mutex) != 0;
	failed |= pthread_mutex_destroy(&params->died_mutex) != 0;
	failed |= pthread_mutex_destroy(&params->eat_mutex) != 0;
	i = 0;
	while (i < params->philosophers)
	{
		failed |= pthread_mutex_destroy(&philos->fork) != 0;
		failed |= pthread_mutex_destroy(&philos->forks_mutex) != 0;
		philos = philos->next;
		i++;
	}
	return (failed);
}

void	ft_usleep(t_philo *philo, unsigned int micros)
{
	unsigned int	life;

	life = philo->last_eat + philo->params->time_to_die - current_time_micros();
	if (life < micros)
		micros = life;
	usleep(micros);
}
