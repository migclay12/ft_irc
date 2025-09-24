#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <time.h>

//ERRORS
#define ERR_WASNOSUCHNICK(source, name)					"401 " + source + " " + name + " :No such nick"
#define ERR_NOSUCHCHANNEL(source, channel)				"403 " + source + " " + channel + " :No such channel"
#define ERR_UNKNOWNCOMMAND(source, command)				"421 " + source + " " + command + " :Unknown command"
#define ERR_NICKNAMEINUSE(source)						"433 " + source + " " + source  + " :Nickname is already in use"
#define ERR_USERNOTINCHANNEL(source, nick, channel)		"441 " + source + " " + nick + " " + channel + " :They aren't on that channel"
#define ERR_NOTONCHANNEL(source, channel)				"442 " + source + " " + channel + " :You're not on that channel"
#define ERR_USERONCHANNEL(source, channel)				"443 " + source + " " + channel + " :User is already in the channel."
#define ERR_NEEDMOREPARAMS(source, command)				"461 " + source + " " + command + " :Not enough parameters"
#define ERR_ALREADYREGISTRED(source)					"462 " + source + " :Unauthorized command (already registered)"
#define ERR_CHANNELISFULL(source, channel)				"471 " + source + " " + channel + " :Cannot join channel (+l)"
#define ERR_UNKNOWNMODE(source, char, channel)			"472 " + source + " " + char + " :is unknown mode char to me for " + channel
#define ERR_INVITEONLYCHAN(source, channel)				"473 " + source + " " + channel + " :Cannot join channel (+i)"
#define ERR_BADCHANNELKEY(source, channel)				"475 " + source + " " + channel + " :Cannot join channel (+k)"
#define ERR_BADCHANMASK(source, channel)				"476 " + source + " " + channel + " :Bad Channel Mask"
#define ERR_CHANOPRIVSNEEDED(source, channel)			"482 " + source + " " + channel + " :You're not channel operator"

//REPLIES
#define RPL_WELCOME(source)								"001 " + source + " :Welcome " + source + " to the ft_irc network"
#define RPL_JOIN(source, channel)						":" + source + " JOIN :" + channel
#define RPL_PART(source, channel)						":" + source + " PART :" + channel
#define RPL_PRIVMSG(source, target, message)			":" + source + " PRIVMSG " + target + " :" + message
#define RPL_NOTOPIC(source, channel)					"331 " + source + " " + channel + " :No topic is set"
#define RPL_TOPIC(source, channel, topic)				"332 " + source + " " + channel + " :" + topic
#define RPL_NAMEREPLY(nick, channel, names)				"353 " + nick + " = " + channel + " :" + names
#define RPL_INVITING(nick, user, channel)				"341 " + nick + " " + user + " :" + channel
