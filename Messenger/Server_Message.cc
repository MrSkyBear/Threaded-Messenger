#include "Server_Message.h"
#include <iostream>


Server_Message::Server_Message(string subject, string message)
{
	this->subject = subject;
	this->message = message;
}


Server_Message::~Server_Message(void)
{
}

string Server_Message::to_string(bool list)
{
	stringstream ss;

	ss << subject;

	if (list)
	{
		ss << "\n";
	}
	else
	{
		ss << " " << message.length();
		ss << "\n";
		ss << message;
	}

	return ss.str();
}
