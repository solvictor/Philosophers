/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 20:49:48 by vegret            #+#    #+#             */
/*   Updated: 2023/02/16 22:33:58 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_state(t_philo *philo, char *action)
{
	const long	current = current_time_millis();

	if (current == 0)
		return ;
	pthread_mutex_lock(&philo->params->print_mutex);
	pthread_mutex_lock(&philo->params->died_mutex);
	if (philo->params->one_died)
	{
		pthread_mutex_unlock(&philo->params->died_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->params->died_mutex);
	printf("%ldms %u %s\n", current - philo->params->start, philo->n, action);
	pthread_mutex_unlock(&philo->params->print_mutex);
}

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	t_params	*params;

	philo = (t_philo *) arg;
	params = philo->params;
	params->start = current_time_millis();
	if (!params->start || !philo)
		return (NULL);
	philo->last_eat = params->start;
	if (philo->n % 2)
		usleep(100);
	while (true)
	{
		pthread_mutex_lock(&params->died_mutex);
		if (params->one_died)
			break ;
		pthread_mutex_unlock(&params->died_mutex);
		pthread_mutex_lock(&params->eat_mutex);
		if (params->time_must_eat != 0 && params->eat_enough >= params->philosophers)
			break ;
		pthread_mutex_unlock(&params->eat_mutex);
		if (current_time_millis() - philo->last_eat > params->time_to_die)
		{
			print_state(philo, "is dead");
			pthread_mutex_lock(&params->died_mutex);
			params->one_died = true;
			pthread_mutex_unlock(&params->died_mutex);
			break ;
		}
		pthread_mutex_lock(&philo->fork_mutex);
		print_state(philo, "has taken a fork");
		if (philo == philo->next)
		{
			usleep(params->time_to_die * 1000 + 1000);
			continue ;
		}
		pthread_mutex_lock(&philo->next->fork_mutex);
		print_state(philo, "has taken a fork");
		print_state(philo, "\033[0;32mis eating\e[m");
		philo->last_eat = current_time_millis();
		if (params->time_must_eat != 0)
		{
			philo->eats++;
			if (params->time_must_eat == philo->eats)
			{
				pthread_mutex_lock(&params->eat_mutex);
				params->eat_enough++;
				if (params->eat_enough >= params->philosophers)
					break ;
				pthread_mutex_unlock(&params->eat_mutex);
			}
		}
		usleep(params->time_to_eat);
		pthread_mutex_unlock(&philo->fork_mutex);
		pthread_mutex_unlock(&philo->next->fork_mutex);
		print_state(philo, "is sleeping");
		usleep(params->time_to_sleep);
		print_state(philo, "is thinking");
	}
	printf("Thread %u stopped!\n", philo->n);
	pthread_mutex_unlock(&params->died_mutex);
	pthread_mutex_destroy(&philo->fork_mutex);
	return (NULL);
}

int	main(int argc, char const *argv[])
{
	t_params	params;
	t_philo		*philos;

	if (!(argc == 5 || argc == 6))
		return (EXIT_FAILURE);
	if (parse_params(argc, argv, &params))
		return (EXIT_FAILURE);
	philos = NULL;
	params.time_to_eat *= 1000;
	params.time_to_sleep *= 1000;
	if (init_philos(&philos, &params)
		|| init_mutexes(philos, &params))
		return (clear_nodes(&philos), EXIT_FAILURE);
	init_threads(philos, &params);
	pthread_mutex_destroy(&params.died_mutex);
	pthread_mutex_destroy(&params.print_mutex);
	pthread_mutex_destroy(&params.eat_mutex);
	return (clear_nodes(&philos), EXIT_SUCCESS);
}
