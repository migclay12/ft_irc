#include "GetCmd.hpp"

std::vector<std::string> split_channels_join(const std::string& input, int i)
{
	std::vector<std::string> channels;
	std::stringstream ss(input);
	std::string token;

	while (std::getline(ss, token, ','))
	{
		if (!token.empty() && i == 0 && token[0] == '0' && input.length() == 1)
			channels.push_back(token);
		if (!token.empty() && i == 0)
		{
			if (token.length() != 1)
				channels.push_back(token);
		}
		if (!token.empty() && i == 1)
			channels.push_back(token);
	}
	return (channels);
}

void leaveAll(User *usr, Server& srv)
{
	std::vector<Channel*> result = srv.get_all_channels();

	for (size_t i = 0; i < result.size(); i++)
	{
		if (usr->msg.get_trailing().empty())
			usr->msg.set_trailing("Leaving");
		Channel* leave = srv.get_channel(result[i]->get_name());
		usr->broadcast(leave, ":" + usr->get_nickname() + " PART " + leave->get_name() + " " + usr->msg.get_trailing() + "\n");
		leave->remove_user(usr);
		if (leave->get_size() == 0)
			srv.delete_channel(leave);
	}
}

void handle_join(User *usr, Server& srv)
{
	std::vector<std::string> params = usr->msg.get_params();
	std::vector<std::string> chan;
	std::vector<std::string> keys;
	
	if (params.empty())
		return(usr->reply("Usage: JOIN <channel>, joins the channel"));

	if (params.size() == 1)
		chan = split_channels_join(params[0], 0);
	else
	{
		chan = split_channels_join(params[0], 0);
		keys = split_channels_join(params[1], 1);
	}

	if (chan.size() == 1 && chan[0] == "0" && params[0][0] != '#')
	{
		leaveAll(usr, srv);
		return ;
	}

	if (keys.empty())
	{
		for (size_t i = 0; i < chan.size(); i++)
			keys.push_back("");
	}
	if (keys.size() < chan.size())
	{
		for (size_t i = keys.size(); i < chan.size(); i++)
			keys.push_back("");
	}

	for (size_t i = 0; i < chan.size(); i++)
	{
		if (chan[i][0] != '#' && chan[i][0] != '&')
		{
			usr->reply(ERR_BADCHANMASK(usr->get_nickname(), chan[i]));
			continue ;
		}

		Channel *channel = srv.get_channel(chan[i]);
		if (!channel)
		{
			srv.create_channel(chan[i], keys[i]);
			channel = srv.get_channel(chan[i]);
		}

		if (channel->get_limit() > 0 && channel->get_size() >= channel->get_limit())
		{
			usr->reply(ERR_CHANNELISFULL(usr->get_nickname(), channel->get_name()));
			continue ;
		}
		if (channel->find_invite(usr->get_nickname()))
		{
			usr->join_channel(channel);
			channel->remove_invite(usr);
			continue ;
		}
		if (channel->get_invite())
		{
			usr->reply(ERR_INVITEONLYCHAN(usr->get_nickname(), channel->get_name()));
			continue ;
		}
		if (!channel->get_key().empty() && (channel->get_key() != keys[i]))
		{
			usr->reply(ERR_BADCHANNELKEY(usr->get_nickname(), channel->get_name()));
			continue ;
		}
		if (channel->find_user(usr->get_nickname()))
		{
			usr->reply(ERR_USERONCHANNEL(usr->get_nickname(), channel->get_name()));
			continue ;
		}
		if (channel->get_size() == 0)
			channel->add_opperator(usr);
		usr->join_channel(channel);
	}
}
