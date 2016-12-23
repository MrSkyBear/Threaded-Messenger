#pragma once
#include <string>
#include <sstream>

using namespace std;

class Server_Message
{
public:
	Server_Message(string subject, string message);
	~Server_Message(void);

	string to_string(bool list);

private:
	string subject;
	string message;
};

