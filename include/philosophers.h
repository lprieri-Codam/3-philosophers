/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/12/09 19:28:21 by lprieri       #+#    #+#                 */
/*   Updated: 2024/12/09 19:37:00 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdatomic.h>
# include <errno.h>

# define RESET		"\033[0m"
# define BLACK		"\033[30m"
# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"
# define BLUE		"\033[34m"
# define MAGENTA	"\033[35m"
# define CYAN		"\033[36m"
# define WHITE		"\033[37m"

# define DAY_MSECS 86400000
# define ERROR_MSECS 20
# define INPUT_EXAMPLE \
"\033[32m<number of philosophers>\033[0m between 1 and 200.\n\
\033[32m<time_to_die>\033[0m in milliseconds, between 60 and 86,400,000.\n\
\033[32m<time_to_eat>\033[0m in MSECS, between 60 and 86,400,000.\n\
\033[32m<time_to_sleep>\033[0m in MSECS, between 60 and 86,400,000.\n\
\033[32m<number_of_meals (opt.)>\033[0m in MSECS, between 60 and 86,400,000."

typedef pthread_t		t_thread;
typedef struct s_table	t_table;

typedef enum e_errno
{
	ERROR = -1,
	SUCCESS = 0,
	FAILURE = 1,
	WRONG_INPUT,
	MALLOC_ERROR,
	INVALID_OPERATION,
	NULL_INPUT,
	NEGATIVE_INPUT,
	LARGE_INPUT,
	INVALID_INPUT,
	MUTEX_INIT_ERROR,
	THREAD_INIT_ERROR,
	SIM_START_ERROR,
	JOIN_ERROR,
	MUTEX_DESTROY_ERROR,
	MUTEX_LOCK_ERROR,
	MUTEX_UNLOCK_ERROR,
	DEAD_OR_FULL_PHILO,
	ERROR_COUNT
}	t_errno;

typedef enum e_unit
{
	MICROSECONDS,
	MILLISECONDS,
	SECONDS,
	FIRST_FORK,
	SECOND_FORK
}	t_unit;

typedef enum e_operation
{
	LOCK,
	UNLOCK
}	t_op;

typedef struct s_mutex
{
	pthread_mutex_t	lock;
	bool			is_locked;
}	t_mutex;

typedef struct s_fork
{
	t_mutex		*lock;
	int			id;
}	t_fork;

typedef struct s_philo
{
	int			philo_id;
	int			meals_eaten;
	long		time_of_last_meal;
	bool		is_dead;
	bool		is_full;
	bool		ended_sim;
	t_thread	ph_thread;
	t_fork		left_fork;
	t_fork		right_fork;
	t_mutex		philo_mtx;
	t_table		*table;
}	t_philo;

typedef struct s_table
{
	int			philo_count;
	int			starvation_time;
	int			eating_duration;
	int			sleeping_duration;
	int			nbr_of_meals;
	long		starting_time;
	bool		sim_started;
	bool		stop_flag;
	int			first_casualty;
	t_philo		*philo_dossiers;
	t_mutex		*forks_array;
	t_mutex		sim_mtx;
	t_mutex		print_mtx;
	t_mutex		errno_mtx;
	t_errno		err_no;
}	t_table;

/*	=========== FUNCTIONS ============	*/

//	---------- ft_error.c ----------

const char	*get_error_msg(t_errno err_no);
void		print_error_msg(t_errno err_no);
void		ft_exit(t_errno err_no, int exit_code);

//	---------- ft_free.c ----------

void		clean_up_and_exit(t_table *table, int exit_code);
void		destroy_mutex(t_table *table, t_mutex *mutex);
void		clean_memory(t_table *table);
void		ft_free(void **arg);

//	---------- init.c ----------

t_errno		initialize_structures(t_table *table);

//	---------- monitoring.c ----------

t_errno		monitor_threads(t_table *table);

//	---------- mutex_utils.c ----------

void		ft_handle_lock(t_table *table, t_mutex *mutex, t_op operation);
void		set_errno(t_table *table, t_errno err_no);
void		set_stop_flag(t_table *table, t_philo *philo);

//	---------- parsing.c ----------

t_errno		parse_input(t_table *table, char **input);

//	---------- sim_actions.c ----------

t_errno		pick_up_fork(t_philo *philo, t_unit fork);

//	---------- simulation.c ----------

t_errno		set_up_simulation(t_table *table);
t_errno		start_eating(t_philo *philo);
t_errno		go_to_sleep(t_philo *philo);
t_errno		start_thinking(t_philo *philo);

//	---------- utils.c ----------

void		print_status(t_philo *philo, char *status);
bool		is_alive(t_philo *philo);
bool		is_full(t_philo *philo);
bool		has_sim_stopped(t_table *table);
bool		has_someone_died(t_table *table);

//	---------- utils2.c ----------

t_errno		ft_atoi(t_errno *err_no, char *str, int	*nbr);
void		*ft_calloc(size_t nmemb, size_t size);
long		ft_gettime(t_unit time_unit);
void		ft_sleep(t_philo *philo, long duration, t_unit unit);
t_errno		join_threads(t_table *table);

#endif