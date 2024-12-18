/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/12/09 19:28:01 by lprieri       #+#    #+#                 */
/*   Updated: 2024/12/09 19:28:02 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	print_status(t_philo *philo, char *status);
bool	is_alive(t_philo *philo);
bool	is_full(t_philo *philo);
bool	has_sim_stopped(t_table *table);
bool	has_someone_died(t_table *table);

void	print_status(t_philo *philo, char *status)
{
	if (!is_alive(philo) || is_full(philo))
		return ;
	ft_handle_lock(philo->table, &philo->table->print_mtx, LOCK);
	printf("%ld\t%d\t%s\n", \
		ft_gettime(MILLISECONDS) - philo->table->starting_time, \
		philo->philo_id, status);
	ft_handle_lock(philo->table, &philo->table->print_mtx, UNLOCK);
}

bool	is_alive(t_philo *philo)
{
	long	last_meal_time;

	ft_handle_lock(philo->table, &philo->philo_mtx, LOCK);
	if (philo->is_dead == true)
	{
		ft_handle_lock(philo->table, &philo->philo_mtx, UNLOCK);
		return (false);
	}
	last_meal_time = philo->time_of_last_meal;
	ft_handle_lock(philo->table, &philo->philo_mtx, UNLOCK);
	if (philo->table->starvation_time
		<= ft_gettime(MILLISECONDS) - last_meal_time)
	{
		ft_handle_lock(philo->table, &philo->philo_mtx, LOCK);
		philo->is_dead = true;
		ft_handle_lock(philo->table, &philo->philo_mtx, UNLOCK);
		ft_handle_lock(philo->table, &philo->table->sim_mtx, LOCK);
		philo->table->stop_flag = true;
		philo->table->first_casualty = philo->philo_id;
		ft_handle_lock(philo->table, &philo->table->sim_mtx, UNLOCK);
		return (false);
	}
	return (true);
}

bool	is_full(t_philo *philo)
{
	if (philo->table->nbr_of_meals == -1)
		return (false);
	ft_handle_lock(philo->table, &philo->philo_mtx, LOCK);
	if (philo->table->nbr_of_meals == philo->meals_eaten)
	{
		ft_handle_lock(philo->table, &philo->philo_mtx, UNLOCK);
		return (true);
	}
	ft_handle_lock(philo->table, &philo->philo_mtx, UNLOCK);
	return (false);
}

bool	has_sim_stopped(t_table *table)
{
	ft_handle_lock(table, &table->sim_mtx, LOCK);
	if (table->stop_flag)
	{
		ft_handle_lock(table, &table->sim_mtx, UNLOCK);
		return (true);
	}
	ft_handle_lock(table, &table->sim_mtx, UNLOCK);
	return (false);
}

bool	has_someone_died(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->philo_count)
	{
		if (!is_alive(&table->philo_dossiers[i]))
			return (true);
	}
	return (false);
}
