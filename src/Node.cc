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

char Node::randomizer_helper(char c, int& modifiedBitNumber)
{
    std::bitset<8> chr(c);
    modifiedBitNumber = rand() % 8;
    int random = pow(2, 7 - modifiedBitNumber);
    std::bitset<8> thief(random);
    chr = chr ^ thief;
    int chr_int = (int)(chr.to_ulong());
    char cnew = (char)chr_int;
    return cnew;
}

std::string Node::modifier(std::string payload, std::string modifierBits, int &modifiedBitNum)
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
        return payload;
        // no sending
        break;

    case 101:
        lostcount++;
        delay = true;
        loss = true;
        return payload;
        // no sending
        break;

    case 110:
        lostcount++;
        duplicate = true;
        loss = true;
        return payload;
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
        cnew = randomizer_helper(payload[index], modifiedBitNum);
        payload[index] = cnew;
        modifiedBitNum += index * 8;
        modified = true;
        return (payload);
        break;

    case 1001:
        modifiedcount++;
        index = rand() % payload.size();
        cnew = randomizer_helper(payload[index], modifiedBitNum);
        payload[index] = cnew;
        modifiedBitNum += index * 8;
        delay = true;
        modified = true;
        return (payload);
        break;

    case 1010:
        modifiedcount++;
        index = rand() % payload.size();
        cnew = randomizer_helper(payload[index], modifiedBitNum);
        modifiedBitNum += index * 8;
        payload[index] = cnew;
        duplicate = true;
        modified = true;
        return (payload);
        break;

    case 1011:
        modifiedcount++;
        index = rand() % payload.size();
        cnew = randomizer_helper(payload[index], modifiedBitNum);
        modifiedBitNum += index * 8;
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
        return payload;
        break;

    case 1101:
        modifiedcount++;
        lostcount++;
        delay = true;
        loss = true;
        modified = true;
        return payload;
        break;

    case 1110:
        modifiedcount++;
        lostcount++;
        duplicate = true;
        loss = true;
        modified = true;
        return payload;
        break;

    case 1111:
        modifiedcount++;
        lostcount++;
        delay = true;
        duplicate = true;
        loss = true;
        modified = true;
        return payload;
        break;

    default: // should return something at default case except if it is guarenteed not to be reached
        break;
    }
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
// 5 -1  0
bool Node::between(int a, int b, int c)
{
    return (a <= b && b < c) || (c < a && a <= b) || (b < c && c < a);
}

void Node::logReadLineSender(std::string modifierCode)
{
    std::ofstream outputFile;
    outputFile.open("output.txt", std::ios_base::app);
    outputFile << "At time " << simTime() << ", Node" << idOfSending << ", Introducing channel error with code = " << modifierCode << ".\n";
    outputFile.close();
}

void Node::logBeforeTrans(double timeAfterProc, std::string sentOrRecieved, int seq_num, std::string payload, std::string trailer, int modifiedBitNum, std::string lost, int dup, double delay)
{
    std::ofstream outputFile;
    outputFile.open("output.txt", std::ios_base::app);
    outputFile << "At time [" << timeAfterProc << "], Node[" << idOfSending << "] " << sentOrRecieved
               << " frame with seq_num = [" << seq_num << "], and payload = [" << payload << "], and trailer = ["
               << trailer << "], Modified[" << modifiedBitNum << "], Lost[" << lost << "], Duplicate[" << dup << "], Delay = [" << delay << "].\n";
    outputFile.close();
}

void Node::logTimeOut(double timerOftimeout, int seq_num)
{
    std::ofstream outputFile;
    outputFile.open("output.txt", std::ios_base::app);
    outputFile << "Timeout event at time [" << timerOftimeout << "], at Node[" << idOfSending << "], for frame with seq_num = " << seq_num << ".\n";
    outputFile.close();
}

void Node::logControlFrame(double timeAfterProc, std::string sentOrReceived,
        std::string ackOrNack, int controlFrameNum)
{
    std::ofstream outputFile;
    outputFile.open("output.txt", std::ios_base::app);
    outputFile << "At time [" << timeAfterProc << "], Node[" << idOfSending <<
            "]," << sentOrReceived << " [" << ackOrNack << "], with number[" << controlFrameNum << "], Loss[No].\n";
    outputFile.close();
}

void Node::initialize()
{
    // TODO - Generated method body
    if (strcmp(getName(), "Node0") == 0)
    {
        readFromFile(0);
        idOfSending = 0;
    }
    else
    {
        readFromFile(1);
        idOfSending = 1;
    }
}

