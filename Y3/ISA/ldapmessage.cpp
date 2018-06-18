/**
 * Server for LDAP protocol.
 * ISA Project
 *
 * @file ldapmessage.cpp
 * @brief LDAPMessage pocessing
 *
 * @author Martin Bazik, xbazik00
 * @version 1.0 12/11/17
 */ 
#include "ldapmessage.h"

/**
 * Getter for message length
 *
 * @return	message length  
 */
int LDAPMessage::getLength(){
    return this->length;
}

/**
 * Getter for messageID
 *
 * @return	messageID 
 */
uint32_t LDAPMessage::getID(){
    return this->messageID;
}

/**
 * Getter for messageID length
 *
 * @return	messageID length  
 */
int LDAPMessage::getIDLength(){
    return this->IDLen;
}

/**
 * Appends to messVector
 *
 * @param   c octet to append 
 */
void LDAPMessage::appendVector(uint8_t c){
    this->messVector.push_back(c);
}

/**
 * Appends to messVector
 *
 * @param   v vector of octets to append 
 */
void LDAPMessage::appendVector(vector<uint8_t> v){
    for(unsigned int i = 0; i < v.size(); i++){
        this->messVector.push_back(v[i]);
    }
}

/**
 * Constructor for received message
 *
 * @param   length  message length
 * @param   message array of received octets 
 */
LDAPMessage::LDAPMessage(int length, uint8_t* message){
    this->protocolOp = NULL;
    this->length = length;
    this->message = new uint8_t[length];
    memcpy(this->message,message,length);
}

/**
 * Method that appends BER length octets
 *
 * @param   vec  vector to append
 * @param   length length to append 
 */
void LDAPMessage::appendLengthToMessage(vector<uint8_t>& vec,uint32_t length){	
	if(length < 0x80){
        vec.push_back(length);
	}
	else{
        // uses array of octets to which the length is mapped
        uint8_t* id = new uint8_t[sizeof(uint32_t)];

        memcpy(id,&length,sizeof(uint32_t));
        uint32_t counter = 1; // It takes at least 2 bytes, number of octets to store length
        while(length > 0xff){
            length =  (length >> 0x8);
            counter++;
        }

        vec.push_back(0x80+counter);    // First octet

		for(uint32_t j = 0; j < counter; j++){  // The rest of octets of length
            vec.push_back(id[counter-j-1]);
        }
    }
}

/**
 * Constructor for creating an envelope for Protocol Operation
 *
 * @param   op  Protocol Operation to envelope
 * @param   id messageID
 * @param   len Lenght of Protocol Operation 
 */
LDAPMessage::LDAPMessage(LDAPProtocolOp op, uint32_t id, int len){
    this->message = NULL;
    this->protocolOp = NULL;
    // BER for MessageID in tmpVector
    vector<uint8_t> tmpVector;
    tmpVector.push_back(0x2);
    tmpVector.push_back(len);
    for(int i = 0; i < len; i++){
        tmpVector.push_back((id>>((len-1-i)*8)) & 0xFF);
    }

    // Protocol Operation with its type and length
    tmpVector.push_back(op.getType());
    this->appendLengthToMessage(tmpVector,op.messVector.size());
    for(unsigned int i = 0; i < op.messVector.size(); i++){
        tmpVector.push_back(op.messVector[i]);
    }

    // Outer part of message envelope
    this->appendVector(MESSAGE_TYPE);
    
    // full lenght
    this->appendLengthToMessage(this->messVector,tmpVector.size());

    // rest of message
    this->appendVector(tmpVector);  


}

/**
 * Method for parsing integer from array of octets
 *
 * @param   item  array of octets
 * @param   size length of integer
 * 
 * @return -1 if error, else parsed integer 
 */
uint32_t LDAPMessage::parseInt(uint8_t* item,int size){
    // max 32 bit
	if(size >= 1 && size <= 4){
		uint8_t* id = new uint8_t[sizeof(uint32_t)];
		memset(id,0,sizeof(uint32_t)*sizeof(uint8_t));

		for(int j = 0; j < size; j++){
			id[size-1-j] = item[j];
        }
        uint32_t item = *(uint32_t*) id;
        delete[] id;
		return item;

	}
	return -1;
}

/**
 * Parses received message. Iterates message octet by octet and check correct octets.
 * It also parses messageID, length of MessageID, Protocol Operation type and length.
 * Rest is put into created Protocol Operation and parsed further. 
 * 
 * @return false if error or end of communication, else true 
 */
bool LDAPMessage::parseMessage(){
    // index for message
    int index = 0;

    int requestSizeLength = 0;

    if(this->message[index++] == 0x2){
        int idLen = int(this->message[index++]);    // length of messageID
        if(idLen >= 1 && idLen <= 4){
            uint8_t* mess = new uint8_t[this->length]; // deleted in LDAPProlocolOp

            this->messageID = this->parseInt(this->message + index ,idLen);            
            this->IDLen = idLen;

            index += idLen;

            int requestType = int(this->message[index++]);  // Protocol Operation type
            int requestLength = int(this->message[index++]);    // Protocol Operation length
            
            if(requestLength >= 0x81){
                requestSizeLength = requestLength - 0x80;
                requestLength = this->parseInt(this->message + index,requestSizeLength);
                index += requestSizeLength;
            }
            
            memcpy(mess,this->message+index,requestLength);
            
            delete[] this->message;

            // Protocol Operation
            this->protocolOp = new LDAPProtocolOp(requestLength,mess,requestType);
            
            return this->protocolOp->parseRequest();
        }
    }
    return false;
}

LDAPMessage::~LDAPMessage(){
    if(this->protocolOp != NULL){
        delete this->protocolOp;
    }
}