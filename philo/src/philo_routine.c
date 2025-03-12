/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vluo <vluo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 13:54:55 by vluo              #+#    #+#             */
/*   Updated: 2025/03/12 14:26:27 by vluo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*philo_routine(void *philo)
{
	t_philo	*ph;
	int		st;

	ph = philo;
	st = ph -> start;
	if (ph -> index % 2 == 1)
	{
		if (ph -> time_to_die > ph -> time_to_eat)
			usleep((ph -> time_to_eat / 2) * 1000);
		else
			usleep((ph -> time_to_die / 2) * 1000);
	}
	while (!check_died(philo))
	{
		printf("%d %d is thinking\n", get_real_time() - st, ph -> index + 1);
		if (!eat(philo))
			return (NULL);
		if (!ph_sleep(philo))
			return (NULL);
	}
	return (NULL);
}

void	*l_rout(void *philo)
{
	t_philo	*ph;

	ph = philo;
	printf("%d %d is thinking\n", get_real_time() - ph -> start, 1);
	pthread_mutex_lock(ph -> mutexs[0]);
	printf("%d %d has taken a fork\n", get_real_time() - ph -> start, 1);
	usleep(ph -> time_to_die * 1000);
	printf("%d %d died\n", get_real_time() - ph -> start, 1);
	return (pthread_mutex_unlock(ph -> mutexs[0]), NULL);
}

pthread_t	**lone_philo(t_philo **philos)
{
	pthread_t	p;

	if (pthread_create(&p, NULL, &l_rout, philos[0]) != 0)
		return (NULL);
	if (pthread_join(p, NULL) != 0)
		return (NULL);
	return (NULL);
}
