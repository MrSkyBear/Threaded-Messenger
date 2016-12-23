#include "Worker.h"

Worker::Worker(Post_Office* p)
{
	post_office = p;
	mailbox = p->get_messenger();
	buflen_ = 1024;
    buf_ = new char[buflen_+1];
}


void
Worker::handle(int client) {
    // loop to handle all requests
    while (1) {
        string request = "";
		// get a request
		request = (get_request(client, 0));
		
		 // break if client is done or an error occurred
        if (request.empty())
            break;
		
		stringstream ss;
		ss.str(request);
		
		string command;
		ss >> command;
		
		if (command == "put")
		{
			string user;
			string subject;
			string length;
			
			ss >> user >> subject >> length;
			
			stringstream convert;
			convert.str(length);
				
			int len;
			convert >> len;
			
			if (!ss.fail())
			{
				//+3 for spaces inbetween fields.
				len += (command.length() + user.length() + subject.length() + length.length() + 3); 
				
				while (request.length() < len)
				{
					len -= request.length();
					string temp = get_request(client, len);
					request.append(temp);
				}
			}
		}

	//cout << "Request before handle: " << request << "|" << endl;
	//parse request and get information to send back

	
	sem_wait(post_office->get_mailbox_sem());
	request = mailbox->handle_request(request);
	sem_post(post_office->get_mailbox_sem());

	//cout << "Sending response: " << request << "|" << endl;

        // send response
        bool success = send_response(client,request);
        // break if an error occurred
        if (!success)
            break;
    }
    close(client);
}

string
Worker::get_request(int client, int length) {
    string request = "";
    // read until we get a newline
    while (request.find("\n") == string::npos) {
        int nread = recv(client,buf_,1024,0);
        if (nread < 0) {
            if (errno == EINTR)
			{
                // the socket call was interrupted -- try again
                continue;
			}
            else
			{
                // an error occurred, so break out
                return "";
			}
        } 
		else if (nread == 0)
		{
            // the socket is closed
            return "";
        }
        // be sure to use append in case we have binary data
        request.append(buf_,nread);
		if (length != 0 && request.length() > length)
		{
			break;
		}
    }
    // a better server would cut off anything after the newline and
    // save it in a cache
    return request;
}

bool
Worker::send_response(int client, string response) {
    // prepare to send response
    const char* ptr = response.c_str();
    int nleft = response.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) {
        if ((nwritten = send(client, ptr, nleft, 0)) < 0) {
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
