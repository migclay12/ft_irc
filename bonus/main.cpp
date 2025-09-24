#include "jokes.hpp"
#include "bot.hpp"

int main(int argc, char **argv)
{
	Jokes::initialize();

	Bot bot(argc, argv);

	if (bot.init() == EXIT_FAILURE)
		exit(EXIT_FAILURE);
	bot.run();
	return (EXIT_SUCCESS);
}
