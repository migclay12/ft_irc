#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "response.hpp"


class Server;
class User;

class Message 
{
	private:
			int							_flag;
			std::string					_buffer;
			std::string					_fullMsg;
			std::string					_prefix;
			std::string					_command;
			std::vector<std::string>	_params;
			std::string					_trailing;
		
	public:
			Message();
			Message(const std::string &Message);
			~Message();
			Message(const Message &cpy);
			Message &operator=(const Message &cpy);

			//GETTERS
			const std::string				get_prefix() const;
			const std::string				get_command() const;
			const std::vector<std::string>	get_params() const;
			const std::string				get_trailing() const;
			const std::string				get_full_message() const;

			//Setters
			void set_trailing(const std::string &trailing);

			//Functions
			void	parse_new_buffer(std::string buffer, User *usr, Server &srv);
			void	parse_message();
			void	parse_full_msg();
			void	print();
			void	clear();
};
