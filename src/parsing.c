/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/12/09 19:27:56 by lprieri       #+#    #+#                 */
/*   Updated: 2024/12/09 19:27:57 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

t_errno			parse_input(t_table *table, char **input);
static t_errno	assign_values(t_table *table, char **input);
static t_errno	check_input_validity(t_table *table);

t_errno	parse_input(t_table *table, char **input)
{
	if (assign_values(table, input))
		return (table->err_no);
	if (check_input_validity(table))
		return (table->err_no);
	return (SUCCESS);
}

static t_errno	assign_values(t_table *table, char **input)
{
	if (ft_atoi(&table->err_no, input[1], &table->philo_count))
		return (table->err_no);
	if (ft_atoi(&table->err_no, input[2], &table->starvation_time))
		return (table->err_no);
	if (ft_atoi(&table->err_no, input[3], &table->eating_duration))
		return (table->err_no);
	if (ft_atoi(&table->err_no, input[4], &table->sleeping_duration))
		return (table->err_no);
	if (input[5])
	{
		if (ft_atoi(&table->err_no, input[5], &table->nbr_of_meals))
			return (table->err_no);
	}
	else
		table->nbr_of_meals = -1;
	return (SUCCESS);
}

static t_errno	check_input_validity(t_table *table)
{
	if (table->philo_count == 0 || table->philo_count > 501)
		return (table->err_no = LARGE_INPUT, table->err_no);
	if (table->starvation_time == 0 || table->starvation_time > DAY_MSECS)
		return (table->err_no = LARGE_INPUT, table->err_no);
	if (table->eating_duration == 0 || table->eating_duration > DAY_MSECS)
		return (table->err_no = LARGE_INPUT, table->err_no);
	if (table->sleeping_duration == 0 || table->sleeping_duration > DAY_MSECS)
		return (table->err_no = LARGE_INPUT, table->err_no);
	return (SUCCESS);
}
