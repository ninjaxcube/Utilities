#include "containers.h"
#include <stdlib.h>

static node_t * node_create(void * p_data)
{
	node_t * p_node = (node_t *)malloc(sizeof(node_t));

	if (NULL != p_node)
	{
		p_node->p_next = NULL;
		p_node->p_data = p_data;
	}

	return p_node;
}

static int8_t hash_resize(hashtable_t * p_hashtable, uint32_t new_capacity)
{
	if (NULL == p_hashtable)
	{
		return -1;
	}

	hashtable_entry_t ** new_buckets = calloc(new_capacity,
	sizeof(hashtable_entry_t *));

	if (NULL == new_buckets)
	{
		return -1;
	}

	for (uint32_t i = 0; i < p_hashtable->capacity; i++)
	{
		hashtable_entry_t * p_entry = p_hashtable->pp_buckets[i];

		while (NULL != p_entry)
		{
			hashtable_entry_t * p_next = p_entry->p_next;
			uint64_t hash = p_hashtable->hash_function(p_entry->p_key);
			uint32_t index = hash % new_capacity;
			p_entry->p_next = new_buckets[index];
			new_buckets[index] = p_entry;
			p_entry = p_next;
		}
	}

	free(p_hashtable->pp_buckets);

	p_hashtable->pp_buckets = new_buckets;
	p_hashtable->capacity = new_capacity;

	return 0;
}

int8_t list_init(list_t * p_list)
{
	if (NULL == p_list)
	{
		return -1;
	}

	p_list->p_head = NULL;
	p_list->p_tail = NULL;
	p_list->size = 0;

	return 0;
}

int8_t list_push_back(list_t * p_list, void * p_data)
{
	node_t * p_node = NULL;

	if (NULL == p_list || NULL == p_data)
	{
		return -1;
	}

	p_node = node_create(p_data);
	if (NULL == p_node)
	{
		return -1;
	}

	if (NULL == p_list->p_tail)
	{
		p_list->p_head = p_node;
	}
	else
	{
		p_list->p_tail->p_next = p_node;
	}

	p_list->p_tail = p_node;
	p_list->size++;

	return 0;
}

int8_t list_push_front(list_t * p_list, void * p_data)
{
	node_t * p_node = NULL;
    
	if (NULL == p_list || NULL == p_data)
	{
		return -1;
	}

	p_node = node_create(p_data);
	if (NULL == p_node)
	{
		return -1;
	}

	p_node->p_next = p_list->p_head;
	p_list->p_head = p_node;

	if (NULL == p_list->p_tail)
	{
		p_list->p_tail = p_node;
	}

	p_list->size++;

	return 0;
}

int8_t list_pop_front(list_t * p_list, void ** p_data)
{
	node_t * p_node = NULL;

	if ((NULL == p_list) || (NULL == p_data) || (NULL == p_list->p_head))
	{
		return -1;
	}

	p_node = p_list->p_head;
	p_list->p_head = p_node->p_next;

	if (NULL == p_list->p_head)
	{
		p_list->p_tail = NULL;
	}

	*p_data = p_node->p_data;
	free(p_node);
	p_list->size--;

	return 0;
}

int8_t list_pop_back(list_t * p_list, void ** p_data)
{
	node_t * p_current;
	node_t * p_previous;

	if ((NULL == p_list) || (NULL == p_data) || (NULL == p_list->p_tail))
	{
		return -1;
	}

	p_current = p_list->p_head;
	p_previous = NULL;

	while (NULL != p_current->p_next)
	{
		p_previous = p_current;
		p_current = p_current->p_next;
	}

	if (NULL == p_previous)
	{
		p_list->p_head = NULL;
		p_list->p_tail = NULL;
	}
	else
	{
		p_previous->p_next = NULL;
		p_list->p_tail = p_previous;
	}

	*p_data = p_current->p_data;
	free(p_current);
	p_list->size--;

	return 0;
}

int8_t list_traverse(list_t * p_list, void (*callback)(void * p_data))
{
	node_t * p_current;

	if ((NULL == p_list) || (NULL == callback))
	{
		return -1;
	}

	p_current = p_list->p_head;
	while (NULL != p_current)
	{
		callback(p_current->p_data);
		p_current = p_current->p_next;
	}

	return 0;
}

int8_t stack_init(stack_t * p_stack)
{
	if (NULL == p_stack)
	{
		return -1;
	}

	p_stack->p_top = NULL;
	p_stack->size = 0;

	return 0;
}

int8_t stack_push(stack_t * p_stack, void * p_data)
{
	node_t * p_node = NULL;

	if (NULL == p_stack)
	{
		return -1;
	}

	p_node = node_create(p_data);

	if (NULL == p_node)
	{
		return -1;
	}

	p_node->p_next = p_stack->p_top;
	p_stack->p_top = p_node;
	p_stack->size++;

	return 0;
}

int8_t stack_pop(stack_t * p_stack, void ** p_data)
{
	node_t * p_node = NULL;

	if ((NULL == p_stack) || (NULL == p_data) || (NULL == p_stack->p_top))
	{
		return -1;
	}

	p_node = p_stack->p_top;
	p_stack->p_top = p_node->p_next;
	*p_data = p_node->p_data;
	free(p_node);
	p_stack->size--;

	return 0;
}

int8_t queue_init(queue_t * p_queue)
{
	if (NULL == p_queue)
	{
		return -1;
	}

	p_queue->p_head = NULL;
	p_queue->p_tail = NULL;
	p_queue->size = 0;

	return 0;
}

