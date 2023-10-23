#include "Pars.hpp"

Command parseCommand(const std::string& cmd) {
    if (cmd == "MODE") {
        return MODE;
    } else if (cmd == "KICK") {
        return KICK;
    } else if (cmd == "PART") {
        return PART;
    } else if (cmd == "JOIN") {
        return JOIN;
    } else if (cmd == "PASS") {
        return PASS;
    } else if (cmd == "TOPIC") {
        return TOPIC;
    } else if (cmd == "USER") {
        return USER;
    } else if (cmd == "QUIT") {
        return QUIT;
    } else if (cmd == "NICK") {
        return NICK;
    }
    return UNKNOWN;
}

//***********************************JOIN FUNCTION*********************
bool setNonBlocking(int socket_fd)
{
    if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) == -1) {
        perror("fcntl");
        return false;
    }
    return true;
}

bool sendMessage(int user_fd, const std::string& message)
{
    if (!setNonBlocking(user_fd)) {
        return false;
    }

    ssize_t bytes_sent = write(user_fd, message.c_str(), message.size());

    if (bytes_sent == -1) {
        if (errno == EAGAIN) {
            std::cout << "Message send would block." << std::endl;
            return false;
        } else {
            perror("write");
            return false;
        }
    }

    return true;
}

void    join(std::vector<std::string> buffers, User& sender, std::vector<Channel*> channelList)
{
    // utiliser Channel ici 1/ Veirifier si channel eiste, si non creer une puis partir dans une fonction channel
    // 
    (void)channelList;
    std::string channelName = buffers[1];
    
    if (buffers.size() == 1 || buffers.size() > 2)
    {
        std::string argument = "PRIVMSG " + channelName + " :This is the channel topic \r\n";
        // argument += "332\r\n"; 
        sendMessage(sender.GetUserFd(), argument);
        return ;
    }
    
    // std::string argument = "PRIVMSG " + channelName + " :Welcome to the channel\r\n";
    std::string argument = "PRIVMSG " + channelName + " :Welcome to the channel \r\n";
    sendMessage(sender.GetUserFd(), argument);

    std::cout << "You used JOIN " << channelName << std::endl;
}
//********************************************************************

User*     find_member_name(std::vector<User*> userList, std::string member_name) {
    for(std::vector<User*>::iterator it = userList.begin(); it != userList.end(); it++) {
        if((*it)->GetUserName() == member_name)
            return *it;
    }
    return 0;
}

Channel*     find_channel_name(std::vector<Channel*> channelList, std::string channel_name) {
    for(std::vector<Channel*>::iterator it = channelList.begin(); it != channelList.end(); it++) {
        if((*it)->getName() == channel_name)
            return *it;
    }
    return 0;
}

void    kick(std::vector<std::string> buffers, User& sender, std::vector<Channel*> channelList, std::vector<User*> userList) {
    std::vector<struct s_replie>    replie;

    setReplie(&replie);
    if (buffers.size() < 3) {
        std::vector<std::string>    tmp;
        tmp.push_back(buffers[1]);
        Server::sendReplie(tmp , 461, sender.GetUserFd(), replie);
        return ;
    }
    Channel* chan = find_channel_name(channelList, buffers[2]);
    if (chan == 0) {
        std::vector<std::string>    tmp;
        tmp.push_back(buffers[2]);
        Server::sendReplie(tmp , 403, sender.GetUserFd(), replie);
        return ;
    }
    std::vector<int>    allOpMember = chan->getAllOpMember();
    if(*find(allOpMember.begin(), allOpMember.end(), sender.GetUserFd()) != sender.GetUserFd()){
        std::vector<std::string>    tmp;
        tmp.push_back(buffers[2]);
        Server::sendReplie(tmp , 403, sender.GetUserFd(), replie);
        return ;
    }
    User*    vic = find_member_name(userList, buffers[3]);
    if(!chan->isInChannel(vic->GetUserFd())) {
        std::vector<std::string>    tmp;
        tmp.push_back(buffers[2]);
        Server::sendReplie(tmp , 403, sender.GetUserFd(), replie);
        return ;
    }
    chan->suppMember(sender.GetUserFd(), vic->GetUserFd());
    return ;
}

void    part(std::vector<std::string> buffers, User& sender, std::vector<Channel*> channelList) {
    std::vector<struct s_replie>    replie;

    setReplie(&replie);
    if (buffers.size() < 2) {
        std::vector<std::string>    tmp;
        tmp.push_back(buffers[1]);
        Server::sendReplie(tmp , 461, sender.GetUserFd(), replie);
        return ;
    }
    for (std::vector<std::string>::iterator it = buffers.begin() + 2; it != buffers.end(); it++) {
        Channel* chan = find_channel_name(channelList, *it);
        if (chan == 0) {
            std::vector<std::string>    tmp;
            tmp.push_back(*it);
            Server::sendReplie(tmp , 403, sender.GetUserFd(), replie);
        }
        if(!chan->isInChannel(sender.GetUserFd())){
            std::vector<std::string>    tmp;
            tmp.push_back(*it);
            Server::sendReplie(tmp , 442, sender.GetUserFd(), replie);
        }
        chan->memberLeave(sender.GetUserFd());
    }
    return ;
}

