#include "client.h"

Client::Client() {
    // setup variables
    buflen_ = 1024;
    buf_ = new char[buflen_+1];
}

Client::~Client() {
}

void Client::run() {
    // connect to the server and run echo program
    create();
    transmit();
}

void
Client::create() {
}

void
Client::close_socket() {
}

string
Client::Server_Protocol(string line)
{
	string end = "\n";
    string command = "";
    string user = "\n";
	string temp = "";
	stringstream ss;
	ss.str(line);

	ss >> command;

	if (command == "send")
	{
		string subject = "\n";
		string message;
		ss >> user >> subject >> end;
		if (subject == "\n" || end != "\n")
		{
			line = "error";
		}
		else
		{
			cout << "- Type your message. End with a newline character -\n";
			getline(cin, message);
			while (getline(cin, temp))
			{
				if (temp == "")
				{
					break;
				}
				else
				{
					message.append(" % ");
					message.append(temp);
					temp = "";
				}
			}
			
			stringstream convert;
			convert << message.length();
			string length;
			convert >> length;
			
			line = "put ";
			line.append(user);
			line.append(" ");
			line.append(subject);
			line.append(" ");
			line.append(length);
			line.append("\n");
			line.append(message);
		}	
	}
	else if(command == "list")
	{
		ss >> user >> end;
		if (user == "\n" || end != "\n")
		{
			line = "error";
		}
		else
		{
			line = "list ";
			line.append(user);
			line.append("\n");
		}
	}
	else if(command == "read")
	{	
		string index;
		ss >> user >> index >> end;
		if (user == "\n" || end != "\n")
		{
			line = "error";
		}
		else
		{
			line = "get ";
			line.append(user);
			line.append(" ");
			line.append(index);
			line.append("\n");
		}
	}
	else if(command == "quit")
	{
		exit(0);
	}
	else
	{
		line = "error";
	}

	return line;

}

void
Client::transmit() {
    string line;
	string temp = "";

    cout << "% ";
    // loop to handle user interface
    while (getline(cin,line))
	{
        // append a newline
        line += "\n";

	line = Server_Protocol(line);
	
	if (line != "error")
	{
		// send request
		bool success = send_request(line);
		// break if an error occurred
		if (not success)
		    break;
		// get a response
		success = get_response();
		// break if an error occurred
		if (not success)
		    break;
	}
	else
	{
		cout << "error incorrect format\n";
	}

	cout << "% ";
    }
    close_socket();
}

bool
Client::send_request(string request) {
    // prepare to send request
    const char* ptr = request.c_str();
    int nleft = request.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) {
        if ((nwritten = send(server_, ptr, nleft, 0)) < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                continue;
            } else {
                // an error occurred, so break out
                perror("write");
                return false;
            }
        } else if (nwritten == 0) {
            // the socket is closed
            return false;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return true;
}

bool
Client::get_response() {
    string response = "";
    // read until we get a newline
    while (response.find("\n") == string::npos) {
        int nread = recv(server_,buf_,1024,0);
        if (nread < 0) {
            if (errno == EINTR)
                // the socket call was interrupted -- try again
                continue;
            else
                // an error occurred, so break out
                return "";
        } else if (nread == 0) {
            // the socket is closed
            return "";
        }
        // be sure to use append in case we have binary data
        response.append(buf_,nread);
    }

	//parse server protocol

	response = parse(response);



    // a better client would cut off anything after the newline and
    // save it in a cache
    cout << response;
    return true;
}

string
Client::parse(string line)
{
	string response = "";
	string command;
	stringstream ss;
	ss.str(line);

	ss >> command;


	if (command == "OK\n")
	{
		response.append(" \n");
		response.append(command);
		return response;
	}
	else if (command == "list")
	{
		int max;
		ss >> max;

		string index;
		string subject;

		for (int i = 0; i < max; i++)
		{
			ss >> index >> subject;
			response.append(index);
			response.append(" ");
			response.append(subject);
			response.append("\n");
		}

		return response;
	}
	else if (command == "message")
	{
		string subject;
		int length;
		string message;

		ss >> subject >> length;

		response.append(subject);
		response.append("\n");

		if (!ss.fail())
		{
			for (int i = 0; i < length; i++)
			{
				string temp;
				ss >> temp;
				if (temp == "%")
				{
					message.append("\n");
				}
				else
				{
					message.append(temp);
					if (i != length-1)
					{
						message.append(" ");
					}
				}
				
			}
		}
		else
		{
			return "error invalid length\n";
		}

		response.append(message);
		response.append("\n");

		return response;
	}
	else
	{
		return line;
	}
}
