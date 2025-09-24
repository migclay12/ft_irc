#include "GetCmd.hpp"

void handle_user(User* usr, Server& srv) 
{
	(void)srv;
	std::vector<std::string>	params	 = usr->msg.get_params();
    std::string					trailing = usr->msg.get_trailing();

	if (params.size() != 3 || trailing.empty())
	{
		usr->reply(":" + usr->get_hostname() + " " + ERR_NEEDMOREPARAMS(usr->get_nickname(), "USER")); 
		usr->reply(":" + usr->get_hostname() + " 650 " + usr->get_nickname() + " USER :<username> <unused> <unused> :<realname>");
		return ;
	}

	if (usr->is_registered())
		return(usr->reply(ERR_ALREADYREGISTRED(usr->get_nickname())));

	usr->set_realname(trailing);
	usr->set_username(params[0]);
	std::cout << "User: " << params[0] << " just registered!\n";

	usr->set_state(REGISTERED);
}
