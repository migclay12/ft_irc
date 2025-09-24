#include "GetCmd.hpp"

void handle_who(User* usr, Server& srv)
{
	std::vector<std::string> params = usr->msg.get_params();

	if (params.empty())
		return(usr->reply(ERR_NEEDMOREPARAMS(usr->get_nickname(), "WHO")));

	std::vector<std::string> channel;

	channel = split_channels(params[0]);

	if (channel.empty())
		return(usr->reply(ERR_NEEDMOREPARAMS(usr->get_nickname(), usr->msg.get_command())));

	Channel* chan = srv.get_channel(channel[0]);
	if (chan == NULL)
		return(usr->reply(ERR_NOSUCHCHANNEL(usr->get_nickname(), channel[0])));
	else if (!chan->find_user(usr->get_nickname()))
		return(usr->reply(ERR_NOTONCHANNEL(usr->get_nickname(), channel[0])));

	std::string name;
	std::vector<std::string> nick = chan->get_nicknames();

	for (size_t i = 0; i < nick.size(); i++)
	{
		if (chan->find_ops(nick[i]))
			name += '@';
		name += nick[i] + ' ';
	}
	if (!chan->get_topic().empty())
		usr->reply(":" + usr->get_hostname() + " " + RPL_TOPIC(usr->get_nickname(), chan->get_name(), chan->get_topic()));
	usr->reply(":" + usr->get_hostname() + " " + RPL_NAMEREPLY(usr->get_nickname(), chan->get_name(), name));
}
