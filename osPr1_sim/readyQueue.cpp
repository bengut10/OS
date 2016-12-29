//
//  readyQueue.cpp
//  sim
//
//  Created by Benjamin Gutierrez on 10/5/16.
//  Copyright © 2016 Benjamin Gutierrez. All rights reserved.


#include "readyQueue.hpp"
#include "sim.hpp"

readyQueue::readyQueue()
{
    helper = 0;
    head = 0;
    finished = false;
    numOfProcess = lastClock  =
    departure = 0;
}

readyQueue::~readyQueue()
{
    process* current = head;
    while( current != 0 )
    {
        process* next = current->next;
        delete current;
        current = next;
    }
    head = 0;
    helper = 0;
}

int readyQueue::getNumOfProcesses()
{
    return numOfProcess;
}

bool readyQueue::empty()
{
    return head == 0;
}

void readyQueue::insert(process * pro)
{
    if(head == 0)
    {
        head = pro;
        tail = pro;
    }
    else
    {
        process * current = head;
        process * previous = head;
        while(current!= 0)
        {
            previous = current;
            current = current->next;
        }
        current = pro;
        previous->next = current;
        tail = pro;
    }
    numOfProcess++;
}

void readyQueue::remove(eventQueue * eveQueue)
{
    assert(!empty());
    eventQueue::event * newDeparture =
    new eventQueue:: event(2, 60, simClock + head->service);
    newDeparture->waiting = simClock - head->arrivalTime;
    newDeparture->turnA = newDeparture->waiting + head->service;
    eveQueue->schedule_event(newDeparture);
    process * current = head->next;
    delete head;
    head = current;
    numOfProcess--;
}

void readyQueue::removeSRTF(float runThisLong, eventQueue * eveQueue)
{
    assert(!empty());
    int counter = 0;
    float simClockSim = 0;
    bool headDeletion = false;
    while(runThisLong != 0 && !empty())
    {
        process * highest = findHighestSRTF();
        if(highest == 0)  //highest is at the head
        {
            headDeletion = true;
            highest = head;
        }
        if(runThisLong >= highest->timeleft)
        {
            SRTFHelper(highest, eveQueue, simClockSim, counter);
            simClockSim = highest->timeleft;
            runThisLong = runThisLong - highest->timeleft;
            if(headDeletion)
            {
                highest = head->next;
                delete head;
                head = highest;
                numOfProcess--;
            }
            else
            {
                helper->next = highest->next;
                delete highest;
                highest = 0;
                numOfProcess--;
            }
        }
        else
        {
            highest->timeleft = highest->timeleft - runThisLong;
            runThisLong = 0;
        }
        counter++;
    }
}

readyQueue::process* readyQueue::findHighestSRTF()
{
    process * highest = 0;
    process * current = head;
    float shortestTime = current->timeleft;
    while(current->next != 0)
    {
        if(current->next->timeleft < shortestTime)
        {
            shortestTime = current->next->timeleft;
            helper = current;
            highest = current->next;
        }
        current = current->next;
    }
    return highest;
}

void readyQueue::SRTFHelper(process *highest, eventQueue * eveQueue,
                            float simClockSim, int count)
{
    float finish = simClock + highest->timeleft + simClockSim;
    float waiting = (finish - highest->arrivalTime) - highest->service;
    eventQueue::event* departure = new eventQueue::
        event(2, 60, (simClock + highest->timeleft) + simClockSim);
    departure->turnA = (finish  - highest->arrivalTime);
    departure->waiting = waiting;
    eveQueue->schedule_event(departure);
}

void readyQueue::remove_HRRN(eventQueue * eveQueue)
{
    assert(!empty());
    process * highest = findHighestPriority();
    if(highest == 0)
    {
        highest = head;
        HRRNHelper(highest, eveQueue);
        highest = head->next;
        delete head;
        head = highest;
    }
    else
    {
        HRRNHelper(highest, eveQueue);
        helper->next = highest->next;
        delete highest;
        highest = 0;
    }
    numOfProcess--;
}

readyQueue::process* readyQueue::findHighestPriority()
{
    float ratio, high = 0;
    process * current = head;
    helper = 0;
    process * highest = 0;
    high = ((current->service +
             (simClock - current->arrivalTime)) / (current->service));
    while(current->next != 0)
    {
        ratio = ((current->next->service +
                  (simClock - current->next->arrivalTime))
                 / (current->next->service));
        if(ratio > high)
        {
            high = ratio;
            helper = current;
            highest = current->next;
        }
        current = current->next;
    }
    return highest;
}

void readyQueue::HRRNHelper(process* highest, eventQueue * eveQueue)
{
    eventQueue::event * newDeparture =
        new eventQueue:: event(2, 60, simClock + highest->service);
    newDeparture->waiting = (simClock - highest->arrivalTime);
    newDeparture->turnA = newDeparture->waiting + highest->service;
    eveQueue->schedule_event(newDeparture);
}

void readyQueue::removeRoundRobin(float quantum, eventQueue * eveQueue)
{
    assert(!empty());
    process* current = head;
    if(current->timeleft > quantum)
    {
        current->timeleft = current->timeleft - quantum;
        eventQueue::event * newQuantum =
            new eventQueue:: event(3, 1240, simClock + quantum);
        eveQueue->schedule_event(newQuantum);
        if(current->next != 0)
        {
            roundRobinSort(current);
        }
    }
    else
    {
        eventQueue::event * departure =
            new eventQueue:: event(2, 60, simClock + current->timeleft);
        departure->waiting = (simClock + current->timeleft) -
            current->arrivalTime - current->service;
        if(departure->waiting < 0)
        {
            departure->waiting = 0;
        }
        departure->turnA = departure->waiting + current->service;
        eveQueue->schedule_event(departure);
        if(current->next != 0)
        {
            current = head->next;
            delete head;
            head = current;
        }
        else
        {
            delete head;
            head = 0;
            current = 0;
            tail = head;
        }
        numOfProcess--;
    }
}

void readyQueue::roundRobinSort(process * positionMe)
{
    head = head->next;
    tail->next = positionMe;
    positionMe->next = 0;
    tail = positionMe;
}


