#include "Server.hpp"
#include <stdlib.h>
#include <signal.h>
#include <string.h>

bool	close_program = false;

int	str_is_digit(char *str)
{
	int	i = 0;

	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	parser(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (str_is_digit(argv[1]) == 0)
	{
		std::cerr << "Invalid port: " << argv[1] << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	signal_handler(int signum)
{
	std::cout << "Signal received: " << signum << " " << strsignal(signum) << "\n";
	if (signum == SIGINT)
	{
		std::cout << "Closing server...\n";
		close_program = true;
	}
}

int	main(int argc, char **argv)
{
	signal(SIGINT, signal_handler);
	parser(argc, argv);

	Server irc_server(atoi(argv[1]), argv[2]);
	irc_server.init();
}
