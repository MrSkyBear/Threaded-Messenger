#include "Post_Office.h"


Post_Office::Post_Office()
{
	
}

int Post_Office::get_client()
{
	sem_wait(get_client_sem());
	sem_wait(get_critical_sem());
	int c = q.front();
	q.pop();
	sem_post(get_critical_sem());
	sem_post(get_buffer_sem());
	return c;
}

Messenger* Post_Office::get_messenger()
{
	return &m;
}

void Post_Office::append(int c)
{
	sem_wait(get_buffer_sem());
	sem_wait(get_critical_sem());
	q.push(c);
	sem_post(get_critical_sem());
	sem_post(get_client_sem());
}

sem_t* Post_Office::get_client_sem()
{
	return &client_semaphore;
}

sem_t* Post_Office::get_critical_sem()
{
	return &critical_semaphore;
}

sem_t* Post_Office::get_buffer_sem()
{
	return &buffer_semaphore;
}

sem_t* Post_Office::get_mailbox_sem()
{
	return &mailbox_semaphore;
}

