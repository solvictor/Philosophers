/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 18:16:51 by vegret            #+#    #+#             */
/*   Updated: 2023/03/02 18:53:57 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	eat(t_philo *philo)
{
	print_state(philo, "is eating");
	pthread_mutex_lock(&philo->prev_eat);
	philo->last_eat = current_time_micros();
	pthread_mutex_unlock(&philo->prev_eat);
	philo->eats++;
	if (philo->eats == philo->params->time_must_eat)
	{
		pthread_mutex_lock(&philo->params->eat_mutex);
		philo->params->eat_enough++;
		pthread_mutex_unlock(&philo->params->eat_mutex);
	}
	ft_usleep(philo->params->time_to_eat);
}

static bool	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork);
	print_state(philo, "has taken a fork");
	if (philo->params->philosophers == 1)
	{
		pthread_mutex_unlock(&philo->fork);
		return (EXIT_FAILURE);
	}
	pthread_mutex_lock(&philo->next->fork);
	print_state(philo, "has taken a fork");
	return (EXIT_SUCCESS);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	sync_thread(philo->params);
	if (philo->n % 2 == 0)
		ft_usleep(10000);
	while (true)
	{
		if (check_stop(philo->params))
			break ;
		if (take_forks(philo))
			break ;
		eat(philo);
		pthread_mutex_unlock(&philo->next->fork);
		pthread_mutex_unlock(&philo->fork);
		print_state(philo, "is sleeping");
		ft_usleep(philo->params->time_to_sleep);
		print_state(philo, "is thinking");
	}
	return (NULL);
}

void	watcher(t_philo *philos, t_params *params)
{
	t_ullong	time;

	ft_usleep(20000);
	while (true)
	{
		pthread_mutex_lock(&philos->prev_eat);
		time = current_time_micros();
		if (time - philos->last_eat >= params->time_to_die)
		{
			pthread_mutex_unlock(&philos->prev_eat);
			pthread_mutex_lock(&params->died_mutex);
			params->one_died = true;
			pthread_mutex_unlock(&params->died_mutex);
			printf("%lldms %u died\n", (time - params->start) / 1000, philos->n);
			break ;
		}
		pthread_mutex_unlock(&philos->prev_eat);
		if (check_stop(params))
			break ;
		philos = philos->next;
	}
}
