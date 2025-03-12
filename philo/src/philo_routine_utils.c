/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vluo <vluo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 13:54:15 by vluo              #+#    #+#             */
/*   Updated: 2025/03/12 13:54:43 by vluo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	check_died(t_philo *philo)
{
	pthread_mutex_lock(philo -> died_lock);
	if (philo -> died == 1)
		return (pthread_mutex_unlock(philo -> died_lock), 1);
	return (pthread_mutex_unlock(philo -> died_lock), 0);
}

int	can_take_fork(t_philo *philo)
{
	pthread_mutex_lock(philo -> mutexs[philo -> index]);
	if (check_died(philo) == 1)
	{
		pthread_mutex_unlock(philo -> mutexs[philo -> index]);
		return (0);
	}
	printf("%d %d has taken a fork\n", get_real_time() - philo -> start,
		philo->index + 1);
	pthread_mutex_lock(philo->mutexs[(philo->index + 1) % philo->nb_philos]);
	if (check_died(philo) == 1)
	{
		pthread_mutex_unlock(philo -> mutexs[philo -> index]);
		pthread_mutex_unlock((philo->mutexs[(philo->index + 1)
				% philo->nb_philos]));
		return (0);
	}
	printf("%d %d has taken a fork\n", get_real_time() - philo -> start,
		philo->index + 1);
	return (1);
}

void	eating(t_philo *philo)
{
	struct timeval	tv;

	if (philo -> time_to_eat >= philo -> time_to_die)
	{
		usleep((philo -> time_to_die / 2) * 1000);
		pthread_mutex_lock(philo -> la_lock);
		gettimeofday(&tv, 0);
		philo -> last_ate = tv;
		pthread_mutex_unlock(philo -> la_lock);
		usleep((philo -> time_to_die - (philo -> time_to_die / 2)) * 1000);
		pthread_mutex_lock(philo -> la_lock);
		gettimeofday(&tv, 0);
		philo -> last_ate = tv;
		pthread_mutex_unlock(philo -> la_lock);
	}
	else
	{
		usleep(philo -> time_to_eat * 1000);
		pthread_mutex_lock(philo -> la_lock);
		gettimeofday(&tv, 0);
		philo -> last_ate = tv;
		pthread_mutex_unlock(philo -> la_lock);
	}
}

int	eat(t_philo *philo)
{
	struct timeval	tv;
	int				st;

	st = philo -> start;
	if (!can_take_fork(philo))
		return (0);
	pthread_mutex_lock(philo -> la_lock);
	gettimeofday(&tv, 0);
	philo -> last_ate = tv;
	pthread_mutex_unlock(philo -> la_lock);
	printf("%d %d is eating\n", get_real_time() - st, philo->index + 1);
	eating(philo);
	if (check_died(philo))
		return (pthread_mutex_unlock((philo -> mutexs[philo -> index])),
			pthread_mutex_unlock(philo->mutexs[(philo->index + 1)
					% philo->nb_philos]), 0);
	pthread_mutex_lock(philo -> eat_lock);
	philo -> nb_eat ++;
	pthread_mutex_unlock(philo -> eat_lock);
	pthread_mutex_unlock((philo -> mutexs[philo -> index]));
	pthread_mutex_unlock(philo->mutexs[(philo->index + 1) % philo->nb_philos]);
	return (1);
}

int	ph_sleep(t_philo *philo)
{
	int	st;

	st = philo -> start;
	if (!check_died(philo))
	{
		if (philo -> time_to_sleep > philo -> time_to_die)
		{	
			printf("%d %d is sleeping\n", get_real_time() - st,
				philo -> index + 1);
			usleep(philo -> time_to_die * 1000);
			pthread_mutex_lock(philo -> died_lock);
			philo -> died = 1;
			return (pthread_mutex_unlock(philo -> died_lock), 0);
		}
		else if (!check_died(philo))
		{
			printf("%d %d is sleeping\n", get_real_time() - st,
				philo -> index + 1);
			usleep(philo -> time_to_sleep * 1000);
			return (1);
		}
	}
	return (0);
}
