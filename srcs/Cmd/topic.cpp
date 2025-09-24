#include "GetCmd.hpp"

void handle_topic(User* usr, Server& srv)
{
	std::vector<std::string> params = usr->msg.get_params();
	std::vector<std::string> channel;
	std::string trail = usr->msg.get_trailing();
	std::string info;

	if (params.empty())
		return(usr->reply(ERR_NEEDMOREPARAMS(usr->get_nickname(), usr->msg.get_command())));

	channel = split_channels(params[0]);

	Channel* chan = srv.get_channel(channel[0]);
	if (chan == NULL)
		return(usr->reply(ERR_NOSUCHCHANNEL(usr->get_nickname(), channel[0])));
	if (!chan->find_user(usr->get_nickname()))
		return(usr->reply(ERR_NOTONCHANNEL(usr->get_nickname(), channel[0])));
	if (trail.empty())
	{
		if (chan->get_topic().empty())
		{
			info = usr->chan_msg(usr, "TOPIC", RPL_NOTOPIC(usr->get_nickname(), chan->get_name()));
			usr->reply(info);
		}
		else
		{
			info = usr->chan_msg(usr, "TOPIC", RPL_TOPIC(usr->get_nickname(), chan->get_name(), chan->get_topic()));
			usr->reply(info);
		}
		return ;
	}
	if (!chan->find_ops(usr->get_nickname()))
		return(usr->reply(":" + usr->get_hostname() + " " + (ERR_CHANOPRIVSNEEDED(usr->get_nickname(), channel[0]))));
	if (trail != chan->get_topic())
	{
		chan->set_topic(trail);
		usr->broadcast(chan, ":" + usr->get_nickname() + "!" + usr->get_username() + "@" + usr->get_hostname() + " TOPIC " + chan->get_name() + " " + chan->get_topic() + "\n");
	}
}
