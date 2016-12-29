//
//  eventQueue.cpp
//  sim
//
//  Created by Benjamin Gutierrez on 10/4/16.
//  Copyright © 2016 Benjamin Gutierrez. All rights reserved.
//

#include "eventQueue.hpp"
#include <math.h>

eventQueue::eventQueue()
{
    head = 0;
}

eventQueue::~eventQueue()
{
    event* current = head;
    while( current != 0 )
    {
        event* next = current->next;
        delete current;
        current = next;
    }
    head = 0;
}
int eventQueue::schedule_event(class event* eve)
{
    event * previous = head;
    event * current = head;
 
    if(head == 0)
    {
        head = eve;
        current = head;
    }
    
    else
    {
        while(current != 0 )
        {
            if(current->time <= eve->time)
            {
                previous = current;
                current = current->next;
            }
            else
            {
                break;
            }
        }
        if(current == 0)
        {
            current = eve;
            previous->next = current;
            current->next = 0;
            
        }
        else
        {
            eve->next = current;
            current = eve;
            previous->next = current;
            
        }
    }
    return 0;
}
