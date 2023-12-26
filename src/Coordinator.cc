//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Coordinator.h"
#include <string>
#include <string>
#include <fstream>
#include <sstream>
using namespace std ;
Define_Module(Coordinator);

void Coordinator::initialize()
{
    // TODO - Generated method body

    // assuming coordinator.txt contains a single line of the form: [1 5] 1->nodeId, 5->startTime
    std::ifstream inputFile("coordinator.txt");
    if (!inputFile)
    {
        EV << "Cannot open input file: coordinator.txt" << "\n";
        return;
    }
    std::string line;

    std::getline(inputFile, line);

    cout << line << endl;


    std::istringstream iss(line);
    char dum;
    int nodeId =0 ;
    double startTime= 1; // random intialize


    if (!(iss >> nodeId >> startTime))
    {
        cout << "Error parsing the line no msg is send " << endl;
        EV << "Error parsing line: " << line << "\n";
        return ;
    }

    cout <<"coordinator reads : " <<  nodeId << " " << startTime << endl;
    // craete a message
    cMessage *msg = new cMessage(std::to_string(nodeId).c_str());
    // scheduale the message to the node
    scheduleAt((simtime_t)startTime, msg);

    inputFile.close();

}

void Coordinator::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    // send the message to the node
    cMessage * start = new cMessage("start");
    cMessage * Nostart = new cMessage("Nostart");
    if(string(msg->getName())== "0"){
        send(start, "out0");
        send(Nostart, "out1");
    } else {
        send(start, "out1");
        send(Nostart, "out0");
    }


}
