#include "lib_philo.h"

#include "utils.h"

int	main(int ac, char **av)
{
	pthread_mutex_t print_mutex;
	long start;

	(void)ac;
	(void)av;

	pthread_mutex_init(&print_mutex, NULL);

	// Test error function (comment this if you want the rest to run)
	// ft_error("Something went wrong!");

	// Test time
	start = ft_get_timestamp();
	printf("Start timestamp: %ld ms\n", start);
	ft_usleep(500);
	printf("After 500ms sleep: %ld ms\n", ft_get_timestamp() - start);

	// Test print_action
	ft_print_action(1, "is eating", &print_mutex, start);
	ft_print_action(2, "is sleeping", &print_mutex, start);
	ft_print_action(3, "is thinking", &print_mutex, start);

	// Destroy mutex before exiting
	pthread_mutex_destroy(&print_mutex);
	return (0);
}
