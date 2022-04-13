// main.cpp
//
// ICS 46 Spring 2020
// Project #2: Time Waits for No One
//
// This is the entry point for your simulation application.
//
// All of the code that makes up your simulation -- which is everything
// except for your DoublyLinkedList and Queue implementations -- should
// be in the "app" directory, though, naturally, it shouldn't all be in
// this file.  A design that keeps separate things separate is always
// part of the requirements.
#include <iostream>
#include <vector>
#include <string>
#include "Queue.hpp"
#include <iomanip>

using namespace std;

void multiEnterLine(vector<Queue<int>> &cusInLine, int numOfCus, int timer, int lengthLine, int &entered, int &totalLost)
{
    int i = 0;
    while (i < numOfCus)
    {
        int lineNum = 0;
        int lineLength = cusInLine[0].size();
        int j = 0;
        while(j < cusInLine.size())
        {
            if (cusInLine[j].size() < lineLength)
            {
                lineLength = cusInLine[j].size();
                lineNum = j;
            }
            j++;
        }
        if (lineLength == lengthLine)
        {
            cout << timer << " lost" << endl;
            totalLost++;
        }
        else
        {
            cusInLine[lineNum].enqueue(timer);
            entered++;
            cout << timer << " entered line " << lineNum + 1 << " length " << lineLength + 1 << endl;
        }
        i++;
    }
}

void multiReg(vector<int> &linesTime, vector<int> &currentTime, vector<Queue<int>> &cusInLine, int timer, int &exitReg, int &exitLine, float &totalWaitTime)
{
    int i = 0;
    while (i < linesTime.size())
    {
        if (linesTime[i] == currentTime[i])
        {
            currentTime[i] = -1;
            cout << timer << " exited register " << i + 1 << endl;
            exitReg++;
        }
        if (currentTime[i] == -1 && cusInLine[i].size() != 0)
        {
            cout << timer << " exited line " << i + 1 << " length " << cusInLine[i].size() - 1 << " wait time " << timer - cusInLine[i].front() << endl;
            totalWaitTime += timer - cusInLine[i].front();
            exitLine++;
            cusInLine[i].dequeue();
            cout << timer << " entered register " << i + 1 << endl;
            currentTime[i] = 0;
        }
        i++;
    }
}

void multipleLines(int end, int length, int lengthLine)
{
    int entered = 0;
    int exitReg = 0;
    int exitLine = 0;
    float totalWaitTime = 0;
    vector<int> linesTime(length);
    for (int i = 0; i < length; i++)
    {
        cin >> linesTime[i];
    }

    vector<int> currentTime(length);
    for (int i = 0; i < length; i++)
    {
        currentTime[i] = -1;
    }

    vector<Queue<int>> cusInLine;
    for (int i = 0; i < length; i++)
    {
        Queue<int> place{};
        cusInLine.push_back(place);
    }
    
    int numOfCus = 0;
    int timeOfCus = 0;
    int totalLost = 0;
    
    cin >> numOfCus >> timeOfCus;

    bool endOfFile = false;
    for (int i = 0; i < end; i++)
    {
        if (i == timeOfCus && !endOfFile)
        {
            multiEnterLine(cusInLine, numOfCus, i, lengthLine, entered, totalLost);
            cin >> numOfCus;
            if (numOfCus == 0)
            {
                endOfFile = true;
            }
            else
            {
                cin >> timeOfCus;
            }
        }
        multiReg(linesTime, currentTime, cusInLine, i, exitReg, exitLine, totalWaitTime);
        for (int i = 0; i < currentTime.size(); i++)
        {
            if (currentTime[i] != -1)
            {
                currentTime[i]++;
            }
        }
    }

    cout << end << " end" << endl;

    cout << endl << "STATS" << endl
    << "Entered Line    : " << entered << endl
    << "Exited Line     : " << exitLine << endl
    << "Exited Register : " << exitReg << endl
    << "Avg Wait Time   : " << fixed << setprecision(2) 
    << totalWaitTime / exitLine << endl
    << "Left In Line    : " << entered - exitLine << endl
    << "Left In Register: " << exitLine - exitReg << endl
    << "Lost            : " << totalLost << endl;

}

