#ifndef PARS_HPP
# define PARS_HPP

class   Pars {
private:
    ~Pars(void);
public:
    Pars(void);
    
    void    join(std::vector<std::string> buffers, User& sender);
    void    kick(std::vector<std::string> buffers, User& sender);
    void    invite(std::vector<std::string> buffers, User& sender);
    void    part(std::vector<std::string> buffers, User& sender);
    void    mode(std::vector<std::string> buffers, User& sender);
    void    who(std::vector<std::string> buffers, User& sender);
    void    whois(std::vector<std::string> buffers, User& sender);
    void    whowas(std::vector<std::string> buffers, User& sender);
    void    kill(std::vector<std::string> buffers, User& sender);
    void    notice(std::vector<std::string> buffers, User& sender);
    void    pass(std::vector<std::string> buffers, User& sender);
    void    oper(std::vector<std::string> buffers, User& sender);
    void    topic(std::vector<std::string> buffers, User& sender);
    void    user(std::vector<std::string> buffers, User& sender);
    void    quit(std::vector<std::string> buffers, User& sender);
    void    nick(std::vector<std::string> buffers, User& sender);
    void    names(std::vector<std::string> buffers, User& sender);
    void    list(std::vector<std::string> buffers, User& sender);
    void    sendNoCmd(std::vector<std::string> buffers, User& sender);
}

#endif