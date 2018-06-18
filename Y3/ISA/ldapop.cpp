/**
 * Server for LDAP protocol.
 * ISA Project
 *
 * @file ldapop.cpp
 * @brief LDAP Protocol Operations
 *
 * @author Martin Bazik
 * @version 1.0 12/11/17
 */

#include "ldapop.h"

/**
 * Length getter
 * 
 * @returns	integer length
 */
int LDAPProtocolOp::getLength(){
	return this->length;
}

/**
 * Appends octet c to result vector
 * 
 * @param	c	octet
 */
void LDAPProtocolOp::appendVector(uint8_t c){
	this->messVector.push_back(c);
}

/**
 * Getter for message and length of received
 * 
 * @param	message	returns message
 * @param	len 	returns lenght
 */
void LDAPProtocolOp::getMessage(uint8_t*& message,int& len){
	message = this->message;
	len = this->length;
}

/**
 * Getter for type
 * 
 * @returns	type
 */
int LDAPProtocolOp::getType(){
	return this->type;
}

/**
 * Setter for type
 * 
 * @param	type
 */
void LDAPProtocolOp::setType(uint32_t type){
	this->type = (operation_t) type;
}

/**
 * Basic constructor
 */
LDAPProtocolOp::LDAPProtocolOp(){
	this->length = 0;
	this->message = NULL;
	this->reqStruct = NULL;
	this->password = NULL;
	this->type = NONE;
}

/**
 * Constructor for received message
 * 
 * @param	length	message length
 * @param	messge	received message
 * @param	type	ProtocolOP type
 */
LDAPProtocolOp::LDAPProtocolOp(int length, uint8_t* message, int type){
	this->type = NONE;
	this->length = length;
	this->password = NULL;
	this->message = new uint8_t[length];
	memcpy(this->message,message,length);
	delete[] message;
	this->type = (operation_t) type;
	this->reqStruct = new LDAPProtocolOp::req;
	this->reqStruct->filter = new stack<fItem*>;
}

LDAPProtocolOp::~LDAPProtocolOp(){
	if(this->message != NULL){
		delete[] this->message;
	}
	if(this->reqStruct != NULL){
		if(this->reqStruct->filter != NULL){
			delete this->reqStruct->filter;
		}
		delete this->reqStruct;
	}
	if(this->password != NULL){
		delete[] this->password;
	}
}


/**
 * Creates vector of octets of attribute for result message
 * 
 * @param	str	name of attribute
 * @param	index index in vector of final records
 * 
 * @returns	vector of octets
 */
