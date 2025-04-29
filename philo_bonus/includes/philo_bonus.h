/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vluo <vluo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:01:48 by vluo              #+#    #+#             */
/*   Updated: 2025/04/29 18:26:35 by vluo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/stat.h> 
# include <signal.h>
# include <sys/types.h>

# define FORKS	"forks"
# define STOP "stop"
# define FINISH "finish"
# define FORKS_LOCK "f_lock"

typedef struct s_locks
{
	sem_t	*f_lock;
	sem_t	*forks;
	sem_t	*finish;
	sem_t	*stop;
}	t_locks;

typedef struct s_philo
{
	t_locks			*locks;
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_must_eat;
	int				start;
	int				index;
	int				nb_eat;
	struct timeval	last_ate;
	int				stop;
}	t_philo;

typedef struct s_process
{
	t_philo	**philos;
	int		*pids;
}	t_process;

/* UTILS */

long		ft_atol(char *str);
void		free_locks(t_locks *locks);
void		free_philos(t_philo **philos);
void		killpids(int *pids, int index);
void		waitpids(int *pids);
int			get_real_time(void);

/* INIT STRUCT */

t_locks		*init_locks(int nb);
t_philo		*init_philo(char **argv, int index,
				struct timeval start_time, t_locks *locks);

/* ROUTINES */

void		lone_philo(t_philo *philo);
void		philo_routine(t_philo *philo);
t_process	*init_process(int nb, t_philo **philos);
void		*died(void *process);
void		*ate(void *process);

#endif
