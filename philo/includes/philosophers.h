/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 20:45:21 by vegret            #+#    #+#             */
/*   Updated: 2023/02/13 13:15:10 by vegret           ###   ########.fr       */
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
# define EAT	0b001
# define SLEEP	0b010
# define THINK	0b100

typedef struct s_params {
	unsigned int	philosophers;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	time_must_eat;
}				t_params;

// A modif
typedef struct s_philo {
	unsigned int	n;
	pthread_t		thread;
	unsigned char	state;
}				t_philo;

typedef struct s_node {
	struct s_node	*prev;
	t_philo			philo;
	struct s_node	*next;
}				t_node;

typedef struct s_table
{
	t_node	*head;
}				t_table;

// Parser
bool	parse_params(int argc, char const *argv[], t_params *params);

// Initializer
bool	init_table(t_table *table, t_params *params);

// Utils
void	clear_nodes(t_table *table);
void	reset_params(t_params *params);

#endif