vector<uint8_t> LDAPProtocolOp::createAttribute(string str,int index){
	vector<uint8_t> tmpAttr;	// full vector
	vector<uint8_t> tmpAttrValue;	// vector for attribute value
	vector<uint8_t> tmpAttrValueList;	// vector for list of attribute values
	if (this->createdAttributes.find(str) == this->createdAttributes.end()){	// not processed yet
		this->createdAttributes.insert(str);	// processed attribute
		if(str.compare("cn") == 0){

			tmpAttrValueList.push_back(0x04);
			tmpAttrValueList.push_back(0x02);
			tmpAttrValueList.push_back('c');
			tmpAttrValueList.push_back('n');
			tmpAttrValueList.push_back(0x31);

			tmpAttrValue.push_back(0x04);
			this->appendLengthToMessage(tmpAttrValue,this->recordVector[index][0].length());
			for(size_t i = 0; i < this->recordVector[index][0].size(); i++) {
				tmpAttrValue.push_back(this->recordVector[index][0][i]);
			}

			this->appendLengthToMessage(tmpAttrValueList,tmpAttrValue.size());

			for(size_t i = 0; i < tmpAttrValue.size(); i++) {
				tmpAttrValueList.push_back(tmpAttrValue[i]);
			}

			tmpAttr.push_back(0x30);
			this->appendLengthToMessage(tmpAttr,tmpAttrValueList.size());
			
			for(size_t i = 0; i < tmpAttrValueList.size(); i++) {
				tmpAttr.push_back(tmpAttrValueList[i]);
			}

		}
		else if(str.compare("uid") == 0){
			tmpAttrValueList.push_back(0x04);
			tmpAttrValueList.push_back(0x03);
			tmpAttrValueList.push_back('u');
			tmpAttrValueList.push_back('i');
			tmpAttrValueList.push_back('d');
			tmpAttrValueList.push_back(0x31);

			tmpAttrValue.push_back(0x04);
			this->appendLengthToMessage(tmpAttrValue,this->recordVector[index][1].length());
			for(size_t i = 0; i < this->recordVector[index][1].size(); i++) {
				tmpAttrValue.push_back(this->recordVector[index][1][i]);
			}

			this->appendLengthToMessage(tmpAttrValueList,tmpAttrValue.size());

			for(size_t i = 0; i < tmpAttrValue.size(); i++) {
				tmpAttrValueList.push_back(tmpAttrValue[i]);
			}

			tmpAttr.push_back(0x30);
			this->appendLengthToMessage(tmpAttr,tmpAttrValueList.size());
			
			for(size_t i = 0; i < tmpAttrValueList.size(); i++) {
				tmpAttr.push_back(tmpAttrValueList[i]);
			}
		}
		else if(str.compare("mail") == 0){


			tmpAttrValueList.push_back(0x04);
			tmpAttrValueList.push_back(0x04);
			tmpAttrValueList.push_back('m');
			tmpAttrValueList.push_back('a');
			tmpAttrValueList.push_back('i');
			tmpAttrValueList.push_back('l');
			tmpAttrValueList.push_back(0x31);

			tmpAttrValue.push_back(0x04);
			this->appendLengthToMessage(tmpAttrValue,this->recordVector[index][2].length());
			for(size_t i = 0; i < this->recordVector[index][2].size(); i++) {
				tmpAttrValue.push_back(this->recordVector[index][2][i]);
			}

			this->appendLengthToMessage(tmpAttrValueList,tmpAttrValue.size());

			for(size_t i = 0; i < tmpAttrValue.size(); i++) {
				tmpAttrValueList.push_back(tmpAttrValue[i]);
			}

			tmpAttr.push_back(0x30);
			this->appendLengthToMessage(tmpAttr,tmpAttrValueList.size());
			
			for(size_t i = 0; i < tmpAttrValueList.size(); i++) {
				tmpAttr.push_back(tmpAttrValueList[i]);
			}
		}
	}
	return tmpAttr;
}

/**
 * Parses given file into vectors of records, one with numbering rows,
 * second containing records.
 * 
 * @param	csvfile	file to be processed
 * @param	record returns vector of records in file
 * 
 * @returns	vector of numbers of rows of records
 */
vector<int> LDAPProtocolOp::parseFile(ifstream& csvfile,vector<vector<string>>& record){
	string line;
	string item;

	int counter = 0;	// for lines
	char delimiter = ';';
	
	vector<int> fullCount;	// vector of line numbers

	//parses file
	while(getline(csvfile, line)){
		stringstream lineStream(line);
		int count = 0;	// for items

		vector<string> tmp;

		// items separated by delimiter
		while(getline(lineStream,item,delimiter)){
			if(item.back() == '\r' || item.back() == '\n'){	// line ends by '\r' or '\n'
				item.pop_back();
			}
			tmp.push_back(item);
			count++;
		}
		record.push_back(tmp);
		fullCount.push_back(counter);
		counter++;
	}
	return fullCount;
}

/**
 * Processes received filter. Each filter has its own type and values.
 * These values are used to generate vector of records that correspond 
 * to given filter.
 * 
 * @param	recordCount	vector of result numbers of rows
 * @param	recrodFullCount	vector of all numbers of rows
 * @param	record	vector of records
 *  
 */
