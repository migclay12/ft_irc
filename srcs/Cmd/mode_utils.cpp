#include "GetCmd.hpp"

void handle_i_on(User* usr, Channel* chan)
{
	if (!chan->get_invite())
		usr->broadcast(chan, ":" + usr->get_nickname() + " MODE " + chan->get_name() + " +i\n");
	chan->set_invite(true);
}

void handle_i_off(User* usr, Channel* chan)
{
	if (chan->get_invite())
		usr->broadcast(chan, ":" + usr->get_nickname() + " MODE " + chan->get_name() + " -i\n");
	chan->set_invite(false);
}

void handle_t_on(User* usr, Channel* chan)
{
	if (!chan->get_topic_status())
		usr->broadcast(chan, ":" + usr->get_nickname() + " MODE " + chan->get_name() + " +t\n");
	chan->set_topic_ops(true);
}

void handle_t_off(User* usr, Channel* chan)
{
	if (chan->get_topic_status())
		usr->broadcast(chan, ":" + usr->get_nickname() + " MODE " + chan->get_name() + " -t\n");
	chan->set_topic_ops(false);
}

void handle_k_on(User* usr, Channel* chan, std::string info)
{
	if (info.empty())
		return(usr->reply(":" + usr->get_hostname() + " 696 " + usr->get_nickname() + " " + chan->get_name() + " k * :You must specify a parameter for the op mode. Syntax: <key>.\n"));
	if (chan->get_key().empty())
	{
		chan->set_key(info);
		usr->broadcast(chan, ":" + usr->get_nickname() + " MODE " + chan->get_name() + " +k " + chan->get_key() + "\n");
	}
}

void handle_k_off(User* usr, Channel* chan, std::string info)
{
	if (info.empty())
		return(usr->reply(":" + usr->get_hostname() + " 696 " + usr->get_nickname() + " " + chan->get_name() + " k * :You must specify a parameter for the op mode. Syntax: <key>.\n"));
	if (info == chan->get_key())
	{
		usr->broadcast(chan, ":" + usr->get_nickname() + " MODE " + chan->get_name() + " -k\n");
		chan->set_key("");
	}
}

void handle_o_on(User* usr, Channel* chan, Server &srv, std::string info)
{
	if (info.empty())
		return(usr->reply(":" + usr->get_hostname() + " 696 " + usr->get_nickname() + " " + chan->get_name() + " o * :You must specify a parameter for the op mode. Syntax: <nick>.\n"));
	User* reciver = srv.get_user(info);
	if (reciver == NULL)
		return(usr->reply(ERR_WASNOSUCHNICK(usr->get_nickname(), info)));
	if (!chan->find_user(info))
		return(usr->reply(ERR_USERNOTINCHANNEL(usr->get_nickname(), info, chan->get_name())));
	if (!chan->find_ops(info))
	{
		chan->add_opperator(chan->get_user_in_chan(info));
		usr->broadcast(chan, ":" + usr->get_nickname() + " MODE " + chan->get_name() + " +o " + info + "\n");
	}
}

void handle_o_off(User* usr, Channel* chan, Server &srv, std::string info)
{
	if (info.empty())
		return(usr->reply(":" + usr->get_hostname() + " 696 " + usr->get_nickname() + " " + chan->get_name() + " o * :You must specify a parameter for the op mode. Syntax: <nick>.\n"));
	User* reciver = srv.get_user(info);
	if (reciver == NULL)
		return(usr->reply(ERR_WASNOSUCHNICK(usr->get_nickname(), info)));
	if (!chan->find_user(info))
		return(usr->reply(ERR_USERNOTINCHANNEL(usr->get_nickname(), info, chan->get_name())));
	if (chan->find_ops(info))
	{
		chan->remove_ops(chan->get_user_in_chan(info));
		usr->broadcast(chan, ":" + usr->get_nickname() + " MODE " + chan->get_name() + " -o " + info + "\n");
	}
}

void handle_l_on(User* usr, Channel* chan, std::string info)
{
	std::size_t i = 0;
	std::stringstream ss;

	if (info.empty())
		return(usr->reply(":" + usr->get_hostname() + " 696 " + usr->get_nickname() + " " + chan->get_name() + " l * :You must specify a parameter for the limit mode. Syntax: <limit>.\n"));
	if (info[i] == '-')
		return(usr->reply(":" + usr->get_hostname() + " 696 " + usr->get_nickname() + " " + chan->get_name() + " l " + info + " :Invalid limit mode parameter. Syntax: <limit>.\n"));
	if (info[i] == '+')
		i++;
	if (i == info.length() || !std::isdigit(info[i]))
		return(usr->reply(":" + usr->get_hostname() + " 696 " + usr->get_nickname() + " " + chan->get_name() + " l " + info + " :Invalid limit mode parameter. Syntax: <limit>.\n"));
	for (; i < info.length(); ++i)
	{
		if (!std::isdigit(info[i]))
			return(usr->reply(":" + usr->get_hostname() + " 696 " + usr->get_nickname() + " " + chan->get_name() + " l " + info + " :Invalid limit mode parameter. Syntax: <limit>.\n"));
	}

	i = std::atoi(info.c_str());
	if (i <= 0)
		return(usr->reply(":" + usr->get_hostname() + " 696 " + usr->get_nickname() + " " + chan->get_name() + " l " + info + " :Invalid limit mode parameter. Syntax: <limit>.\n"));
	
	ss << i;
	usr->broadcast(chan, ":" + usr->get_nickname() + " MODE " + chan->get_name() + " +l " + ss.str() + "\n");
	chan->set_limit(i);
}

void handle_l_off(User* usr, Channel* chan)
{
	if (chan->get_limit() != 0)
	{
		usr->broadcast(chan, ":" + usr->get_nickname() + " MODE " + chan->get_name() + " -l\n");
		chan->set_limit(0);
	}
}
