/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 20:45:21 by vegret            #+#    #+#             */
/*   Updated: 2023/02/15 00:01:32 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <limits.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>
# define EAT	0b0001
# define SLEEP	0b0010
# define THINK	0b0100
# define DEAD	0b1000

typedef struct s_params {
	unsigned int	philosophers;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	time_must_eat;
}				t_params;

typedef struct s_philo {
	struct s_philo	*prev;
	unsigned int	n;
	unsigned int	forks; // ptet inutile
	unsigned char	state;
	pthread_t		thread;
	pthread_mutex_t	mutex;
	t_params		params;
	struct s_philo	*next;
}				t_philo;

typedef struct s_table
{
	t_philo	*first;
}				t_table;

// Parser
bool	parse_params(int argc, char const *argv[], t_params *params);

// Initializer
bool	init_table(t_table *table, t_params *params);

// Utils
void	clear_nodes(t_table *table);
void	reset_params(t_params *params);
bool	has_dead(t_philo *start);

void	*philo_routine(void *philo);

#endif
