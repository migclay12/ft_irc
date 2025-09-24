#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <netdb.h>

#include "Channel.hpp"
#include "Message.hpp"
#include "GetCmd.hpp"
#include "User.hpp"
#include "response.hpp"

class User;
class Channel;

extern bool close_program;

# define MAX_CLIENTS 1024

# ifndef HOSTNAME
#  define HOSTNAME "127.0.0.1"
# endif

class Server
{
	typedef std::map<int, User>::iterator				user_iterator;
	typedef std::vector<pollfd>::iterator				poll_iterator;
	typedef std::map<std::string, Channel>::iterator	channel_iterator;

	private:
		int									_port;
		std::string							_password;
		std::vector<pollfd>					_pollfds;
	
		std::map<int, User>					_users;
		std::map<std::string, Channel>		_channels;

		void accept_client(int server_socket);
		void check_client_revents(size_t index);

	public:
		Server(int port, std::string password);
		~Server();

		//Getters
		User*					get_user(const std::string &nickname);
		User*					get_user_fd(int fd);
		Channel*				get_channel(const std::string &name);
		std::string&			get_password(void);
		std::vector<Channel*>	get_all_channels();

		//Functions
		void	create_channel(std::string name, std::string key);
		void	delete_channel(Channel *chan);
		void	print_channels() const;
		void	init(void);
		void	close_client_connection(size_t index);
		void	leave_all_srv(User *usr);
};
