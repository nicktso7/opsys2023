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
    table->temp_round=table->round + 1;
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

    table->cur=0;
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


hash_bucket* hash_table_handle_overflow(hash_bucket* bucket, int postal)
{
    if((bucket->overflow_bucket) == NULL)
    {
        bucket->overflow_bucket=malloc(INITIAL_BUCKETS * sizeof(hash_bucket));
        if((bucket->overflow_bucket) == NULL)
        {
            printf("Malloc failed\n");
            return NULL;
        }
        //for(int i=0;i<INITIAL_BUCKETS;i++)
        //{
        //    //bucket->overflow_bucket[i]=malloc(sizeof(hash_bucket)); //create each bucket that has predefined number of nodes, at this case 2
            //if((bucket->overflow_bucket[i]) == NULL)
            //{
            //    printf("Malloc failed\n");
            //    return NULL;
            //}
        bucket->overflow_bucket->current_size=0; //initial size of occupied nodes in bucket
        bucket->overflow_bucket->bucket_index=0;
        bucket->overflow_bucket->overflow_bucket=NULL;        
        for(int j=0;j<INITIAL_NODES;j++)
        {
            bucket->overflow_bucket->nodes[j]=malloc(sizeof(hash_node)); //initialize each node
            if((bucket->overflow_bucket->nodes[j]) == NULL)
            {
                printf("Malloc failed\n");
                return NULL;
            }
            bucket->overflow_bucket->nodes[j]->is_occupied=false;
            bucket->overflow_bucket->nodes[j]->node=NULL;
        }
        //}
        
        bucket->overflow_bucket->nodes[bucket->overflow_bucket->current_size]->value=postal;
        bucket->overflow_bucket->nodes[bucket->overflow_bucket->current_size]->is_occupied=true;
        bucket->overflow_bucket->current_size+=1;

        return bucket;
    }
    else
    {
        if((bucket->overflow_bucket->current_size) == INITIAL_NODES)
        {
            bucket->overflow_bucket=hash_table_handle_overflow(bucket->overflow_bucket,postal);
            return bucket;
        }
        else
        {
            int i=INITIAL_NODES-bucket->overflow_bucket->current_size;
            bucket->overflow_bucket->nodes[i]=malloc(sizeof(hash_node)); //initialize new node
            if((bucket->overflow_bucket->nodes[i]) == NULL)
            {
                printf("Malloc failed\n");
                return NULL;
            }
            bucket->overflow_bucket->nodes[i]->is_occupied=false;
            bucket->overflow_bucket->nodes[i]->node=NULL;
            
            bucket->overflow_bucket->current_size+=1;
            bucket->overflow_bucket->nodes[i]->value=postal;
            bucket->overflow_bucket->nodes[i]->is_occupied=true;

            return bucket;
        }
    }
}


hash_table* hashtable_insert(hash_table *table,int postal,int round,int temp_round)
{
    printf("round is %d\n",round);
    printf("postal is %d\n",postal);
    int index=hash_function(postal,round,INITIAL_BUCKETS);
    printf("index %d\n",index);
    if((index < (table->current->bucket_index)) && (round != temp_round))
    {
        int new_round=table->round + 1;
        index=hash_function(postal,new_round,INITIAL_BUCKETS);
        printf("new index %d\n",index);
    }

    printf("bucket %d current size is %d\n",index,table->buckets[index]->current_size);

    if((table->buckets[index]->current_size) < INITIAL_NODES) //handle insertion of nodes when there is space in bucket
    {
        printf("not in overflow index is %d\n",index);
        printf("bucket to be inserted current size is %d\n",table->buckets[index]->current_size);
        for(int i=0;i<INITIAL_NODES;i++)
        {
            if(table->buckets[index]->nodes[i]->is_occupied == false)
            {
                table->buckets[index]->current_size+=1;
                table->buckets[index]->nodes[i]->value=postal;
                table->buckets[index]->nodes[i]->is_occupied=true;
                table->occupied+=1;
                printf("bucket that value is inserted current size is %d\n",table->buckets[index]->current_size);
                return table;
            }
        }
        printf("bucket that value is inserted current size is %d\n",table->buckets[index]->current_size);
    }
    else //create and handle overflow buckets
    {

        printf("in overflow handling for bucket %d with size %d entering value %d\n",table->buckets[index]->bucket_index,table->buckets[index]->current_size,postal);
        /*
        start of initializing overflow buckets
        */
        
        table->buckets[index]=hash_table_handle_overflow(table->buckets[index],postal);
        table->occupied+=1;
        return table;

    }


    return table;

}


