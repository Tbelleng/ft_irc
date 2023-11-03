#include "irc.hpp"

std::vector<std::string> ft_split(const std::string& inputString)
{
    std::vector<std::string> result;
    std::string currentWord;

    for (size_t i = 0; i < inputString.length(); ++i) {
        char c = inputString[i];

        if (c == ' ' || c == '\t') {
            if (!currentWord.empty()) {
                result.push_back(currentWord);
                currentWord.clear();
            }
        } else {
            currentWord += c;
        }
    }

    if (!currentWord.empty()) {
        result.push_back(currentWord);
    }

    return result;
}
// void    _send(const char* message, int member, int epfd, struct epoll_event& ev) {
//     ev.events = EPOLLOUT;
//     ev.data.fd = member;

//     // Attends que le socket soit prêt pour l'écriture
//     struct epoll_event events[1];
//     int nfds = epoll_wait(epfd, events, 1, 5000);

//     if(nfds == -1) {
//         std::cerr << "epoll_wait failed for client: " << member << std::endl;
//         // check si c'est bien securise 
//     } else if(nfds > 0) {
//         ssize_t bytesSent = send(member, message, std::strlen(message), 0);
//         if(bytesSent == -1) {
//             std::cerr << "Failed to send message to client: " << member << std::endl;
//             // check si c'est bien securise 
//         }
//     } else {
//         std::cerr << "Send to client " << member << " timed out." << std::endl;
//     }

void    _send(const char* message, int member) {
        ssize_t bytesSent = send(member, message, std::strlen(message), 0);
        if(bytesSent == -1) {
            std::cerr << "Failed to send message to client: " << member << std::endl;
            // check si c'est bien securise 
        }
}
