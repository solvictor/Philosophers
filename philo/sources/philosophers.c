/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 20:49:48 by vegret            #+#    #+#             */
/*   Updated: 2023/02/20 20:45:16 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_state(t_philo *philo, char *action)
{
	const long long	current = current_time_micros();

	if (current == 0)
		return ;
	pthread_mutex_lock(&philo->params->print_mutex);
	pthread_mutex_lock(&philo->params->died_mutex);
	if (philo->params->one_died)
	{
		pthread_mutex_unlock(&philo->params->died_mutex);
		pthread_mutex_unlock(&philo->params->print_mutex);
		return ;
	}
	if (current_time_micros() - philo->last_eat >= philo->params->time_to_die)
	{
		printf("%lldms %u is dead\n", (current - philo->start) / 1000, philo->n);
		philo->params->one_died = true;
		pthread_mutex_unlock(&philo->params->died_mutex);
		pthread_mutex_unlock(&philo->params->print_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->params->died_mutex);
	printf("%lldms %u %s\n", (current - philo->start) / 1000, philo->n, action);
	pthread_mutex_unlock(&philo->params->print_mutex);
}

void	*philo_routine(void *arg)
{
	t_philo			*philo;
	t_params		*params;
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	philo = (t_philo *) arg;
	params = philo->params;
	philo->start = current_time_micros();
	if (!philo || !philo->start)
		return (NULL);
	philo->last_eat = philo->start;
	if (philo->n % 2)
	{
		first = &philo->next->fork_mutex;
		second = &philo->fork_mutex;
	}
	else
	{
		first = &philo->fork_mutex;
		second = &philo->next->fork_mutex;
	}
	while (true)
	{
		pthread_mutex_lock(&params->died_mutex);
		if (params->one_died)
			break ;
		pthread_mutex_unlock(&params->died_mutex);
		if (current_time_micros() - philo->last_eat >= params->time_to_die)
		{
			print_state(philo, "is dead");
			pthread_mutex_lock(&params->died_mutex);
			params->one_died = true;
			break ;
		}
		print_state(philo, "is thinking");
		pthread_mutex_lock(first);
		print_state(philo, "has taken a fork");
		if (philo == philo->next)
		{
			usleep(params->time_to_die + 1000);
			pthread_mutex_unlock(first);
			continue ;
		}
		pthread_mutex_lock(second);
		print_state(philo, "has taken a fork");
		print_state(philo, "is eating");
		philo->last_eat = current_time_micros();
		usleep(params->time_to_eat);
		pthread_mutex_unlock(second);
		pthread_mutex_unlock(first);
		print_state(philo, "is sleeping");
		usleep(params->time_to_sleep);
	}
	pthread_mutex_unlock(&params->died_mutex);
	return (NULL);
}

int	main(int argc, char const *argv[])
{
	t_params	params;
	t_philo		*philos;
	int			state;

	if (!(argc == 5 || argc == 6))
		return (EXIT_FAILURE);
	if (parse_params(argc, argv, &params))
		return (EXIT_FAILURE);
	philos = NULL;
	params.time_to_eat *= 1000;
	params.time_to_sleep *= 1000;
	params.time_to_die *= 1000;
	if (init_philos(&philos, &params)
		|| init_mutexes(philos, &params))
		return (clear_nodes(&philos), EXIT_FAILURE);
	init_threads(philos, &params);
	state = destroy_mutexes(philos, &params);
	return (clear_nodes(&philos), state);
}
