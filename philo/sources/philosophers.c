/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 20:49:48 by vegret            #+#    #+#             */
/*   Updated: 2023/02/27 00:11:01 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_state(t_philo *philo, char *action)
{
	t_ullong	current;
	t_params	*params;

	params = philo->params;
	if (check_stop(params))
		return ;
	pthread_mutex_lock(&params->died_mutex);
	current = current_time_micros();
	if (current - philo->last_eat >= params->time_to_die)
	{
		action = "is dead";
		params->one_died = true;
	}
	pthread_mutex_unlock(&params->died_mutex);
	pthread_mutex_lock(&params->print_mutex);
	printf("%lldms %u %s\n", (current - philo->start) / 1000, philo->n, action);
	pthread_mutex_unlock(&params->print_mutex);
}

void	*philo_routine(void *arg)
{
	t_philo			*philo;
	t_params		*params;
	bool			can_take_forks;
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;
	pthread_mutex_t	*first2;
	pthread_mutex_t	*second2;

	philo = (t_philo *) arg;
	philo->start = current_time_micros();
	params = philo->params;
	philo->last_eat = philo->start;
	if (philo->n % 2)
	{
		first = &philo->next->fork;
		second = &philo->fork;
	}
	else
	{
		first = &philo->fork;
		second = &philo->next->fork;
	}
	if (params->philosophers == 2)
	{
		first2 = &philo->forks_mutex;
		second2 = &philo->next->forks_mutex;
	}
	else if (philo->n % 4 < 2)
	{
		first2 = &philo->next->forks_mutex;
		second2 = &philo->prev->forks_mutex;
	}
	else
	{
		first2 = &philo->prev->forks_mutex;
		second2 = &philo->next->forks_mutex;
	}
	while (true)
	{
		if (check_stop(params))
			break ;
		if (current_time_micros() - philo->last_eat >= params->time_to_die)
		{
			print_state(philo, "is dead");
			break ;
		}
		if (params->philosophers == 1)
		{
			print_state(philo, "is thinking");
			print_state(philo, "has taken a fork");
			ft_usleep(philo, params->time_to_die);
			continue ;
		}
		pthread_mutex_lock(first2);
		pthread_mutex_lock(second2);
		can_take_forks = (philo->prev->forks == 0 && philo->next->forks == 0);
		pthread_mutex_unlock(first2);
		pthread_mutex_unlock(second2);
		if (can_take_forks)
		{
			print_state(philo, "is thinking");
			pthread_mutex_lock(&philo->forks_mutex);
			philo->forks++;
			pthread_mutex_unlock(&philo->forks_mutex);
			pthread_mutex_lock(first);
			print_state(philo, "has taken a fork");
			pthread_mutex_lock(&philo->forks_mutex);
			philo->forks--;
			pthread_mutex_unlock(&philo->forks_mutex);
			pthread_mutex_lock(second);
			print_state(philo, "has taken a fork");
			print_state(philo, "is eating");
			philo->last_eat = current_time_micros();
			philo->eats++;
			pthread_mutex_lock(&params->eat_mutex);
			if (philo->eats == params->time_must_eat)
				params->eat_enough++;
			pthread_mutex_unlock(&params->eat_mutex);
			if (check_stop(params))
			{
				pthread_mutex_unlock(second);
				pthread_mutex_unlock(first);
				break ;
			}
			ft_usleep(philo, params->time_to_eat);
			pthread_mutex_unlock(second);
			pthread_mutex_unlock(first);
			print_state(philo, "is sleeping");
			ft_usleep(philo, params->time_to_sleep);
		}
	}
	return (NULL);
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
