/**
 * Server for LDAP protocol.
 * ISA Project
 *
 * @file ldap.h
 * @brief LDAP library
 *
 * @author Martin Bazik, xbazik00
 * @version 1.0 12/11/17
 */

#ifndef __LDAP_H_INCLUDED__
#define __LDAP_H_INCLUDED__

#include "ldapmessage.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>


#define BUF_SIZE (2048)

/**
  *  Class that maintains LDAP Protocol
  */ 
class LDAP {
    private:
        int socket;

        // received LDAP message
        LDAPMessage* request;

        // vector of result LDAP messages 
        vector<LDAPMessage> response;
        
        // Responds to the client 
        void respond();

        // Error response
        void respondError();
        
        // Receives data from the server and parses the message
        bool receive();

        // Processes the parsed message and creates output
        void process(ifstream&);

    public:
        // Constructor
        LDAP();

        // Whole client-server communication takes place here.
        int communicate(int, ifstream&);
};
#endif
