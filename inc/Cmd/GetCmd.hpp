#pragma once

#include <string>
#include "User.hpp"
#include "Message.hpp"
#include "Channel.hpp"
#include "response.hpp"

class Server;

// Declaración del tipo de función handler
typedef void (*CommandFunc)(User *usr, Server&);

// Estructura para relacionar nombre de comando y su función
struct CommandHandler 
{
	const char* name;
	CommandFunc func;
};

// Función que llama a la función asociada a un comando
void	handle_command(User *usr, Server& srv);

// Funciones específicas para cada comando
void	handle_pass(User* usr, Server& srv);
void	handle_nick(User *usr, Server& srv);
void	handle_user(User *usr, Server& srv);
void	handle_privmsg(User *usr, Server& srv);
void	handle_join(User *usr, Server& srv);
void	handle_part(User *usr, Server& srv);
void	handle_topic(User* usr, Server& srv);
void	handle_mode(User* usr, Server& srv);
void	handle_who(User* usr, Server& srv);
void	handle_kick(User* usr, Server& srv);
void	handle_invite(User* usr, Server& srv);
void	handle_quit(User *usr, Server& srv);

//Modes
void	handle_i_on(User* usr, Channel* chan);
void	handle_i_off(User* usr, Channel* chan);
void	handle_t_on(User* usr, Channel* chan);
void	handle_t_off(User* usr, Channel* chan);
void	handle_k_on(User* usr, Channel* chan, std::string param);
void	handle_k_off(User* usr, Channel* chan, std::string param);
void	handle_o_on(User* usr, Channel* chan, Server &srv, std::string info);
void	handle_o_off(User* usr, Channel* chan, Server &srv, std::string info);
void	handle_l_on(User* usr, Channel* chan, std::string param);
void	handle_l_off(User* usr, Channel* chan);

//Functions
void						print_user_info(User *usr, Server& srv);
void						print_channel_users(User *usr, Server& srv);
std::vector<std::string>	split_channels(const std::string& input);

#include "Server.hpp"