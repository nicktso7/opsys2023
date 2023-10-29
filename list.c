#ifndef LIST
#define LIST

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

listout* list_create()
{
    listout *list=malloc(sizeof(listout));
    if(list==NULL)
    {
        printf("Malloc failed\n");
        return NULL;
    }

    list->next=NULL;
    list->node=NULL;
    list->postal=0; //initializing first postal as zero to help with adding data

    return list;

}


void list_innerinsert(listnode *node,int postal, char name, char surname) //isnert data in the same postal code list in postal codes list
{
    listnode *temp = node;
    while((temp->next) != NULL)
    {
        temp=temp->next;
    }
    temp->next=malloc(sizeof(listnode));
    if((temp->next) == NULL)
    {
        printf("Malloc failed\n");
        return NULL;
    }
    temp->next->postal=postal;
    temp->name=name;
    temp->surname=surname;
    temp->next=NULL;
}


listout* list_insert(listout* list, int postal, char name, char surname) //insert new data into the list starting with outer nodes
{
    if((list->node)==NULL)
    {
        list->postal=postal;
        list->node=malloc(sizeof(listnode));
        if((list->node) == NULL)
        {
            printf("Malloc failed\n");
            return NULL;
        }
        list->node->postal=postal;
        list->node->next=NULL;
        list->node->name=name;
        list->node->surname=surname;
        return list;
    }
    else
    {
        if(postal < list->postal)
        {
            listout* new_node=malloc(sizeof(listout));
            if((new_node) == NULL)
            {
                printf("Malloc failed\n");
                return NULL;
            }

            new_node->node=malloc(sizeof(listnode));
            if((new_node->node) == NULL)
            {
                printf("Malloc failed\n");
                return NULL;
            }
            new_node->node->postal=postal;
            new_node->node->next=NULL;
            new_node->node->name=name;
            new_node->node->surname=surname;

            new_node->next=list;
            return new_node;
        }
        else
        {
            listout* temp = list;
        
            while(((temp->next)!=NULL) && (postal < temp->postal))
            {
                temp=temp->next;
            }
            
            if(postal == temp->postal)
            {
                list_innerinsert(temp->node,postal,name,surname);

                return list;
            }
            else
            {
                listout* new_node=malloc(sizeof(listout));
                if((new_node) == NULL)
                {
                    printf("Malloc failed\n");
                    return NULL;
                }
                
                new_node->node=malloc(sizeof(listnode));
                if((new_node->node) == NULL)
                {
                    printf("Malloc failed\n");
                    return NULL;
                }
                new_node->node->postal=postal;
                new_node->node->next=NULL;
                new_node->node->name=name;
                new_node->node->surname=surname;

                temp->next=new_node;
                return list;
            }
        }
    }
}

listnode* list_search(listout* list, int postal,char name);


#endif