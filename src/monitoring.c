/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   monitoring.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/12/09 19:27:52 by lprieri       #+#    #+#                 */
/*   Updated: 2024/12/10 12:06:01 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

t_errno		monitor_threads(t_table *table);
static bool	are_they_full(t_table *table);
static void	handle_termination(t_table *table);
static void	kill_philosophers(t_table *table);
static void	wait_philos(t_table *table);

t_errno	monitor_threads(t_table *table)
{
	while (1)
	{
		ft_handle_lock(table, &table->errno_mtx, LOCK);
		if (table->err_no)
		{
			kill_philosophers(table);
			ft_handle_lock(table, &table->errno_mtx, UNLOCK);
			break ;
		}
		ft_handle_lock(table, &table->errno_mtx, UNLOCK);
		if (are_they_full(table))
			break ;
		if (has_sim_stopped(table) || has_someone_died(table))
		{
			handle_termination(table);
			break ;
		}
	}
	wait_philos(table);
	return (SUCCESS);
}

static bool	are_they_full(t_table *table)
{
	int	i;
	int	full_count;

	if (table->nbr_of_meals == -1)
		return (false);
	i = -1;
	full_count = 0;
	while (++i < table->philo_count)
	{
		if (is_full(&table->philo_dossiers[i]))
			full_count++;
	}
	if (full_count == table->philo_count)
		return (true);
	return (false);
}

static void	handle_termination(t_table *table)
{
	ft_handle_lock(table, &table->print_mtx, LOCK);
	kill_philosophers(table);
	printf("%ld\t%d\t%s\n", \
	ft_gettime(MILLISECONDS) - table->starting_time, \
		table->first_casualty, "died");
	ft_handle_lock(table, &table->print_mtx, UNLOCK);
}

static void	kill_philosophers(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->philo_count)
	{
		ft_handle_lock(table, &table->philo_dossiers[i].philo_mtx, LOCK);
		table->philo_dossiers[i].is_dead = true;
		ft_handle_lock(table, &table->philo_dossiers[i].philo_mtx, UNLOCK);
	}
	return ;
}

static void	wait_philos(t_table *table)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (++i < table->philo_count)
	{
		ft_handle_lock(table, &table->philo_dossiers[i].philo_mtx, LOCK);
		if (table->philo_dossiers[i].ended_sim)
			count++;
		ft_handle_lock(table, &table->philo_dossiers[i].philo_mtx, UNLOCK);
		if (count == table->philo_count)
			break ;
		if (i + 1 == table->philo_count)
		{
			i = -1;
			count = 0;
		}
	}
}