void singleEnterLine(Queue<int> &cusInLine, int numOfCus, int timer, int lengthLine, int &entered, int &totalLost)
{
    int i = 0;
    while (i < numOfCus)
    {
        if (cusInLine.size() == lengthLine)
        {
            cout << timer << " lost" << endl;
            totalLost++;
        }
        else
        {
            cusInLine.enqueue(timer);
            entered++;
            cout << timer << " entered line " << 1 << " length " << cusInLine.size() + 1 << endl;
        }
        i++;
    }
}

void singleReg(vector<int> &linesTime, vector<int> &currentTime, Queue<int> &cusInLine, int timer, int &exitReg, int &exitLine, float &totalWaitTime)
{
    int i = 0;
    while (i < linesTime.size())
    {
        if (linesTime[i] == currentTime[i])
        {
            currentTime[i] = -1;
            cout << timer << " exited register " << i + 1 << endl;
            exitReg++;
        }
        if (currentTime[i] == -1 && cusInLine.size() != 0)
        {
            cout << timer << " exited line " << 1 << " length " << cusInLine.size() - 1 << " wait time " << timer - cusInLine.front() << endl;
            totalWaitTime += timer - cusInLine.front();
            exitLine++;
            cusInLine.dequeue();
            cout << timer << " entered register " << i + 1 << endl;
            currentTime[i] = 0;
        }
        i++;
    }
}

void singleLine(int end, int length, int lengthLine)
{
    int entered = 0;
    int exitReg = 0;
    int exitLine = 0;
    float totalWaitTime = 0;
    vector<int> linesTime(length);
    for (int i = 0; i < length; i++)
    {
        cin >> linesTime[i];
    }
    
    vector<int> currentTime(length);
    for (int i = 0; i < length; i++)
    {
        currentTime[i] = -1;
    }

    Queue<int> cusInLine{};
    
    int numOfCus = 0;
    int timeOfCus = 0;
    int totalLost = 0;
    
    cin >> numOfCus >> timeOfCus;

    bool endOfFile = false;
    for (int i = 0; i < end; i++)
    {
        if (i == timeOfCus && !endOfFile)
        {
            singleEnterLine(cusInLine, numOfCus, i, lengthLine, entered, totalLost);
            cin >> numOfCus;
            if (numOfCus == 0)
            {
                endOfFile = true;
            }
            else
            {
                cin >> timeOfCus;
            }
        }
        singleReg(linesTime, currentTime, cusInLine, i, exitReg, exitLine, totalWaitTime);
        for (int i = 0; i < currentTime.size(); i++)
        {
            if (currentTime[i] != -1)
            {
                currentTime[i]++;
            }
            
        }
        
        
    }

    cout << end << " end" << endl;

    cout << endl << "STATS" << endl
    << "Entered Line    : " << entered << endl
    << "Exited Line     : " << exitLine << endl
    << "Exited Register : " << exitReg << endl
    << "Avg Wait Time   : " << fixed << setprecision(2) 
    << totalWaitTime / exitLine << endl
    << "Left In Line    : " << entered - exitLine << endl
    << "Left In Register: " << exitLine - exitReg << endl
    << "Lost            : " << totalLost << endl;
}

void startSimulation(int end, int length, int lengthLine, char mode)
{
    cout << "LOG" << endl << "0 start" << endl;
    if (mode == 'M')
    {
        multipleLines(end, length, lengthLine);
    }
    else if (mode == 'S')
    {
        singleLine(end, length, lengthLine);
    }
}



int main()
{
    int end;
    int length;
    int lengthLine;
    char mode;
    cin >> end >> length >> lengthLine >> mode;
    end*=60;

    startSimulation(end, length, lengthLine, mode);
}