/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 18:16:51 by vegret            #+#    #+#             */
/*   Updated: 2023/05/15 15:37:50 by vegret           ###   ########.fr       */
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
		pthread_mutex_lock(&philo->params->eat);
		philo->params->eat_enough++;
		pthread_mutex_unlock(&philo->params->eat);
	}
	usleep(philo->params->time_to_eat);
}

static void	choose_forks(t_philo *philo)
{
	philo->first = &philo->fork;
	philo->second = &philo->next->fork;
	if (philo->n % 2)
	{
		philo->first = &philo->next->fork;
		philo->second = &philo->fork;
	}
}

static bool	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->first);
	print_state(philo, "has taken a fork");
	if (philo->params->philosophers == 1)
	{
		pthread_mutex_unlock(philo->first);
		return (EXIT_FAILURE);
	}
	pthread_mutex_lock(philo->second);
	print_state(philo, "has taken a fork");
	return (EXIT_SUCCESS);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	choose_forks(philo);
	sync_thread(philo->params);
	if (philo->n & 1)
		usleep(10000);
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
		usleep(philo->params->time_to_sleep);
		print_state(philo, "is thinking");
		usleep(10);
	}
	return (NULL);
}

void	watcher(t_philo *philos, t_params *params)
{
	t_ulong	time;

	usleep(20000);
	while (true)
	{
		pthread_mutex_lock(&philos->prev_eat);
		time = current_time_micros();
		if (time - philos->last_eat >= params->time_to_die)
		{
			pthread_mutex_unlock(&philos->prev_eat);
			pthread_mutex_lock(&params->exit);
			params->should_exit = true;
			pthread_mutex_unlock(&params->exit);
			pthread_mutex_lock(&params->display);
			printf("%lums %u died\n", (time - params->start) / 1000, philos->n);
			pthread_mutex_unlock(&params->display);
			break ;
		}
		pthread_mutex_unlock(&philos->prev_eat);
		if (check_stop(params))
			break ;
		philos = philos->next;
		usleep(1000);
	}
}
