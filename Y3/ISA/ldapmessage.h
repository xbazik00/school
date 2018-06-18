/**
 * Server for LDAP protocol.
 * ISA Project
 *
 * @file ldapmessage.h
 * @brief LDAPMessage pocessing
 *
 * @author Martin Bazik, xbazik00
 * @version 1.0 12/11/17
 */ 

#ifndef __LDAPMESS_H_INCLUDED__
#define __LDAPMESS_H_INCLUDED__

#include "ldapop.h"
#include <iostream>

#define MESSAGE_TYPE (0x30)

/**
 * Class for parsing and creating LDAPMessages
 */
class LDAPMessage {
private:
    // length of received message
    int length;

    // MessageID of message
    uint32_t messageID;

    // Length of MessageID
    int IDLen;

    // Message itself
    uint8_t* message;
      
public:

    // Message Vector
    vector<uint8_t> messVector;

    // Pointer to Protocol Operation
    LDAPProtocolOp* protocolOp;

    // Constructor for received message
    LDAPMessage(int, uint8_t*);

    // Constructor for creating an envelope for Protocol Operation
    LDAPMessage(LDAPProtocolOp,uint32_t,int);

    ~LDAPMessage();
    
    // Method for parsing integer from array of octets
    static uint32_t parseInt(uint8_t*,int);

    // Method that appends BER length octets
    void appendLengthToMessage(vector<uint8_t>&,uint32_t);

    // Parses message
    bool parseMessage();

    // Setter
    void setMessage();

    // Appends to messVector
    void appendVector(uint8_t);

    // Appends to messVector
    void appendVector(vector<uint8_t>);

    // Getter dor length
    int getLength();

    // getter for messageID
    uint32_t getID();

    // getter for MessageID length
    int getIDLength();
};

#endif
