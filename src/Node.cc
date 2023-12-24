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

std::string Node::framingFunc(std::string originalMessage)
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

std::string Node::deframingFunc(std::string framedMessage)
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

char Node::randomizer_helper(char c)
{
    std::bitset<8> chr(c);
    int random = pow(rand() % 8, 2); // why the pow of 2?
    std::bitset<8> thief(random);
    chr = chr ^ thief;
    int chr_int = (int)(chr.to_ulong());
    char cnew = (char)chr_int;
    return cnew;
}

std::string Node::modifier(std::string payload, std::string modifierBits)
{
    loss = false;
    modified = false;
    delay = false;
    duplicate = false;
    int error = stoi(modifierBits);
    int index;
    char cnew;

    switch (error)
    {
    case 0:
        return (payload);
        break;

    case 1:
        delay = true;
        return (payload);
        break;

    case 10:
        duplicate = true;
        return (payload);
        break;

    case 11:
        delay = true;
        duplicate = true;
        return (payload);
        break;

    case 100:
        lostcount++;
        loss = true;
        return ("");
        // no sending
        break;

    case 101:
        lostcount++;
        delay = true;
        loss = true;
        return ("");
        // no sending
        break;

    case 110:
        lostcount++;
        duplicate = true;
        loss = true;
        return ("");
        // no sending
        break;

    case 111:
        lostcount++;
        delay = true;
        duplicate = true;
        loss = true;
        return (payload);
        // no sending
        break;

    case 1000:
        modifiedcount++;
        index = rand() % payload.size(); // to be reviewed
        cnew = randomizer_helper(payload[index]);
        payload[index] = cnew;
        modified = true;
        return (payload);
        break;

    case 1001:
        modifiedcount++;
        index = rand() % payload.size();
        cnew = randomizer_helper(payload[index]);
        payload[index] = cnew;
        delay = true;
        modified = true;
        return (payload);
        break;

    case 1010:
        modifiedcount++;
        index = rand() % payload.size();
        cnew = randomizer_helper(payload[index]);
        payload[index] = cnew;
        duplicate = true;
        modified = true;
        return (payload);
        break;

    case 1011:
        modifiedcount++;
        index = rand() % payload.size();
        cnew = randomizer_helper(payload[index]);
        payload[index] = cnew;
        delay = true;
        duplicate = true;
        modified = true;
        return (payload);
        break;

    case 1100:
        modifiedcount++;
        lostcount++;
        loss = true;
        modified = true;
        return ("");
        break;

    case 1101:
        modifiedcount++;
        lostcount++;
        delay = true;
        loss = true;
        modified = true;
        return ("");
        break;

    case 1110:
        modifiedcount++;
        lostcount++;
        duplicate = true;
        loss = true;
        modified = true;
        return ("");
        break;

    case 1111:
        modifiedcount++;
        lostcount++;
        delay = true;
        duplicate = true;
        loss = true;
        modified = true;
        return ("");
        break;

    default: // should return something at default case except if it is guarenteed not to be reached
        break;
    }
}

void Node::logEvents(std::string modifierCode, bool sentOrReceived, int seq_num,
                     std::string payload, std::string trailer, bool modified, bool lost,
                     int duplicate, double delay, bool AckOrNack)
{
}

void Node::readFromFile(int nodeIndex)
{
    std::string filename = "input" + std::to_string(nodeIndex) + ".txt"; // construct the filename based on the node index
    std::ifstream inputFile(filename);
    if (!inputFile)
    {
        EV << "Cannot open input file: " << filename << "\n";
        return;
    }
    std::string line;
    std::cout << "We don't know\n";
    while (std::getline(inputFile, line))
    {
        inputModifiers.push_back(line.substr(0, 4)); // store the 4-bit binary prefix
        inputMessages.push_back(line.substr(5));     // store the message
    }
    inputFile.close();
}

std::bitset<8> Node::generateCheckSum(std::string message)
{
    // EV << message << endl;
    std::bitset<8> checkSum(0);
    int message_size = message.length();
    for (int i = 0; i < message_size; i++)
    {
        std::bitset<8> character(message[i]);
        checkSum ^= character;
    }

    return checkSum;
}

bool Node::between(int a, int b, int c)
{
    return (a <= b && b < c) || (c < a && a <= b) || (b < c && c < a);
}

void Node::initialize()
{
    // TODO - Generated method body
    if (strcmp(getName(), "Node0") == 0)
        readFromFile(0);
    else
        readFromFile(1);
}

