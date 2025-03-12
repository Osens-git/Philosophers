/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vluo <vluo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 11:34:04 by vluo              #+#    #+#             */
/*   Updated: 2025/03/12 13:55:33 by vluo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static pthread_t	*create_thread(t_philo *philo)
{
	pthread_t	*thread;

	thread = malloc(sizeof(pthread_t));
	if (thread == 0)
		return (0);
	if (pthread_create(thread, NULL, &philo_routine, philo) != 0)
		return (free(thread), NULL);
	return (thread);
}

static void	join_threads(pthread_t **threads, pthread_t *main)
{
	int	i;

	i = -1;
	while (threads[++i] != 0)
	{
		if (pthread_join(*threads[i], NULL) != 0)
			return (free_threads(threads));
	}
	if (pthread_join(*main, NULL) != 0)
		return (free_threads(threads));
}

pthread_t	**create_threads(t_philo **philos, int nb_philos, pthread_t *main)
{
	int			i;
	pthread_t	**threads;

	threads = malloc(sizeof(pthread_t *) * (nb_philos + 1));
	if (threads == 0)
		return (0);
	i = -1;
	while (++i < nb_philos)
	{
		threads[i] = create_thread(philos[i]);
		if (threads[i] == 0)
		{
			while (--i >= 0)
			{
				pthread_detach(*threads[i]);
				free(threads[i]);
			}
			return (free(threads), NULL);
		}
	}
	threads[i] = 0;
	join_threads(threads, main);
	return (threads);
}
