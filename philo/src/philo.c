/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vluo <vluo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:43:00 by vluo              #+#    #+#             */
/*   Updated: 2025/03/12 14:24:32 by vluo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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

static t_philo	**parse_args(char **argv)
{
	t_philo			**philos;
	pthread_mutex_t	**mutexs;
	int				i;
	struct timeval	tv;

	mutexs = init_mutexs(ft_atol(argv[1]));
	if (mutexs == 0)
		return (0);
	philos = malloc(sizeof(t_philo *) * (ft_atol(argv[1]) + 1));
	if (philos == 0)
		return (free_mutexs(mutexs), NULL);
	i = -1;
	gettimeofday(&tv, 0);
	while (++i < ft_atol(argv[1]))
	{
		philos[i] = init_philo(argv, mutexs, i, tv);
		if (philos[i] == 0)
		{
			while (--i >= 0)
				free(philos[i]);
			return (free(philos), free_mutexs(mutexs), NULL);
		}
	}
	philos[i] = 0;
	return (philos);
}

int	main(int argc, char **argv)
{
	t_philo			**philos;
	pthread_t		**threads;
	pthread_t		main;

	if (argc != 5 && argc != 6)
		return (printf("Error: incorrect number of arg\n"), 0);
	philos = 0;
	if (check_args(argc, argv))
		philos = parse_args(argv);
	if (philos == 0)
		return (0);
	if (philos[0]->nb_philos == 1)
		threads = lone_philo(philos);
	else
	{
		if (pthread_create(&main, NULL, &obs, philos) != 0)
			return (free_philos(philos), 0);
		threads = create_threads(philos, ft_atol(argv[1]), &main);
	}
	if (threads == 0)
		return (free_philos(philos), 0);
	free_threads(threads);
	free_philos(philos);
	return (0);
}
