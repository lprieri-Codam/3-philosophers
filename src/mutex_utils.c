/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mutex_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/12/09 19:27:53 by lprieri       #+#    #+#                 */
/*   Updated: 2024/12/09 19:32:51 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	ft_handle_lock(t_table *table, t_mutex *mutex, t_op operation);
void	set_errno(t_table *table, t_errno err_no);
void	set_stop_flag(t_table *table, t_philo *philo);

void	ft_handle_lock(t_table *table, t_mutex *mutex, t_op operation)
{
	int	failed;

	if (operation == LOCK)
	{
		failed = pthread_mutex_lock(&mutex->lock);
		if (failed && errno)
		{
			perror("Error locking a mutex");
			set_errno(table, MUTEX_LOCK_ERROR);
		}
		mutex->is_locked = true;
	}
	else if (operation == UNLOCK)
	{
		failed = pthread_mutex_unlock(&mutex->lock);
		if (failed && errno)
		{
			perror("Error unlocking a mutex");
			set_errno(table, MUTEX_UNLOCK_ERROR);
		}
		mutex->is_locked = false;
	}
}

void	set_errno(t_table *table, t_errno err_no)
{
	ft_handle_lock(table, &table->errno_mtx, LOCK);
	table->err_no = err_no;
	ft_handle_lock(table, &table->errno_mtx, UNLOCK);
}

void	set_stop_flag(t_table *table, t_philo *philo)
{
	ft_handle_lock(table, &table->sim_mtx, LOCK);
	table->stop_flag = true;
	ft_handle_lock(table, &table->sim_mtx, UNLOCK);
	print_status(philo, "died");
}
