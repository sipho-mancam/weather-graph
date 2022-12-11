#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

struct data{
char heading[50];
char date[50];
int numDP;
float data[361];
int state;

struct data *next;
struct data *prev;
};

typedef struct data DATA;
typedef DATA* NodePtr;

void insertElement(NodePtr *hPtr, DATA data)
{
    NodePtr newData = (DATA *)malloc(sizeof(DATA));

    if(newData == NULL)puts("Sorry no memory available for allocation");
    else
    {

        //put the new data inside the newPtr

        *(newData) = data;

        NodePtr currentPtr = *hPtr;
        NodePtr previousPtr = NULL;

        //go to the last element of the list
        while(currentPtr != NULL)
        {
            previousPtr = currentPtr;
            currentPtr = currentPtr->next;
        }

        //if we don't have the first element, create one and leave the function
        if(previousPtr == NULL)
        {
            currentPtr = newData;
            (*hPtr) = currentPtr;
            (*hPtr)->next = NULL;
            (*hPtr)->prev = NULL;
        }
        else
        {
            if(newData != NULL){
                currentPtr = newData;
                currentPtr->next = NULL;
                currentPtr->prev = previousPtr;
                previousPtr->next = currentPtr;
            }
        }
    }
}

int listSize(NodePtr *head)
{
    int j=0;
    NodePtr currentPtr = *head;
    NodePtr previousPtr = NULL;

    while(currentPtr != NULL)
    {
        previousPtr =currentPtr;
        currentPtr = currentPtr->next;
        j++;
    }

    return j;
}

int isEmpty(NodePtr *head)
{
    if(*head == NULL)return 1;
    else return 0;
}

void listToBuf(NodePtr *head, DATA * buf)
{
    int i=0;
    NodePtr currentPtr = *head;

    if(isEmpty(head))
    {
        std::cout<<"Sorry List is empty"<<std::endl;
    }
    else
    {
        i = listSize(head);

        for(int j=0; j<i; j++)
        {

            for(int k=0; k<(currentPtr->numDP)-1; k++)
            {
                buf[j].data[k]=currentPtr->data[k];
                printf("%.5f\n", buf[0].data[k]);

            }
            return;
            strcpy(buf[j].date, currentPtr->date);
            strcpy(buf[j].heading, currentPtr->heading);
            buf[j].numDP = currentPtr->numDP;
            buf[j].state = currentPtr->state;


            if(currentPtr->next != NULL)
            currentPtr = currentPtr->next;
        }

    }

}

#endif // LINKEDLIST_H_INCLUDED
