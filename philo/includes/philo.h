/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vluo <vluo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:43:47 by vluo              #+#    #+#             */
/*   Updated: 2025/03/12 13:56:04 by vluo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	pthread_mutex_t	**mutexs;
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_must_eat;
	int				start;
	int				index;
	pthread_mutex_t	*eat_lock;
	int				nb_eat;
	struct timeval	last_ate;
	pthread_mutex_t	*la_lock;
	int				died;
	pthread_mutex_t	*died_lock;
}	t_philo;

/* UTILS */

long			ft_atol(char *str);
void			free_mutexs(pthread_mutex_t **mutexs);
void			free_philos(t_philo **philos);
void			free_threads(pthread_t **threads);
int				get_real_time(void);

/* INIT STRUCT */

pthread_mutex_t	**init_mutexs(int nb_philos);
t_philo			*init_philo(char **argv, pthread_mutex_t **mutexs, int index,
					struct timeval start_time);

/* PHILO ROUTINE */

int				check_died(t_philo *philo);
int				eat(t_philo *philo);
int				ph_sleep(t_philo *philo);
void			*philo_routine(void *philo);
pthread_t		**lone_philo(t_philo **philos);

/* THREADS */

void			*obs(void *philos);
pthread_t		**create_threads(t_philo **philos, int nb_philos,
					pthread_t *main);

#endif