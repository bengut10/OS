//
//  sim.cpp
//  sim
//
//  Created by Benjamin Gutierrez on 10/4/16.
//  Copyright © 2016 Benjamin Gutierrez. All rights reserved.
//

#include "sim.hpp"
#include <math.h>
#include <stdlib.h>
#include <time.h>

float simClock = 0;

sim::sim()
{
}

sim::~sim()
{
    delete proQueue;
    delete eveQueue;
    proQueue = 0;
    eveQueue = 0;
}

sim::sim(int scheduler, float lambda, float quantum, float inverse)
{
    proQueue = new readyQueue();
    eveQueue = new eventQueue();
    this->inverse = inverse;
    this->scheduler = scheduler;
    this->quantum = quantum;
    this->lambda = lambda;
    arrivalRate = 1 / lambda;
    cpuBusy = false;
    departure = arrival = id = 0;
    idle = nextEvent = lastOn = waiting =
         numOfProcesses = turnAround =
         waitingAccum = turnAccum =0;
}

void sim::init()
{
    cpuBusy = false;
    eventQueue::event * newArrival =
        new eventQueue:: event(1, id, simClock);
    newArrival->service = exp(inverse);;
    id++;
    eveQueue->schedule_event(newArrival);
    eventQueue::event * newArrival2 =
        new eventQueue:: event(1, id, simClock + arrivalRate);
    newArrival2->service = exp(inverse);
    id++;
    eveQueue->schedule_event(newArrival2);
}

void sim::run_sim()
{
    eventQueue::event *test;
    int i = 0;
    while(departure < 10000)
    {
        test = eveQueue->head;
        simClock = test->time;
        switch (test->type)
        {
            case 1:
                process_ArrivalEvent(test);
                numOfProcesses = numOfProcesses + proQueue->getNumOfProcesses();
                break;
            case 2:
                departure++;
                process_DepEvent(test);
                break;
            case 3:
                process_TimeSliceEvent(quantum);
                break;
            default:
                break;
        }
        eveQueue->head = test->next;
        delete test;
        test = 0;
        i = i+1;
    }
    genResults();
}
int sim::process_ArrivalEvent(eventQueue::event *eve)
{
    arrival++;
    if(scheduler == 2)
    {
        if(cpuBusy == false)
        {
            cpuBusy = true;
            idle = idle + (simClock - lastOn);
        }
        readyQueue::process * newProcess =
            new readyQueue:: process(id, simClock, eve->service);
        proQueue->insert(newProcess);
        process_preemption();
    }
    else if(scheduler == 4)
    {
        cpuBusy = true;
        if(proQueue->empty())
        {
            idle = idle + (simClock - lastOn);
            readyQueue::process * newProcess =
                new readyQueue:: process(id, simClock, eve->service);
            proQueue->insert(newProcess);
            process_TimeSliceEvent(quantum);
        }
        else
        {
            readyQueue::process * newProcess =
                new readyQueue:: process(id, simClock, eve->service);
            proQueue->insert(newProcess);
        }
    }
    else
    {
        if(cpuBusy == false )
        {
            cpuBusy = true;
            idle = idle + (simClock - lastOn);
            eventQueue::event * newDeparture =
                new eventQueue::event(2, 60, simClock + eve->service);
            newDeparture->turnA = eve->service;
            eveQueue->schedule_event(newDeparture);
        }
        else
        {
            readyQueue::process * newProcess =
            new readyQueue::process(id, simClock, eve->service);
            proQueue->insert(newProcess);
        }
    }
    eventQueue::event * newArrival =
        new eventQueue:: event(1, id, simClock + (2 * arrivalRate));
    newArrival->service = exp(inverse);
    id++;
    eveQueue->schedule_event(newArrival);
    return 0;
}

int sim::process_DepEvent(eventQueue::event *eve)
{
    waitingAccum = waitingAccum + eve->waiting;
    turnAccum = turnAccum + eve->turnA;
    if(scheduler == 2)
    {
        if(proQueue->empty())
        {
            cpuBusy = false;
            lastOn = simClock;
        }
    }
    else if(scheduler == 4)
    {
        process_TimeSliceEvent(quantum);
    }
    
    else
    {
        if(proQueue->empty())
        {
            cpuBusy = false;
            lastOn = simClock;
        }
        else
        {
            if(scheduler == 1)
            {
                proQueue->remove(eveQueue);
            }
            else
            {
                proQueue->remove_HRRN(eveQueue);
            }
        }
    }
    return 0;
}

int sim::process_preemption()
{
    if(proQueue->empty())
    {
        cpuBusy = false;
        lastOn = simClock;
    }
    else
    {
        proQueue->removeSRTF(arrivalRate, eveQueue);
    }
    return 0;
}

int sim::process_TimeSliceEvent(float quantum)
{
    if(!proQueue->empty())
    {
        proQueue->removeRoundRobin(quantum, eveQueue);
    }
    else
    {
        cpuBusy = false;
        lastOn = simClock;
    }
    return 0;
}
void sim::writeHeader()
{
    std::cout << std::setfill(' ') << "Scheduler" << std::setw(8);
    std::cout << std::setfill(' ') << "Quantum"<< std::setw(10);
    std::cout << std::setfill(' ') << "Lambda"<< std::setw(13);
    std::cout << std::setfill(' ') <<  "Utilization" <<std::setw(13);
    std::cout << std::setfill(' ') << "Throughput"<<std::setw(18);
    std::cout << std::setfill(' ') <<"Avg # in Queue" << std::setw(20);
    std::cout << std::setfill(' ') <<"Avg W.T. in Queue" <<std::setw(20);
    std::cout << std::setfill(' ') <<"Turn Around Time";
    std::cout << std::endl;
    myfile.open("sim.data",
                std::ofstream::out | std::ofstream::app);
    myfile << "Scheduler"<< ",";
    myfile << "Quantum"<< ",";
    myfile << "Lambda"<< ",";
    myfile << "Utilization" << ",";
    myfile << "Throughput"<< ",";
    myfile << "Average Number of processes in the Queue" << ",";
    myfile << "Average Waiting Time in the Queue" << ",";
    myfile << "Turn Around Time";
    myfile << std::endl;
    myfile.close();

}
void sim::genResults()
{
    std::cout << std::setfill(' ') << scheduler<< std::setw(13);
    std::cout << std::setfill(' ') << quantum << std::setw(9);
    std::cout << std::setfill(' ') << lambda << std::setw(16);
    std::cout << std::setfill(' ') << 1 - (idle /simClock) << std::setw(13);
    std::cout << std::setfill(' ') << 10000/simClock << std::setw(16);
    std::cout << std::setfill(' ') << numOfProcesses/arrival << std::setw(20);
    std::cout << std::setfill(' ') << waitingAccum / 10000 << std::setw(20);
    std::cout << std::setfill(' ') << turnAccum / 10000;
    std::cout << std::endl;
    myfile.open("sim.data",
        std::ofstream::out | std::ofstream::app);
    myfile << scheduler<< " ,";
    myfile << quantum << " ,";
    myfile << lambda << " ,";
    myfile << 1 - (idle /simClock) << " ,";
    myfile << 10000/simClock << " ,";
    myfile << numOfProcesses/arrival << " ,";
    myfile << waitingAccum / 10000 << " ,";
    myfile << turnAccum / 10000;
    myfile << std::endl;
    myfile.close();
}

float sim::urand()
{
    return ((float) rand()) / RAND_MAX;
}

float sim::exp(float lambda)
{
    float u,x;
    x = 0;
    while(x == 0)
    {
        u = urand();
        x = (-1/lambda) * log(u);
    }
    return x;
}
