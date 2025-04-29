/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vluo <vluo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 14:11:26 by vluo              #+#    #+#             */
/*   Updated: 2025/04/29 18:49:49 by vluo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	create_main_thread(t_process *process)
{
	pthread_t	died_thread;
	pthread_t	ate_thread;

	if (pthread_create(&died_thread, NULL, &died, process) != 0)
		return ;
	if (pthread_create(&ate_thread, NULL, &ate, process) != 0)
	{
		pthread_detach(died_thread);
		return ;
	}
	if (pthread_join(died_thread, NULL))
	{
		pthread_detach(ate_thread);
		return ;
	}
	if (pthread_join(ate_thread, NULL))
		return ;
}

void	*check(void	*philo)
{
	t_philo	*ph;
	int		la;
	int		st;

	ph = philo;
	st = ph -> start;
	while (ph -> stop != 1)
	{
		la = (ph -> last_ate.tv_sec * 1000 + ph -> last_ate.tv_usec / 1000);
		if (get_real_time() >= la + ph -> time_to_die)
		{
			printf("%d %d died\n", get_real_time() - st, ph -> index + 1);
			sem_post(ph -> locks -> finish);
			return (NULL);
		}
	}
	sem_post(ph -> locks -> stop);
	return (NULL);
}

void	create_proc_thread(t_philo *philo)
{
	pthread_t	proc;

	if (pthread_create(&proc, NULL, &check, philo) != 0)
		return ;
	if (philo -> nb_philos == 1)
		lone_philo(philo);
	else
		philo_routine(philo);
	if (pthread_join(proc, NULL) != 0)
		return ;
	return ;
}

t_process	*init_main_proc(int *pids, t_philo **philos)
{
	t_process	*procs;

	procs = malloc(sizeof(t_process));
	if (procs == 0)
		return (0);
	procs -> philos = philos;
	procs -> pids = pids;
	create_main_thread(procs);
	waitpids(pids);
	return (procs);
}

t_process	*init_process(int nb, t_philo **philos)
{
	int			i;
	int			*pids;

	pids = malloc(sizeof(int) * (nb + 1));
	pids[nb] = 0;
	if (pids == 0)
		return (NULL);
	i = -1;
	while (++i < nb)
	{
		pids[i] = fork();
		if (pids[i] < 0)
			return (free(pids), NULL);
		if (pids[i] == 0)
		{
			create_proc_thread(philos[i]);
			return (free(pids), NULL);
		}
	}
	return (init_main_proc(pids, philos));
}
