/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 20:49:48 by vegret            #+#    #+#             */
/*   Updated: 2023/02/15 00:27:45 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_state(t_philo *philo, struct timeval *start)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) != 0)
		return ;
	printf("Test %lums %u\n", (time.tv_usec - start->tv_usec) / 1000, philo->n);
}

void	*philo_routine(void *philo)
{
	t_philo			*phil;
	struct timeval	start;

	phil = (t_philo *) philo;
	if (gettimeofday(&start, NULL) != 0)
		return (NULL);
	printf("Thread %u\n", phil->n);
	pthread_mutex_init(&phil->mutex, NULL);
	while (!has_dead(phil))
	{
		if (phil->state == THINK)
		{
			if (phil->forks == 1 && phil->next->forks == 1)
			{
				pthread_mutex_lock(&phil->mutex);
				phil->forks++;
				phil->next->forks--;
				phil->state = EAT;
				pthread_mutex_unlock(&phil->mutex);
			}
		}
		else if (phil->state == SLEEP)
		{

		}
		else if (phil->state == EAT)
		{
			
		}
		print_state(phil, &start);
	}
	pthread_mutex_destroy(&phil->mutex);
	return (NULL);
}

int	main(int argc, char const *argv[])
{
	t_params	params;
	t_table		table;

	if (!(argc == 5 || argc == 6))
		return (EXIT_FAILURE);
	if (parse_params(argc, argv, &params))
		return (EXIT_FAILURE);
	if (init_table(&table, &params))
		return (EXIT_FAILURE);
	t_philo *cur = table.first;
	for (size_t i = 0; i < params.philosophers; i++)
	{
		cur->params = params;
		pthread_create(&cur->thread, NULL,
			&philo_routine, (void *) cur);
		cur = cur->next;
	}
	cur = table.first;
	for (size_t i = 0; i < params.philosophers; i++)
	{
		pthread_join(cur->thread, NULL);
		cur = cur->next;
	}
	clear_nodes(&table);
	return (EXIT_SUCCESS);
}
