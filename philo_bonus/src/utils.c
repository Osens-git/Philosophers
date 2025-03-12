/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vluo <vluo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:31:07 by vluo              #+#    #+#             */
/*   Updated: 2025/02/28 16:46:50 by vluo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

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

void	waitpids(int *pids)
{
	int	i;

	i = -1;
	while (pids[++i] != 0)
		waitpid(pids[i], NULL, 0);
}

void	killpids(int *pids, int index)
{
	int	i;

	i = 0;
	while (++i < index)
		kill(pids[i], SIGKILL);
	return ;
}

int	get_real_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, 0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
