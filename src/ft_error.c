/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_error.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/12/09 19:27:38 by lprieri       #+#    #+#                 */
/*   Updated: 2024/12/10 12:05:17 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

const char	*get_error_msg(t_errno err_no);
void		print_error_msg(t_errno err_no);
void		ft_exit(t_errno err_no, int exit_code);

const char	*get_error_msg(t_errno err_no)
{
	char	*error_msg[ERROR_COUNT];

	error_msg[SUCCESS] = "Success!";
	error_msg[FAILURE] = "There was a failure!";
	error_msg[MALLOC_ERROR] = "There was a problem allocating memory.";
	error_msg[INVALID_OPERATION] = \
	"An invalid operation was attempted on a mutex.";
	error_msg[NULL_INPUT] = "The input was null.";
	error_msg[NEGATIVE_INPUT] = "The input given was negative.";
	error_msg[LARGE_INPUT] = \
	"The input given was too big. Try smaller numbers.";
	error_msg[INVALID_INPUT] = "The input given was invalid.";
	error_msg[MUTEX_INIT_ERROR] = "There was a problem initializing a mutex.";
	error_msg[THREAD_INIT_ERROR] = "There was a problem creating a thread.";
	error_msg[SIM_START_ERROR] = "The simulation failed to start.";
	error_msg[JOIN_ERROR] = "There was an error joining a thread";
	error_msg[MUTEX_DESTROY_ERROR] = "There was a problem destroying a mutex.";
	error_msg[MUTEX_LOCK_ERROR] = "There was an error locking a mutex.";
	error_msg[MUTEX_UNLOCK_ERROR] = "There was an error unlocking a mutex.";
	if (err_no < ERROR_COUNT && err_no >= 0)
		return (error_msg[err_no]);
	return (NULL);
}

void	print_error_msg(t_errno err_no)
{
	const char	*str;

	str = get_error_msg(err_no);
	if (!str)
		return ;
	printf("Error\n%s\n", str);
	return ;
}

void	ft_exit(t_errno err_no, int exit_code)
{
	if (err_no == WRONG_INPUT)
	{
		printf("%sError!%s\n", RED, RESET);
		printf("These are the parameters you should enter:\n%s\n", \
		INPUT_EXAMPLE);
	}
	else if (err_no)
		print_error_msg(err_no);
	exit(exit_code);
}
