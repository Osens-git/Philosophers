/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vluo <vluo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 14:17:11 by vluo              #+#    #+#             */
/*   Updated: 2025/03/05 16:25:00 by vluo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	*died(void *process)
{
	t_process	*procs;
	int			i;

	procs = process;
	sem_wait(procs -> philos[0]->locks -> finish);
	i = -1;
	while (++i < procs -> philos[0]-> nb_philos)
		sem_post(procs -> philos[0]->locks -> stop);
	i = 0;
	while (procs -> pids[++i])
		kill(procs -> pids[i], SIGKILL);
	return (NULL);
}

void	*ate(void *process)
{
	t_process	*procs;
	int			i;
	int			j;

	procs = process;
	i = -1;
	while (++i < procs -> philos[0]-> nb_philos)
		sem_wait(procs -> philos[0]->locks -> stop);
	sem_post(procs -> philos[0]->locks -> finish);
	j = 0;
	while (procs -> pids[++j])
		kill(procs -> pids[j], SIGKILL);
	return (NULL);
}
