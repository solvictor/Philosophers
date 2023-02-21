/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 20:45:21 by vegret            #+#    #+#             */
/*   Updated: 2023/02/20 17:44:12 by vegret           ###   ########.fr       */
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

typedef struct s_params {
	bool			one_died;
	pthread_mutex_t	died_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	eat_mutex;
	unsigned int	philosophers;
	unsigned int	eat_enough;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	time_must_eat;
}				t_params;

typedef struct s_philo {
	struct s_philo	*prev;
	pthread_t		thread;
	pthread_mutex_t	fork_mutex;
	t_params		*params;
	long long		start;
	long long		last_eat;
	unsigned int	n;
	unsigned int	eats;
	unsigned int	forks;
	struct s_philo	*next;
}				t_philo;

// Parser
bool		parse_params(int argc, char const *argv[], t_params *params);

// Initializer
bool		init_philos(t_philo **philos, t_params *params);
bool		init_mutexes(t_philo *philos, t_params *params);
bool		init_threads(t_philo *philos, t_params *params);

// Utils
void		clear_nodes(t_philo **philos);
void		reset_params(t_params *params);
bool		destroy_mutexes(t_philo *philos, t_params *params);
long long	current_time_micros(void);
void		*philo_routine(void *philo);

#endif