void LDAPProtocolOp::processFilter(vector<vector<int>>& recordCount,
								vector<int> recordFullCount,vector<vector<string>>& record){

	vector<int> recordEmptyCount;	// empty vector
	stack<fItem> tmpItemStack;	// stack of filters
	
	// Iterates requested filters
	while (!this->reqStruct->filter->empty()){
		vector<int> tmpRecordCount;	// temporary vector for record counts

		// read filters one by ones
		fItem item = *(this->reqStruct->filter->top());
		switch(item.type){
			case EQ:	// equality by attribute type
				if(item.leftOp == "cn"){
					for(size_t i = 0; i != record.size(); i++) {
						if(record[i][0].compare(item.rightOp) == 0){
							tmpRecordCount.push_back(i);
						}
					}
				}
				else if(item.leftOp == "uid"){
					for(size_t i = 0; i != record.size(); i++) {
						if(record[i][1].compare(item.rightOp) == 0){
							tmpRecordCount.push_back(i);
						}
					}
				}
				else if(item.leftOp == "mail"){
					for(size_t i = 0; i != record.size(); i++) {
						if(record[i][2].compare(item.rightOp) == 0){
							tmpRecordCount.push_back(i);
						}
					}
				}

				recordCount.push_back(tmpRecordCount);
				break;
				
			case SUB:	// substring by attribute type
				if(item.leftOp == "cn"){
					for(size_t i = 0; i != record.size(); i++) {
						if(regex_match(record[i][0],regex(item.rightOp))){
							tmpRecordCount.push_back(i);
						}
					}
				}
				else if(item.leftOp == "uid"){
					for(size_t i = 0; i != record.size(); i++) {
						if(regex_match(record[i][1],regex(item.rightOp))){
							tmpRecordCount.push_back(i);
						}
					}
				}
				else if(item.leftOp == "mail"){
					
					for(size_t i = 0; i != record.size(); i++) {
						if(regex_match(record[i][2],regex(item.rightOp))){
							tmpRecordCount.push_back(i);
						}
					}
				}

				recordCount.push_back(tmpRecordCount);
				break;
			
			case AND:	// and	
				if(recordCount.size() >=2){
					size_t numberOfItems = 0;
					int sizeOfItems = 0;
					int andSize = item.size;

					// number of filters to be used by and operation
					while(sizeOfItems < andSize){
						sizeOfItems += tmpItemStack.top().size + 1 + tmpItemStack.top().sizeLen;
						tmpItemStack.pop();
						numberOfItems++;
					}

					// uses set operation of set intersection to find "and" of given filters
					vector<int> tmpVector;
					if(numberOfItems >= 2){	// 2 filters				
						sort(recordCount.rbegin()[0].begin(), recordCount.rbegin()[0].end());
						sort(recordCount.rbegin()[1].begin(), recordCount.rbegin()[1].end());
						set_intersection(recordCount.rbegin()[0].begin(), recordCount.rbegin()[0].end(),
							recordCount.rbegin()[1].begin(), recordCount.rbegin()[1].end(),
							back_inserter(tmpVector));
						for(size_t i = 2; i < numberOfItems; i++){	// more than 2 filters
							vector<int> tmpVector2;
							set_intersection(recordCount.rbegin()[i].begin(),recordCount.rbegin()[i].end(),
								tmpVector.begin(),tmpVector.end(),
								back_inserter(tmpVector2));
							tmpVector = tmpVector2;
						}
						for(size_t i = 2; i < numberOfItems; i++){
							recordCount.pop_back();	
						}
						recordCount.pop_back();
						recordCount.pop_back();
						recordCount.push_back(tmpVector);
					}
				}
				break;

			case OR:	//or
				if(recordCount.size() >=2){
					size_t numberOfItems = 0;
					int sizeOfItems = 0;
					int orSize = item.size;
					
					// number of filters to be used by or operation
					while(sizeOfItems < orSize){
						sizeOfItems += tmpItemStack.top().size + 1 + tmpItemStack.top().sizeLen;
						tmpItemStack.pop();
						numberOfItems++;
					}

					vector<int> tmpVector;
					
					// uses set operation of set union to find "or" of given filters
					if(numberOfItems >= 2){	// 2 filters
						vector<int> tmpVector;
						sort(recordCount.rbegin()[0].begin(), recordCount.rbegin()[0].end());
						sort(recordCount.rbegin()[1].begin(), recordCount.rbegin()[1].end());
						set_union(recordCount.rbegin()[0].begin(), recordCount.rbegin()[0].end(),
							recordCount.rbegin()[1].begin(), recordCount.rbegin()[1].end(),
							back_inserter(tmpVector));
						
						
						for(size_t i = 2; i < numberOfItems; i++){	// more than 2 filters
							vector<int> tmpVector2;
							sort(recordCount.rbegin()[i].begin(), recordCount.rbegin()[i].end());
							set_union(recordCount.rbegin()[i].begin(),recordCount.rbegin()[i].end(),
								tmpVector.begin(),tmpVector.end(),
								back_inserter(tmpVector2));
							tmpVector = tmpVector2;
						}
						
						for(size_t i = 2; i < numberOfItems; i++){
							recordCount.pop_back();	
						}
						
						recordCount.pop_back();
						recordCount.pop_back();
						recordCount.push_back(tmpVector);
								
					}
				}
				break;
				
			case NOT:	// not
				
				//it uses set difference to record count vector of all records to simulate not filter 
				if(recordCount.size() >=1){
					vector<int> tmpVector;
					sort(recordCount.back().begin(), recordCount.back().end());
					sort(recordFullCount.begin(), recordFullCount.end());
					set_difference(recordFullCount.begin(), recordFullCount.end(),
						recordCount.back().begin(), recordCount.back().end(),
						back_inserter(tmpVector));
					recordCount.pop_back();
					recordCount.push_back(tmpVector);
					tmpItemStack.pop();
				}
				
				break;

			case PRESENT:	// present filter
				if(item.leftOp == "cn" || item.leftOp == "uid" || item.leftOp == "mail" || item.leftOp == "objectclass" ){
					recordCount.push_back(recordFullCount);
				}
				else{
					recordCount.push_back(recordEmptyCount);
				}
				break;
			default:
				break;
		}
		tmpItemStack.push(*(this->reqStruct->filter->top()));
		delete this->reqStruct->filter->top();
		this->reqStruct->filter->pop();
	}
}

