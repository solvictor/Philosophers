/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 00:31:57 by vegret            #+#    #+#             */
/*   Updated: 2023/05/24 18:48:00 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_state(t_philo *philo, char *action)
{
	t_params	*params;

	params = philo->params;
	if (check_stop(params))
		return ;
	pthread_mutex_lock(&params->display);
	printf("%lums %u %s\n",
		(current_time_micros() - params->start) / 1000, philo->n, action);
	pthread_mutex_unlock(&params->display);
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

bool	check_stop(t_params *params)
{
	bool	stop;

	pthread_mutex_lock(&params->exit);
	stop = params->should_exit;
	pthread_mutex_unlock(&params->exit);
	pthread_mutex_lock(&params->eat);
	stop |= params->eat_enough >= params->philosophers;
	pthread_mutex_unlock(&params->eat);
	return (stop);
}

bool	destroy_mutexes(t_philo *philos, t_params *params)
{
	unsigned int	i;
	bool			failed;

	if (!philos || !params)
		return (EXIT_FAILURE);
	failed = false;
	failed |= pthread_mutex_destroy(&params->sync) != 0;
	failed |= pthread_mutex_destroy(&params->display) != 0;
	failed |= pthread_mutex_destroy(&params->exit) != 0;
	failed |= pthread_mutex_destroy(&params->eat) != 0;
	i = 0;
	while (i < params->philosophers)
	{
		failed |= pthread_mutex_destroy(&philos->prev_eat) != 0;
		failed |= pthread_mutex_destroy(&philos->fork) != 0;
		philos = philos->next;
		i++;
	}
	return (failed);
}

void	ft_usleep(t_philo *philo, unsigned int micros)
{
	unsigned int	time_left;

	time_left = philo->params->time_to_die;
	time_left -= current_time_micros() - philo->last_eat;
	if (micros > time_left)
		micros = time_left;
	usleep(micros);
}
