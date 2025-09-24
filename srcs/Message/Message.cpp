#include "Message.hpp"

Message::Message()
{
	_flag = 0;
}

Message::Message(const std::string &Message)
{
	_flag = 0;
	_fullMsg = Message;
}

Message::~Message()
{
}

Message::Message(const Message &cpy)
{
	*this = cpy;
}

Message	&Message::operator=(const Message &cpy)
{
	if (this != &cpy)
	{
		_flag = cpy._flag;
		_buffer = cpy._buffer;
		_fullMsg = cpy._fullMsg;
		_prefix = cpy._prefix;
		_command = cpy._command;
		_params = cpy._params;
		_trailing = cpy._trailing;
	}
	return(*this);
}

//Getters
const std::string	Message::get_prefix() const
{
	return(_prefix);
}

const std::string	Message::get_command() const
{  
	return(_command);
}

const std::vector<std::string>	Message::get_params() const
{
	return(_params);
}

const std::string	Message::get_trailing() const
{
	return(_trailing);
}

const std::string	Message::get_full_message() const
{
	return _fullMsg;
}

void	Message::set_trailing(const std::string &trailing)
{
	_trailing = trailing;
}

#include "GetCmd.hpp"

void	Message::clear()
{
	_flag = 0;
	_buffer.clear();
	_fullMsg.clear();
	_prefix.clear();
	_command.clear();
	_trailing.clear();
	_params.clear();
}

void	Message::parse_new_buffer(std::string buffer, User *usr, Server &srv)
{
	size_t nl_pos = buffer.find('\n');
	if (nl_pos == std::string::npos) 
	{
		this->_buffer += buffer;
		return ;
	}
	else
	{
		this->_buffer += buffer.substr(0, nl_pos + 1);
		buffer = buffer.substr(nl_pos + 1);
		this->_fullMsg = this->_buffer;
		this->parse_full_msg();
		this->parse_message();
		//this->print();
		handle_command(usr, srv);
		this->clear();
		if (usr->get_state() == ERROR)
			return ;
		this->parse_new_buffer(buffer, usr, srv);
	}
}

void	Message::parse_full_msg()
{
	if (_fullMsg.size() > 512)
	{
		_flag = 1;
		std::cerr << "417 * :Input line too long\n";
		_fullMsg.clear();
		return ;
	}
	if (_fullMsg[0] == ' ')
	{
		_flag = 1;
		std::cerr << "400 * :Invalid message format (start with invalid format)\n";
		_fullMsg.clear();
		return ;
	}
	std::string aux;
	size_t colonPos = _fullMsg.find(" :");
	if (colonPos != std::string::npos)
		aux = _fullMsg.substr(0, colonPos);
	else
		aux = _fullMsg;
	if (aux.find("  ") != std::string::npos || (colonPos == std::string::npos && aux[aux.size() - 1] == ' '))
	{
		_flag = 1;
		std::cerr << "400 * :Invalid message format (multiple spaces or trailing spaces before `:`)\n";
		_fullMsg.clear();
		return ;
	}
}

void	Message::parse_message()
{
	std::istringstream iss(_fullMsg); 
	std::string token;

	if (_flag == 1)
		return ;
	_prefix.erase();
	_command.erase();
	_params.erase(_params.begin(), _params.end());
	_trailing.erase();

	if (!_fullMsg.empty() && _fullMsg[0] == ':')
	{
		if (_fullMsg[1] == ':')
		{
			_flag = 1;
			std::cerr << "400 * :Unknown error: Invalid prefix format\n";
			return ;
		}
		iss >> _prefix;
		_prefix.erase(0, 1); 
	}
	if (!(iss >> _command))
	{
		_flag = 1;
		std::cerr << "421 <command> :Unknown command\n";
		return ;
	}
	while (iss >> token)
	{
		if (token[0] == ':') 
		{
			_trailing = token.substr(1); 
			std::getline(iss, token);
			_trailing += token;
			break;
		}
		else
		{
			if (_params.size() >= 15)
			{
				_flag = 1;
				std::cerr << "461 * : Too many parameters\n";
				return ;
			}
			_params.push_back(token);
		}
	}
}

void	Message::print()
{
	std::cerr << "--------MSG_DATA--------\n";
	if (_flag != 0)
		std::cerr << "_flag:     " << _flag	<< "\n";
	if (_fullMsg.length() > 0)
		std::cerr << "_fullMsg:  " << _fullMsg	<< std::flush;
	if (_prefix != "")
		std::cerr << "_prefix:   " << _prefix	<< "\n";
	if (_command != "")
		std::cerr << "_command:  " << _command	<< "\n";

	if (_params.size() > 0)
	{
		std::vector<std::string>::iterator it;
		std::cerr << "_params:\n";
		int i = 0;
		for (it = _params.begin(); it != _params.end(); ++it, ++i)
			std::cerr << "    " << i << ": " << *it << "\n";
	}
	if (_trailing != "")
		std::cerr << "_trailing: " << _trailing << "\n";
	std::cerr << "----------------------\n";
}
