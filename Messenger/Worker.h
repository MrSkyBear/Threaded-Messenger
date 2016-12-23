#pragma once

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>
#include <iostream>
#include <vector>


#include <queue>
#include "Messenger.h"
#include "Post_Office.h"
using namespace std;

class Worker
{

private:
	Messenger* mailbox;
	Post_Office* post_office;

public:

	Worker(Post_Office* p);
	void handle(int client);
	string get_request(int client, int length);
	bool send_response(int client, string response);

	int server_;
    int buflen_;
    char* buf_;
};