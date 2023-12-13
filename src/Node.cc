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

std::string framingFunc(std::string originalMessage)
{
    std::string framedMessage = "#"; // start with the starting flag
    for (char c : originalMessage)
    {
        if (c == '#' || c == '/')
        {
            framedMessage += '/'; // insert escape character before flag or escape character in the message
        }
        framedMessage += c;
    }
    framedMessage += '#'; // end with the ending flag
    return framedMessage;
}

std::string deframingFunc(std::string framedMessage)
{
    std::string originalMessage;
    bool isEscaped = false;
    for (char c : framedMessage)
    {
        if (c == '#' && !isEscaped)
        {
            continue; // skip the flags
        }
        if (c == '/' && !isEscaped)
        {
            isEscaped = true; // next character is escaped
            continue;
        }
        originalMessage += c;
        isEscaped = false; // reset the escape flag
    }
    return originalMessage;
}

std::char noname(std::char c)
{
    bitset<8> chr(c);
    int random = pow(rand(0, 7),2);
    bitset<8> thief(random);
    chr = chr ^ thief;
    int chr_int = (int)(chr.to_ulong());
    char cnew = (char)chr_int;
    return cnew;
}

std::string modifier(std::string payload, std::string modifierBits)
{
    int error = stoi(modifierBits);

    switch (error)
    {
    case 0000:
        return (payload);
        break;

    case 0001:
        delay = true;
        return (paylaod);
        break;

    case 0010:
        duplicate = true;
        return (payload);
        break;

    case 0011:
        delay = true;
        duplicate = true;
        return (payload);
        break;

    case 0100:
        loss = true;
        return ("");
        // no sending
        break;

    case 0101:
        delay = true;
        loss = true;
        return ("");
        // no sending
        break;

    case 0110:
        duplicate = true;
        loss = true;
        return ("");
        // no sending
        break;

    case 0111:
        delay = true;
        duplicate = true;
        loss = true;
        return (payload);
        // no sending
        break;

    case 1000:
        int index = rand(0, payload.size());
        char cnew = noname(payload[index]);
        payload[index] = cnew;
        modified = true;
        return (payload);
        break;

    case 1001:
        int index = rand(0, payload.size());
        char cnew = noname(payload[index]);
        payload[index] = cnew;
        delay = true;
        modified = true;
        return (payload);
        break;
    case 1010:
        int index = rand(0, payload.size());
        char cnew = noname(payload[index]);
        payload[index] = cnew;
        duplicate = true;
        modified = true;
        return (payload);
        break;

    case 1011:
        int index = rand(0, payload.size());
        char cnew = noname(payload[index]);
        payload[index] = cnew;
        delay = true;
        duplicate = true;
        modified = true;
        return (payload);
        break;

    case 1100:
        loss = true;
        modified = true;
        return ("");
        break;

    case 1101:
        delay = true;
        loss = true;
        modified = true;
        return ("");
        break;

    case 1110:
        duplicate = true;
        loss = true;
        modified = true;
        return ("");
        break;

    case 1111:
        delay = true;
        duplicate = true;
        loss = true;
        modified = true;
        return ("");
        break;

    default:
        break;
    }
}

void logEvents(std::string modifierCode, bool sentOrReceived, int seq_num, std::string payload, std::string trailer,
               bool modified, bool lost, int duplicate, double delay, bool AckOrNack)
{
}

void readFromFile(int nodeIndex)
{
    std::string filename = "input" + std::to_string(nodeIndex) + ".txt"; // construct the filename based on the node index
    std::ifstream inputFile(filename);
    if (!inputFile)
    {
        EV << "Cannot open input file: " << filename << "\n";
        return;
    }
    std::string line;
    while (std::getline(inputFile, line))
    {
        inputModifiers.push_back(line.substr(0, 4)); // store the 4-bit binary prefix
        inputMessages.push_back(line.substr(5));     // store the message
    }
    inputFile.close();
}

void Node::initialize()
{
    // TODO - Generated method body
}

void Node::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}