/**
 * Processes received Operation and creates response vector.
 * 
 * @param	csvfile	database file
 * 
 * @returns	vector of response Protocol Operations 
 */
vector<LDAPProtocolOp> LDAPProtocolOp::processProtocolOp(ifstream& csvfile){
	vector<LDAPProtocolOp> resultVector;
	LDAPProtocolOp tmp;	// temporary ProtocolOP

	vector<vector<string>> record;	// vector of all records
	vector<int> recordFullCount;	// vector of all numbers of rows	
	vector<vector<int>> recordCount;	// vector of result numbers of rows

	uint8_t resultCode = 0;
	size_t attrSize = 0;
	size_t wantedSize = 0;

	switch (this->type){
		case BIND_REQUEST:
			
			//positive bind response
			tmp = LDAPProtocolOp();
			tmp.appendVector(0x0A);
			tmp.appendVector(1);
			tmp.appendVector(resultCode);
			tmp.appendVector(4);
			tmp.appendVector(0);
			tmp.appendVector(4);
			tmp.appendVector(0);

			tmp.type = BIND_RESPONSE;
			resultVector.push_back(tmp);

			return resultVector;

		case SEARCH_REQUEST:

			// Parses file into two vectors. First represents recordFullCount range of
			// individual records. In second one there are the records.
			recordFullCount = this->parseFile(csvfile,record);
			
			processFilter(recordCount,recordFullCount,record);

			// Pushes remaining records to final record vector
			for(size_t i = 0; i < recordCount.size(); i++){
				for(size_t j = 0; j < recordCount[i].size(); j++){
					this->recordVector.push_back(record[recordCount[i][j]]);
				}
			}

			// checks if the size is within wanted range 
			wantedSize = (this->reqStruct->size <= 0 || this->reqStruct->size > this->recordVector.size()) ? this->recordVector.size() : this->reqStruct->size; 
			
			//	if it is longer
			if(this->reqStruct->size !=0 && this->reqStruct->size < this->recordVector.size()){
				resultCode = 4;
			}

			// Creates response Protocol OPeration vector of octets
			for(size_t i = 0; i < wantedSize; i++){
				tmp = LDAPProtocolOp();

				// Attributes
				vector<uint8_t> tmpAttr;
				vector<vector<uint8_t>> tmpAttrVector;
				if(this->attributes.size() == 0){
					tmpAttr = createAttribute("cn",i);
					tmpAttrVector.push_back(tmpAttr);
					tmpAttr = createAttribute("uid",i);
					tmpAttrVector.push_back(tmpAttr);
					tmpAttr = createAttribute("mail",i);
					tmpAttrVector.push_back(tmpAttr);
					this->createdAttributes.clear();
				}
				else{
					for(size_t j = 0; j < this->attributes.size(); j++){
						tmpAttr = createAttribute(this->attributes[j],i);
						tmpAttrVector.push_back(tmpAttr);
					}
					this->createdAttributes.clear();
				}

				// Distinguished name
				tmp.appendVector(0x04);

				vector<uint8_t> tmpDN;

				tmpDN.push_back('u');
				tmpDN.push_back('i');
				tmpDN.push_back('d');
				tmpDN.push_back('=');
				
				for(size_t j = 0; j < this->recordVector[i][1].size(); j++) {
					tmpDN.push_back(this->recordVector[i][1][j]);
				}

				// Added Domain Component that is not needed
				/*tmpDN.push_back(',');
				tmpDN.push_back('d');
				tmpDN.push_back('c');
				tmpDN.push_back('=');
				tmpDN.push_back('f');
				tmpDN.push_back('i');
				tmpDN.push_back('t');
				tmpDN.push_back(',');
				tmpDN.push_back('d');
				tmpDN.push_back('c');
				tmpDN.push_back('=');
				tmpDN.push_back('v');
				tmpDN.push_back('u');
				tmpDN.push_back('t');
				tmpDN.push_back('b');
				tmpDN.push_back('r');
				tmpDN.push_back(',');
				tmpDN.push_back('d');
				tmpDN.push_back('c');
				tmpDN.push_back('=');
				tmpDN.push_back('c');
				tmpDN.push_back('z');*/

				// length of DN
				this->appendLengthToMessage(tmp.messVector,tmpDN.size());
				
				// DN itself
				for(size_t j = 0; j < tmpDN.size(); j++) {
					tmp.messVector.push_back(tmpDN[j]);
				}
				
				// Attributes
				tmp.appendVector(0x30);

				attrSize = 0;

				for(size_t i = 0; i < tmpAttrVector.size(); i++) {
					attrSize += tmpAttrVector[i].size();
				}

				this->appendLengthToMessage(tmp.messVector,attrSize);
				for(size_t i = 0; i < tmpAttrVector.size(); i++) {
					for(size_t j = 0; j < tmpAttrVector[i].size(); j++) {
						tmp.appendVector(tmpAttrVector[i][j]);
					}
				}
		
				tmp.type = SEARCH_RESULT_ENTRY;
				resultVector.push_back(tmp);
			}

			// Search Done
			tmp = LDAPProtocolOp();
			tmp.appendVector(0x0A);
			tmp.appendVector(1);
			tmp.appendVector(resultCode);
			tmp.appendVector(4);
			tmp.appendVector(0);
			tmp.appendVector(4);
			tmp.appendVector(0);
			tmp.type = SEARCH_RESULT_DONE;

			resultVector.push_back(tmp);

			return resultVector;
		default:
			return resultVector;
	}
}

