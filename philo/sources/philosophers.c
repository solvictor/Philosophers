/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 20:49:48 by vegret            #+#    #+#             */
/*   Updated: 2023/02/15 22:56:02 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_state(t_philo *philo, char *action)
{
	const long	current = current_time_millis();

	if (current == 0 || !philo)
		return ;
	pthread_mutex_lock(&philo->params->print_mutex);
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
	if (!params->start)
		return (NULL);
	print_state(philo, "is eating");
	while (true)
	{
		pthread_mutex_lock(&params->died_mutex);
		if (params->one_died)
			break ;
		pthread_mutex_unlock(&params->died_mutex);
		
		//print_state(philo, "is eating");
	}
	pthread_mutex_unlock(&params->died_mutex);
	pthread_mutex_destroy(&philo->mutex);
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
	if (init_philos(&philos, &params)
		|| init_mutexes(philos, &params))
		return (clear_nodes(&philos), EXIT_FAILURE);
	init_threads(philos, &params);
	pthread_mutex_destroy(&params.died_mutex);
	pthread_mutex_destroy(&params.print_mutex);
	return (clear_nodes(&philos), EXIT_SUCCESS);
}
