#include "Blockchain/UserSystem.h"
#include "Blockchain/User.h"

void UserSystem::createNewUser(int seed) {
    User* newUser = new User(seed);
    std::string publicKey = newUser->getPublicKey();
    if(users.find(publicKey) == users.end()) {
        users[publicKey] = newUser;
    } else {
        delete newUser; 
    }
}

std::vector<User*> UserSystem::getSpecifiedNumberOfUsers(int numberOfUsers) {
    std::vector<User*> returnableUsers;
    returnableUsers.reserve(std::min(numberOfUsers, static_cast<int>(users.size())));
    
    int count = 0;
    for (const auto& pair : users) {
        if (count >= numberOfUsers) break;
        returnableUsers.push_back(pair.second);
        count++;
    }
    return returnableUsers;
}

User* UserSystem::findUserMatchingPublicKey(std::string publicKey) {
    if(users.find(publicKey) != users.end()) {
        return users[publicKey];
    }
    return nullptr;
}
