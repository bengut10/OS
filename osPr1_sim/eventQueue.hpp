//
//  eventQueue.hpp
//  sim
//
//  Created by Benjamin Gutierrez on 10/4/16.
//  Copyright © 2016 Benjamin Gutierrez. All rights reserved.
//

#ifndef eventQueue_hpp
#define eventQueue_hpp
#include <stdio.h>
#include <iostream>

class eventQueue
{
    
public:
    class event
    {
    public:
        event * next;
        float time;
        int type;
        int id;
        float service,waiting, turnA;
       
        event()
        {
        }
        
        event(int type, int id, float time)
        {
            this->time = time;
            this->type = type;
            this->id = id;
            this->next = 0;
            this->waiting = 0;
            this->turnA = 0;
        }
    };
    
    event *head;
    eventQueue();
    ~eventQueue();
    int schedule_event(class event*);
};

#endif /* eventQueue_hpp */
