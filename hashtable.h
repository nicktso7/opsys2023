#define INITIAL_BUCKETS 2
#define INITIAL_NODES 2
#define L 0.75

#include "list.h"
#include <stdbool.h>

typedef struct hash_bucket hash_bucket;

typedef struct hash_node
{
    int value;
    bool is_occupied;
    listnode *node;
}hash_node;

struct hash_bucket
{
    hash_node *nodes[INITIAL_NODES];
    int current_size; //size of occupied nodes in bucket
    int bucket_index;
    hash_bucket *overflow_bucket;
};

typedef struct hash_table
{
    int free;
    int occupied;
    int round;
    int temp_round;
    int buckets_num;
    int cur;
    hash_bucket *current;
    hash_bucket **buckets;
}hash_table;

hash_table* hashtable_create();

hash_bucket* hash_table_handle_overflow(hash_bucket* bucket,int postal);

hash_table* hashtable_insert(hash_table* table,int postal,int round,int temp_round);

hash_table* hash_table_split_bucket(hash_table* table, int index); //function to split a bucket indicated by index number

hash_table* hash_table_check_load(hash_table* hash_table);

hash_node* hash_table_lookup(hash_table* table,int postal,int round);

void print_overflow(hash_bucket* bucket);

void print_hash(hash_table* table);

