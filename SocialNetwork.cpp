
#include <iostream>
#include <cctype>
#include <string>
using namespace std;

struct Post
{
    std::string username;
    std::string message;
};

class Profile
{
private:
    std::string username;
    std::string displayname;
public:
    // Profile constructor for a user (initializing 
    // private variables username=usrn, displayname=dspn)
    Profile(std::string usrn, std::string dspn) : username(usrn), displayname(dspn) {}
    // Default Profile constructor (username="", displayname="")
    Profile() : username(" "), displayname(" ") {}
    // Return username
    std::string getUsername()
    {
        return username;
    }
    // Return name in the format: "displayname (@username)"
    std::string getFullName()
    {
        std::string fullDisplay = displayname + " (@" + username + ")";
        return fullDisplay;
    }
    // Change display name
    void setDisplayName(std::string dspn)
    {
        displayname = dspn;
    }
};

class Network
{
private:
    static const int MAX_USERS = 20; // max number of user profiles
    int numUsers;                    // number of registered users
    Profile profiles[MAX_USERS];     // user profiles array:
                                     // mapping integer ID -> Profile
    bool following[MAX_USERS][MAX_USERS];  // friendship matrix:
                                           // following[id1][id2] == true when id1 is following id2
    static const int MAX_POSTS = 100; // max number of posts
    int numPosts;                    // number of posts
    Post posts[MAX_POSTS];           // array of all posts
    // Returns user ID (index in the 'profiles' array) by their username
    // (or -1 if username is not found)
    int findID(string usrn)
    {
        for (int i = 0; i < MAX_USERS; i++)
        {
            if (profiles[i].getUsername() == usrn)
                return i;
        }

        return -1;
    }
public:
    // Constructor, makes an empty network (numUsers = 0)
    Network() : numUsers(0), numPosts(0)
    {
        for (int row = 0; row < MAX_USERS; row++)
            for (int col = 0; col < MAX_USERS; col++)
                following[row][col] = false;
    }
    // Attempts to sign up a new user with specified username and displayname
    // return true if the operation was successful, otherwise return false
    bool addUser(string usrn, string dspn)
    {
        // Checks to see if there is still place left for new users
        if (numUsers < MAX_USERS)
        {
            // Makes sure all characters are alphanumerical
            for (int i = 0; i < usrn.size(); i++)
            {
                if (!((usrn[i]) || isdigit(usrn[i])))
                    return false;
            }

            // Checks to see if the username exists already
            if (findID(usrn) != -1)
                return false;

            Profile newUser(usrn, dspn);   // Creates a new user Profile
            profiles[numUsers] = newUser;  // Adds user to the array of profiles
            numUsers++;                    // Increments the number of users
            return true;
        }

        return false;
    }

    // Make 'usrn1' follow 'usrn2' (if both usernames are in the network).
    // return true if success (if both usernames exist), otherwise return false
    bool follow(string usrn1, string usrn2)
    {
        // Makes sure both users exist
        if (findID(usrn1) != -1 && findID(usrn2) != -1)
        {
            following[findID(usrn1)][findID(usrn2)] = true;
            return true;
        }

        return false;
    }

    // Print Dot file (graphical representation of the network)
    void printDot()
    {
        std::cout << "digraph {" << std::endl;
        for (int i = 0; i < numUsers; i++)
            std::cout << "\t\"@" << profiles[i].getUsername() << "\"""" << std::endl;

        std::cout << std::endl;
        for (int row = 0; row < MAX_USERS; row++)
        {
            for (int col = 0; col < MAX_USERS; col++)
            {
                if (following[row][col] == true)
                    std::cout << "\t\"@" << profiles[row].getUsername() << "\"""" << " -> " << "\"@" << profiles[col].getUsername() << "\"""" << std::endl;
            }
        }
        std::cout << "}" << std::endl;
    }

    // Add a new post
    bool writePost(string usrn, string msg)
    {
        if (numPosts < MAX_POSTS)
        {
            if (findID(usrn) != -1)
            {
                Post newMsg;
                newMsg.username = usrn;
                newMsg.message = msg;

                posts[numPosts] = newMsg;
                numPosts++;
                return true;
            }
        }

        return false;
    }

    // Print user's "timeline"
    void printTimeline(string usrn) 
    {
        for (int i = numPosts; i >= 0; i--)
        {
            if (posts[i].username == usrn)
                std::cout << profiles[findID(usrn)].getFullName() << ": " << posts[i].message << std::endl;

            else if (following[findID(usrn)][findID(posts[i].username)] == true)
                std::cout << profiles[findID(posts[i].username)].getFullName() << ": " << posts[i].message << std::endl;
        }
    }
};
