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
#include <fstream>
#include <bitset>
#include <cstdlib>
#include "Mmsg_m.h"

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Node : public cSimpleModule
{
protected:
  int totalsent = 0;
  int lengths = 0;
  double time_to_send = 0;

  int senderOrReceiver = 0; // 0: Receiver, 1: Sender
  int ackExpected = 0; int next_frame_to_send = 0;
  int frameExpected = 0; int too_far = 0;
  bool no_nak = 0;
  int lostcount = 0;
  int modifiedcount = 0;
  bool duplicate = false;
  bool modified = false;
  bool loss = false;
  bool delay = false;
  int outputType = 0; // 0: Reading, 1: Transmission of Data, 2: TimeOut, 3: ControlFrame
  int* window = 0;
  int windowCount = 0;
  cMessage** msgPointers = 0;
  int MAX_SEQ = 0;
  int counter = 0;
  int idOfSending = 0;
  virtual void initialize() override;
  virtual void handleMessage(cMessage *msg) override;
  std::vector<std::string> inputMessages;
  std::vector<std::string> inputModifiers;
  std::vector<std::string> senderLogBuffer;

  virtual std::string framingFunc(std::string message);
  virtual std::string deframingFunc(std::string payload);
  virtual std::string modifier(std::string payload, std::string modifierBits,int& modifiedBitNum);
  virtual char randomizer_helper(char c, int& modifiedBitNUmber);
  virtual void logReadLineSender(std::string modifierCode); // 0: Sent, 1: Received
  virtual void logBeforeTrans(double timeAfterProc, std::string sentOrRecieved, int seq_num, std::string payload, std::string trailer, int modifiedBitNum, std::string lost, int dup, double delay);
  virtual void logTimeOut(double timerOftimeout, int seq_num);
  virtual void logControlFrame(double timeAfterProc, std::string sentOrReceived, std::string ackOrNack, int controlFrameNum);
  virtual void readFromFile(int nodeIndex);
  virtual std::bitset<8> generateCheckSum(std::string message);
  virtual void finish();
  virtual bool between(int a, int b, int c); // a: frame_expected, b: frame, c: too_far
};

#endif