void Node::handleMessage(cMessage *msg)
{
    bool sender = false;
    bool retransmit = false;
    // TODO - Generated method body
    if (strcmp(msg->getName(), "start") == 0)
    {
        window = new int[par("WindowSender").intValue()];
        msgPointers = new cMessage *[par("WindowSender").intValue()];
        MAX_SEQ = 2 * par("WindowSender").intValue() - 1;
        sender = true;
        counter = 0;
        ackExpected = 0;
        next_frame_to_send = 0;
        windowCount = 0;
    }
    else if (strcmp(msg->getName(), "Nostart") == 0)
    {
        window = new int[par("WindowReceiver").intValue()];
        for (int i = 0; i < par("WindowReceiver").intValue(); i++)
        {
            window[i] = 0;
        }
        MAX_SEQ = 2 * par("WindowReceiver").intValue() - 1;
        frameExpected = 0;
        too_far = par("WindowReceiver").intValue();
        no_nak = false;
    }
    else if (strcmp(msg->getName(), "Process") == 0)
    {
        sender = true;
    }
    else if (strcmp(msg->getName(), "To Send") == 0)
    {
        Mmsg_Base *scheduled = check_and_cast<Mmsg_Base *>(msg);
        if (scheduled->getFrameType() == 0)
            scheduled->setName("Message");
        else if (scheduled->getFrameType() == 1)
        {
            std::string idk = "Ack " + std::to_string(scheduled->getAckNum());
            scheduled->setName(idk.c_str());
        }
        else if (scheduled->getFrameType() == 2)
        {
            std::string idk = "Nack " + std::to_string(scheduled->getAckNum());
            scheduled->setName(idk.c_str());
        }
        send(scheduled, "out");
    }
    else if (strcmp(msg->getName(), "Message") == 0)
    {
        Mmsg_Base *received = check_and_cast<Mmsg_Base *>(msg);
        simtime_t delayNum = par("ProcessingTime").doubleValue() + par("TransmissionDelay").doubleValue();
        std::string deframedPayload = deframingFunc(received->getPayload());
        std::bitset<8> checksum = generateCheckSum(deframedPayload);
        if (checksum.to_ulong() == received->getChecksum().to_ulong())
        {
            if (received->getSN() == frameExpected)
            {
                frameExpected++;
                frameExpected %= MAX_SEQ;
                too_far++;
                too_far %= MAX_SEQ;
                while (window[frameExpected % par("WindowReceiver").intValue()] == 1)
                {
                    frameExpected++;
                    frameExpected %= MAX_SEQ;
                    too_far++;
                    too_far %= MAX_SEQ;
                    window[received->getSN() % par("WindowReceiver").intValue()] = 0;
                }
                received->setName("To Send");
                received->setAckNum(frameExpected);
                received->setFrameType(1);
                scheduleAt(simTime() + delayNum, received);
                no_nak = false;
                return;
            }
            else if (between(frameExpected, received->getSN(), too_far))
            {
                window[received->getSN() % par("WindowReceiver").intValue()] = 1;
            }
            else
                return;
        }
        if (!no_nak)
        {
            received->setName("To Send");
            received->setAckNum(frameExpected);
            received->setFrameType(2);
            no_nak = true;
            scheduleAt(simTime() + delayNum, received);
        }
    }
    else if (strcmp(msg->getName(), "TimeOut") == 0) {
        Mmsg_Base *controlFrame = check_and_cast<Mmsg_Base *>(msg);
        controlFrame->setAckNum(controlFrame->getAckNum());
        msg = controlFrame;
        retransmit = true;
        sender = true;
    }
    else
    {
        Mmsg_Base *controlFrame = check_and_cast<Mmsg_Base *>(msg);
        if (controlFrame->getFrameType() == 1)
        {
            while (between(ackExpected, controlFrame->getAckNum() - 1, next_frame_to_send)) {
                cancelEvent(msgPointers[ackExpected]);
                ackExpected++;
                ackExpected %= MAX_SEQ;
                windowCount--;
                if (counter != inputMessages.size()) sender = true;
            }
        }
        else if (controlFrame->getFrameType() == 2 && ackExpected != inputMessages.size())
            // NACKs never get out of range. Check.
        {
            retransmit = true;
            sender = true;
        }
    }

    if (sender)
    {
        std::string modifiedPayload;
        int tempInt;
        if (retransmit) {
            Mmsg_Base *controlFrame = check_and_cast<Mmsg_Base *>(msg);
            tempInt = controlFrame->getAckNum();
            modifiedPayload = inputMessages[window[tempInt]];
        }
        else {
            tempInt = counter;
            modifiedPayload = modifier(inputMessages[tempInt],
                                               inputModifiers[tempInt]);
        }
        std::string stuffedPayload = framingFunc(modifiedPayload);
        Mmsg_Base *sentMsg = new Mmsg_Base("To Send");
        sentMsg->setPayload(stuffedPayload.c_str());
        sentMsg->setSN(tempInt % MAX_SEQ);
        sentMsg->setFrameType(0);
        sentMsg->setChecksum(generateCheckSum(inputMessages[tempInt]));

        double sentNumber = par("ProcessingTime").doubleValue() + par("TransmissionDelay").doubleValue();

        if (retransmit) {
            scheduleAt(simTime() + sentNumber, sentMsg);
            return;
        }

        if (loss)
        {
        }
        else
        {
            if (delay)
            {
                double delayNumber = par("ErrorDelay").doubleValue();
                sentNumber = sentNumber + delayNumber;
                scheduleAt(simTime() + sentNumber, sentMsg);
            }
            else
                scheduleAt(simTime() + sentNumber, sentMsg);

            if (duplicate)
            {
                double duplicateNumber = par("DuplicationDelay").doubleValue();
                sentNumber = sentNumber + duplicateNumber;
                std::cout << "Duplicated\n"
                          << simTime() + sentNumber;
                Mmsg_Base *sentMsg_dup = sentMsg->dup();
                scheduleAt(simTime() + sentNumber, sentMsg_dup);
            }
        }

        Mmsg_Base *sentTimer = new Mmsg_Base("TimeOut");
        sentTimer->setSN(counter % MAX_SEQ);
        window[counter % par("WindowSender").intValue()] = counter;
        msgPointers[counter % par("WindowSender").intValue()] = sentTimer;

        simtime_t timeOutNumber = par("Timeout").intValue();
        scheduleAt(simTime() + timeOutNumber, sentTimer);

        next_frame_to_send++;
        next_frame_to_send %= MAX_SEQ;
        counter++;
        windowCount++;
        if (windowCount != par("WindowSender").intValue() && counter != inputMessages.size())
        {
            Mmsg_Base *Processing = new Mmsg_Base("Process");
            simtime_t processingTime = par("ProcessingTime").doubleValue();
            scheduleAt(simTime() + processingTime,
                       Processing);
        }
    }
}
