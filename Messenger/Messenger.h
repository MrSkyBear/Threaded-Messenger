#pragma once
#include <string>
#include <map>
#include <vector>
#include <utility>

#include "Server_Message.h"

using namespace std;

class Messenger
{
public:
	Messenger();
	~Messenger();

	string handle_request(string request);
	string add_message(string name, string subject, string message);
	string list_messages(string name);
	string print_messages(string name, int message_number);
	string clear();

private:
	map<string, vector<Server_Message> > mailbox;
	map<string, vector<Server_Message> >::iterator it;
};

