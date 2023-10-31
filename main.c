#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#define BUFFER 200


int main(void)
{
    FILE *file=fopen("voters50.csv","r");
    if(file == NULL)
    {
        perror("File open error");
        return 1;
    }
    char line[BUFFER];
    //int code;
    char name[20];
    char lasname[20];
    int postal;
    
    hash_table *table=hashtable_create();
    listout *list=list_create();

    //printf("duuuuuumb %d \n\n",table->buckets[1]->current_size);

    int val;
    int x;
    printf("Hello world, good to have you with us in this testing!\n");
    while(1)
    {
        while(fgets(line,sizeof(line),file))
        {
            int tokens_read=sscanf(line,"%d %s %s %d",&val,name,lasname,&postal);
            table=hashtable_insert(table,val,table->round,table->temp_round);
            //print_hash(table);
            list=list_insert(list,postal,'s','d');
            list_print(list);
            table=hash_table_check_load(table);
            //print_hash(table);
            //printf("tokens read %d",tokens_read);
        }
        //printf("Please insert value:\n");
        scanf("%d",&x);
        switch (x)
        {
        case 1:
            printf("exiting program\n");
            return 0;
            break;
        
        case 2:
            printf("printing table\n");
            print_hash(table);
            break;
        
        case 3:
            printf("printing list\n");
            list_print(list);
            break;

        default:
            printf("exiting program\n");
            return 0;
            break;
        }
    }
}