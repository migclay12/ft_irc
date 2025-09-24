#include "GetCmd.hpp"

void handle_invite(User* usr, Server& srv)
{
	std::vector<std::string> params = usr->msg.get_params();
	std::string info;

	if (params.empty() || params.size() <= 1)
		return (usr->reply("Usage: INVITE <nick> [<channel>], invites someone to a channel, by default the current channe"));

	Channel* chan = srv.get_channel(params[1]);
	if (chan == NULL)
		return(usr->reply(ERR_NOSUCHCHANNEL(usr->get_nickname(), params[1])));
	if (!chan->find_user(usr->get_nickname()))
		return(usr->reply(ERR_NOTONCHANNEL(usr->get_nickname(), params[1])));
	if (!chan->find_ops(usr->get_nickname()))
		return(usr->reply(ERR_CHANOPRIVSNEEDED(usr->get_nickname(), params[1])));

	User* reciver = srv.get_user(params[0]);
	if (reciver == NULL)
		return(usr->reply(ERR_WASNOSUCHNICK(usr->get_nickname(), params[0])));
	if (chan->find_user(params[0]))
		return(usr->reply(ERR_USERONCHANNEL(usr->get_nickname(), chan->get_name())));
	
	if (!chan->find_invite(params[0]))
		chan->add_invite(reciver);
	usr->reply(":" + usr->get_hostname() + " " + RPL_INVITING(usr->get_nickname(), reciver->get_nickname(), chan->get_name()));
	usr->reply(":" + usr->get_nickname() + "!" + usr->get_username() + "@" + usr->get_hostname() + " INVITE " + reciver->get_nickname() + " :" + chan->get_name());
	reciver->reply(":" + usr->get_nickname() + "!" + usr->get_username() + "@" + usr->get_hostname() + " INVITE " + reciver->get_nickname() + " :" + chan->get_name());
}
