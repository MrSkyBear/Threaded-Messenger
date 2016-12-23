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

#include "Post_Office.h"
#include "Worker.h"

using namespace std;

class Server {
public:
    Server();
    ~Server();

    void run();
    
protected:
    virtual void create();
    virtual void close_socket();
    void serve();
    void handle(int);
    string get_request(int, int);
    bool send_response(int, string);
	static void* work(void* ptr);

    int server_;
    int buflen_;
    char* buf_;
	int num_threads;
	vector<pthread_t*> threads;

};
