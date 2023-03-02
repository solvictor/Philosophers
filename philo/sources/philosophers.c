/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 20:49:48 by vegret            #+#    #+#             */
/*   Updated: 2023/03/02 01:53:10 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_state(t_philo *philo, char *action)
{
	t_params	*params;

	params = philo->params;
	if (check_stop(params))
		return ;
	pthread_mutex_lock(&params->print_mutex);
	printf("%lldms %u %s\n",
		(current_time_micros() - params->start) / 1000, philo->n, action);
	pthread_mutex_unlock(&params->print_mutex);
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
		pthread_mutex_lock(&philo->fork);
		print_state(philo, "has taken a fork");
		if (philo->params->philosophers == 1)
		{
			pthread_mutex_unlock(&philo->fork);
			break ;
		}
		pthread_mutex_lock(&philo->next->fork);
		print_state(philo, "has taken a fork");
		print_state(philo, "is eating");
		pthread_mutex_lock(&philo->prev_eat);
		philo->last_eat = current_time_micros();
		pthread_mutex_unlock(&philo->prev_eat);
		if (++philo->eats == philo->params->time_must_eat)
		{
			pthread_mutex_lock(&philo->params->eat_mutex);
			philo->params->eat_enough++;
			pthread_mutex_unlock(&philo->params->eat_mutex);
		}
		ft_usleep(philo->params->time_to_eat);
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
		pthread_mutex_lock(&params->eat_mutex);
		if (params->time_must_eat && philos->eats == params->time_must_eat)
			params->eat_enough++;
		pthread_mutex_unlock(&params->eat_mutex);
		if (check_stop(params))
			break ;
		philos = philos->next;
	}
}

int	main(int argc, char const *argv[])
{
	int			state;
	t_params	params;
	t_philo		*philos;

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
