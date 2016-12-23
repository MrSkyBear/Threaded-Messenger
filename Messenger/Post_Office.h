#pragma once
#include "Messenger.h"
#include <pthread.h>
#include <semaphore.h>
#include <queue>

using namespace std;

class Post_Office
{
public:
	Post_Office();
	int get_client();
	sem_t* get_client_sem();
	sem_t* get_critical_sem();
	sem_t* get_buffer_sem();
	sem_t* get_mailbox_sem();
	Messenger* get_messenger();
	void append(int c);

private:

	sem_t client_semaphore;
	sem_t critical_semaphore;
	sem_t buffer_semaphore;
	sem_t mailbox_semaphore;
	Messenger m;
	queue<int> q;
};