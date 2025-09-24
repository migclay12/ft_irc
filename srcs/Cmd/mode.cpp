#include "GetCmd.hpp"

void work_mode(User* usr, Channel* chan, Server& srv)
{
	std::vector<std::string> params = usr->msg.get_params();
	std::string mode = params[1];
	size_t count = 2;

	for (size_t i = 0; i < mode.size(); i++)
	{
		if (params.size() <= count)
			params.push_back("");
		if (mode[i] == '+')
			i++;
		if (mode[i] == 'i')
			handle_i_on(usr, chan);
		else if (mode[i] == 't')
			handle_t_on(usr, chan);
		else if (mode[i] == 'k')
			handle_k_on(usr, chan, params[count++]);
		else if (mode[i] == 'o')
			handle_o_on(usr, chan, srv, params[count++]);
		else if (mode[i] == 'l')
			handle_l_on(usr, chan, params[count++]);
		else if (mode[i] == '-')
		{
			for (; i < mode.size(); i++)
			{
				if (params.size() <= count)
					params.push_back("");
				if (mode[i] == '-')
					i++;
				if (mode[i] == 'i')
					handle_i_off(usr, chan);
				else if (mode[i] == 't')
					handle_t_off(usr, chan);
				else if (mode[i] == 'k')
					handle_k_off(usr, chan, params[count++]);
				else if (mode[i] == 'o')
					handle_o_off(usr, chan, srv, params[count++]);
				else if (mode[i] == 'l')
					handle_l_off(usr, chan);
				else if (mode[i] == '+')
					break ;
				else
					usr->reply(ERR_UNKNOWNMODE(usr->get_nickname(), mode[i], chan->get_name()));
			}
		}
		else
			usr->reply(ERR_UNKNOWNMODE(usr->get_nickname(), mode[i], chan->get_name()));
	}
}

void print_modes(User *usr, Channel *chan)
{
	std::string modes = "+";
	std::string params;
	std::stringstream ss;

	if (chan->get_invite())
		modes += "i";
	if (chan->get_topic_status())
		modes += "t";
	if (chan->get_limit() != 0)
	{
		modes += "l";
		ss << chan->get_limit();
		params = " " + ss.str();
	}
	if (!chan->get_key().empty())
	{
		modes += "k";
		params += " :" + chan->get_key();
	}
	if (!params.empty())
		usr->reply(":" + usr->get_hostname() + " 324 " + usr->get_nickname() + " " + chan->get_name() + " :" + modes + params + "\n");
	else
		usr->reply(":" + usr->get_hostname() + " 324 " + usr->get_nickname() + " " + chan->get_name() + " :" + modes + "\n");
}

void handle_mode(User *usr, Server& srv)
{
	std::vector<std::string> params = usr->msg.get_params();
	int flag = 0;

	if (params.empty())
	{
		usr->reply(ERR_NEEDMOREPARAMS(usr->get_nickname(), "MODE"));
		usr->reply("650 " + usr->get_nickname() + " MODE " + ":<target> [[(+|-)]<modes> [<mode-parameters>]]");
		return ;
	}
	if (params.size() < 2)
		flag = 1;

	std::vector<std::string> channel;

	channel = split_channels(params[0]);

	if (channel.empty())
		return (usr->reply(ERR_NEEDMOREPARAMS(usr->get_nickname(), usr->msg.get_command())));

	Channel* chan = srv.get_channel(channel[0]);
	if (chan == NULL)
		return(usr->reply(ERR_NOSUCHCHANNEL(usr->get_nickname(), channel[0])));
	else if (!chan->find_user(usr->get_nickname()))
		return(usr->reply(ERR_NOTONCHANNEL(usr->get_nickname(), channel[0])));
	else if (flag == 1)
		print_modes(usr, chan);
	else if (!chan->find_ops(usr->get_nickname()))
		return(usr->reply(":" + usr->get_hostname() + " " + (ERR_CHANOPRIVSNEEDED(usr->get_nickname(), channel[0]))));
	else
		work_mode(usr, chan, srv);
}