/**
 * Appends lenght in BER format into defined vector vec.
 * 
 * @param	vec	returns octet vector
 * @param	length	given length
 */
void LDAPProtocolOp::appendLengthToMessage(vector<uint8_t>& vec,uint32_t length){	
	if(length < 0x80){
        vec.push_back(length);
	}
	else{	// length >= 0x80
        uint8_t* id = new uint8_t[sizeof(uint32_t)];	// array of octets

        memcpy(id,&length,sizeof(uint32_t));	// maps length to array of octets
        uint32_t counter = 1; // It takes at least 1 bytes, number of size octets
        while(length > 0xff){
            length =  (length >> 0x8);
            counter++;
        }

        vec.push_back(0x80+counter);

		for(uint32_t j = 0; j < counter; j++){
            vec.push_back(id[counter-j-1]);
		}
		delete[] id;

    }
}

/**
 * Parses BIND REQEUST. Sets up version, length, password
 * 
 * @returns	true if OK, else false
 */
bool LDAPProtocolOp::parseBindRequest(){
	if(int(this->message[this->index]) == 0x2){
		this->incrementIndex();
		if(this->message[this->index] == 0x1){
			this->incrementIndex();
			this->version = int(this->message[this->index]);
			this->incrementIndex();
			if(this->message[this->index] == 0x4){
				this->incrementIndex();
				int len = int(this->message[this->index]);
				this->incrementIndex(len+1);
				if(this->message[this->index] == 0x80){	//simple authen
					this->incrementIndex();
					int passLen = int(this->message[this->index]);	// password length
					this->password = new char[passLen];
					memcpy(this->password,this->message+this->index,passLen);
					return true;
				}
			}
		}
	}
	return false;
}

