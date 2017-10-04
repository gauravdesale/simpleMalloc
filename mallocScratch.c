#include<stdio.h>

void *malloc(size_t size)//this is the malloc method we are implementing
{
	size_t total_size;//size of the whole chunk
	void *block;//the actual chunk 
	struct header_t *header;//the assigned header and payload memory size values
	if (!size)
		return NULL;
	pthread_mutex_lock(&global_malloc_lock);//prevent multi threading from accessing the same memory
	header = get_free_block(size);//get the first headers
	if (header) {
		header->is_free = 0;//from struct assign values
		pthread_mutex_unlock(&global_malloc_lock);//run this again to make sure
		return (void*)(header + 1);//return the headers size
	}
	total_size = sizeof(struct header_t) + size;//new header values with the new headers 
	block = sbrk(total_size);//break here and get the size of the current chunk
	if (block == (void*) -1) {//if this is real
		pthread_mutex_unlock(&global_malloc_lock);
		return NULL;
	}
	header = block;
	header->size = size;
	header->is_free = 0;//new values from the struct
	header->next = NULL;
	if (!head)
		head = header;//update the header
	if (tail)
		tail->next = header;//if tail exits then assign it to the same value
	tail = header;
	pthread_mutex_unlock(&global_malloc_lock);
	return (void*)(header + 1);
}

struct header_t *get_free_block(size_t size)//free the memory 
{
	struct header_t *curr = head;//this is a current header value
	while(curr) {
		if (curr->is_free && curr->size >= size)//if these two values are satisfied
			return curr;
		curr = curr->next;//this is updating the current values
	}
	return NULL;
}
