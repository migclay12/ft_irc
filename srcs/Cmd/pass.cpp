#include "GetCmd.hpp"

void handle_pass(User* usr, Server& srv) 
{
	std::vector<std::string>	params	 = usr->msg.get_params();
    std::string					trailing = usr->msg.get_trailing();

	if (usr->is_registered())
		return(usr->reply(ERR_ALREADYREGISTRED(usr->get_nickname())));
	if (params.size() != 1 || !trailing.empty())
	{
		usr->reply(ERR_NEEDMOREPARAMS(usr->get_nickname(), "PASS"));
		usr->reply("650 " + usr->get_nickname() + " PASS " + ":<password>");
		usr->set_state(ERROR);
		return ;
	}
	if (params[0] != srv.get_password())
	{
		usr->reply("Wrong password");
		usr->set_state(ERROR);
		return ;
	}
	usr->set_state(LOGIN);
}
