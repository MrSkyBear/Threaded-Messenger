I created a class called Post_Office. It contains an instance of my mailbox class which hold all the messages for the server. The post_office also contains the queue where clients are put, and four different semaphores. 

In my server.cc I create a new instance of my post_office and initalize the semaphores. I wrote a function for the post office that returns a pointer to the semaphore I need. I put most of my sem_wait and post function calls inside my post office class to make the server class a little neater. My get_client function is basically my consumer, and the append function is my producer. 

I also have sem_wait and post functions around my handle_request function inside my worker class on line 63. This makes sure that only one thread can manipulate my data structure at a time.   