#ifndef LIB_PHILO_H
# define LIB_PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef s_data
{
	int		philo_id;
	long	start_time;
}	t_data;

// ---------- Error ----------
void	ft_error(char *msg);

// ---------- Free ----------
void	ft_free_all(void *data);

// ---------- Print ----------
void	ft_print_action(int id, char *action, pthread_mutex_t *print_mutex, long start_time);

// ---------- Time ----------
long	ft_get_timestamp(void);
void	ft_usleep(long ms);

#endif