#include "server.h"
#include <pthread.h>

Server::Server() {
    // setup variables
    buflen_ = 1024;
    buf_ = new char[buflen_+1];
	num_threads = 0;
}

Server::~Server() {
    delete buf_;
}

void
Server::run() {
    // create and run the server
	num_threads = 10;
    create();
    serve();
	
}

void
Server::create() {
}

void
Server::close_socket() {
}

void
Server::serve() {
    // setup client
    int client;
    struct sockaddr_in client_addr;
    socklen_t clientlen = sizeof(client_addr);

	Post_Office* p = new Post_Office();

	sem_init(p->get_client_sem(), 0, 0);
	sem_init(p->get_critical_sem(), 0, 1);
	sem_init(p->get_buffer_sem(), 0, buflen_);
	sem_init(p->get_mailbox_sem(), 0, 1);

	void* param = (void*)p;
	for (int i = 0; i < num_threads; ++i)
	{
		pthread_t* thread = new pthread_t;
		pthread_create(thread, NULL, &work, param);
		threads.push_back(thread);
	}

      // accept clients
    while (1)
	{
		client = accept(server_,(struct sockaddr *)&client_addr,&clientlen);
		p->append(client);
    }

    close_socket();
}

void* Server::work(void* ptr)
{

	while (1)
	{
		Post_Office* p = (Post_Office*)ptr;
		int client = p->get_client();
		Worker w = Worker(p);
		
		w.handle(client);
	}
}

