#include "GetCmd.hpp"

void handle_part(User *usr, Server& srv)
{
	std::vector<std::string> 	params = usr->msg.get_params();
	std::vector<std::string> 	chan;
	std::string trail;

	if (params.empty())
		return(usr->reply("Usage: PART [<channel>] [<reason>], leaves the channel, by default the current one"));

	chan = split_channels(params[0]);

	if (usr->msg.get_trailing().empty())
		usr->msg.set_trailing("Leaving");

	for (size_t i = 0; i < chan.size(); i++)
	{
		Channel *channel = srv.get_channel(chan[i]);
		if (!channel)
		{
			usr->reply(ERR_NOSUCHCHANNEL(usr->get_username(), chan[i]));
			continue ;
		}
		if (!channel->find_user(usr->get_nickname()))
		{
			usr->reply(ERR_NOTONCHANNEL(usr->get_nickname(), channel->get_name()));
			continue ;
		}
		else
		{
			usr->broadcast(channel, ":" + usr->get_nickname() + " PART " + channel->get_name() + " " + usr->msg.get_trailing() + "\n");
			channel->remove_user(usr);
		}
		if (channel->get_size() == 0)
			srv.delete_channel(channel);
	}
}
