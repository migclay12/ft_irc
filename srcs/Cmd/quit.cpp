#include "GetCmd.hpp"

void handle_quit(User *usr, Server& srv)
{
	std::vector<std::string> 	params = usr->msg.get_params();
	std::string trail;

	if (usr->msg.get_trailing().empty())
		usr->msg.set_trailing(":Quit: Leaving");
	else
		usr->msg.set_trailing(":Quit: " + usr->msg.get_trailing());

	std::vector<Channel*> channels = srv.get_all_channels();

	for (size_t i = 0; i < channels.size(); i++)
		usr->broadcast(channels[i], ":" + usr->get_nickname() + " QUIT " + usr->msg.get_trailing() + "\n");
}
