#include "GetCmd.hpp"

void handle_kick(User* usr, Server& srv)
{
	std::vector<std::string> params = usr->msg.get_params();
	std::vector<std::string> channels;
	std::vector<std::string> users;
	std::string info;

	if (params.empty() || params.size() <= 1)
		return (usr->reply(" Usage: KICK <nick> [reason], kicks the nick from the current channel (needs chanop)"));

	channels = split_channels(params[0]);
	users = split_channels(params[1]);

	if (channels.empty() || users.empty())
		return (usr->reply(" Usage: KICK <nick> [reason], kicks the nick from the current channel (needs chanop)"));

	if (params.size() > 2)
	{
		for (size_t i = 1; i < params.size(); i++)
			info += params[i] + " ";
		if (!usr->msg.get_trailing().empty())
			info += usr->msg.get_trailing();
		usr->msg.set_trailing(info);
	}

	if (usr->msg.get_trailing().empty())
		usr->msg.set_trailing(usr->get_nickname());

	for (size_t i = 0; i < channels.size(); i++)
	{
		Channel* chan = srv.get_channel(channels[i]);
		if (chan == NULL)
		{
			usr->reply(ERR_NOSUCHCHANNEL(usr->get_nickname(), channels[i]));
			continue ;
		}
		if (!chan->find_user(usr->get_nickname()))
		{
			usr->reply(ERR_NOTONCHANNEL(usr->get_nickname(), channels[i]));
			continue ;
		}
		if (!chan->find_ops(usr->get_nickname()))
		{
			usr->reply(ERR_CHANOPRIVSNEEDED(usr->get_nickname(), channels[i]));
			continue ;
		}
		for (size_t i = 0; i < users.size(); i++)
		{
			User* reciver = srv.get_user(users[i]);
			if (reciver == NULL)
			{
				usr->reply(ERR_WASNOSUCHNICK(usr->get_nickname(), users[i]));
				continue ;
			}
			if (!chan->find_user(users[i]))
			{
				usr->reply(ERR_USERNOTINCHANNEL(usr->get_nickname(), users[i], chan->get_name()));
				continue ;
			}
			else
			{
				usr->broadcast(chan, ":" + usr->get_nickname() + "!" + usr->get_username() + "@" + usr->get_hostname() + " KICK " + chan->get_name() + " " + users[i] + " " + usr->msg.get_trailing() + "\n");
				chan->remove_user(chan->get_user_in_chan(users[i]));
			}
		}
		if (chan->get_size() == 0)
			srv.delete_channel(chan);
	}
}
