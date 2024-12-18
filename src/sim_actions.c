/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sim_actions.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/12/09 19:27:58 by lprieri       #+#    #+#                 */
/*   Updated: 2024/12/09 19:27:59 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

t_errno	pick_up_fork(t_philo *philo, t_unit fork);
t_errno	start_eating(t_philo *philo);
t_errno	go_to_sleep(t_philo *philo);
t_errno	start_thinking(t_philo *philo);

t_errno	pick_up_fork(t_philo *philo, t_unit fork)
{
	if (!is_alive(philo) || is_full(philo))
		return (DEAD_OR_FULL_PHILO);
	if ((fork == FIRST_FORK && philo->philo_id % 2 == 0)
		|| (fork == SECOND_FORK && philo->philo_id % 2 != 0))
	{
		if (philo->left_fork.id == -1)
			return (FAILURE);
		ft_handle_lock(philo->table, philo->left_fork.lock, LOCK);
	}
	else if ((fork == SECOND_FORK && philo->philo_id % 2 == 0)
		|| (fork == FIRST_FORK && philo->philo_id % 2 != 0))
	{
		if (philo->right_fork.id == -1)
			return (FAILURE);
		ft_handle_lock(philo->table, philo->right_fork.lock, LOCK);
	}
	print_status(philo, "has taken a fork");
	return (SUCCESS);
}

t_errno	start_eating(t_philo *philo)
{
	if (!is_alive(philo) || is_full(philo))
		return (DEAD_OR_FULL_PHILO);
	print_status(philo, "is eating");
	ft_handle_lock(philo->table, &philo->philo_mtx, LOCK);
	philo->time_of_last_meal = ft_gettime(MILLISECONDS);
	philo->meals_eaten++;
	ft_handle_lock(philo->table, &philo->philo_mtx, UNLOCK);
	ft_sleep(philo, philo->table->eating_duration, MILLISECONDS);
	ft_handle_lock(philo->table, philo->left_fork.lock, UNLOCK);
	ft_handle_lock(philo->table, philo->right_fork.lock, UNLOCK);
	return (SUCCESS);
}

t_errno	go_to_sleep(t_philo *philo)
{
	if (!is_alive(philo) || is_full(philo))
		return (DEAD_OR_FULL_PHILO);
	print_status(philo, "is sleeping");
	ft_sleep(philo, philo->table->sleeping_duration, MILLISECONDS);
	return (SUCCESS);
}

t_errno	start_thinking(t_philo *philo)
{
	int	thinking_duration;

	if (!is_alive(philo) || is_full(philo))
		return (DEAD_OR_FULL_PHILO);
	print_status(philo, "is thinking");
	if (philo->table->philo_count % 2 != 0
		&& philo->table->eating_duration >= philo->table->sleeping_duration)
	{
		thinking_duration = philo->table->eating_duration \
			- philo->table->sleeping_duration + ERROR_MSECS;
		ft_sleep(philo, thinking_duration, MILLISECONDS);
	}
	return (SUCCESS);
}
