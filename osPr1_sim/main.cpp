//  Created by Benjamin Gutierrez on 10/4/16.
//  Copyright © 2016 Benjamin Gutierrez. All rights reserved.
//

#include <iostream>
#include "sim.hpp"
#include <math.h>

int displayMenu()
{
    int choice;
    std::cout << "Select Scheduler to Run " << std::endl;
    std::cout << "1 - First Come First Serve " << std::endl;
    std::cout << "2 - Shortest Job First: " << std::endl;
    std::cout << "3 - HRRN" << std::endl;
    std::cout << "4 - Round Robin: " << std::endl;
    std::cout << "5 - Exit" << std::endl;
    std::cin >> choice;
    return choice;
}

int getStartingArrival()
{
    int arrivalStart;
    std::cout <<"Enter the number of the arrivals (per second)"
                        << "range you want to start the test with: " << std::endl;
    std::cin >> arrivalStart;
    return arrivalStart;
}

int getEndingArrival()
{
    int arrivalEnd;
    std::cout <<"Enter the number of the arrivals (per second)"
                        <<"range you want to end the test with: " << std::endl;
    std::cin >> arrivalEnd;
    return arrivalEnd;
}

float getArrivalRate()
{
    float arrivalRate;
    std::cout <<"Enter the average arrival"
        "rate (per second) for the test: " << std::endl;
    std::cin >> arrivalRate;
    return arrivalRate;
}

float getQuantum()
{
    float quantum;
    std::cout <<"Enter the quantum time: " << std::endl;
    std::cin >> quantum;
    return quantum;
    
}

void runSim(int scheduler, int arrival, int end,
            float arrivalRate, float quantum)
{
    end = end+1;
    while(arrival < end)
    {
        simClock = 0;
        sim * obj;
        obj = new sim(scheduler, arrival, quantum, (1/arrivalRate));
        obj->init();
        obj->run_sim();
        delete obj;
        obj = 0;
        arrival++;
    }
}

bool endProgram()
{
    int choice = 0;
    std::cout << "Run Again?" <<std::endl;
    std::cout << "1: YES " << std::endl;
    std::cout << "2: NO " << std::endl;
    std::cin  >> choice;
    switch (choice)
    {
        case 1:
            return true;
        case 2:
            return false;
        default:
            std::cout << "Invalid Choice " << std::endl;
            break;
    }
    return false;
}


int main(int argc, const char * argv[]) {
    
    int choice = 0;
    int arrivalStart;
    int arrivalEnd;
    float arrivalRate;
    float quantum = 0;
    sim *s1 = new sim();
    
    
    do
    {
        arrivalStart = getStartingArrival();
        arrivalEnd = getEndingArrival();
        arrivalRate = getArrivalRate();
        int choice = displayMenu();
        switch (choice)
        {
            case 1:
                s1->writeHeader();
                runSim(1, arrivalStart, arrivalEnd, arrivalRate, quantum);
                if(!endProgram())
                {
                    return 0;
                }
                break;
            case 2:
                s1->writeHeader();
                runSim(2, arrivalStart, arrivalEnd, arrivalRate, quantum);
                if(!endProgram())
                {
                    return 0;
                }
                break;
            case 3:
                s1->writeHeader();
                runSim(3, arrivalStart, arrivalEnd, arrivalRate, quantum);
                if(!endProgram())
                {
                    return 0;
                }
                break;
            case 4:
                quantum = getQuantum();
                s1->writeHeader();
		runSim(4, arrivalStart, arrivalEnd, arrivalRate, quantum);
                if(!endProgram())
                {
                    return 0;
                }
                break;
            case 5:
                return 0;
            default:
                std::cout << "**************Invalid Choice**********" << std:: endl;
                break;
        }
    }
    while(choice != 5);
    return 0;
}

