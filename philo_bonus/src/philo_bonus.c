/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vluo <vluo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:01:25 by vluo              #+#    #+#             */
/*   Updated: 2025/03/12 14:24:13 by vluo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

static int	check_isnb(char *arg)
{
	int	i;

	i = 0;
	while ((arg[i] >= 9 && arg[i] <= 13) || arg[i] == 32)
		i ++;
	if (!(arg[i] == '+' || arg[i] == '-'))
		i --;
	while (arg[++i])
		if (!(arg[i] >= '0' && arg[i] <= '9'))
			return (arg[i] == 0);
	return (1);
}

static int	check_args(int argc, char **argv)
{
	int		i;
	long	nb;

	i = 1;
	while (i < argc)
	{
		nb = ft_atol(argv[i]);
		if (!check_isnb(argv[i]))
			return (printf("Error : Arg %d not a number\n", i), 0);
		if (nb == 0 && i == 5)
			return (0);
		if (nb == 0 && (i == 1 || i == 2))
			return (printf("Error : Arg %d is 0\n", i), 0);
		if (nb < 0)
			return (printf("Error: Arg %d is a Negative number\n", i), 0);
		if (INT_MIN > nb || nb > INT_MAX)
			return (printf("Error: Arg %d is not an Integer\n", i), 0);
		i ++;
	}
	return (1);
}

t_philo	**parse_args(char **argv, t_locks *locks)
{
	t_philo			**philos;
	int				i;
	struct timeval	tv;

	philos = malloc(sizeof(t_philo *) * (ft_atol(argv[1]) + 1));
	if (philos == 0)
		return (0);
	i = -1;
	gettimeofday(&tv, 0);
	while (++i < ft_atol(argv[1]))
	{
		philos[i] = init_philo(argv, i, tv, locks);
		if (philos[i] == 0)
		{
			while (--i >= 0)
				free(philos[i]);
			return (free(philos), NULL);
		}
	}
	philos[i] = 0;
	return (philos);
}

int	main(int argc, char **argv)
{
	t_philo		**philos;
	t_process	*procs;
	t_locks		*locks;

	if (argc != 5 && argc != 6)
		return (printf("Error: incorrect number of arg\n"), 0);
	if (!check_args(argc, argv))
		return (0);
	locks = init_locks(ft_atol(argv[1]));
	philos = parse_args(argv, locks);
	if (philos == 0)
		return (free_locks(locks), 0);
	procs = init_process(ft_atol(argv[1]), philos);
	if (procs == 0)
		return (free_philos(philos), free_locks(locks), 0);
	free_philos(philos);
	free_locks(locks);
	free(procs -> pids);
	free(procs);
	return (0);
}
