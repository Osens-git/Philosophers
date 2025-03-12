/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vluo <vluo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 10:48:05 by vluo              #+#    #+#             */
/*   Updated: 2025/02/28 11:40:21 by vluo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	can_eat(t_philo **philos)
{
	int	i;
	int	la;

	i = -1;
	while (philos[++i])
	{
		pthread_mutex_lock(philos[i]->la_lock);
		la = (philos[i]->last_ate).tv_sec * 1000
			+ ((philos[i]->last_ate).tv_usec / 1000);
		if (get_real_time() - la >= philos[i]->time_to_die)
		{
			pthread_mutex_lock(philos[i]->died_lock);
			philos[i]->died = 1;
			pthread_mutex_unlock(philos[i]->died_lock);
			pthread_mutex_unlock(philos[i]->la_lock);
			return ;
		}
		pthread_mutex_unlock(philos[i]->la_lock);
	}
	return ;
}

int	all_ate(t_philo **philos)
{
	int	i;

	if (philos[0]->nb_must_eat == -1)
		return (0);
	i = -1;
	while (philos[++i])
	{
		pthread_mutex_lock(philos[i]->eat_lock);
		if (philos[i]->nb_eat < philos[i]->nb_must_eat)
		{
			pthread_mutex_unlock(philos[i]->eat_lock);
			break ;
		}
		pthread_mutex_unlock(philos[i]->eat_lock);
	}
	if (philos[i] != 0)
		return (0);
	i = -1;
	while (philos[++i])
	{	
		pthread_mutex_lock(philos[i]->died_lock);
		philos[i]->died = 1;
		pthread_mutex_unlock(philos[i]->died_lock);
	}
	return (1);
}

int	checks_died(t_philo **philos)
{
	int	i;

	i = 0;
	while (philos[i])
	{
		can_eat(philos);
		pthread_mutex_lock(philos[i]->died_lock);
		if (philos[i]->died == 1)
		{
			printf("%d %d died\n", get_real_time() - philos[0]->start, i + 1);
			pthread_mutex_unlock(philos[i]->died_lock);
			i = -1;
			while (philos[++i])
			{	
				pthread_mutex_lock(philos[i]->died_lock);
				philos[i]->died = 1;
				pthread_mutex_unlock(philos[i]->died_lock);
			}
			return (1);
		}
		pthread_mutex_unlock(philos[i]->died_lock);
		i ++;
	}
	return (0);
}

void	*obs(void *philos)
{
	while (1)
	{
		if (all_ate(philos) || checks_died(philos))
			return (NULL);
	}
}
