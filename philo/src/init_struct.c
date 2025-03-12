/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vluo <vluo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 11:21:17 by vluo              #+#    #+#             */
/*   Updated: 2025/02/27 10:44:40 by vluo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	create_mutex(pthread_mutex_t **mutexs, int i)
{
	mutexs[i] = malloc(sizeof(pthread_mutex_t));
	if (mutexs[i] == 0)
	{
		while (--i >= 0)
		{
			pthread_mutex_destroy(mutexs[i]);
			free(mutexs[i]);
		}
		return (free(mutexs));
	}
	if (pthread_mutex_init(mutexs[i], NULL) != 0)
	{
		while (-- i >= 0)
		{
			pthread_mutex_destroy(mutexs[i]);
			free(mutexs[i]);
		}
		return (free(mutexs));
	}
}

pthread_mutex_t	**init_mutexs(int nb_philos)
{
	int				i;
	pthread_mutex_t	**mutexs;

	mutexs = malloc(sizeof(pthread_mutex_t *) * (nb_philos + 1));
	if (mutexs == 0)
		return (0);
	i = -1;
	while (++i < nb_philos)
		create_mutex(mutexs, i);
	mutexs[i] = 0;
	return (mutexs);
}

int	init_locks(t_philo *ph)
{
	ph -> la_lock = malloc(sizeof(pthread_mutex_t));
	if (ph -> la_lock == 0)
		return (free(ph), 0);
	if (pthread_mutex_init(ph -> la_lock, NULL) != 0)
		return (free(ph -> la_lock), free(ph), 0);
	ph -> died_lock = malloc(sizeof(pthread_mutex_t));
	if (ph -> died_lock == 0)
		return (pthread_mutex_destroy(ph -> la_lock),
			free(ph -> la_lock), free(ph), 0);
	if (pthread_mutex_init(ph -> died_lock, NULL) != 0)
		return (pthread_mutex_destroy(ph -> la_lock),
			free(ph -> la_lock), free(ph -> died_lock), free(ph), 0);
	ph -> eat_lock = malloc(sizeof(pthread_mutex_t));
	if (ph -> eat_lock == 0)
		return (pthread_mutex_destroy(ph -> la_lock), free(ph -> la_lock),
			pthread_mutex_destroy(ph -> died_lock), free(ph -> died_lock),
			free(ph), 0);
	if (pthread_mutex_init(ph -> eat_lock, NULL) != 0)
		return (pthread_mutex_destroy(ph -> la_lock), free(ph -> la_lock),
			pthread_mutex_destroy(ph -> died_lock), free(ph -> died_lock),
			free(ph -> eat_lock), free(ph), 0);
	return (1);
}

t_philo	*init_philo(char **argv, pthread_mutex_t **mutexs, int index,
	struct timeval start_time)
{
	t_philo			*philo;

	philo = malloc(sizeof(t_philo));
	if (philo == 0)
		return (NULL);
	philo -> nb_philos = ft_atol(argv[1]);
	philo -> time_to_die = ft_atol(argv[2]);
	philo -> time_to_eat = ft_atol(argv[3]);
	philo -> time_to_sleep = ft_atol(argv[4]);
	if (argv[5])
		philo -> nb_must_eat = ft_atol(argv[5]);
	else
		philo -> nb_must_eat = -1;
	philo -> mutexs = mutexs;
	philo -> index = index;
	philo -> start = (start_time.tv_sec * 1000) + (start_time.tv_usec / 1000);
	philo -> last_ate = start_time;
	philo -> nb_eat = 0;
	philo -> died = 0;
	if (init_locks(philo))
		return (philo);
	return (NULL);
}
