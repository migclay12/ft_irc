#include "GetCmd.hpp"

void handle_nick(User* usr, Server& srv) 
{
	std::vector<std::string> get_nick = usr->msg.get_params();
	if (get_nick.empty())
		return(usr->reply("Usage: NICK <nickname>, sets your nick"));

	std::string nick = get_nick[0];
	User* exists = srv.get_user(nick);
	if (exists)
		return(usr->reply(ERR_NICKNAMEINUSE(nick)));

	std::string old_nick = usr->get_nickname();
	usr->set_nickname(nick);

	std::string msg = ":" + old_nick + "!" + usr->get_username() + "@" + usr->get_hostname() + " NICK :" + usr->get_nickname();

	usr->reply(msg);
	msg += "\r\n";
	std::vector<Channel*> user_channels = srv.get_all_channels();
	for (size_t i = 0; i < user_channels.size(); ++i)
	{
		if (user_channels[i]->find_user(nick))
			usr->broadcast(user_channels[i], msg);
	}
}
