#include "lib_philo.h"

void	ft_print_action(int id, char *action, pthread_mutex_t *print_mutex, long start_time)
{
	long timestamp;

	pthread_mutex_lock(print_mutex);
	timestamp = ft_get_timestamp() - start_time;
	printf("%ld %d %s\n", timestamp, id, action);
	pthread_mutex_unlock(print_mutex);
}
