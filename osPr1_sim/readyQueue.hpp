//
//  readyQueue.hpp
//  sim
//
//  Created by Benjamin Gutierrez on 10/5/16.
//  Copyright © 2016 Benjamin Gutierrez. All rights reserved.
//

#ifndef readyQueue_hpp
#define readyQueue_hpp

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cassert>
#include "eventQueue.hpp"

class readyQueue
{
    
private:
    int departure, numOfProcess;
    float lastClock;
    bool finished;
    std::ofstream myfile;
    
public:
    class process
    {
    public:
        process * next;
        float arrivalTime;
        float service;
        int id;
        float timeleft;
        
        process() {}
        
        process(int id, float arrival, float service)
        {
            this->id = id;
            this->arrivalTime = arrival;
            this->service = service;
            this->timeleft = service;
            next = 0;
        }
    };
    
    process *helper;
    process *tail;
    process *head;
    readyQueue();
    ~readyQueue();
    bool empty();
    int getNumOfProcesses();
    void insert(process * pro);
    void remove(eventQueue * eveQueue);
    void roundRobinSort(process * positionMe);
    void removeRoundRobin(float quantum, eventQueue * eveQueue);
    void remove_HRRN(eventQueue * eveQueue);
    process * findHighestPriority();
    void HRRNHelper(process* highest, eventQueue * eveQueue);
    void removeSRTF(float runThisLong, eventQueue * eveQueue);
    process * findHighestSRTF();
    void SRTFHelper(process* highest, eventQueue * eveQueue,
                    float simClockSim, int count);
};


#endif /* readyQueue_hpp */
