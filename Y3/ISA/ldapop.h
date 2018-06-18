/**
 * Server for LDAP protocol.
 * ISA Project
 *
 * @file ldapop.h
 * @brief LDAP Protocol Operations
 *
 * @author Martin Bazik
 * @version 1.0 12/11/17
 */

#ifndef __LDAPOP_H_INCLUDED__
#define __LDAPOP_H_INCLUDED__

#include <stdlib.h>
#include <vector>
#include <set>
#include <string.h>
#include <sstream>
#include <stack> 
#include <fstream>
#include <iostream>
#include <regex>
#include <algorithm>

using namespace std;


/**
 *  Class that maintains LDAP Protocol Operation  
 */ 
class LDAPProtocolOp{
private:
    
    // ProtocolOP length
    int length;

    // ProtocolOP received octets
    uint8_t* message;

    // password
    char* password;
    
    // Index to iterate received message
    int index;

    // Substring 
    string substr;

    // Vector of parsed requested attributes
    vector<string> attributes;

    // vector of vectors of records corresponding to the filter
    vector<vector<string>> recordVector;

    // type for operations
    enum operation_t {NONE = 0x00, BIND_REQUEST = 0x60, BIND_RESPONSE = 0x61, SEARCH_REQUEST = 0x63,
        SEARCH_RESULT_ENTRY = 0x64, SEARCH_RESULT_DONE = 0x65, UNBIND_REQUEST = 0x42};
    
    // type for state automaton
    enum state_t {START,SKIP,BASE,SCOPE,DEREF,SIZE,TIME,TYPES,FILTER,ATTR,ERROR,FINISH};
    
    // type for individual filters
    enum filter_t {AND=0xa0,OR=0xa1,NOT=0xa2,EQ=0xa3,SUB=0xa4,PRESENT=0x87};
    
    // type of ProtocolOP
    operation_t type;

    // ProtocolOP version
    int version;

    // final set of created attributes
    set<string> createdAttributes;
    
    // struct for filter items, stores information about filter
    struct fItem{
        filter_t type;
        string leftOp;
        string rightOp;
        size_t size;
        uint32_t sizeLen;
    };

    // struct that contains request on size, time, and filter
    struct req{
        uint32_t size = 0;
        uint32_t time = 0;
        stack<fItem*> *filter = NULL;
    } *reqStruct;
    
    // Parsing methods
    // Parses Integer from BER format
    bool parseIntBER(uint32_t&);

    // Parses string from BER format
    bool parseString(string&,uint32_t&);

    // Creates string from BER
    void createString(string&,uint32_t&);

    // Parses size part of BER
    uint32_t parseSizeBER(uint32_t&);

    // Parses integer of BER
    uint32_t parseInt(uint32_t);

    // Parses substring from BER
    bool parseSubString(string&);

    // Parses filter from BER
    bool parseFilter();

    // Parses attributes from BER
    bool parseAttributes();

    // Increments index
    void incrementIndex();
    void incrementIndex(int);

    // Creates attribute result vector from string
    vector<uint8_t> createAttribute(string,int);

    // Parses given file into vectors of records, one with numbering rows,
    // second containing records.
    vector<int> parseFile(ifstream&,vector<vector<string>>&);

    // Creates response octet vector
    bool createResponeVector(vector<LDAPProtocolOp>&);

    // Processes given filters
    void processFilter(vector<vector<int>>&,vector<int>,vector<vector<string>>&);

    // returns Length of Size part of BER
    uint32_t getBerSizeLen(uint32_t size);

    // appends lenght in BER format into output vector
    void appendLengthToMessage(vector<uint8_t>&,uint32_t);

    // parses BindRequest
    bool parseBindRequest();

    // Parses Search Request
    bool parseSearchRequest();


public:

    // exception for received messsage array
    class out_of_range : public exception
    {
        virtual const char* what() const throw(){
            return "Index out of range!";
        }
    } indexException;
    
    // Publicly accessable response vector
    vector<uint8_t> messVector;

    // constructor for received data
    LDAPProtocolOp(int,uint8_t*,int);

    // constructor
    LDAPProtocolOp();

    // Destructor
    ~LDAPProtocolOp();

    // Parses received request
    bool parseRequest();

    // Creates respons
    LDAPProtocolOp createResponse();

    // Processes received Operation and creates response vector
    vector<LDAPProtocolOp> processProtocolOp(ifstream&);

    // Getters
    void getMessage(uint8_t*&,int&);
    int getType();
    int getLength();

    // Setter
    void setType(uint32_t type);

    // Appends to result vector
    void appendVector(uint8_t);
};

#endif