int8_t queue_enqueue(queue_t * p_queue, void * p_data)
{
	node_t * p_node = NULL;

	if (NULL == p_queue || NULL == p_data)
	{
		return -1;
	}

	p_node = node_create(p_data);

	if (NULL == p_node)
	{
		return -1;
	}

	if (NULL == p_queue->p_tail)
	{
		p_queue->p_head = p_node;
	}
	else
	{
		p_queue->p_tail->p_next = p_node;
	}

	p_queue->p_tail = p_node;
	p_queue->size++;

	return 0;
}

int8_t queue_dequeue(queue_t * p_queue, void ** p_data)
{
	node_t * p_node;

	if ((NULL == p_queue) || (NULL == p_data) || (NULL == p_queue->p_head))
	{
		return -1;
	}

	p_node = p_queue->p_head;
	p_queue->p_head = p_node->p_next;

	if (NULL == p_queue->p_head)
	{
		p_queue->p_tail = NULL;
	}

	*p_data = p_node->p_data;
	free(p_node);
	p_queue->size--;

	return 0;
}

uint64_t hash_fnv1a(const void *key, size_t len)
{
    const uint8_t *bytes = (const uint8_t *)key;
    uint64_t hash = FNV_OFFSET;

    for (size_t i = 0; i < len; i++) {
        hash ^= bytes[i];
        hash *= FNV_PRIME;
    }

    return hash;
}

int8_t hashtable_init(hashtable_t ** pp_hashtable,
    hash_function_t hash_func, compare_function_t compare_func)
{
	*pp_hashtable = NULL;
    hashtable_t * p_hashtable = NULL;

    if ((NULL == pp_hashtable) || (NULL == hash_func) || (NULL == compare_func))
    {
        return -1;
    }

    *pp_hashtable = (hashtable_t *)malloc(sizeof(hashtable_t));

    if (NULL == *pp_hashtable)
    {
        return -1;
    }

    (*pp_hashtable)->pp_buckets = (hashtable_entry_t **)calloc(INITIAL_HASHTABLE_CAPACITY, sizeof(hashtable_entry_t *));

    if (NULL == (*pp_hashtable)->pp_buckets)
    {
        free(*pp_hashtable);
        return -1;
    }

    (*pp_hashtable)->capacity = INITIAL_HASHTABLE_CAPACITY;
    (*pp_hashtable)->size = 0;
    (*pp_hashtable)->hash_function = hash_func;
    (*pp_hashtable)->compare_function = compare_func;

    return 0;
}

int8_t hashtable_insert(hashtable_t * p_hashtable, void * p_key, void * p_value)
{
	if (NULL == p_hashtable || NULL == p_key || NULL == p_value)
	{
		return -1;
	}

	if (p_hashtable->size / p_hashtable->capacity > 1.00f)
	{
		if (hashtable_resize(p_hashtable, p_hashtable->capacity * 2) != 0)
		{
			return -1;
		}
	}

	uint32_t index = p_hashtable->hash_function(p_key) % p_hashtable->capacity;
	hashtable_entry_t * p_entry = p_hashtable->pp_buckets[index];
	
	while (p_entry != NULL)
	{
		if (p_hashtable->compare_function(p_entry->p_key, p_key) == 0)
		{
			p_entry->p_value = p_value;
			return 0;
		}
		p_entry = p_entry->p_next;
	}

	p_entry = (hashtable_entry_t *)malloc(sizeof(hashtable_entry_t));
	if (NULL == p_entry)
	{
		return -1;
	}

	p_entry->p_key = p_key;
	p_entry->p_value = p_value;
	p_entry->p_next = p_hashtable->pp_buckets[index];
	p_hashtable->pp_buckets[index] = p_entry;
	p_hashtable->size++;

	return 0;

}
int8_t hashtable_search(hashtable_t * p_hashtable, void * p_key, void ** p_data)
{
	if (NULL == p_hashtable || NULL == p_key || NULL == p_data)
	{
		return -1;
	}

	uint32_t index = p_hashtable->hash_function(p_key) % p_hashtable->capacity;
	hashtable_entry_t * p_entry = p_hashtable->pp_buckets[index];

	while (p_entry != NULL)
	{
		if (p_hashtable->compare_function(p_entry->p_key, p_key) == 0)
		{
			*p_data = p_entry->p_value;
			return 0;
		}
		p_entry = p_entry->p_next;
	}

	return -1;
}
int8_t hashtable_delete(hashtable_t * p_hashtable, void * p_key)
{
	if (NULL == p_hashtable || NULL == p_key)
	{
		return -1;
	}

	uint32_t index = p_hashtable->hash_function(p_key) % p_hashtable->capacity;
	hashtable_entry_t * p_entry = p_hashtable->pp_buckets[index];
	hashtable_entry_t * p_prev = NULL;

	while (p_entry != NULL)
	{
		if (p_hashtable->compare_function(p_entry->p_key, p_key) == 0)
		{
			if (p_prev == NULL)
			{
				p_hashtable->pp_buckets[index] = p_entry->p_next;
			}
			else
			{
				p_prev->p_next = p_entry->p_next;
			}
			free(p_entry);
			p_hashtable->size--;
			return 0;
		}
		p_prev = p_entry;
		p_entry = p_entry->p_next;
	}

	return -1;
}

int8_t hashtable_destroy(hashtable_t ** pp_hashtable)
{
	if (NULL == pp_hashtable || NULL == *pp_hashtable)
	{
		return -1;
	}

	hashtable_t * p_hashtable = *pp_hashtable;

	for (uint32_t i = 0; i < p_hashtable->capacity; i++)
	{
		hashtable_entry_t * p_entry = p_hashtable->pp_buckets[i];
		while (p_entry != NULL)
		{
			hashtable_entry_t * p_next = p_entry->p_next;
			free(p_entry);
			p_entry = p_next;
		}
	}

	free(p_hashtable->pp_buckets);
	free(p_hashtable);
	*pp_hashtable = NULL;

	return 0;
}