void    mode(std::vector<std::string> buffers, User& sender) {
    (void)buffers;
    (void)sender;
    // return error : ERR_NEEDMOREPARAMS(461)<cmd> ERR_CHANOPRIVSNEEDED(482)<channel> ERR_NOSUCHNICK(401)<nickname> ERR_UNKNOWMODE(472)<char> ERR_NOSUCHCHANNEL(403)<channel name> ERR_USERDONTMATCH(502)
    // replie : RPL_UNMODEIS(221)<user mode string>
    //std::cout << "You are in JOIN" << std::endl;
}

void    topic(std::vector<std::string> buffers, User& sender, std::vector<Channel*> channelList) {
    std::vector<struct s_replie>    replie;

    setReplie(&replie);
    if (buffers.size() < 2) {
        std::vector<std::string>    tmp;
        tmp.push_back(buffers[1]);
        Server::sendReplie(tmp , 461, sender.GetUserFd(), replie);
        return ;
    }
    Channel* chan = find_channel_name(channelList, buffers[2]);
    if (chan == 0) {
        std::vector<std::string>    tmp;
        tmp.push_back(buffers[2]);
        Server::sendReplie(tmp , 403, sender.GetUserFd(), replie);
        return ;
    }
    if(!chan->isInChannel(sender.GetUserFd())){
        std::vector<std::string>    tmp;
        tmp.push_back(chan->getName());
        Server::sendReplie(tmp , 442, sender.GetUserFd(), replie);
        return ;
    }
    std::string topic = chan->getTopic();
    if (topic == "")
    {
        if(buffers.size() >= 3) {
                std::string topic = 0;
            for(std::vector<std::string>::iterator it = buffers.begin() + 3; it != buffers.end(); it++)
               topic += *it; 
            if (!chan->setTopic(sender.GetUserFd(), topic)) {
                    std::vector<std::string>    tmp;
                    tmp.push_back(chan->getName());
                    Server::sendReplie(tmp , 482, sender.GetUserFd(), replie);
                    return ;
            }
        }
        std::vector<std::string>    tmp;
        tmp.push_back(chan->getName());
        Server::sendReplie(tmp , 331, sender.GetUserFd(), replie);
    } else {
        if(buffers.size() >= 3) {
                std::string topic = 0;
            for(std::vector<std::string>::iterator it = buffers.begin() + 3; it != buffers.end(); it++)
               topic += *it; 
            if (!chan->setTopic(sender.GetUserFd(), topic)) {
                    std::vector<std::string>    tmp;
                    tmp.push_back(chan->getName());
                    Server::sendReplie(tmp , 482, sender.GetUserFd(), replie);
                    return ;
            }
        }
        std::vector<std::string>    tmp;
        tmp.push_back(chan->getName());
        tmp.push_back(topic);
        Server::sendReplie(tmp , 332, sender.GetUserFd(), replie);
    }
}

void    user(std::vector<std::string> buffers, User& sender) {
    (void)buffers;
    (void)sender;
    //std::string argument = "PRIVMSG : Welcome to our IRC Server !";
    //"332 " + channelName + " :This is the channel topic";
    // argument += "332\r\n"; 
    //sendMessage(sender.GetUserFd(), argument);
    // return error : ERR_NEEDMOREPARAMS(461)<cmd> ERR_ALREADYREGISTRED(462)
}

void    quit(std::vector<std::string> buffers, User& sender) {
    (void)buffers;
    (void)sender;
    std::cout << "You used QUIT" << std::endl;
}


void    nick(std::vector<std::string> buffers, User& sender, std::vector<User*> members) {
    std::vector<struct s_replie>    replie;

    setReplie(&replie);
    if (buffers.size() < 2) {
        std::vector<std::string>    tmp;
        Server::sendReplie(tmp , 431, sender.GetUserFd(), replie);
        return ;
    }
    if(find_member_name(members, buffers[2])) {
        std::vector<std::string>    tmp;
        tmp.push_back(buffers[2]);
        Server::sendReplie(tmp , 433, sender.GetUserFd(), replie);
        return ;
    }
    sender.ChangeNickname(buffers[2]);
    return ;
}

void    pass(std::vector<std::string> buffers, User& sender) {
    (void)buffers;
    (void)sender;
    // return error : ERR_NEEDMOREPARAMS(461)<cmd> ERR_ALREADYREGISTRED(462)
}

void    sendNoCmd(std::vector<std::string> buffers, User& sender) {
    (void)buffers;
    (void)sender;
}

