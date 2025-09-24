#include "GetCmd.hpp"

void handle_privmsg(User* usr, Server& srv) 
{
	std::vector<std::string> params = usr->msg.get_params();
	std::vector<std::string> targets;
	std::vector<std::string> users;
	std::string info;

	if (params.empty())
	{
		usr->reply(":" + usr->get_hostname() + " " + ERR_NEEDMOREPARAMS(usr->get_nickname(), "PRIVMSG")); 
		usr->reply(":" + usr->get_hostname() + " 650 " + usr->get_nickname() + " PRIVMSG :<target>[,<target>]+ :<message>");
		return ;
	}
	
	targets = split_channels(params[0]);

	if (params.size() > 1)
	{
		for (size_t i = 1; i < params.size(); i++)
			info += params[i] + " ";
		if (!usr->msg.get_trailing().empty())
			info += usr->msg.get_trailing();
		usr->msg.set_trailing(info);
	}

	if (usr->msg.get_trailing().empty() || targets.empty())
		return(usr->reply(ERR_NEEDMOREPARAMS(usr->get_nickname(), usr->msg.get_command())));

	for (size_t i = 0; i < targets.size(); i++)
	{
		if (targets[i][0] == '#' || targets[i][0] == '&')
		{
			Channel* chan = srv.get_channel(targets[i]);
			if (chan == NULL)
			{
				usr->reply(ERR_NOSUCHCHANNEL(usr->get_nickname(), targets[i]));
				continue ;
			}
			if (!chan->find_user(usr->get_nickname()))
			{
				usr->reply(ERR_NOTONCHANNEL(usr->get_nickname(), targets[i]));
				continue ;
			}
			info = usr->chan_msg(usr, "PRIVMSG", (chan->get_name() + " " + ":" + usr->msg.get_trailing() + "\n"));
			users = chan->get_nicknames();
			const char *msg = info.c_str();
			for (size_t i = 0; i < users.size(); i++)
			{
				if (users[i] != usr->get_nickname())
				{
					User* nick = srv.get_user(users[i]);
					send(nick->get_fd(), msg, strlen(msg), 0);
				}
			}
		}
		else
		{
			User* reciver = srv.get_user(targets[i]);
			if (reciver == NULL)
			{
				usr->reply(ERR_WASNOSUCHNICK(usr->get_nickname(), targets[i]));
				continue ;
			}
			info = usr->chan_msg(usr, "PRIVMSG", (targets[i] + " " + ":" + usr->msg.get_trailing() + "\n"));
			const char *msg = info.c_str();
			send(reciver->get_fd(), msg, strlen(msg), 0);
		}
	}
}
