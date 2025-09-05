#include "lib_philo.h"

void	ft_free_all(void *data)
{
	if (data)
	{
		free(data);
		data = NULL;
	}
}
