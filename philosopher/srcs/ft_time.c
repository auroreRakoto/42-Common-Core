#include "lib_philo.h"

long	ft_get_timestamp(void)
{
	struct timeval	tv;
	long			res;

	gettimeofday(&tv, NULL);
	res = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (res);
}

void	ft_usleep(long ms)
{
	long	start;

	start = ft_get_timestamp();
	while ((ft_get_timestamp() - start) < ms)
		usleep(100);
}
