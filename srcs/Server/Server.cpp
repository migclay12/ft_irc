#include "Server.hpp"

Server::Server(int port, std::string password)
{
	std::cout << "Creating server..\n";
	std::cout << "HOSTNAME: [" << HOSTNAME << "]\n";
	std::cout << "Port: [" << port << "]\n";
	std::cout << "Password: [" << password << "]\n";

	this->_port = port;
	this->_password = password;
}

Server::~Server()
{
	std::cout << "Deleting server..." << std::endl;
	for (size_t i = 0; i < this->_pollfds.size(); i++)
	{
		std::cout << "Closing client: " << i << " with fd: " << this->_pollfds[i].fd << std::endl;
		close(this->_pollfds[i].fd);
	}
	this->_pollfds.clear();
}

//Getters
User*	Server::get_user(const std::string& nickname)
{
	user_iterator it_b = _users.begin();
	user_iterator it_e = _users.end();

	while (it_b != it_e)
	{
		if (!nickname.compare(it_b->second.get_nickname()))
			return (&it_b->second);
		it_b++;
	}
	return (NULL);
}

User*	Server::get_user_fd(int fd)
{
	user_iterator it_b = _users.begin();
	user_iterator it_e = _users.end();

	while (it_b != it_e)
	{
		if (fd == it_b->second.get_fd())
			return (&it_b->second);
		it_b++;
	}
	return (NULL);
}

std::string&	Server::get_password(void)
{
	return (this->_password);
}

Channel*	Server::get_channel(const std::string& name)
{
	channel_iterator it = _channels.find(name);
	if (it != _channels.end())
		return &(it->second);
	return (NULL);
}

std::vector<Channel*>	Server::get_all_channels()
{
	std::vector<Channel*> result;
	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		result.push_back(&(it->second));
	return result;
}

//Functions
void	Server::create_channel(std::string name, std::string key)
{
	Channel channel = Channel(name, key);
	_channels.insert(std::make_pair(name, Channel(name, key)));
}

void	Server::delete_channel(Channel* chan)
{
	if (!chan)
		return;

	channel_iterator it = _channels.begin();

	while (it != _channels.end())
	{
		if (&(it->second) == chan)
		{
			_channels.erase(it);
			break;
		}
		++it;
	}
}

void	Server::print_channels() const
{
	std::cout << "\nChannels:\n";
	for (std::map<std::string, Channel>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
		std::cout << "- " << it->first << "\n";
}

void	Server::leave_all_srv(User *usr)
{
	std::vector<Channel*> result = this->get_all_channels();

	for (size_t i = 0; i < result.size(); i++)
	{
		if (usr->msg.get_trailing().empty())
			usr->msg.set_trailing("Leaving");
		result[i]->remove_user(usr);
		if (result[i]->get_size() == 0)
			this->delete_channel(result[i]);
	}
}
