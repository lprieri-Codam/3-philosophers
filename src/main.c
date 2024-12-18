/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/12/09 19:27:50 by lprieri       #+#    #+#                 */
/*   Updated: 2024/12/09 19:32:14 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

t_errno	join_threads(t_table *table);

int	main(int argc, char *argv[])
{
	t_table	table;

	if (argc != 5 && argc != 6)
		ft_exit(WRONG_INPUT, EXIT_FAILURE);
	else
	{
		if (parse_input(&table, argv))
			ft_exit(table.err_no, EXIT_FAILURE);
		if (initialize_structures(&table))
			ft_exit(table.err_no, EXIT_FAILURE);
		if (set_up_simulation(&table))
			clean_up_and_exit(&table, EXIT_FAILURE);
		if (monitor_threads(&table))
			clean_up_and_exit(&table, EXIT_FAILURE);
		if (join_threads(&table))
			clean_up_and_exit(&table, EXIT_FAILURE);
		clean_up_and_exit(&table, EXIT_SUCCESS);
	}
}
