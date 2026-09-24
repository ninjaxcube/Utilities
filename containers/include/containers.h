#ifndef CONTAINERS_H
#define CONTAINERS_H
#include <stdint.h>

#define FNV_OFFSET 14695981039346656037ULL
#define FNV_PRIME 1099511628211ULL
#define INITIAL_HASHTABLE_CAPACITY 16

typedef uint64_t (*hash_function_t)(const void * p_key);
typedef int8_t (*compare_function_t)(const void * p_key1, const void * p_key2);


typedef struct node
{
    struct node * p_next;
    void * p_data;
} node_t;

typedef struct list
{
    struct node * p_head;
    struct node * p_tail;
    uint32_t size;
} list_t;

typedef struct stack
{
    struct node * p_top;
    uint32_t size;
} stack_t;

typedef struct queue
{
    struct node * p_head;
    struct node * p_tail;
    uint32_t size;
} queue_t;

typedef struct hashtable_entry
{
    void * p_key;
    void * p_value;
    struct hashtable_entry * p_next;
} hashtable_entry_t;

typedef struct hashtable
{
    hashtable_entry_t ** pp_buckets;
    uint32_t capacity;
    uint32_t size;
    hash_function_t hash_function;
    compare_function_t compare_function;
} hashtable_t;

int8_t list_init(list_t * p_list);
int8_t list_push_back(list_t * p_list, void * p_data);
int8_t list_push_front(list_t * p_list, void * p_data);
int8_t list_pop_back(list_t * p_list, void ** p_data);
int8_t list_pop_front(list_t * p_list, void ** p_data);
int8_t list_traverse(list_t * p_list, void (*callback)(void * p_data));
int8_t stack_init(stack_t * p_stack);
int8_t stack_push(stack_t * p_stack, void * p_data);
int8_t stack_pop(stack_t * p_stack, void ** p_data);
int8_t queue_init(queue_t * p_queue);
int8_t queue_enqueue(queue_t * p_queue, void * p_data);
int8_t queue_dequeue(queue_t * p_queue, void ** p_data);
int8_t hashtable_init(hashtable_t ** pp_hashtable,
    hash_function_t hash_func, compare_function_t compare_func);
int8_t hashtable_insert(hashtable_t * p_hashtable, void * p_key, void * p_value);
int8_t hashtable_search(hashtable_t * p_hashtable, void * p_key, void ** pp_value);
int8_t hashtable_delete(hashtable_t * p_hashtable, void * p_key);
int8_t hashtable_destroy(hashtable_t ** pp_hashtable);
uint64_t hash_fnv1a(const void *key, size_t len);

#endif