/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vluo <vluo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:48:49 by vluo              #+#    #+#             */
/*   Updated: 2025/03/12 12:32:24 by vluo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

t_locks	*init_locks(int nb)
{
	t_locks	*locks;
	int		i;

	locks = malloc(sizeof(t_locks));
	if (locks == 0)
		return (0);
	sem_unlink(FORKS);
	locks -> forks = sem_open(FORKS, O_CREAT, S_IRUSR | S_IWUSR, 1);
	if (locks -> forks == SEM_FAILED)
		return (free(locks), NULL);
	i = 0;
	while (++i < nb)
		sem_post(locks -> forks);
	sem_unlink(FINISH);
	locks -> finish = sem_open(FINISH, O_CREAT, S_IRUSR | S_IWUSR, 1);
	sem_unlink(STOP);
	locks -> stop = sem_open(STOP, O_CREAT, S_IRUSR | S_IWUSR, 1);
	if (!(locks -> finish || locks -> stop))
		return (sem_close(locks -> forks), sem_close(locks -> finish),
			sem_close(locks -> stop), free(locks), NULL);
	sem_wait(locks -> finish);
	sem_wait(locks -> stop);
	return (locks);
}

void	free_locks(t_locks *locks)
{
	sem_close(locks->forks);
	sem_close(locks->finish);
	sem_close(locks->stop);
	free(locks);
}

t_philo	*init_philo(char **argv, int index,
	struct timeval start_time, t_locks *locks)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (philo == 0)
		return (NULL);
	philo -> locks = locks;
	if (locks == 0)
		return (free(philo), NULL);
	philo -> nb_philos = ft_atol(argv[1]);
	philo -> time_to_die = ft_atol(argv[2]);
	philo -> time_to_eat = ft_atol(argv[3]);
	philo -> time_to_sleep = ft_atol(argv[4]);
	philo -> nb_must_eat = -1;
	if (argv[5])
		philo -> nb_must_eat = ft_atol(argv[5]);
	philo -> index = index;
	philo -> start = (start_time.tv_sec * 1000) + (start_time.tv_usec / 1000);
	philo -> last_ate = start_time;
	philo -> nb_eat = 0;
	philo -> stop = 0;
	return (philo);
}

void	free_philos(t_philo **philos)
{
	int	i;

	i = -1;
	while (philos[++i])
		free(philos[i]);
	free(philos);
}