void Node::handleMessage(cMessage *msg)
{
    int bitToBeModified = -1;
    bool sender = false;
    bool retransmit = false;

    // TODO - Generated method body
    if (strcmp(msg->getName(), "start") == 0)
    {
        window = new int[par("WindowSender").intValue()];
        msgPointers = new cMessage *[par("WindowSender").intValue()];
        MAX_SEQ = 2 * par("WindowSender").intValue();
        sender = true;
        counter = 0;
        std::cout << "Messages Count: " << inputMessages.size() << endl;
        ackExpected = 0;
        next_frame_to_send = 0;
        windowCount = 0;
        senderOrReceiver = 1;
        std::ofstream outputFile;
        outputFile.open("output.txt", std::ios_base::trunc);
        outputFile.close();
        delete msg;
    }
    else if (strcmp(msg->getName(), "Nostart") == 0)
    {
        window = new int[par("WindowReceiver").intValue()];
        for (int i = 0; i < par("WindowReceiver").intValue(); i++)
        {
            window[i] = 0;
        }
        MAX_SEQ = 2 * par("WindowReceiver").intValue();
        frameExpected = 0;
        too_far = par("WindowReceiver").intValue();
        no_nak = false;
        delete msg;
    }
    else if (strcmp(msg->getName(), "Process") == 0)
    {
        sender = true;
    }
    else if (strcmp(msg->getName(), "To Send") == 0)
    {
        Mmsg_Base *scheduled = check_and_cast<Mmsg_Base *>(msg);
        if (scheduled->getFrameType() == 0) {
            scheduled->setName("Message");
            totalsent += 1;
            lengths += std::string(scheduled->getPayload()).size() * 8;
        }
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
        Mmsg_Base *log = new Mmsg_Base("");
        Mmsg_Base *received = check_and_cast<Mmsg_Base *>(msg);
        simtime_t delayNum = par("ProcessingTime").doubleValue() + par("TransmissionDelay").doubleValue();
        std::string deframedPayload = deframingFunc(received->getPayload());
        std::bitset<8> checksum = generateCheckSum(deframedPayload);
        std::cout << "Payload: " << received->getPayload() << endl;

        int modBit = -1;
        if (received->getModifiedBitNumber() != -1)
            modBit = 1;

        logBeforeTrans(simTime().dbl(), "received", received->getSN(), deframedPayload, received->getChecksum().to_string(),
                       modBit, "No", 0, 0);
        if (received->getSN() == frameExpected)
        {
            if (checksum.to_ulong() == received->getChecksum().to_ulong())
            {
                log->setAckNum(received->getSN() + 1);
                log->setName("ACK_Log");

                frameExpected++;
                frameExpected %= MAX_SEQ;
                too_far++;
                too_far %= MAX_SEQ;
                while (window[frameExpected % par("WindowReceiver").intValue()] == 1)
                {
                    window[frameExpected % par("WindowReceiver").intValue()] = 0;
                    frameExpected++;
                    frameExpected %= MAX_SEQ;
                    too_far++;
                    too_far %= MAX_SEQ;
                }
                received->setName("To Send");
                received->setAckNum(frameExpected);
                received->setFrameType(1);
                scheduleAt(simTime() + delayNum, received);
                no_nak = false;
                std::cout << "I sent some ack at " << simTime().dbl() << endl;
                scheduleAt(simTime() + par("ProcessingTime").doubleValue(), log);
                return;
            }
        }
        else if (between(frameExpected, received->getSN(), too_far))
        {
            if (checksum.to_ulong() == received->getChecksum().to_ulong())
                window[received->getSN() % par("WindowReceiver").intValue()] = 1;
        }
        else {
            delete msg;
            return;
        }

        if (!no_nak)
        {
            log->setAckNum(received->getSN());
            log->setName("NACK_Log");
            received->setName("To Send");
            std::cout << "I sent some nack at " << simTime().dbl() << endl;
            received->setAckNum(frameExpected);
            received->setFrameType(2);
            no_nak = true;
            scheduleAt(simTime() + par("ProcessingTime").doubleValue(), log);
            scheduleAt(simTime() + delayNum, received);
        }
    }
    else if (strcmp(msg->getName(), "TimeOut") == 0)
    {
        Mmsg_Base *controlFrame = check_and_cast<Mmsg_Base *>(msg);
        controlFrame->setAckNum(controlFrame->getSN());
        msg = controlFrame->dup();
        retransmit = true;
        sender = true;
        logTimeOut(simTime().dbl(), controlFrame->getSN());
    }
    else if (strcmp(msg->getName(), "Log") == 0)
    {

        Mmsg_Base *logPointer = check_and_cast<Mmsg_Base *>(msg);
        std::string lossOfPacket = "No";
        double delayedPacket = 0;

        if (logPointer->getInputModifiers(2))
            lossOfPacket = "Yes";

        if (logPointer->getInputModifiers(0))
            delayedPacket = par("ErrorDelay").doubleValue();

        logBeforeTrans(simTime().dbl(), "sent", logPointer->getSN(), logPointer->getPayload(),
                       logPointer->getChecksum().to_string(), logPointer->getModifiedBitNumber(),
                       lossOfPacket, logPointer->getDupCount(), delayedPacket);

        delete logPointer;
    }
    else if (strcmp(msg->getName(), "ACK_Log") == 0)
    {
        Mmsg_Base *logPointer = check_and_cast<Mmsg_Base *>(msg);
        logControlFrame(simTime().dbl(), "sent", "ACK", logPointer->getAckNum());
        delete logPointer;
    }
    else if (strcmp(msg->getName(), "NACK_Log") == 0)
    {
        Mmsg_Base *logPointer = check_and_cast<Mmsg_Base *>(msg);
        logControlFrame(simTime().dbl(), "sent", "NACK", logPointer->getAckNum());
        delete logPointer;
    }
    else
    {

        Mmsg_Base *controlFrame = check_and_cast<Mmsg_Base *>(msg);

        if (controlFrame->getFrameType() == 1)
        {
            logControlFrame(simTime().dbl(), "received", "ACK", controlFrame->getAckNum());
            while (between(ackExpected, controlFrame->getAckNum() - 1, next_frame_to_send))
            {
                time_to_send += simTime().dbl();
                cancelEvent(msgPointers[ackExpected % par("WindowSender").intValue()]);
                delete msgPointers[ackExpected % par("WindowSender").intValue()];
                ackExpected++;
                ackExpected %= MAX_SEQ;
                windowCount--;
                if (counter != inputMessages.size())
                    sender = true;
            }
            delete msg;
        }
        else if (controlFrame->getFrameType() == 2 && ackExpected != inputMessages.size())
        // NACKs never get out of range. Check.
        {
            logControlFrame(simTime().dbl(), "received", "NACK", controlFrame->getAckNum());
            retransmit = true;
            sender = true;
        }
    }

    if (sender)
    {

        std::string modifiedPayload;
        int tempInt;
        Mmsg_Base *sentMsg = new Mmsg_Base("To Send");
        if (retransmit)
        {
            Mmsg_Base *controlFrame = check_and_cast<Mmsg_Base *>(msg);
            tempInt = window[controlFrame->getAckNum() % par("WindowSender").intValue()];
            delete controlFrame;
            modifiedPayload = inputMessages[tempInt];
            logReadLineSender("0000");
            sentMsg->setInputModifiers(0, 0);
            sentMsg->setInputModifiers(1, 0);
            sentMsg->setInputModifiers(2, 0);
            sentMsg->setInputModifiers(3, 0);
            sentMsg->setDupCount(0);
        }
        else
        {

            tempInt = counter;
            modifiedPayload = modifier(inputMessages[tempInt],
                                       inputModifiers[tempInt], bitToBeModified);
            logReadLineSender(inputModifiers[tempInt]);
            sentMsg->setInputModifiers(3, modified);
            sentMsg->setInputModifiers(2, loss);
            sentMsg->setInputModifiers(1, duplicate);
            sentMsg->setInputModifiers(0, delay);
            sentMsg->setDupCount(duplicate);
        }

        std::string stuffedPayload = framingFunc(modifiedPayload);
        sentMsg->setPayload(stuffedPayload.c_str());
        sentMsg->setSN(tempInt % MAX_SEQ);
        sentMsg->setFrameType(0);
        sentMsg->setChecksum(generateCheckSum(inputMessages[tempInt]));
        sentMsg->setModifiedBitNumber(bitToBeModified);

        double sentNumber = par("ProcessingTime").doubleValue() + par("TransmissionDelay").doubleValue();

        Mmsg_Base *logmsg = sentMsg->dup();
        logmsg->setName("Log");
        scheduleAt(simTime() + par("ProcessingTime").doubleValue(), logmsg);

        if (retransmit)
        {

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
                std::cout << "Delay Sum: " << simTime().dbl() << " + " << sentNumber << " = " << (simTime() + sentNumber).dbl() << endl;
                scheduleAt(simTime() + sentNumber, sentMsg);
            }
            else
                scheduleAt(simTime() + sentNumber, sentMsg);

            if (duplicate)
            {
                Mmsg_Base *duplogmsg = sentMsg->dup();
                duplogmsg->setName("Log");
                duplogmsg->setDupCount(2);

                double duplicateNumber = par("DuplicationDelay").doubleValue();
                scheduleAt(simTime() + par("ProcessingTime").doubleValue() + duplicateNumber, duplogmsg);
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

        time_to_send -= simTime().dbl();
        next_frame_to_send++;
        next_frame_to_send %= MAX_SEQ;
        counter++;
        windowCount++;
        std::cout << "I sent: " << sentMsg->getPayload() << "@ " << simTime() << endl;
        if (windowCount != par("WindowSender").intValue() && counter != inputMessages.size())
        {
            Mmsg_Base *Processing = new Mmsg_Base("Process");
            simtime_t processingTime = par("ProcessingTime").doubleValue();
            scheduleAt(simTime() + processingTime,
                       Processing);
        }
    }
}

void Node::finish()
{
    if(totalsent != 0)
    {
        std::cout << "All messages have been successfully processed." << endl;
        std::cout << "Loss Rate = " << (lostcount*1.000) / totalsent << endl;
        std::cout << "Modifications Rate = " << (modifiedcount*1.000) / totalsent << endl;
        std::cout << "Channel Efficiency = " << 1 - (double)inputMessages.size() / totalsent << endl;
        std::cout << "Average BW = " << (lengths*1.000) / totalsent << endl;
        std::cout << "Average time taken to send message = " << time_to_send/totalsent << endl;
    }
}
