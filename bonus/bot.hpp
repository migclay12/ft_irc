#pragma once

# include <iostream>
# include <cstring>
# include <cerrno>
# include <vector>

# include <arpa/inet.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# define MAX_CLIENTS 5
# define BUFFER_SIZE 2048

# define BOT_NICK "ircbot"

# include "jokes.hpp"
# include "time.hpp"

struct IRCMessage {
	std::string prefix;
	std::string nick;
	std::string user;
	std::string host;
	std::string command;
	std::vector<std::string> params;
	std::string trailing;
};

class Bot {
	private:
		int			_server_socket;

		std::string	_hostname;
		std::string	_ip_address;
		int			_port;

		std::string	_password;
		std::string	_channel;

		bool		_error;

		void		selectBotMode(std::string prefix, IRCMessage *msg_struct);
	public:
		Bot(int argc, char **argv);
		~Bot();

		int		init(void);
		int		run(void);
		void	handleMessage(const std::string& raw);
		void	sendMsg(const std::string& message);
};

std::string	resolve_hostname(const char *hostname);
int			parse_port(const char *port);
