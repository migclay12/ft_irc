#pragma once

#include <iostream>
#include <map>
#include <string.h>
#include <sys/socket.h>
#include "Message.hpp"
#include "Channel.hpp"
#include "response.hpp"

class Channel;

enum ClientState
{
	HANDSHAKE,
	LOGIN,
	REGISTERED,
	DISCONNECTED,
	ERROR
};

class User {
	private:
		int			_fd;
		int			_port;

		std::string	_nickname;
		std::string	_username;
		std::string	_realname;
		std::string	_hostname;

		ClientState	_state;

	public:
		Message	msg;

		User();
		User(int fd, int port, const std::string &hostname);
		~User();
		User(const User &cpy);
		User &operator=(const User &cpy);

		// Getters
		int			get_fd() const;
		int			get_port() const;
		ClientState	get_state() const;
		std::string	get_state_str() const;
		std::string	get_nickname() const;
		std::string	get_username() const;
		std::string	get_hostname() const;
		std::string	get_realname() const;

		// Setters
		void set_state(ClientState new_state);
		void set_nickname(const std::string &nickname);
		void set_username(const std::string &username);
		void set_realname(const std::string &realname);

		//Functions
		bool		is_registered();
		void		clear_message();
		void		join_channel(Channel* channel);
		void		reply(const std::string& reply);
		void		broadcast(Channel *chan, const std::string& reply);
		std::string	chan_msg(User *usr, const std::string& command, const std::string& info);
};
