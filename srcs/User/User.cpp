#include "User.hpp"

User::User(int fd, int port, const std::string &hostname)
	: _fd(fd), _port(port), _hostname(hostname), _state(HANDSHAKE) {}

User::~User() {}

User::User() {}

User::User(const User &cpy)
{
	*this = cpy;
}

User &User::operator=(const User &cpy)
{
	if (this != &cpy)
	{
		_fd			= cpy._fd;
		_port		= cpy._port;

		_nickname	= cpy._nickname;
		_username	= cpy._username;
		_realname	= cpy._realname;
		_hostname	= cpy._hostname;

		_state		= cpy._state;

		msg	= cpy.msg;
	}
	return (*this);
}

// Getters
int	User::get_fd() const
{
	return (_fd);
}

int	User::get_port() const
{
	return (_port);
}

ClientState	User::get_state() const
{
	return (this->_state);
}

std::string	User::get_state_str() const
{
	if (_state == HANDSHAKE)
		return ("HANDSHAKE");
	if (_state == LOGIN)
		return ("LOGIN");
	if (_state == REGISTERED)
		return ("REGISTERED");
	if (_state == DISCONNECTED)
		return ("DISCONNECTED");
	return (NULL);
}

std::string	User::get_nickname() const
{
	return (_nickname);
}

std::string	User::get_username() const
{
	return (_username);
}

std::string	User::get_realname() const
{
	return (_realname);
}

std::string	User::get_hostname() const
{
	return (_hostname);
}

// Setters
void	User::set_state(ClientState new_state)
{
	this->_state = new_state;
}

void	User::set_nickname(const std::string &nickname)
{
	_nickname = nickname;
}

void	User::set_username(const std::string &username)
{
	_username = username;
}

void	User::set_realname(const std::string &realname)
{
	_realname = realname;
}

//Functions
bool	User::is_registered()
{
	return (_state == REGISTERED);
}

void	User::clear_message()
{
	msg.clear();
}

void	User::join_channel(Channel* channel)
{
	channel->add_user(this);
	this->broadcast(channel, ":" + this->get_nickname() + " JOIN " + channel->get_name() + " * " + this->get_realname() + "\n");
}

std::string	User::chan_msg(User *usr, const std::string& command, const std::string& info)
{
	if (command.empty() || command == "TOPIC" || command == "JOIN")
		return(":" + usr->get_nickname() + "!" + usr->get_username() + "@" + usr->get_hostname() + " " + info);
	if (command == "PRIVMSG" || command == "PART" || command == "NICK" || command == "MODE")
		return(":" + usr->get_nickname() + "!" + usr->get_username() + "@" + usr->get_hostname() + " " + command + " " + info);
	return (NULL);
}

void	User::reply(const std::string& reply)
{
	std::string buffer = reply + "\r\n";
	if (send(_fd, buffer.c_str(), buffer.length(), 0) < 0)
		std::cout << "Error while sending a message to a User!\n";
}

void	User::broadcast(Channel *chan, const std::string& reply)
{
	std::vector<std::string>	users = chan->get_nicknames();
	const char					*msg = reply.c_str();

	for (size_t i = 0; i < users.size(); ++i)
	{
		User* nick = chan->get_user_in_chan(users[i]);
		if (send(nick->get_fd(), msg, strlen(msg), 0) < 0)
			std::cout << "Error while sending a message to a User!\n";
	}
}
