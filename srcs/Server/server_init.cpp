#include "Server.hpp"

static std::string	resolve_hostname(const char *hostname)
{
	struct addrinfo	in_info, *result;
	
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
	std::cout << "IP: [" << ip_str << "]" << std::endl;
	return (std::string(ip_str));
}

static int	create_server_socket(int port)
{
	try
	{
		int	server_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (server_socket == -1)
			throw std::runtime_error("socket creation failed: " + std::string(strerror(errno)));

		struct sockaddr_in address;
		std::string	ip_addr_str = resolve_hostname(HOSTNAME);

		address.sin_family = AF_INET;
		address.sin_addr.s_addr = inet_addr(ip_addr_str.c_str());
		address.sin_port = htons(port);

		int optval = 1;
		if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
			throw std::runtime_error("setsockopt failed: " + std::string(strerror(errno)));

		//Used to make the socket non-blocking
		if (fcntl(server_socket, F_SETFL, O_NONBLOCK))
			throw std::runtime_error("fcntl failed: " + std::string(strerror(errno)));

		if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
			throw std::runtime_error("bind failed: " + std::string(strerror(errno)));

		if (listen(server_socket, MAX_CLIENTS) < 0)
			throw std::runtime_error("listen failed: " + std::string(strerror(errno)));

		return (server_socket);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		exit(EXIT_FAILURE);
	}
}

void	print_events(int events)
{
	if (events == (POLLIN | POLLOUT | POLLHUP))
		std::cout << "POLLIN | POLLOUT | POLLHUP";
	else if (events == (POLLIN | POLLOUT))
		std::cout << "POLLIN | POLLOUT";
	else if (events == (POLLIN))
		std::cout << "POLLIN";
	else if (events == (POLLOUT))
		std::cout << "POLLOUT";
	else
		std::cout << "[]";
}

void	print_pollfd(pollfd socket)
{
	std::cout << "FD: [" << socket.fd << "]";
	std::cout << " Events: ";
	print_events(socket.events);
	std::cout << " Revents: ";
	print_events(socket.revents);
	std::cout << std::endl;
}

void	Server:: accept_client(int server_socket)
{
	struct sockaddr_in	client_addr;
	socklen_t			addr_len = sizeof(client_addr);
	int					client_fd = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
	pollfd				client_socket;

	if (client_fd < 0)
	{
		std::cerr << "accept failed\n";
		exit(EXIT_FAILURE);
	}

	client_socket.fd = client_fd;
	client_socket.events = POLLIN | POLLHUP;
	this->_pollfds.push_back(client_socket);
	
	char hostname[NI_MAXHOST];
	int res = getnameinfo((struct sockaddr *) &client_addr, sizeof(client_addr), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
	if (res != 0)
	{
		throw std::runtime_error("Error while getting a hostname on a new client!");
	}

	User user = User(client_fd, ntohs(client_addr.sin_port), hostname);
	_users.insert(std::make_pair(client_fd, user));
}

void	Server::close_client_connection(size_t index)
{
	int		client_fd = this->_pollfds[index].fd;
	User*	usr = get_user_fd(client_fd);

	leave_all_srv(usr);

	close(this->_pollfds[index].fd);
	poll_iterator it_poll = this->_pollfds.begin();
	this->_pollfds.erase(it_poll + index);

	user_iterator it_user = this->_users.begin();
	it_user = this->_users.find(client_fd);
	if (it_user != this->_users.end())
		this->_users.erase(it_user);
}

void	Server:: check_client_revents(size_t index)
{
	int	client_fd = this->_pollfds[index].fd;

	if (this->_pollfds[index].revents & POLLIN)
	{
		char buffer[1024 + 1];
	
		int ret_recv = recv(this->_pollfds[index].fd, buffer, 1024, 0);
		if (ret_recv == -1) // SOCKET IS CLOSED
		{
			std::cout << "recv failed with client: " << index << " fd: " << this->_pollfds[index].fd << std::endl;
		}
		else if (ret_recv == 0) // CLIENT DISCONECTED
		{
			std::cout << "client: " << index << " fd: " << this->_pollfds[index].fd << " has disconected" << std::endl;
			close_client_connection(index);
		}
		else // GOT SOMETHING FROM THE CLIENT
		{
			buffer[ret_recv] = '\0';
			this->_users[client_fd].msg.parse_new_buffer(std::string(buffer), &this->_users[client_fd], *this);
			if (this->_users[client_fd].get_state() == ERROR)
			{
				std::cout << "client: " << index << " fd: " << this->_pollfds[index].fd << " has disconected" << std::endl;
				close_client_connection(index);
			}
		}
		memset(&buffer, 0, sizeof(buffer));
	}
	if (this->_pollfds[index].revents & POLLHUP)
	{
		std::cout << "Client desconected in a forced way\n";
	}
}

void	Server::init(void)
{
	pollfd	server_socket;

	server_socket.fd = create_server_socket(this->_port);
	server_socket.events = POLLIN;

	this->_pollfds.push_back(server_socket);

	std::cout << "Waiting for incoming connections...\n";
	while (close_program == false)
	{
		int ret_poll = poll(this->_pollfds.data(), this->_pollfds.size(), -1);
		if (ret_poll == -1)
		{
			std::cerr << "there was an error in POLL\n";
			break ;
		}
		else if (ret_poll == 0)
		{
			std::cout << "Timeout\n";
		}
		else
		{
			if (this->_pollfds[0].revents & POLLIN)
			{
				std::cout << "There is a new client\n";
				accept_client(this->_pollfds[0].fd);
				continue;
			}
			for (size_t index = 1; index < this->_pollfds.size(); index++)
				check_client_revents(index);
		}
	}
}
