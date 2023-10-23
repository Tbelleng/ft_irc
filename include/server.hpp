/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luciefer <luciefer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 17:59:39 by tbelleng          #+#    #+#             */
/*   Updated: 2023/10/23 13:56:51 by luciefer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

# define DEBUG_CHANNEL 0
# define DEBUG_TOPIC 0
# define RPL_TOPIC 332

# include <algorithm>
# include <vector>
# include <map>
# include <iostream>
# include <string>
# include <fcntl.h>
# include <cstring>
# include <sstream>
# include <stdio.h>
# include <iomanip>
# include <errno.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <arpa/inet.h>
# include <netinet/in.h> 
# include <vector>
# include "Topic.hpp"
# include "Channel.hpp"
# include "User.hpp"
# include "Pars.hpp"
# include "irc.hpp"
# include "Message.hpp"

void    _send(const char* message, int member);

struct  s_replie {
    int nbReplie;
    std::string rplReplie;
};

class Server
{
	private :
	
	std::string password;
	unsigned int port;
	int serverSocket;
	int epoll_fd;
	struct sockaddr_in serverAddress;
	struct epoll_event event;
	std::vector<int> clientSockets;
	std::vector<User*> userList;
	std::vector<Channel*> channelList;
    std::vector<struct s_replie> _replie;
	
	public :
	
	Server(int port, std::string password);
	~Server(void);
	void ServerStart(void);
	void SetPort(unsigned int port);
	int  SetSocket(unsigned int port);
	void ServerRun(void);
	void ShowUserList(std::vector<User*> userList);
	int AddingNewClient(int epoll_fd, struct epoll_event* events);
	int ClientCheck(int user_fd);
	void GetUserInfo(int user_fd, std::string& buffer);
	User& whichUser(int user_fd);
    void    sendError(std::string str, int error, int socket_client);
    void    sendReplie(std::vector<std::string> buffer, int replie, int socket_client);

};


bool    _parcing(std::string buffer, User* sender, std::vector<Channel*> channelList);
//User&   getUser(int socket_client);

#endif
