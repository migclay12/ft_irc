#include "Channel.hpp"

Channel::Channel(const std::string &name, const std::string &key)
	: _name(name), _limit(0), _inviteOnly(false), _topicOps(true), _key(key) {}

Channel::~Channel() {}

Channel::Channel(const Channel &cpy)
{
		*this = cpy;
}

Channel &Channel::operator=(const Channel &cpy)
{
	if (this != &cpy)
	{
		_name = cpy._name;
		_topic = cpy._topic;
		_users = cpy._users;
		_operators = cpy._operators;
		_inviteOnly = cpy._inviteOnly;
		_limit = cpy._limit;
		_key = cpy._key;
		_topicOps = cpy._topicOps;
	}
	return (*this);
}

// Getters
std::string	Channel::get_name() const
{
	return (_name);
}

std::string	Channel::get_topic() const
{
	return (_topic);
}

std::string	Channel::get_key() const
{
	return (_key);
}

bool	Channel::get_topic_status() const
{
	return (_topicOps);
}

bool	Channel::get_invite() const
{
	return (_inviteOnly);
}

int		Channel::get_limit() const
{
	return (_limit);
}

int	Channel::get_size() const
{
	return (_users.size());
}

User*	Channel::get_admin() const
{
	if (_operators.empty())
		return (NULL);
	return (_operators[0]);
}

User*	Channel::get_user_in_chan(const std::string &nick)
{
	for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if ((*it)->get_nickname() == nick)
			return *it;
	}
	return NULL;
}

std::vector<std::string>	Channel::get_nicknames()
{
	std::vector<std::string>	nicknames;
	user_iterator				it_b = _users.begin();
	user_iterator				it_e = _users.end();

	while (it_b != it_e)
	{
		User* user = *it_b;

		if (user != NULL)
		{
			std::string nick = user->get_nickname();
			nicknames.push_back(nick);
		}
		++it_b;
	}
	return (nicknames);
}

std::vector<std::string>	Channel::get_ops_nicks()
{
	std::vector<std::string>	nicknames;
	user_iterator				it_b = _operators.begin();
	user_iterator				it_e = _operators.end();

	while (it_b != it_e)
	{
		User* user = *it_b;

		if (user != NULL)
		{
			std::string nick = user->get_nickname();
			nicknames.push_back(nick);
		}
		++it_b;
	}
	return (nicknames);
}

std::vector<std::string>	Channel::get_invite_nicks()
{
	std::vector<std::string>	nicknames;
	user_iterator				it_b = _invite.begin();
	user_iterator				it_e = _invite.end();

	while (it_b != it_e)
	{
		User* user = *it_b;

		if (user != NULL)
		{
			std::string nick = user->get_nickname();
			nicknames.push_back(nick);
		}
		++it_b;
	}
	return (nicknames);
}

// Setters
void	Channel::set_topic(const std::string &topic)
{
	_topic = topic;
}

void	Channel::set_invite(bool invite)
{
	_inviteOnly = invite;
}

void	Channel::set_limit(const int limit)
{
	_limit = limit;
}

void	Channel::set_key(const std::string &key)
{
	_key = key;
}

void	Channel::set_topic_ops(bool topic)
{
	_topicOps = topic;
}

//Others
void	Channel::add_user(User* usr)
{
	_users.push_back(usr);
}

void	Channel::add_opperator(User* usr)
{
	_operators.push_back(usr);
}

void	Channel::add_invite(User* usr)
{
	_invite.push_back(usr);
}

void	Channel::remove_user(User* usr)
{
	user_iterator it_b = _users.begin();
	user_iterator it_e = _users.end();

	while (it_b != it_e)
	{
		if (*it_b == usr)
			_users.erase(it_b);
		it_b++;
	}

	this->remove_ops(usr);
	this->remove_invite(usr);
}

void	Channel::remove_ops(User* usr)
{
	user_iterator	it_b = _operators.begin();
	user_iterator	it_e = _operators.end();

	while (it_b != it_e)
	{
		if (*it_b == usr)
			_operators.erase(it_b);
		it_b++;
	}
}

void	Channel::remove_invite(User* usr)
{
	user_iterator	it_b = _invite.begin();
	user_iterator	it_e = _invite.end();

	while (it_b != it_e)
	{
		if (*it_b == usr)
			_invite.erase(it_b);
		it_b++;
	}
}

bool	Channel::find_user(const std::string &nick)
{
	std::vector<std::string>	names = this->get_nicknames();

	for (size_t i = 0; i < names.size(); i++)
	{
		if (names[i] == nick)
			return (true);
	}
	return (false);
}

bool	Channel::find_ops(const std::string &nick)
{
	std::vector<std::string>	names = this->get_ops_nicks();

	for (size_t i = 0; i < names.size(); i++)
	{
		if (names[i] == nick)
			return (true);
	}
	return (false);
}

bool	Channel::find_invite(const std::string &nick)
{
	std::vector<std::string>	names = this->get_invite_nicks();

	for (size_t i = 0; i < names.size(); i++)
	{
		if (names[i] == nick)
			return (true);
	}
	return (false);
}
