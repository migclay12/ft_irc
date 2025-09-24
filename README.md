# ft_irc - Internet Relay Chat Server

A fully functional IRC server implementation in C++98, developed as part of the 42 curriculum.

![IRC Server](https://img.shields.io/badge/IRC-Server-blue)
![C++](https://img.shields.io/badge/C%2B%2B-98-orange)
![42](https://img.shields.io/badge/42-School-black)

## 🚀 Features

- **Core IRC Protocol**: Complete implementation of IRC server functionality
- **Multi-client Support**: Handles multiple clients simultaneously using non-blocking I/O
- **Channel Management**: Full channel operations with operator privileges
- **Command Support**: All essential IRC commands (PASS, NICK, USER, JOIN, PART, PRIVMSG, TOPIC, MODE, KICK, INVITE, QUIT, WHO)
- **Channel Modes**: Support for +i (invite-only), +t (topic restriction), +k (channel key), +o (operator), +l (user limit)
- **Bonus Features**: IRC Bot with jokes and time functions

## 🛠️ Technical Implementation

- **Language**: C++98 standard
- **I/O Model**: Non-blocking with poll() for efficient client handling
- **Architecture**: Object-oriented design with Server, User, Channel, and Message classes
- **Network**: TCP/IP socket programming
- **Memory Management**: RAII principles with proper resource cleanup

## 📋 Requirements

- C++98 compatible compiler (g++, clang++)
- Unix-like system (Linux/macOS)
- Make utility

## 🔨 Building

```bash
# Clone the repository
git clone https://github.com/migclay12/ft_irc.git
cd ft_irc

# Compile the main server
make

# Compile the bonus bot
make bonus

# Clean build files
make clean

# Full rebuild
make re
```

## 🚀 Usage

### Starting the IRC Server

```bash
# Start the IRC server
./ircserv <port> <password>

# Example
./ircserv 6667 mypassword
```

### Starting the Bot (Bonus)

```bash
# Start the bot
./bot <server_ip> <port> <password> <channel>

# Example
./bot 127.0.0.1 6667 mypassword #general
```

## 🧪 Testing

Connect using any IRC client:

### Using irssi
```bash
irssi -c 127.0.0.1 -p 6667
```

### Using hexchat
- Connect to `127.0.0.1:6667`
- Set password: `mypassword`

### Using netcat (for testing)
```bash
nc 127.0.0.1 6667
```

### Basic IRC Session Example
```
PASS mypassword
NICK mynick
USER myuser 0 * :My Real Name
JOIN #general
PRIVMSG #general :Hello everyone!
QUIT :Goodbye!
```

## 📁 Project Structure

```
ft_irc/
├── inc/                    # Header files
│   ├── Server/            # Server class
│   │   └── Server.hpp
│   ├── User/              # User management
│   │   └── User.hpp
│   ├── Channel/           # Channel operations
│   │   └── Channel.hpp
│   ├── Message/           # Message parsing
│   │   └── Message.hpp
│   ├── Cmd/               # Command handlers
│   │   └── GetCmd.hpp
│   └── response.hpp       # IRC response definitions
├── srcs/                  # Source files
│   ├── main.cpp           # Entry point
│   ├── Server/            # Server implementation
│   ├── User/              # User management
│   ├── Channel/           # Channel operations
│   ├── Message/           # Message parsing
│   └── Cmd/               # Command implementations
├── bonus/                 # Bonus bot implementation
│   ├── bot.cpp            # Bot main logic
│   ├── bot.hpp            # Bot header
│   ├── jokes.cpp          # Joke responses
│   ├── jokes.hpp          # Joke definitions
│   ├── time.cpp           # Time functions
│   ├── time.hpp           # Time definitions
│   └── main.cpp           # Bot entry point
├── Makefile               # Build configuration
└── README.md              # This file
```

## 📚 IRC Commands Supported

| Command | Description | Example |
|---------|-------------|---------|
| **PASS** | Server password authentication | `PASS mypassword` |
| **NICK** | Set nickname | `NICK alice` |
| **USER** | Set username and realname | `USER alice 0 * :Alice Smith` |
| **JOIN** | Join a channel | `JOIN #general` |
| **PART** | Leave a channel | `PART #general` |
| **PRIVMSG** | Send private/channel messages | `PRIVMSG #general :Hello!` |
| **TOPIC** | Set/view channel topic | `TOPIC #general :Welcome to general chat` |
| **MODE** | Change channel modes | `MODE #general +i` |
| **KICK** | Remove user from channel | `KICK #general bob :Being rude` |
| **INVITE** | Invite user to channel | `INVITE bob #general` |
| **QUIT** | Disconnect from server | `QUIT :Goodbye everyone!` |
| **WHO** | List channel users | `WHO #general` |

## 🔧 Channel Modes

| Mode | Description | Usage |
|------|-------------|-------|
| **+i** | Invite-only channel | `MODE #channel +i` |
| **+t** | Topic restriction to operators | `MODE #channel +t` |
| **+k** | Channel key (password) | `MODE #channel +k secret` |
| **+o** | Give operator privilege | `MODE #channel +o user` |
| **+l** | Set user limit | `MODE #channel +l 10` |

## 🏆 Bonus Features

### IRC Bot
- **Automated Responses**: Bot responds to user messages in channels
- **Joke System**: Random joke responses
- **Time Functions**: Current time and date responses
- **Interactive Commands**: Responds to specific bot commands

### Bot Commands
- `!joke` - Get a random joke
- `!time` - Get current time
- `!date` - Get current date

## 🧪 Testing Scenarios

### Connection Test
```bash
# Test basic connection
nc 127.0.0.1 6667
PASS testpass
NICK testuser
USER testuser 0 * :Test User
```

### Channel Operations Test
```bash
# Test channel joining and messaging
JOIN #test
PRIVMSG #test :Hello channel!
TOPIC #test :Test channel topic
```

### Mode Testing
```bash
# Test channel modes
MODE #test +i
MODE #test +k secretkey
MODE #test +l 5
```

## 🐛 Known Issues

- Server handles partial message reception correctly
- All IRC protocol errors are properly implemented
- Non-blocking I/O prevents server hanging

## 📚 42 Curriculum

This project is part of the 42 curriculum's **ft_irc** assignment, focusing on:

- **Network Programming**: Socket programming and TCP/IP
- **IRC Protocol**: Understanding and implementing IRC standards
- **Non-blocking I/O**: Efficient server architecture
- **Multi-client Handling**: Concurrent client management
- **C++98 Standards**: Legacy C++ programming practices

## 🤝 Contributing

This is a 42 school project, but suggestions and improvements are welcome!

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## 📄 License

This project is part of the 42 curriculum and follows 42's academic integrity policies.

## 👨‍💻 Author

**Miguel González Clayton** - 42 Student

- GitHub: [@migclay12](https://github.com/migclay12)
- 42 Profile: [migclay12.42.fr](https://profile.intra.42.fr/users/migclay12)

## 🙏 Acknowledgments

- 42 School for the challenging curriculum
- IRC protocol documentation
- The IRC community for inspiration

---

*Developed with ❤️ at 42 School*

![42 Logo](https://42.fr/wp-content/uploads/2021/05/42_logo_black.svg)
