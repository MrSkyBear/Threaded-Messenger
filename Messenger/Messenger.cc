#include "Messenger.h"
#include <iostream>


Messenger::Messenger()
{

}


Messenger::~Messenger(void)
{
}

string Messenger::handle_request(string request)
{
	stringstream ss;
	ss.str(request);
	string command;

//	cout << "Request: " << request << "|" << endl; 
	ss >> command;
	if (command == "put")
	{
		string name;
		string subject;
		string message;
		int length;
		string blank;

		ss >> name >> subject >> length;

		if (!ss.fail())
		{
			while (message.length() < length)
			{
				string temp;
				ss >> temp;
				
				message.append(temp);
				
				if (message.length() < length)
				{
					message.append(" ");
				}
			}
			return add_message(name, subject, message);
		}
		else
		{
			return "error invalid index\n";
		}
	}
	else if (command == "list")
	{
		string name;
		ss >> name;
		return list_messages(name);
	}
	else if (command == "get")
	{
		string name;
		int index;

		ss >> name >> index;
		if (ss.fail())
		{
			return "error invalid index\n";
		}
		else
		{
			return print_messages(name, index);
		}
	}
	else if (command == "reset")
	{
		return clear();
	}
	else
	{
		return "error command not recognized\n";
	}
	
}




//these functions are returning protocol messages that the client should parse and print out as necessary.
string Messenger::add_message(string name, string subject, string message)
{ 
	Server_Message temp(subject, message);

	it = mailbox.find(name);

	if (it != mailbox.end())
	{
		it->second.push_back(temp);
	}
	else
	{
		vector<Server_Message> new_box;
		new_box.push_back(temp);

		pair< string, vector<Server_Message> > entry;
		entry = make_pair(name, new_box);

		mailbox.insert(entry);
	}
	return "OK\n";

}

string Messenger::list_messages(string name)
{
	stringstream ss;

	it = mailbox.find(name);

	if (it != mailbox.end())
	{
		int max = it->second.size();
		ss << "list " << max << "\n";
		for (int i = 0; i < max; ++i)
		{
			ss << i+1 << " " << it->second[i].to_string(true);
		}
	}
	else
	{
		ss << "error ";
		ss << "Person does not exist in this mailbox\n";
	}
	
	return ss.str();
}

string Messenger::print_messages(string name, int message_number)
{
	stringstream ss;
	it = mailbox.find(name);

	if (it != mailbox.end())
	{
		if (message_number <= it->second.size() && message_number >= 0)
		{
			ss << "message ";
			ss << it->second[message_number-1].to_string(false);
		}
		else
		{
			ss << "error ";
			ss << "That message does not exist\n";
		}
	}
	else
	{
		ss << "error ";
		ss << "Person does not exist in this mailbox\n";
	}

	return ss.str();
}

string Messenger::clear()
{
	mailbox.clear();
	return "OK\n";
}
