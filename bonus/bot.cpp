#include "bot.hpp"

Bot::Bot(int argc, char **argv)
{
	this->_error = false;
	this->_server_socket = -1;
	try {
		if (argc != 5)
			throw std::runtime_error("Usage: " + std::string(argv[0]) + " <ip_address> <port> <password> <#channel>");
		this->_password = std::string(argv[3]);
		this->_channel = std::string(argv[4]);
		if (this->_password.empty() || this->_channel.empty())
			throw std::runtime_error("Invalid password or channel");
		this->_hostname = std::string(argv[1]);
		this->_ip_address = resolve_hostname(argv[1]);
		this->_port = parse_port(argv[2]);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		this->_error = true;
	};
}

Bot::~Bot() {}

int parse_port(const char *port)
{
	for (int i = 0; port[i]; i++)
	{
		if (std::isdigit(port[i]) == 0)
			throw std::runtime_error("Invalid port: " + std::string(port));
	}
	int port_nbr = std::atoi(port);
	if (port_nbr < 0 || port_nbr > 65535)
		throw std::runtime_error("Invalid port: " + std::string(port));
	return (port_nbr);
}

std::string	resolve_hostname(const char *hostname)
{
	struct addrinfo in_info, *result;

	memset(&in_info, 0, sizeof(in_info));
	in_info.ai_family   = AF_UNSPEC;
	in_info.ai_socktype = SOCK_RAW;

	int ret_addrinf = getaddrinfo(hostname, NULL, &in_info, &result);
	if (ret_addrinf != 0)
		throw std::runtime_error("getaddrinfo: " + std::string(gai_strerror(ret_addrinf)));

	if (result->ai_family == AF_INET6)
	{
		freeaddrinfo(result);
		throw std::runtime_error("(" + std::string(hostname) + ") is an invalid IPv4");
	}

	struct sockaddr_in  *ipv4 = (struct sockaddr_in *)result->ai_addr;
	void                *addr = &(ipv4->sin_addr);

	char    ip_str[INET6_ADDRSTRLEN];
	inet_ntop(result->ai_family, addr, ip_str, sizeof(ip_str));

	freeaddrinfo(result);
	return (std::string(ip_str));
}

void Bot::sendMsg(const std::string& message)
{
	std::string msg = message + "\n";
	if (send(this->_server_socket, msg.c_str(), msg.length(), 0) < 0)
		throw std::runtime_error("Send error: " + std::string(strerror(errno)));
}

int Bot::init(void)
{
	if (this->_error == true)
		return (EXIT_FAILURE);
	std::cout << "Joining server " << this->_hostname << " (" << this->_ip_address << ":"<< this->_port << ")\n";

	try {
		this->_server_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (this->_server_socket < 0)
			throw std::runtime_error("socket_creation_error: " + std::string(strerror(errno)));

		struct sockaddr_in srv_addr;

		srv_addr.sin_family = AF_INET;
		srv_addr.sin_port = htons(this->_port);

		if (inet_pton(AF_INET, this->_ip_address.c_str(), &srv_addr.sin_addr) <= 0)
			throw std::runtime_error("Invalid address/Address not supported");
		
		if (connect(this->_server_socket, (struct sockaddr*)&srv_addr, sizeof(srv_addr)))
			throw std::runtime_error("Connection failed: " + std::string(strerror(errno)));

		this->sendMsg("PASS " + this->_password);
		this->sendMsg(std::string("NICK ") + BOT_NICK);
		this->sendMsg("USER ircbot ircbot ircbot :ircbot");
		this->sendMsg("JOIN " + this->_channel);
		this->sendMsg("TOPIC " + this->_channel + " :The available commands are JOKE and TIME.");

	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;

		if (this->_server_socket != 0)
			close(this->_server_socket);
		exit(EXIT_FAILURE);
	};
	return (EXIT_SUCCESS);
}

IRCMessage parseIRCMessage(const std::string& message)
{
	IRCMessage msg;
	std::string input = message;
	size_t pos = 0;

	if (input[0] == ':')
	{
		pos = input.find(' ');
		msg.prefix = input.substr(1, pos - 1);
		input.erase(0, pos + 1);

		size_t nick_end = msg.prefix.find('!');
		if (nick_end != std::string::npos)
		{
			msg.nick = msg.prefix.substr(0, nick_end);
			size_t user_end = msg.prefix.find('@', nick_end);
			if (user_end != std::string::npos)
			{
				msg.user = msg.prefix.substr(nick_end + 1, user_end - nick_end - 1);
				msg.host = msg.prefix.substr(user_end + 1);
			}
		}
		else
			msg.host = msg.prefix;
	}

	pos = input.find(' ');
	if (pos == std::string::npos)
	{
		msg.command = input;
		return (msg);
	}
	msg.command = input.substr(0, pos);
	input.erase(0, pos + 1);

	while (!input.empty())
	{
		if (input[0] == ':')
		{
			msg.trailing = input.substr(1);
			break ;
		}

		pos = input.find(' ');
		if (pos != std::string::npos)
		{
			msg.params.push_back(input.substr(0, pos));
			input.erase(0, pos + 1);
		}
		else
		{
			msg.params.push_back(input);
			break;
		}
	}
	return (msg);
}

void Bot::selectBotMode(std::string prefix, IRCMessage *msg_struct)
{
	std::string msg = msg_struct->trailing;

	if (msg.rfind("JOKE\r\n", 0) == 0)
	{
		std::vector<std::string> joke_v;

		joke_v = Jokes::get_joke_vect();
		for (size_t i = 0; i < joke_v.size(); i++)
			this->sendMsg(prefix + joke_v[i]);

	} else if ((msg.rfind("TIME\r\n", 0) == 0)) {
		this->sendMsg(prefix + "Current time: " + getCurrentDateTime());
	} else {
		this->sendMsg(prefix + "Unkown Comand: The available commands are JOKE and TIME.");
	}
}

void Bot::handleMessage(const std::string& raw)
{
	IRCMessage msg = parseIRCMessage(raw);

	if (msg.nick != BOT_NICK && msg.command == "PRIVMSG")
	{
		std::string sender = msg.nick;
		if (msg.nick.empty())
			sender = msg.host;
		std::string target = "";
		if (!msg.params.empty())
			target = msg.params[0];
		std::string message = msg.trailing;

		if (target == BOT_NICK)
			target = sender;
		std::string prefix_msg = "PRIVMSG " + target + " :";

		selectBotMode(prefix_msg, &msg);
	}
}

int Bot::run(void)
{
	char message[BUFFER_SIZE + 1];
	
	while (true)
	{
		fd_set read_fds;
		FD_ZERO(&read_fds);
		FD_SET(this->_server_socket, &read_fds);

		struct timeval timeout = {.tv_sec = 1, .tv_usec = 0};

		int ret_select = select(this->_server_socket + 1, &read_fds, NULL, NULL, &timeout);
		if (ret_select < 0)
		{
			if (ret_select == EINTR) continue;
			std::cerr << "Select error: " << strerror(errno) << std::endl;
			break;
		}

		if (FD_ISSET(this->_server_socket, &read_fds))
		{
			int ret_recv = recv(this->_server_socket, message, BUFFER_SIZE, 0);
			if (ret_recv <= 0)
			{
				std::cerr << "Connection closed by server" << std::endl;
				break;
			}
			message[ret_recv] = '\0';

			handleMessage(std::string(message));
			memset(message, 0, BUFFER_SIZE + 1);
		}
	}
	close(this->_server_socket);
	return (EXIT_SUCCESS);
}
