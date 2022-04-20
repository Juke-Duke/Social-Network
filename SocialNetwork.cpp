#include <iostream>
#include <cctype>
#include <string>
using std::string;

struct Post { string username, message; };

class Profile
{
private:
    string username, displayname, fullDisplay;

public:
    Profile() : username(" "), displayname(" "), fullDisplay(" ") {}

    Profile(const string& usrn, const string& dspn) : username(usrn), displayname(dspn) { fullDisplay = displayname + " (@" + username + ")"; }

    string getUsername() const { return username; }

    string getFullName() const { return fullDisplay; }

    void setDisplayName(const string& dspn) { displayname = dspn; }
};

class Network
{
private:
    static const int MAX_USERS = 20, MAX_POSTS = 100;
    int numUsers, numPosts;
    Post posts[MAX_POSTS];
    Profile profiles[MAX_USERS];
    bool following[MAX_USERS][MAX_USERS];

    int findID(const string& usrn) const
    {
        for (int i = 0; i < numUsers; ++i)
            if (profiles[i].getUsername() == usrn)
                return i;

        return -1;
    }

public: 
    Network() : numUsers(0), numPosts(0)
    {
        for (int row = 0; row < MAX_USERS; ++row)
            for (int col = 0; col < MAX_USERS; ++col)
                following[row][col] = false;
    }

    bool addUser(const string& usrn, const string& dspn)
    {
        if (numUsers == MAX_USERS || findID(usrn) != -1)
            return false;

        for (char letter : usrn)
            if (!isalnum(letter))
                return false;

        profiles[numUsers] = Profile(usrn, dspn);
        ++numUsers;

        return true;
    }

    bool follow(const string& usrn1, const string& usrn2)
    {
        int id1 = findID(usrn1), id2 = findID(usrn2);

        if (id1 == -1 || id2 == -1)
            return false;

        following[id1][id2] = true;

        return true;
    }

    void printDot() const
    {
        std::cout << "digraph \n{" << std::endl;
        for (int i = 0; i < numUsers; ++i)
            std::cout << "\t" << "\"@" << profiles[i].getUsername() << "\"""" << std::endl;

        std::cout << std::endl;
        for (int row = 0; row < MAX_USERS; ++row)
            for (int col = 0; col < MAX_USERS; ++col)
                if (following[row][col])
                    std::cout << "\t\"@" << profiles[row].getUsername() << "\"""" << " -> " << "\"@" << profiles[col].getUsername() << "\"""" << std::endl;
        std::cout << "}" << std::endl;
    }

    bool writePost(const string& usrn, const string& msg)
    {
        if (numPosts == MAX_POSTS || findID(usrn) == -1)
            return false;

        Post newMsg;
        newMsg.username = usrn;
        newMsg.message = msg;

        posts[numPosts] = newMsg;
        ++numPosts;

        return true;
    }

    void printTimeline(const string& usrn) const
    {
        for (int i = numPosts; i >= 0; --i)
        {
            int id = findID(usrn), followId = findID(posts[i].username);

            if (posts[i].username == usrn)
                std::cout << profiles[id].getFullName() << ": " << posts[i].message << std::endl;

            else if (following[id][followId])
                std::cout << profiles[followId].getFullName() << ": " << posts[i].message << std::endl;
        }
    }
};