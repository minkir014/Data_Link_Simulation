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

#include "Node.h"

Define_Module(Node);

std::string framingFunc(std::string message) {

}

std::string deframingFunc(std::string payload) {

}

std::string modifier(std::string payload, std::string modifierBits) {

}

void logEvents(std::string modifierCode, bool sentOrReceived, int seq_num, std::string payload, std::string trailer,
            bool modified, bool lost, int duplicate, double delay, bool AckOrNack) {

}

void readFromFile() {

}

void Node::initialize()
{
    // TODO - Generated method body

}

void Node::handleMessage(cMessage *msg)
{
    // TODO - Generated method body

}
