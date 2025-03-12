/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vluo <vluo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 16:42:39 by vluo              #+#    #+#             */
/*   Updated: 2025/02/27 12:27:55 by vluo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long	ft_atol(char *str)
{
	int		i;
	long	signe;
	long	res;

	i = 0;
	signe = 1;
	while (str[i] && (str[i] == ' ' || (str[i] >= 8 && str[i] <= 13)))
		i ++;
	if (str[i] && (str[i] == '+' || str[i] == '-'))
	{
		if (str[i] == '-')
			signe = -1;
		i ++;
	}
	res = 0;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
		res = (res * 10) + (str[i ++] - '0');
	return (res * signe);
}

void	free_mutexs(pthread_mutex_t **mutexs)
{
	int	i;

	i = -1;
	while (mutexs[++i] != 0)
	{
		pthread_mutex_destroy(mutexs[i]);
		free(mutexs[i]);
	}
	free(mutexs);
}

void	free_philos(t_philo **philos)
{
	int	i;

	free_mutexs(philos[0]-> mutexs);
	pthread_mutex_destroy(philos[0]->la_lock);
	free(philos[0]->la_lock);
	pthread_mutex_destroy(philos[0]->died_lock);
	free(philos[0]->died_lock);
	pthread_mutex_destroy(philos[0]->eat_lock);
	free(philos[0]->eat_lock);
	free(philos[0]);
	i = 0;
	while (philos[++i] != 0)
	{
		pthread_mutex_destroy(philos[i]->la_lock);
		free(philos[i]->la_lock);
		pthread_mutex_destroy(philos[i]->died_lock);
		free(philos[i]->died_lock);
		pthread_mutex_destroy(philos[i]->eat_lock);
		free(philos[i]->eat_lock);
		free(philos[i]);
	}
	free(philos);
}

void	free_threads(pthread_t **threads)
{
	int	i;

	if (threads != 0)
	{
		i = -1;
		while (threads[++i] != 0)
			free(threads[i]);
		free(threads);
	}
}

int	get_real_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, 0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