/**
 * Parses string.
 * 
 * @param	str returns string
 * @param	length 	returns length
 */
bool LDAPProtocolOp::parseString(string& str,uint32_t& length){
	uint8_t item = this->message[this->index];
	if(item == 0x04){
		this->incrementIndex();
		this->createString(str,length);
		return true;
	}
	return false;
}

	
/**
 * Creates string.
 * 
 * @param	str returns string
 * @param	length 	returns length
 */
void LDAPProtocolOp::createString(string& s, uint32_t& length){
	uint32_t sizeLen = 0;
	length = parseSizeBER(sizeLen);
	char* str = new char[length+1];
	memset(str,0,(length+1)*sizeof(char));
	memcpy(str,this->message+this->index,length*sizeof(uint8_t));
	s = string(str);
	delete[] str;
}

/**
 * Parses substring for substring filter.
 * 
 * @param	str	returns subtring string
 * 
 * @returns	true if OK, uses recursion
 */
bool LDAPProtocolOp::parseSubString(string& str){
	uint8_t item = this->message[this->index];	// type of substring
	
	// temporary string
	string tmp;
	uint32_t stringLen = 0;
	switch (item){
		case 0x80:
			this->incrementIndex();
			this->createString(tmp,stringLen);
			this->incrementIndex(tmp.length());
			this->substr += tmp + string(".*");
			break;

		case 0x81:
			this->incrementIndex();
			this->createString(tmp,stringLen);
			this->incrementIndex(tmp.length());
			this->substr += string(".*") + tmp + string(".*");
			
			break;
		case 0x82:
			
			this->incrementIndex();
			this->createString(tmp,stringLen);
			this->incrementIndex(tmp.length());
			this->substr += string(".*") + tmp;
			
			break;
		default:
			str = this->substr;
			return true;
			break;
	}
	return this->parseSubString(str);
}

/**
 * Parses filter depending on the type of filter. Filters are stored in
 * struct reqStruct a filter.
 * 
 * @returns	true if OK, else false, uses recursion
 */
