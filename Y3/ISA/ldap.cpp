/**
 * Server for LDAP protocol.
 * ISA Project
 *
 * @file ldap.cpp
 * @brief LDAP library
 *
 * @author Martin Bazik, xbazik00
 * @version 1.0 12/11/17
 */

#include "ldap.h"

LDAP::LDAP(){
    this->request = NULL;
}

/**
 * Sends Error response
 */
void LDAP::respondError(){
    if(this->request != NULL){
        LDAPProtocolOp tmp;
        vector<uint8_t> messVector;
        uint8_t* answer;
        if(this->request->protocolOp != NULL){
            switch(this->request->protocolOp->getType()){
                case 0x60:
                    tmp = LDAPProtocolOp();
                    tmp.appendVector(0x0A);
                    tmp.appendVector(1);
                    tmp.appendVector(0x2);
                    tmp.appendVector(4);
                    tmp.appendVector(0);
                    tmp.appendVector(4);
                    tmp.appendVector(0);
                    tmp.setType(0x61);
                    messVector = (LDAPMessage(tmp,this->request->getID(),
                        this->request->getIDLength())).messVector;
                    answer = messVector.data();
                    send(this->socket,answer,messVector.size(),0);
                    break;
                case 0x63:
                    tmp = LDAPProtocolOp();
                    tmp.appendVector(0x0A);
                    tmp.appendVector(1);
                    tmp.appendVector(0x2);
                    tmp.appendVector(4);
                    tmp.appendVector(0);
                    tmp.appendVector(4);
                    tmp.appendVector(0);
                    tmp.setType(0x65);
                    messVector = (LDAPMessage(tmp,this->request->getID(),
                        this->request->getIDLength())).messVector;
                    answer = messVector.data();
                    send(this->socket,answer,messVector.size(),0);
                    break;
                default:
                    break;
            }
        }
        if(this->request != NULL){
            delete this->request;
        }
        this->request = NULL;
    }
}

/**
 * Sends responses one by one
 */
void LDAP::respond(){
    for(size_t i = 0; i < this->response.size(); i++){
        uint8_t* answer = this->response[i].messVector.data();
        send(this->socket,answer,this->response[i].messVector.size(),0);
    }
    if(this->request != NULL){
        delete this->request;
    }
    this->request = NULL;
}

/**
 * Processes LDAPMessage and creates vector of responsess.
 *
 * @return	false in case of error or UNBIND, else true  
 */
void LDAP::process(ifstream& file){
    this->response = vector<LDAPMessage>();
    
    // gets vector of Protocol operation that need to be enveloped in LDAPMessage 
    vector<LDAPProtocolOp> response = this->request->protocolOp->processProtocolOp(file);

    // making envelopes
    for(size_t i = 0; i < response.size(); i++){
        this->response.push_back(LDAPMessage(response[i],this->request->getID(),this->request->getIDLength()));
    }
}


/**
 * Receives LDAPMessage and parses it. As a result a LDAPMessage object this->request is created.
 * It parses data even further.
 *
 * @return	false in case of error or UNBIND, else true  
 */
bool LDAP::receive(){
    uint8_t buffer[BUF_SIZE]; // incomming buffer
    int length = 0; // lenght of received LDAPMessage

    memset(buffer,0,BUF_SIZE);

    // receives first octet
    int n = recv(this->socket, buffer, 1, 0);

    // It is LDAPMessage
    if(n == 1 && buffer[0] == MESSAGE_TYPE){
        
        memset(buffer,0,BUF_SIZE);

        // receives another octet
        n = recv(this->socket, buffer, 1, 0);
        length = int(buffer[0]);
        
        // length is on more than one octet
        if(length >= 0x81){
            int sizeOfLength = length - 0x80;
            memset(buffer,0,BUF_SIZE);
            
            // length is on more octets
            n = recv(this->socket, buffer, sizeOfLength, 0);

            // length of the LDAPMessage
            length = LDAPMessage::parseInt(buffer,sizeOfLength);
            if(length == -1){
                return false;
            }
        }

        memset(buffer,0,BUF_SIZE);        
        
        // receives LDAPMessage
        n = recv(this->socket, buffer, length, 0);
        
        
        // if lenght is correct
        if(n == length){

            // creates LDAPMessage object
            this->request = new LDAPMessage(length,buffer);
            try{
                // parses it
                return this->request->parseMessage();
            }
            catch (exception& e){
                // in case of any parsing error
                return false;
            }
        }
    }
    return false;
}


/**
 * Communicates with the client specified in "acc_soc" socket. Firstly it receives
 * and parses the given request, secondly it processes the parsed message, and
 * lastly it sends response to the request. 
 *
 * @param	acc_soc		socket for
 * @param	path		received socket address
 *
 * @return	ERROR in case of error, else 0  
 */
int LDAP::communicate(int acc_soc, ifstream& file){
    this->socket = acc_soc;
    while(1){
        if(!this->receive()){
            this->respondError();
            break;
        }
        this->process(file);	
        this->respond();
    }
    cout << "Connection ended!" << endl;
    return 0;
}
