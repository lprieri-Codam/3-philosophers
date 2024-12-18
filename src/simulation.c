/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   simulation.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/12/09 19:27:59 by lprieri       #+#    #+#                 */
/*   Updated: 2024/12/10 12:04:57 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

t_errno		set_up_simulation(t_table *table);
void		*philo_routine(void *arg);
t_errno		check_starting_condition(t_table *table);
static void	end_simulation(t_philo *philo);

t_errno	set_up_simulation(t_table *table)
{
	int	i;

	table->starting_time = ft_gettime(MILLISECONDS);
	ft_handle_lock(table, &table->sim_mtx, LOCK);
	if (table->err_no)
		ft_exit(table->err_no, EXIT_FAILURE);
	i = -1;
	while (++i < table->philo_count)
	{
		table->philo_dossiers[i].time_of_last_meal = table->starting_time;
		if (pthread_create(&table->philo_dossiers[i].ph_thread, \
			NULL, &philo_routine, &table->philo_dossiers[i]))
			return (table->err_no = THREAD_INIT_ERROR, table->err_no);
	}
	table->sim_started = true;
	ft_handle_lock(table, &table->sim_mtx, UNLOCK);
	return (SUCCESS);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	if (check_starting_condition(philo->table))
		return (NULL);
	print_status(philo, "is thinking");
	if (philo->philo_id % 2 == 0)
		ft_sleep(philo, ERROR_MSECS, MILLISECONDS);
	while (is_alive(philo) && !is_full(philo))
	{
		if (pick_up_fork(philo, FIRST_FORK))
			break ;
		if (pick_up_fork(philo, SECOND_FORK))
			break ;
		if (start_eating(philo))
			break ;
		if (go_to_sleep(philo))
			break ;
		if (start_thinking(philo))
			break ;
	}
	end_simulation(philo);
	return (NULL);
}

t_errno	check_starting_condition(t_table *table)
{
	ft_handle_lock(table, &table->sim_mtx, LOCK);
	if (!table->sim_started)
	{
		ft_handle_lock(table, &table->sim_mtx, UNLOCK);
		set_errno(table, SIM_START_ERROR);
		return (table->err_no);
	}
	ft_handle_lock(table, &table->sim_mtx, UNLOCK);
	return (SUCCESS);
}

static void	end_simulation(t_philo *philo)
{
	if (philo->left_fork.lock && philo->left_fork.lock->is_locked)
		ft_handle_lock(philo->table, philo->left_fork.lock, UNLOCK);
	if (philo->right_fork.lock && philo->right_fork.lock->is_locked)
		ft_handle_lock(philo->table, philo->right_fork.lock, UNLOCK);
	while (!is_full(philo) && is_alive(philo))
		;
	ft_handle_lock(philo->table, &philo->philo_mtx, LOCK);
	philo->ended_sim = true;
	ft_handle_lock(philo->table, &philo->philo_mtx, UNLOCK);
	return ;
}