bool LDAPProtocolOp::parseFilter(){
	string key;
	string val;
	string substr;
	LDAPProtocolOp::fItem *filterItem = new LDAPProtocolOp::fItem;
	uint32_t stringLen = 0;
	switch (this->message[this->index]){
		case AND:
			this->incrementIndex();
			filterItem->size = this->parseSizeBER(filterItem->sizeLen);
			filterItem->type = AND;
			this->reqStruct->filter->push(filterItem);
			break;

		case OR:
			this->incrementIndex();
			filterItem->size = this->parseSizeBER(filterItem->sizeLen);
			filterItem->type = OR;
			this->reqStruct->filter->push(filterItem);
			break;

		case EQ:
			this->incrementIndex();
			filterItem->size = this->parseSizeBER(filterItem->sizeLen);
			if(this->parseString(key,stringLen)){
				this->incrementIndex(key.length());
				if(this->parseString(val,stringLen)){
					this->incrementIndex(val.length());
					filterItem->type = EQ;
					filterItem->leftOp = key;
					filterItem->rightOp = val;
					this->reqStruct->filter->push(filterItem);
					break;
				}
			}
			return false;

		case SUB:	
			this->incrementIndex();
			filterItem->size = this->parseSizeBER(filterItem->sizeLen);
			if(this->parseString(key,stringLen)){
				this->incrementIndex(key.length());
				if(this->message[this->index] == 0x30){
					this->incrementIndex();
					uint32_t sizeSubstrLen;
					this->parseSizeBER(sizeSubstrLen);
					this->substr = string();
					if(this->parseSubString(substr)){
						filterItem->type = SUB;
						filterItem->leftOp= key;
						filterItem->rightOp= substr;
						this->reqStruct->filter->push(filterItem);
						break;
					}
				}
			}
			return false;

		case NOT:
			this->incrementIndex();
			filterItem->size = this->parseSizeBER(filterItem->sizeLen);
			filterItem->type = NOT;
			this->reqStruct->filter->push(filterItem);
			break;

		case PRESENT:
			this->incrementIndex();
			filterItem->size = this->parseSizeBER(filterItem->sizeLen);
			filterItem->type = PRESENT;
			this->index--;	// need to go back
			createString(filterItem->leftOp,stringLen);
			this->reqStruct->filter->push(filterItem);
			this->incrementIndex(filterItem->size);
			break;

		default:
			this->index--;	// one back
			return true;
			break;
	}
	return this->parseFilter();
}

/**
 * Parses Size BER.
 * 
 * @param realSizeLen real length in octetsincluding extra octet
 * 
 * @returns	size
 */
uint32_t LDAPProtocolOp::parseSizeBER(uint32_t &realSizeLen){
	uint32_t sizeLength;
	uint32_t size = this->message[this->index];
	if(size > 0){
		this->incrementIndex();
		if(size >= 0x81){
			sizeLength = size - 0x80;
			realSizeLen = sizeLength + 1;
			return this->parseInt(sizeLength);
		}
	}
	realSizeLen = 1;
	return size;
}

/**
 * Parses integer BER.
 * 
 * @param size length of integer in octets
 * 
 * @returns	integer, if error -1
 */
uint32_t LDAPProtocolOp::parseInt(uint32_t size){
	if(size >= 1 && size <= 4){
		uint8_t* id = new uint8_t[sizeof(uint32_t)];
		memset(id,0,sizeof(uint32_t)*sizeof(uint8_t));

		uint32_t j;
		for(j = 0; j < size; j++){
			id[size-1-j] = this->message[this->index+j];
        }
		
		this->incrementIndex(j);
		
		uint32_t result = *(uint32_t*) id;
		delete[] id;
		return result;

	}
	return -1;

}

/**
 * Parses integer BER.
 * 
 * @param item returns value
 * 
 * @returns	if error false, else true
 */
bool LDAPProtocolOp::parseIntBER(uint32_t &item){
	this->incrementIndex();
	int idLen = int(this->message[this->index]);
	if(idLen >= 1 && idLen <= 4){
		uint8_t* id = new uint8_t[sizeof(uint32_t)];
		memset(id,0,sizeof(uint32_t)*sizeof(uint8_t));
		
		this->incrementIndex();
		int j;
		for(j = 0; j < idLen; j++){
			id[j] = this->message[this->index+idLen-j-1];
		}
		this->incrementIndex(j-1);
		item = * (uint32_t*) id;

		delete[] id;

		return true;

	}
	return false;
}

/**
 * Increments index, if there is an error raises exception.
 */
void LDAPProtocolOp::incrementIndex(){
	if(this->index < this->length - 1){
		this->index++;
	}
	else{
		throw indexException;
	}
}

/**
 * Increments index by x, if there is an error raises exception.
 * 
 * @param	x value too increment by
 */
void LDAPProtocolOp::incrementIndex(int x){
	if(this->index < this->length - x){
		this->index += x;
	}
	else{
		throw indexException;
	}
}

