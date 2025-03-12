/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vluo <vluo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:42:44 by vluo              #+#    #+#             */
/*   Updated: 2025/03/12 16:02:50 by vluo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	eating(t_philo *philo)
{
	struct timeval	tv;

	gettimeofday(&tv, 0);
	philo -> last_ate = tv;
	if (philo -> time_to_eat >= philo -> time_to_die)
	{
		usleep((philo -> time_to_die / 2) * 1000);
		gettimeofday(&tv, 0);
		philo -> last_ate = tv;
		usleep((philo -> time_to_die - (philo -> time_to_die / 2)) * 1000);
		gettimeofday(&tv, 0);
		philo -> last_ate = tv;
	}
	else
	{
		usleep((philo -> time_to_eat) * 1000);
		gettimeofday(&tv, 0);
		philo -> last_ate = tv;
	}
}

int	eat(t_philo *philo)
{
	int	st;

	st = philo -> start;
	sem_wait(philo -> locks -> forks);
	printf("%d %d has taken a fork\n", get_real_time() - st, philo->index + 1);
	sem_wait(philo -> locks -> forks);
	printf("%d %d has taken a fork\n", get_real_time() - st, philo->index + 1);
	printf("%d %d is eating\n", get_real_time() - st, philo->index + 1);
	eating(philo);
	sem_post(philo -> locks -> forks);
	sem_post(philo -> locks -> forks);
	philo -> nb_eat ++;
	if (philo -> nb_eat == philo -> nb_must_eat)
		return (sem_post(philo -> locks -> stop), 0);
	return (1);
}

int	ph_sleep(t_philo *philo)
{
	int	st;

	st = philo -> start;
	if (philo -> time_to_sleep > philo -> time_to_die)
	{
		printf("%d %d is sleeping\n", get_real_time() - st,
			philo -> index + 1);
		usleep(philo -> time_to_die * 1000);
		return (printf("%d %d died\n", get_real_time() - st, philo->index + 1),
			sem_post(philo -> locks -> finish), 0);
	}
	else
	{
		printf("%d %d is sleeping\n", get_real_time() - st,
			philo -> index + 1);
		usleep(philo -> time_to_sleep * 1000);
		return (1);
	}
	return (0);
}

void	philo_routine(t_philo *philo)
{
	int		st;

	st = philo -> start;
	if (philo -> time_to_die > philo -> time_to_eat)
		usleep((philo -> time_to_eat / 2) * 1000);
	else
		usleep((philo -> time_to_die / 2) * 1000);
	while (1)
	{
		if (philo -> stop != 1)
		{	
			printf("%d %d is thinking\n", get_real_time() - st,
				philo->index + 1);
			if (!eat(philo))
				break ;
			if (!ph_sleep(philo))
				break ;
		}
	}
	philo -> stop = 1;
	return ;
}

void	lone_philo(t_philo *philo)
{
	int	st;

	st = philo -> start;
	printf("%d %d is thinking\n", get_real_time() - st, philo -> index + 1);
	sem_wait(philo -> locks -> forks);
	printf("%d %d has taken a fork\n", get_real_time() - st, philo -> index + 1);
	usleep(philo -> time_to_die * 1000);
	printf("%d %d died\n", get_real_time() - st, philo->index + 1);
	sem_post(philo -> locks -> finish);
}
