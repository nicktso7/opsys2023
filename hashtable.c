#ifndef HASH
#define HASH

#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "utilities.h"

hash_table* hashtable_create()
{
    hash_table *table=malloc(sizeof(hash_table)); //create outer struct that holds buckets for nodes
    if(table == NULL)
    {
        printf("Malloc failed\n");
        return NULL;
    }

    table->free=0;;
    table->occupied=0;
    table->round=0;
    table->buckets_num=INITIAL_BUCKETS;

    table->buckets=malloc(INITIAL_BUCKETS * sizeof(hash_bucket*));
    if((table->buckets) == NULL)
    {
        printf("Malloc failed\n");
        return NULL;
    }


    for(int i=0;i<INITIAL_BUCKETS;i++)
    {
        table->buckets[i]=malloc(sizeof(hash_bucket)); //create each bucket that has predefined number of nodes, at this case 2
        if((table->buckets[i]) == NULL)
        {
            printf("Malloc failed\n");
            return NULL;
        }
        
        table->buckets[i]->current_size=0; //initial size of occupied nodes in bucket
        table->buckets[i]->bucket_index=i;
        table->buckets[i]->overflow_bucket=NULL;

        for(int j=0;j<INITIAL_NODES;j++)
        {
            table->buckets[i]->nodes[j]=malloc(sizeof(hash_node)); //initialize each node
            if((table->buckets[i]->nodes[j]) == NULL)
            {
                printf("Malloc failed\n");
                return NULL;
            }
            table->buckets[i]->nodes[j]->is_occupied=false;
            table->buckets[i]->nodes[j]->node=NULL;
        }
    }

    table->current=table->buckets[0];
    return table;
}

hash_table* hash_table_split_bucket(hash_table *table, int index)
{

    printf("testing split \n");

    table->buckets_num+=1;
    table->buckets=realloc(table->buckets,(table->buckets_num)*sizeof(hash_bucket*)); //realloc array of buckets to create a new bucket
    if(table->buckets == NULL)
    {
        printf("realloc failed\n");
        return NULL;
    }
    table->buckets[(table->buckets_num) - 1]=NULL;
    //table->buckets_num+=1; //add to number of total buckets
    table->free+=INITIAL_NODES; // add to total of free nodes to insert data    

    table->buckets[(table->buckets_num) - 1]=malloc(sizeof(hash_bucket)); //create new bucket and initialize its values, creating a node could have been a function, mostly copy paste of above code
    if ((table->buckets[(table->buckets_num) - 1]) == NULL)
    {
        printf("malloc failed\n");
        return NULL;
    }
    
    table->buckets[(table->buckets_num) - 1]->bucket_index=(table->buckets_num) - 1;
    table->buckets[(table->buckets_num) - 1]->current_size=0;
    for(int j=0;j<INITIAL_NODES;j++)
    {
        table->buckets[(table->buckets_num) - 1]->nodes[j]=malloc(sizeof(hash_node));
        if((table->buckets[(table->buckets_num) - 1]->nodes[j]) == NULL)
        {
            printf("Malloc failed\n");
            return NULL;
        }
        table->buckets[(table->buckets_num) - 1]->nodes[j]->is_occupied=false;
        table->buckets[(table->buckets_num) - 1]->nodes[j]->node=NULL;
    }

   return table;

}