/**
 * Parses search request part by part. Represented by state automaton.
 * 
 * @returns	true if OK, else false
 */
bool LDAPProtocolOp::parseSearchRequest(){
	state_t state = START;
	// iterates ProtocolOP
	for(this->index = 0; this->index < this->length; this->index++){
		uint8_t byte = this->message[this->index];	// current octet
		switch(state){
			case START:
				if(byte == 0x04){
					this->incrementIndex();
					byte = this->message[this->index];
					if(byte != 0){
						this->incrementIndex(int(byte));
					}
					state = SCOPE;
					break;
				}
				state = ERROR;
				break;
			case SCOPE:
				if(byte == 0x0A){
					this->incrementIndex();
					byte = this->message[this->index];
					if(byte == 0x01){
						this->incrementIndex();
						byte = this->message[this->index];
						if(byte == 0){
							state = DEREF;
							break;
						}
					} 
				}
				state = ERROR;
				break;
			case DEREF:
				if(byte == 0x0A){
					this->incrementIndex();
					byte = this->message[this->index];
					if(byte != 0){
						this->incrementIndex(int(byte));
					}
					state = SIZE;
					break;
				}
				state = ERROR;
				break;
			case SIZE:
				if(byte == 0x02){
					if(this->parseIntBER(this->reqStruct->size)){
						state = TIME;
						break;
					}
				}				
				state = ERROR;
				break;
			case TIME:
				if(byte == 0x02){
					if(this->parseIntBER(this->reqStruct->time)){
						state = TYPES;
						break;
					}
				}
				state = ERROR;
				break;
			case TYPES:
				if(byte == 0x01){
					this->incrementIndex();
					byte = this->message[this->index];
					if(byte == 0x01){
						this->incrementIndex();
						state = FILTER;
						break;
					}
				}
				state = ERROR;
				break;
			case FILTER:
				if(this->parseFilter()){
					state = ATTR;
					break;
				}
				state = ERROR;
				break;
			case ATTR:
				if(byte == 0x30){
					if(this->parseAttributes()){
						return true;
						break;
					}
				}
								
				state = ERROR;
				break;
			case ERROR:
			default:
				return false;
		}
	}
	return false;
}

/**
 * Parses individual attributes.
 * 
 * @returns	true if OK, else false
 */
bool LDAPProtocolOp::parseAttributes(){
	string attribute;
	
	this->incrementIndex();
	
	uint32_t sizeLen = 0;
	uint32_t size = parseSizeBER(sizeLen);
	uint32_t sizeOfStringLen = 0;
	
	this->index--;
	
	uint32_t stringLen = 0;
	
	// iterates and parses attributes 
	for(uint32_t i = 0; i < size; i += attribute.length() + 1 + sizeOfStringLen){
		this->incrementIndex();
		if(this->message[this->index] == 0x04){
			if(this->parseString(attribute,stringLen)){
				sizeOfStringLen = getBerSizeLen(stringLen);
				this->attributes.push_back(attribute);
				this->incrementIndex(stringLen-1);
				continue;
			}
		}
		return false;
	}
	return true;
}

/**
 * Returns number of octets used by size
 * 
 * @param	size size BER
 * 
 * @returns	number of octets
 */
uint32_t LDAPProtocolOp::getBerSizeLen(uint32_t size){
	if(size < 0x80){
		return 1;
	}
	else{
		uint32_t counter = 2; // It takes at least 2 bytes 
		while(size > 0xff){
			size =  (size >> 0x8);
			counter++;
		}
		return counter;
	}
}

/**
 * Parses requests depending on its type
 * 
 * @returns	false if error or unbind, else true
 */
bool LDAPProtocolOp::parseRequest(){
	switch (this->type){
		case BIND_REQUEST:
			return this->parseBindRequest();
        case SEARCH_REQUEST:
            return this->parseSearchRequest();
		case UNBIND_REQUEST:						
			return false;
		default:
			cout << "here" << endl;
			return false;
	}
	return true;
}
