/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 20:49:48 by vegret            #+#    #+#             */
/*   Updated: 2023/02/13 18:46:06 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philo_routine(void *philo)
{
	const t_philo	*phil = (t_philo *) philo;

	printf("Thread %u\n", phil->n);
	return (NULL);
}

int	main(int argc, char const *argv[])
{
	t_params		params;
	t_table			table;
	struct timeval	start;

	if (!(argc == 5 || argc == 6))
		return (EXIT_FAILURE);
	if (gettimeofday(&start, NULL) != 0 || parse_params(argc, argv, &params))
		return (EXIT_FAILURE);
	if (init_table(&table, &params))
		return (EXIT_FAILURE);
	t_node *cur = table.head;
	for (size_t i = 0; i < params.philosophers; i++)
	{
		pthread_create(&cur->philo.thread, NULL,
			&philo_routine, (void *) &cur->philo);
		cur = cur->next;
	}
	cur = table.head;
	for (size_t i = 0; i < params.philosophers; i++)
	{
		pthread_join(cur->philo.thread, NULL);
		cur = cur->next;
	}
	clear_nodes(&table);
	return (EXIT_SUCCESS);
}
