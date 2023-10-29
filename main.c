#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"


int main(void)
{
    hash_table *table=hashtable_create();

    printf("duuuuuumb %d \n\n",table->buckets[1]->current_size);

    int val;
    printf("Hello world, good to have you with us in this testing!\n");
    while(1)
    {
        printf("Please insert value:\n");
        scanf("%d",&val);
        table=hashtable_insert(table,val,table->round);
        table=hash_table_check_load(table);
    }
}