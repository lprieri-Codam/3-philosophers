/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils2.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/12/09 19:28:02 by lprieri       #+#    #+#                 */
/*   Updated: 2024/12/09 19:38:16 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

t_errno	ft_atoi(t_errno *err_no, char *str, int	*nbr);
void	*ft_calloc(size_t nmemb, size_t size);
long	ft_gettime(t_unit time_unit);
void	ft_sleep(t_philo *philo, long duration, t_unit unit);
t_errno	join_threads(t_table *table);

t_errno	ft_atoi(t_errno *err_no, char *str, int	*nbr)
{
	int		negative_count;
	long	n;

	if (!str || !str[0])
		return (*err_no = NULL_INPUT, *err_no);
	while (*str && (*str == '\t' || *str == ' '))
		str++;
	negative_count = 0;
	while (*str && (*str == '+' || *str == '-'))
	{
		if (*str == '-')
			negative_count++;
		str++;
	}
	if (negative_count % 2 != 0)
		return (*err_no = NEGATIVE_INPUT, *err_no);
	n = 0;
	while (*str && (*str >= '0' && *str <= '9') && !(n > __INT_MAX__))
	{
		n = (n) * 10 + *str - '0';
		str++;
	}
	if (*str != '\0')
		return (*err_no = INVALID_INPUT, *err_no);
	return (*nbr = (int) n, SUCCESS);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	i;

	if (nmemb == 0 || size == 0)
		return (NULL);
	if ((nmemb * size) / nmemb != size)
		return (NULL);
	ptr = malloc(nmemb * size);
	if (!ptr)
	{
		perror("ft_calloc");
		return (NULL);
	}
	i = 0;
	while (i < (nmemb * size))
	{
		((char *)ptr)[i] = '\0';
		i++;
	}
	return (ptr);
}

long	ft_gettime(t_unit time_unit)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL))
	{
		if (errno)
			perror("ft_gettime");
		return (-1);
	}
	if (time_unit == MICROSECONDS)
		return (time.tv_sec * 1000000 + time.tv_usec);
	else if (time_unit == MILLISECONDS)
		return (time.tv_sec * 1000 + time.tv_usec / 1000);
	else if (time_unit == SECONDS)
		return (time.tv_sec + time.tv_usec / 1000000);
	return (-1);
}

void	ft_sleep(t_philo *philo, long duration, t_unit unit)
{
	long	current_time;

	current_time = ft_gettime(unit);
	while (is_alive(philo) && ft_gettime(unit) - current_time < duration)
		usleep(500);
}

t_errno	join_threads(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->philo_count)
	{
		if (pthread_join(table->philo_dossiers[i].ph_thread, NULL))
			return (set_errno(table, JOIN_ERROR), JOIN_ERROR);
	}
	return (SUCCESS);
}
