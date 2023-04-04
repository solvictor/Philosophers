/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 01:16:43 by vegret            #+#    #+#             */
/*   Updated: 2023/04/04 14:21:13 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	reset_params(t_params *params)
{
	params->philosophers = 0;
	params->synced = 0;
	params->start = 0;
	params->time_to_die = 0;
	params->time_to_eat = 0;
	params->time_to_sleep = 0;
	params->eat_enough = 0;
	params->time_must_eat = 0;
	params->should_exit = false;
}

static bool	parse_uint(char const *str, unsigned int *dst)
{
	unsigned long	res;
	int				i;

	res = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9' || res > UINT_MAX - 1)
			return (EXIT_FAILURE);
		else
		{
			res = res * 10 + str[i] - '0';
		}
		i++;
	}
	if (res > UINT_MAX - 1)
		return (EXIT_FAILURE);
	*dst = (unsigned int) res;
	return (EXIT_SUCCESS);
}

bool	parse_params(int argc, char const *argv[], t_params *params)
{
	if (!params)
		return (EXIT_FAILURE);
	reset_params(params);
	if (parse_uint(argv[1], &params->philosophers) || !params->philosophers)
		return (EXIT_FAILURE);
	if (parse_uint(argv[2], &params->time_to_die) || !params->time_to_die)
		return (EXIT_FAILURE);
	if (parse_uint(argv[3], &params->time_to_eat) || !params->time_to_eat)
		return (EXIT_FAILURE);
	if (parse_uint(argv[4], &params->time_to_sleep) || !params->time_to_sleep)
		return (EXIT_FAILURE);
	if (argc == 6 && (parse_uint(argv[5], &params->time_must_eat)
			|| !params->time_must_eat))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
