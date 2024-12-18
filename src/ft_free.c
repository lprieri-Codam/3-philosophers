/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_free.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/12/09 19:27:45 by lprieri       #+#    #+#                 */
/*   Updated: 2024/12/09 19:27:46 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void		clean_up_and_exit(t_table *table, int exit_code);
static void	destroy_mutexes(t_table	*table);
void		destroy_mutex(t_table *table, t_mutex *mutex);
void		clean_memory(t_table *table);
void		ft_free(void **arg);

void	clean_up_and_exit(t_table *table, int exit_code)
{
	destroy_mutexes(table);
	clean_memory(table);
	ft_exit(table->err_no, exit_code);
}

void	destroy_mutexes(t_table	*table)
{
	int	i;

	destroy_mutex(table, &table->sim_mtx);
	destroy_mutex(table, &table->print_mtx);
	destroy_mutex(table, &table->errno_mtx);
	i = -1;
	while (++i < table->philo_count)
	{
		if (&table->forks_array[i] != NULL)
			destroy_mutex(table, &table->forks_array[i]);
	}
}

void	destroy_mutex(t_table *table, t_mutex *mutex)
{
	if (mutex->is_locked)
		ft_handle_lock(table, mutex, UNLOCK);
	if (pthread_mutex_destroy(&mutex->lock) && errno)
		perror("Error destroying a mutex");
}

void	clean_memory(t_table *table)
{
	ft_free((void **) &table->forks_array);
	ft_free((void **) &table->philo_dossiers);
}

void	ft_free(void **arg)
{
	if (!arg || !*arg)
		return ;
	free(*arg);
	*arg = NULL;
}
