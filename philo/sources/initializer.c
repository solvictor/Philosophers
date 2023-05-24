/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 00:29:49 by vegret            #+#    #+#             */
/*   Updated: 2023/05/24 18:45:41 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static t_philo	*new_philo(unsigned int n, t_params *params)
{
	t_philo	*new;

	new = malloc(sizeof(t_philo));
	if (!new)
		return (NULL);
	new->n = n;
	new->eats = 0;
	new->last_eat = 0;
	new->params = params;
	new->prev = new;
	new->next = new;
	return (new);
}

bool	init_philos(t_philo **philos, t_params *params)
{
	unsigned int	i;
	t_philo			*new;

	if (!philos || !params)
		return (EXIT_FAILURE);
	i = 0;
	while (i < params->philosophers)
	{
		new = new_philo(i + 1, params);
		if (!new)
			return (clear_nodes(philos), EXIT_FAILURE);
		if (*philos)
		{
			new->next = *philos;
			new->prev = (*philos)->prev;
			(*philos)->prev->next = new;
			(*philos)->prev = new;
		}
		else
			*philos = new;
		i++;
	}
	return (EXIT_SUCCESS);
}

bool	init_mutexes(t_philo *philos, t_params *params)
{
	unsigned int	i;
	bool			failed;

	if (!philos || !params)
		return (EXIT_FAILURE);
	failed = false;
	failed |= pthread_mutex_init(&params->display, NULL) != 0;
	failed |= pthread_mutex_init(&params->sync, NULL) != 0;
	failed |= pthread_mutex_init(&params->exit, NULL) != 0;
	failed |= pthread_mutex_init(&params->eat, NULL) != 0;
	i = 0;
	while (i < params->philosophers)
	{
		failed |= pthread_mutex_init(&philos->prev_eat, NULL) != 0;
		failed |= pthread_mutex_init(&philos->fork, NULL) != 0;
		philos = philos->next;
		i++;
	}
	return (failed);
}

static bool	join_threads(t_philo *philos, t_params *params)
{
	unsigned int	i;
	bool			failed;

	failed = false;
	i = 0;
	while (i < params->philosophers)
	{
		failed |= pthread_join(philos->thread, NULL) != 0;
		philos = philos->next;
		i++;
	}
	return (failed);
}

bool	init_threads(t_philo *philos, t_params *params)
{
	unsigned int	i;

	i = 0;
	while (i < params->philosophers)
	{
		if (pthread_create(&philos->thread, NULL, &philo_routine, philos) != 0)
		{
			params->should_exit = true;
			return (EXIT_FAILURE);
		}
		philos = philos->next;
		i++;
	}
	set_start(philos, params);
	watcher(philos, params);
	return (join_threads(philos, params));
}
