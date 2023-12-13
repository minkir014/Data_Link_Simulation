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

#ifndef __DATALINKSIMULATION_NODE_H_
#define __DATALINKSIMULATION_NODE_H_

#include <omnetpp.h>
#include <vector>
#include <string>

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Node : public cSimpleModule
{
  protected:
    bool duplicate;
    bool modified;
    bool loss;
    bool delay;
    int outputType; // 0: Reading, 1: Transmission of Data, 2: TimeOut, 3: ControlFrame
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    std::vector<std::string> inputMessages;
    std::vector<std::string> inputModifiers;
    virtual std::string framingFunc(std::string message);
    virtual std::string deframingFunc(std::string payload);
    virtual std::string modifier(std::string payload, std::string modifierBits);
    virtual void logEvents(std::string modifierCode, bool sentOrReceived, int seq_num, std::string payload, std::string trailer,
            bool modified, bool lost, int duplicate, double delay, bool AckOrNack); // 0: Sent, 1: Received
                                                                   //0: ACK, 1: NACK

    virtual void readFromFile(int nodeIndex);

};

#endif
