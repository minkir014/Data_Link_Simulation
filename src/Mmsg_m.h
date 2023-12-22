//
// Generated file, do not edit! Created by opp_msgtool 6.0 from Mmsg.msg.
//

#ifndef __MMSG_M_H
#define __MMSG_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// opp_msgtool version check
#define MSGC_VERSION 0x0600
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgtool: 'make clean' should help.
#endif

class Mmsg;
// cplusplus {{
#include <bitset>
typedef  std::bitset<8> bits;
// }}

/**
 * Class generated from <tt>Mmsg.msg:25</tt> by opp_msgtool.
 * <pre>
 * packet Mmsg
 * {
 *     \@customize(true);  // see the generated C++ header for more info
 *     int SN;
 *     int frameType;
 *     int ackNum;
 *     string payload;
 *     bits checksum;
 * }
 * </pre>
 *
 * Mmsg_Base is only useful if it gets subclassed, and Mmsg is derived from it.
 * The minimum code to be written for Mmsg is the following:
 *
 * <pre>
 * class Mmsg : public Mmsg_Base
 * {
 *   private:
 *     void copy(const Mmsg& other) { ... }

 *   public:
 *     Mmsg(const char *name=nullptr, short kind=0) : Mmsg_Base(name,kind) {}
 *     Mmsg(const Mmsg& other) : Mmsg_Base(other) {copy(other);}
 *     Mmsg& operator=(const Mmsg& other) {if (this==&other) return *this; Mmsg_Base::operator=(other); copy(other); return *this;}
 *     virtual Mmsg *dup() const override {return new Mmsg(*this);}
 *     // ADD CODE HERE to redefine and implement pure virtual functions from Mmsg_Base
 * };
 * </pre>
 *
 * The following should go into a .cc (.cpp) file:
 *
 * <pre>
 * Register_Class(Mmsg)
 * </pre>
 */
class Mmsg_Base : public ::omnetpp::cPacket
{
  protected:
    int SN = 0;
    int frameType = 0;
    int ackNum = 0;
    omnetpp::opp_string payload;
    bits checksum;

  private:
    void copy(const Mmsg_Base& other);

  protected:
    bool operator==(const Mmsg_Base&) = delete;
    // make constructors protected to avoid instantiation
    Mmsg_Base(const Mmsg_Base& other);
    // make assignment operator protected to force the user override it
    Mmsg_Base& operator=(const Mmsg_Base& other);

  public:
    Mmsg_Base(const char *name=nullptr, short kind=0);
    virtual ~Mmsg_Base();
    virtual Mmsg_Base *dup() const override {return new Mmsg_Base(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    virtual int getSN() const;
    virtual void setSN(int SN);

    virtual int getFrameType() const;
    virtual void setFrameType(int frameType);

    virtual int getAckNum() const;
    virtual void setAckNum(int ackNum);

    virtual const char * getPayload() const;
    virtual void setPayload(const char * payload);

    virtual const bits& getChecksum() const;
    virtual bits& getChecksumForUpdate() { return const_cast<bits&>(const_cast<Mmsg_Base*>(this)->getChecksum());}
    virtual void setChecksum(const bits& checksum);
};


namespace omnetpp {

inline any_ptr toAnyPtr(const bits *p) {if (auto obj = as_cObject(p)) return any_ptr(obj); else return any_ptr(p);}
template<> inline bits *fromAnyPtr(any_ptr ptr) { return ptr.get<bits>(); }
template<> inline Mmsg_Base *fromAnyPtr(any_ptr ptr) { return check_and_cast<Mmsg_Base*>(ptr.get<cObject>()); }

}  // namespace omnetpp

#endif // ifndef __MMSG_M_H