hash_table* hashtable_insert(hash_table *table,int postal,int round)
{
    printf("here dumbass for future after splitting 5555 %d\n",round);
    printf("here dumbass for future after splitting 5555 %d\n",postal);
    int index=hash_function(postal,round,INITIAL_BUCKETS);
    printf("here dumbass for future after splitting %d\n",index);
    if(index < (table->current->bucket_index))
    {
        int new_round=table->round + 1;
        index=hash_function(postal,new_round,INITIAL_BUCKETS);
    }

    printf("here dumbass 2234 %d\n",table->buckets[index]->current_size);

    if((table->buckets[index]->current_size) < INITIAL_NODES) //handle insertion of nodes when there is space in bucket
    {
        printf("here dumbass %d\n",index);
        for(int i=0;i<INITIAL_NODES;i++)
        {
            if(table->buckets[index]->nodes[i]->is_occupied == false)
            {
                table->buckets[index]->current_size+=1;
                table->buckets[index]->nodes[i]->value=postal;
                table->buckets[index]->nodes[i]->is_occupied=true;
                table->occupied+=1;
                return table;
            }
            //else
            //{
            //    printf("Something went wrong, terminating program\n");
            //}
        }
    }
    else //create and handle overflow buckets
    {

        printf("here dumbass 3 %d\n",table->buckets[index]->current_size);
        /*
        start of initializing overflow buckets
        */
        
        if ((table->buckets[index]->overflow_bucket) == NULL)
        {
            table->buckets[index]->overflow_bucket=malloc(INITIAL_BUCKETS * sizeof(hash_bucket*));
            if((table->buckets[index]->overflow_bucket) == NULL)
            {
                printf("Malloc failed\n");
                return NULL;
            }

            for(int i=0;i<INITIAL_BUCKETS;i++)
            {
                table->buckets[index]->overflow_bucket[i]=malloc(sizeof(hash_bucket)); //create each bucket that has predefined number of nodes, at this case 2
                if((table->buckets[index]->overflow_bucket[i]) == NULL)
                {
                    printf("Malloc failed\n");
                    return NULL;
                }

                table->buckets[index]->overflow_bucket[i]->current_size=0; //initial size of occupied nodes in bucket
                table->buckets[index]->overflow_bucket[i]->bucket_index=i;

                for(int j=0;i<INITIAL_NODES;j++)
                {
                    table->buckets[index]->overflow_bucket[i]->nodes[j]=malloc(sizeof(hash_node)); //initialize each node
                    if((table->buckets[index]->overflow_bucket[i]->nodes[j]) == NULL)
                    {
                        printf("Malloc failed\n");
                        return NULL;
                    }
                    table->buckets[index]->overflow_bucket[i]->nodes[j]->is_occupied=false;
                    table->buckets[index]->overflow_bucket[i]->nodes[j]->node=NULL;
                }
            }
        }
        else
        {
            
        }
        

        /*
        end of initializing overflow buckets
        */

       //table=hashtable_insert(table,postal,table->round);

    }


    return table;

}


hash_table* hash_table_check_load(hash_table *table)
{
    float l=((table->occupied) / ((table->buckets_num) * INITIAL_NODES));
    if(l>L) //check if load exceeds predefined value
    {
        table=hash_table_split_bucket(table,table->current->bucket_index);
        int array_of_postal[INITIAL_NODES];
        for(int i=0;i<INITIAL_NODES;i++)
        {
            array_of_postal[i]=table->buckets[table->current->bucket_index]->nodes[i]->value;
        }

        /*delete previously inserted values and reenter them in hashing table
        use new array
        */

        for(int k=0;k<INITIAL_NODES;k++)
        {
            table->buckets[table->current->bucket_index]->nodes[k]->is_occupied=false;
            table->buckets[table->current->bucket_index]->nodes[k]->value=0;
            table->buckets[table->current->bucket_index]->current_size-=1;
        }


        int temp_round=table->round + 1;
        for(int j=0;j<INITIAL_BUCKETS;j++)
        {
            table=hashtable_insert(table,array_of_postal[j],temp_round);
        }

        table->current++;

        if(((table->buckets_num) % 2) == 0)
        {
            table->round++;
            table->current=table->buckets[0];
        }

        printf("\n\n\n\n %d \n\n\n\n",table->buckets_num);
    }
    
    return table;

}


hash_node* hash_table_lookup(hash_table* table,int postal,int round)
{
    int index=hash_function(postal,table->round,INITIAL_BUCKETS);
    if(index < (table->current->bucket_index))
    {
        int new_round=table->round + 1;
        //index=hash_function(table,new_round,postal);
    }

    while(1)
    {
        //for(());
    }

}


#endif