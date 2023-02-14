/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 00:29:49 by vegret            #+#    #+#             */
/*   Updated: 2023/02/14 18:40:28 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static bool	add_philo(t_philo **head, unsigned int n)
{
	t_philo	*new;

	new = malloc(sizeof(t_philo));
	if (!head || !new)
		return (EXIT_FAILURE);
	new->n = n;
	new->forks = 1;
	new->state = THINK;
	new->prev = new;
	new->next = new;
	if (*head)
	{
		new->next = *head;
		new->prev = (*head)->prev;
		(*head)->prev->next = new;
		(*head)->prev = new;
	}
	else
		*head = new;
	return (EXIT_SUCCESS);
}

bool	init_table(t_table *table, t_params *params)
{
	unsigned int	i;

	if (!table || !params)
		return (EXIT_FAILURE);
	i = 0;
	table->first = NULL;
	while (i < params->philosophers)
	{
		if (add_philo(&table->first, i + 1))
			return (clear_nodes(table), EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}
