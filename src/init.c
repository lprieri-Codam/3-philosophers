/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/12/09 19:27:48 by lprieri       #+#    #+#                 */
/*   Updated: 2024/12/09 19:27:48 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

t_errno			initialize_structures(t_table *table);
static t_errno	allocate_memory(t_table *table);
static void		initialize_values(t_table *table);
static t_errno	initialize_mutexes(t_table *table);
static	t_errno	initialize_forks_and_philo_mtx(t_table *table, int i);

t_errno	initialize_structures(t_table *table)
{
	if (allocate_memory(table))
		return (table->err_no);
	initialize_values(table);
	if (initialize_mutexes(table))
	{
		clean_memory(table);
		return (table->err_no);
	}
	return (SUCCESS);
}

static t_errno	allocate_memory(t_table *table)
{
	table->philo_dossiers = ft_calloc(table->philo_count, sizeof(t_philo));
	if (!table->philo_dossiers)
		return (table->err_no = MALLOC_ERROR, table->err_no);
	table->forks_array = ft_calloc(table->philo_count, sizeof(t_mutex));
	if (!table->forks_array)
	{
		ft_free((void **) &table->philo_dossiers);
		return (table->err_no = MALLOC_ERROR, table->err_no);
	}
	return (SUCCESS);
}

static void	initialize_values(t_table *table)
{
	int	i;

	table->starting_time = -1;
	table->sim_started = false;
	table->stop_flag = false;
	table->first_casualty = -1;
	table->err_no = SUCCESS;
	i = -1;
	while (++i < table->philo_count)
	{
		table->philo_dossiers[i].philo_id = i + 1;
		table->philo_dossiers[i].meals_eaten = 0;
		table->philo_dossiers[i].time_of_last_meal = 0;
		table->philo_dossiers[i].is_dead = false;
		table->philo_dossiers[i].is_full = false;
		table->philo_dossiers[i].ended_sim = false;
		table->philo_dossiers[i].left_fork.lock = NULL;
		table->philo_dossiers[i].right_fork.lock = NULL;
		table->philo_dossiers[i].table = table;
	}
	return ;
}

static t_errno	initialize_mutexes(t_table *table)
{
	int	i;

	if (pthread_mutex_init(&table->sim_mtx.lock, NULL))
		return (table->err_no = MUTEX_INIT_ERROR, table->err_no);
	table->sim_mtx.is_locked = false;
	if (pthread_mutex_init(&table->print_mtx.lock, NULL))
	{
		destroy_mutex(table, &table->sim_mtx);
		return (table->err_no = MUTEX_INIT_ERROR, table->err_no);
	}
	table->print_mtx.is_locked = false;
	if (pthread_mutex_init(&table->errno_mtx.lock, NULL))
	{
		destroy_mutex(table, &table->sim_mtx);
		destroy_mutex(table, &table->print_mtx);
		return (table->err_no = MUTEX_INIT_ERROR, table->err_no);
	}
	table->errno_mtx.is_locked = false;
	i = -1;
	while (++i < table->philo_count)
	{
		if (initialize_forks_and_philo_mtx(table, i))
			clean_up_and_exit(table, EXIT_FAILURE);
	}
	return (SUCCESS);
}

static	t_errno	initialize_forks_and_philo_mtx(t_table *table, int i)
{
	if (pthread_mutex_init(&table->forks_array[i].lock, NULL))
		return (table->err_no = MUTEX_INIT_ERROR, table->err_no);
	table->forks_array[i].is_locked = false;
	if (i == 0 && table->philo_count - 1 == 0)
	{
		table->philo_dossiers->left_fork.lock = NULL;
		table->philo_dossiers->left_fork.id = -1;
	}
	else if (i == 0 && table->philo_count - 1 != 0)
	{
		table->philo_dossiers[i].left_fork.lock = \
			&table->forks_array[table->philo_count - 1];
		table->philo_dossiers[i].left_fork.id = table->philo_count - 1;
	}
	else
	{
		table->philo_dossiers[i].left_fork.lock = &table->forks_array[i - 1];
		table->philo_dossiers[i].left_fork.id = i - 1;
	}
	table->philo_dossiers[i].right_fork.lock = &table->forks_array[i];
	table->philo_dossiers[i].right_fork.id = i;
	if (pthread_mutex_init(&table->philo_dossiers[i].philo_mtx.lock, NULL))
		return (table->err_no = MUTEX_INIT_ERROR, table->err_no);
	table->philo_dossiers[i].philo_mtx.is_locked = false;
	return (SUCCESS);
}
