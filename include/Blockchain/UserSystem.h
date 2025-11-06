#ifndef USERSYS_H
#define USERSYS_H

#include "SystemAlgorithm.h"

#include <vector>
#include <algorithm>
#include <map>
#include <string>

// Forward declarations to avoid circular dependency
class User;
class Transaction;

//prolly would be better to have this class as blockchain manager which would also create blocks, etc. to fulfill SOLID but a bit too late
class UserSystem : SystemAlgorithm {
private:
    std::map<std::string, User*> users;
    UserSystem() {
    }

    UserSystem(const UserSystem &) = delete;

    UserSystem &operator=(const UserSystem &) = delete;

public:
    static UserSystem &getInstance() {
        static UserSystem instance;
        return instance;
    }

    void createNewUser(int seed);

    std::vector<User*> getSpecifiedNumberOfUsers(int numberOfUsers);

    std::vector<User*> getUsers() {
        std::vector<User*> userList;
        for (const auto& pair : users) {
            userList.push_back(pair.second);
        }
        return userList;
    }

    User* findUserMatchingPublicKey(std::string publicKey);

};

#endif
