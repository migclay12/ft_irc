#include "GetCmd.hpp"

std::vector<std::string>	split_channels(const std::string& input)
{
	std::vector<std::string>	channels;
	std::stringstream			ss(input);
	std::string					token;

	while (std::getline(ss, token, ','))
	{
		if (!token.empty())
			channels.push_back(token);
	}
	return channels;
}

void	print_user_info(User *usr, Server& srv)
{
	(void)usr, (void)srv;
	std::cout << "\n\nUSER INFO:\n";
	std::cout << "fd: " << usr->get_fd() << "\nport: " << usr->get_port() << "\nstate: " << usr->get_state_str() << "\n\n";
	std::cout << "nickname: " << usr->get_nickname() << "\nusername: " << usr->get_username() << "\nrealname: " << usr->get_realname() << "\nhostname: " << usr->get_hostname() << "\n";
}

void	handleHandshake(User* usr, Server& srv)
{
	const std::string&	cmd = usr->msg.get_command();

	if (cmd == "PASS")
		return handle_pass(usr, srv);
	std::cerr << "421 " << cmd << " :Invalid command, need to introduce PASS first\n";
}

void	handleLogin(User* usr, Server& srv)
{
	const std::string&	cmd = usr->msg.get_command();

	if (cmd == "NICK")
	{
		handle_nick(usr, srv);
	}
	else if (cmd == "USER")
	{
		if (usr->get_nickname().empty())
			return ;
		handle_user(usr, srv);

		usr->reply(RPL_WELCOME(usr->get_nickname()));
	}
}

void	printChannelInfo(Channel *chan)
{
	std::cout << "Name: " << chan->get_name() << "\n";
	std::cout << "Topic: " << chan->get_topic() << "\n";
	std::cout << "Invite only: " << chan->get_invite() << "\n";
	std::cout << "Limit: " << chan->get_limit() << "\n";
	std::cout << "Key: " << chan->get_key() << "\n";
	std::cout << "Topic ops: " << chan->get_topic_status() << "\n";
}

void	print_channel_users(User *usr, Server& srv)
{
	std::vector<std::string> params = usr->msg.get_params();
	if (params.empty())
	{
		std::cout << "PRINT channel\n";
		return ;
	}

	Channel *channel = srv.get_channel(params[0]);

	if (!channel)
	{
		std::cout << "PRINT no such channel\n";
		srv.print_channels();
		return ;
	}

	std::vector<std::string> nicknames = channel->get_nicknames();
	std::cout << "\n\nUSERS IN " << channel->get_name() << ":\n";
	for (size_t i = 0; i < nicknames.size(); i++)
		std::cout << i << " - " << nicknames[i] << "\n";

	std::vector<std::string> operators = channel->get_ops_nicks();
	std::cout << "\n\nOPERATORS IN " << channel->get_name() << ":\n";
	for (size_t i = 0; i < operators.size(); i++)
	{
		if (i == 0)
			std::cout << "ADMIN: " << operators[i] << "\n";
		else
			std::cout << i << " - " << operators[i] << "\n";
	}

	std::cout << "\n\nAll channel info:\n";
	printChannelInfo(channel);
	
	srv.print_channels();
}

void	handle_command(User* usr, Server& srv)
{
	const std::string&	cmd = usr->msg.get_command();
	const ClientState	usr_state = usr->get_state();

	if (usr_state == ERROR)	{
		return ;
	} else if (usr_state == HANDSHAKE) {
		handleHandshake(usr, srv);
	} else if (usr_state == LOGIN) {
		handleLogin(usr, srv);
	} else if (usr_state == REGISTERED) {

		static const CommandHandler handlers[] = 
		{
			{ "PASS",		handle_pass			},
			{ "NICK",		handle_nick			},
			{ "USER",		handle_user			},
			{ "PRIVMSG",	handle_privmsg		},
			{ "JOIN", 		handle_join 		},
			{ "PART", 		handle_part 		},
			{ "TOPIC",		handle_topic		},
			{ "MODE",		handle_mode			},
			{ "WHO",		handle_who			},
			{ "KICK",		handle_kick			},
			{ "INVITE",		handle_invite		},
			{ "QUIT",		handle_quit			},
			{ "INFO",		print_user_info		},
			{ "PRINT", 		print_channel_users 	}
		};

		const size_t numCommands = sizeof(handlers) / sizeof(handlers[0]);
		for (size_t i = 0; i < numCommands; ++i) 
		{
			if (cmd == handlers[i].name) 
			{
				handlers[i].func(usr, srv); 
				return;
			}
		}
		usr->reply(ERR_UNKNOWNCOMMAND(usr->get_nickname(), cmd));
	}
}