hash_table* hash_table_check_load(hash_table *table)
{
    
    printf("total occupied are %d an total buckets are %d\n",table->occupied,table->buckets_num);
    double l=((double)(table->occupied) /(double)((table->buckets_num) * INITIAL_NODES));
    //printf("l equals to %lf\n",l);
    if(l>L) //check if load exceeds predefined value
    {
        int* array_of_postal;
        printf("bucket to be splitted %d \n",table->current->bucket_index);
        
        //if bucket to be splitted has overflow buckets

        table=hash_table_split_bucket(table,table->current->bucket_index);
        //if bucket to be splitted has overflow buckets

        if((table->current->overflow_bucket) != NULL)
        {
            printf("split overflow\n");
            //find last hash table to be occupied
            hash_bucket* temp=table->current->overflow_bucket;
            while ((temp->overflow_bucket) != NULL)
            {
                temp=temp->overflow_bucket;
            }

            array_of_postal=malloc((temp->current_size) * sizeof(int));
            //array_of_postal[temp->current_size];
            for(int i=(temp->current_size);i>0;i--)
            {
                array_of_postal[i-1]=temp->nodes[i-1]->value;
                temp->nodes[i-1]->is_occupied=false;
                temp->nodes[i-1]->value=0;
                temp->current_size-=1;
            }
            //free(temp);
            table->current->overflow_bucket=NULL;
            
        }
        else
        {
            printf("split normal\n");
            array_of_postal=malloc((table->current->current_size) * sizeof(int));
            //array_of_postal[table->current->current_size];
            //printf("split normal completed\n");
            for(int i=(table->buckets[table->current->bucket_index]->current_size);i>0;i--)
            {
                printf("bucket to be splitted current size is %d",table->buckets[table->current->bucket_index]->current_size);
                array_of_postal[i-1]=table->buckets[table->current->bucket_index]->nodes[i-1]->value;
                table->buckets[table->current->bucket_index]->nodes[i-1]->is_occupied=false;
                table->buckets[table->current->bucket_index]->nodes[i-1]->value=0;
                table->buckets[table->current->bucket_index]->current_size-=1;
                printf("%d current size is %d\n",array_of_postal[i-1],table->current->current_size);
            }
            printf("\nsplit normal completed\n");
        }
        

        /*delete previously inserted values and reenter them in hashing table
        use new array
        */

        //for(int k=0;k<INITIAL_NODES;k++)
        //{
        //    table->buckets[table->current->bucket_index]->nodes[k]->is_occupied=false;
        //    table->buckets[table->current->bucket_index]->nodes[k]->value=0;
        //    table->buckets[table->current->bucket_index]->current_size-=1;
        //}


        int temp_round=table->round + 1;
        for(int j=0;j<INITIAL_BUCKETS;j++)
        {
            table=hashtable_insert(table,array_of_postal[j],temp_round,table->temp_round);
            table->occupied-=1;
        }

        //free(array_of_postal);
        table->cur+=1;
        table->current=table->buckets[table->cur];

        if(((is_power_of_two(table->buckets_num)) == 1) && (table->buckets_num != 2))
        {
            printf("current round is %d\n",table->round);
            table->round++;
            table->temp_round++;
            table->cur=0;
            table->current=table->buckets[0];
            printf("current round after rounding up is %d\n",table->round);
        }
        

    //printf("\n\n\n\n %d \n\n\n\n",table->buckets_num);
    }
    
    //if(((is_power_of_two(table->buckets_num)) == 1) && (table->buckets_num != 2))
    //    {
    //        printf("current round is %d\n",table->round);
    //        table->round++;
    //        table->temp_round++;
    //        table->current=table->buckets[0];
    //        printf("current round after rounding up is %d\n",table->round);
    //    }

    
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

void print_overflow(hash_bucket* bucket)
{
    
    //else
    //{
    printf("oveflow 2\n");
    printf(" Overflow bucket for bucket %d:",bucket->bucket_index);
    for(int i=0;i<INITIAL_NODES;i++)
    {
        printf("%d ",bucket->overflow_bucket->nodes[i]->value);
    }
    printf("\n");
    //}
    if((bucket->overflow_bucket->overflow_bucket) != NULL)
    {
        printf("oveflow 1\n");
        print_overflow(bucket->overflow_bucket);
    }
}


void print_hash(hash_table* table)
{
    for(int i=0;i<table->buckets_num;i++)
    {
        printf("Bucket %d: ", i);
        for(int j=0;j<INITIAL_NODES;j++)
        {
            printf("%d ",table->buckets[i]->nodes[j]->value);
        }
        if ((table->buckets[i]->overflow_bucket) != NULL)
        {
            print_overflow(table->buckets[i]);
        }
        
        printf("\n");
    }
}

#endif