/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 20:49:48 by vegret            #+#    #+#             */
/*   Updated: 2023/04/19 18:17:06 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char const *argv[])
{
	int			state;
	t_params	params;
	t_philo		*philos;

	if (argc < 5 || argc > 6)
		return (EXIT_FAILURE);
	if (parse_params(argc, argv, &params))
		return (EXIT_FAILURE);
	philos = NULL;
	params.time_to_eat *= 1000;
	params.time_to_sleep *= 1000;
	params.time_to_die *= 1000;
	if (init_philos(&philos, &params))
		return (clear_nodes(&philos), EXIT_FAILURE);
	if (init_mutexes(philos, &params))
	{
		state = destroy_mutexes(philos, &params);
		return (clear_nodes(&philos), state);
	}
	state = init_threads(philos, &params);
	state |= destroy_mutexes(philos, &params);
	return (clear_nodes(&philos), state);
}
