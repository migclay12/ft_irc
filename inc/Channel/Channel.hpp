#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include "User.hpp"
#include "response.hpp"

class User;

class Channel {
	
	typedef std::vector<User*>::iterator user_iterator;
	
	private:
		std::string			_name;
		std::string			_topic;
		std::vector<User*>	_users;
		std::vector<User*>	_operators;	//+o operator privilege
		std::vector<User*>	_invite;
		
		// Modes
		int				_limit;			//+l  limit to channel
		bool			_inviteOnly;	//+i Invite-only
		bool			_topicOps;		//+t restrictions of the topic
		std::string		_key;			//+k channel key

		Channel();
	public:
		Channel(const std::string &name, const std::string &key);
		~Channel();
		Channel(const Channel &cpy);
		Channel &operator=(const Channel &cpy);

		//Getters
		std::string					get_name() const;
		std::string					get_topic() const;
		std::string					get_key() const;
		bool						get_topic_status() const;
		bool						get_invite() const;
		int							get_limit() const;
		int							get_size() const;
		User*						get_admin() const;
		User*						get_user_in_chan(const std::string &nick);
		std::vector<std::string>	get_nicknames();
		std::vector<std::string>	get_ops_nicks();
		std::vector<std::string>	get_invite_nicks();

		//Setters
		void	set_topic(const std::string &topic);
		void	set_invite(bool invite);
		void	set_limit(const int limit);
		void	set_key(const std::string &key);
		void	set_topic_ops(bool topic);

		//Functions
		void	add_user(User* usr);
		void	add_opperator(User* usr);
		void	add_invite(User* usr);
		void	remove_user(User* usr);
		void	remove_ops(User* usr);
		void	remove_invite(User* usr);
		bool	find_user(const std::string &nick);
		bool	find_ops(const std::string &nick);
		bool	find_invite(const std::string &nick);
};
