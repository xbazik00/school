#!/bin/env python3.6
"""
" File: xqp.py
" Author: xbazik00
" Brief: Implements XML query
" Date: 09.04.2017
"""

import getopt
import sys
import string
import re
import xml.etree.ElementTree as ET
import io

class XML(object):
    """ 
    " Object that represents xml parsed from the xml file
    """
    def __init__(self,filename):
        """ 
        " Object initializer that parses the XML file into element tree format
        """
        if(filename):   # Reading from file
            try:
                self.input_tree = ET.parse(filename)
            except Exception as error:
                print("Error: Invalid input file")
                sys.exit(2)
        else:           # Reading form stdout
            string = io.StringIO(sys.stdin.read())
            self.input_tree = ET.parse(string)
        self.root = self.input_tree.getroot()    # Root of the element tree
        self.parsed = []    # Array of parsed XML elements 
        self.xml_parsed = ""    # Parsed XML string

    
    def process_xml(self,que):
        """
        " Processes XML tree using query stored inside object que.
        " It uses each part of query for processing XML.
        " Result is stored inside attribute xml_parsed
        """

        """
        " Looks for elements to search in.
        """
        found = False
        if(que.query["from_el"] == "ROOT"): # ROOT was given -> search whole tree
            found = True
        else:
            if(que.query["from_el"]):   # element was found
                for element in self.root.iter(que.query["from_el"]):

                    if(not que.query["from_attr"]): # no attribute
                        self.root = element
                        found = True
                        break
                    else:
                        if(que.query["from_attr"] in element.attrib.keys()): # attribute was given
                            self.root = element
                            found = True
                            break
            else:   # no element
                for element in self.root.iter():
                    if(que.query["from_attr"] in element.attrib.keys()):    # but attribute
                        self.root = element
                        found = True
                        break
        
        if(not found):  # nothing after FROM
            self.parsed = ""
            return True
    
        select_elements = []    # Elements selected from tree

        for element in self.root.iter():
            if(element == self.root and que.query["from_el"] != "ROOT"):
                continue
            if(element.tag == que.query["element"]):
                select_elements.append(element) 
        
        final_elements = []     # Selected elements corresponding condition

        for elem in select_elements:
            if(not que.query["cond_op"]):
                final_elements = select_elements
                break

            found = False
            cond = ""

            """
            " Element or attribute to apply condition
            """
            if(que.query["cond_el"]):
                for element in elem.iter(que.query["cond_el"]):
                    if(not que.query["cond_attr"]):
                        found = True
                        cond = element.text
                        break
                    else:
                        if(que.query["cond_attr"] in element.attrib.keys()):
                            cond = element.attrib[que.query["cond_attr"]]
                            found = True
                            break
            else:
                for element in elem.iter():
                    if(que.query["cond_attr"] in element.attrib.keys()):
                        cond = element.attrib[que.query["cond_attr"]]
                        found = True
                        break
            
            """
            " If condition is a number it needs to be converted
            """
            if(que.query["cond_right_type"] == "number"):
                try:
                    cond_number = float(cond)
                except ValueError:
                    continue

            """
            " Choosing condition operation due to type of condition operand
            """
            if(que.query["cond_right_type"] == "string"):
                if(que.query["cond_op"] in ("CONTAINS","<",">","=")):
                    ap_elem = self.cond_choose(elem,que,cond,0)
                    if(not (ap_elem is None)):
                        final_elements.append(ap_elem)
            else:
                try:
                    right_int = int(que.query["cond_right"])    # Converting
                except ValueError:
                    print("Error: Invalid query")
                    sys.exit(80)
                if(que.query["cond_op"] in (">","<","=")):
                    ap_elem = self.cond_choose(elem,que,cond_number,right_int)
                    if(not (ap_elem is None)):
                        final_elements.append(ap_elem)
                else:
                    print("Error: Invalid query", file=sys.stderr)
                    sys.exit(80)

        
        """
        " Final paring by value of limit
        """
        count = 0
        for el in final_elements:
            if(que.query["limit"] == -1 or count < que.query["limit"]):
                self.parsed.append(el)
                count += 1

    def generate_output(self,filename,root,no_head):
        """
        " Generates output file using parsed XML string and the given flags.
        " Output can be given by filename or stdout.
        """

        """
        " XML string from element tree.
        """
        for el in self.parsed:
            self.xml_parsed += ET.tostring(el,"unicode")
        
        """
        " Generating using flags.
        """
        if(root is None):
            if(not no_head):
                output = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + self.xml_parsed
            else:
                output = self.xml_parsed
        else:
            if(not no_head):
                output = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + "<" + root + ">" + self.xml_parsed + "</" + root + ">"
            else:
                output = "<" + root + ">" + self.xml_parsed + "</" + root + ">"
        if(filename is None):
            print(output)       # stdout
        else:
            try:
                with open(filename, 'wb') as myfile:
                    myfile.write((output+"\n").encode("UTF-8"))
            except Exception as error: 
                print("Error: Could not open output file", file=sys.stderr)
                sys.exit(3)

         
    def cond_choose(self,element,que,cond,right_int):
        """
        " Returns element if condition is true else it returns None
        " Condition is stored in element (given), query, cond (condition value for
        " element), right_int if cond is a number.
        """

        """
        " Operations for strings
        """
        if(que.query["cond_right_type"] == "string"):
            if(que.query["cond_op"] == "CONTAINS"):
                if(que.query["cond_right"] in cond):
                    if(not que.query["cond_not"]):
                        return element
                else:
                    if(que.query["cond_not"]):
                        return element
            elif(que.query["cond_op"] == ">"):
                if(cond > que.query["cond_right"]):
                    if(not que.query["cond_not"]):
                        return element
                else:
                    if(que.query["cond_not"]):
                        return element
            elif(que.query["cond_op"] == "<"):
                if(cond < que.query["cond_right"]):
                    if(not que.query["cond_not"]):
                        return element
                else:
                    if(que.query["cond_not"]):
                        return element
            elif(que.query["cond_op"] == "="):
                if(cond == que.query["cond_right"]):
                    if(not que.query["cond_not"]):
                        return element
                else:
                    if(que.query["cond_not"]):
                        return element
        
        """
        " Operations for number
        """
        elif(que.query["cond_right_type"] == "number"):
            if(que.query["cond_op"] == ">"):
                if(cond > right_int):
                    if(not que.query["cond_not"]):
                        return element
                else:
                    if(que.query["cond_not"]):
                        return element
            elif(que.query["cond_op"] == "<"):
                if(cond < right_int):
                    if(not que.query["cond_not"]):
                        return element
                else:
                    if(que.query["cond_not"]):
                        return element
            elif(que.query["cond_op"] == "="):
                if(cond == right_int):
                    if(not que.query["cond_not"]):
                        return element
                else:
                    if(que.query["cond_not"]):
                        return element
        
        """
        " False condition
        """
        return None


class Query(object):
    """
    " Object that represents the given query containing each part
    " individually inside a dictionary.
    """

    def __init__(self):
        """
        " Object initializer, contains default dictionary.
        """

        self.query = dict(
            element = "",
            from_el = "",
            from_attr = "",
            limit = -1,
            cond_el = "",
            cond_attr = "",
            cond_op = "",
            cond_right = "",
            cond_right_type = "",
            cond_not = False
            )

    def parse(self,pquery):
        """
        " Parses the query into the query dictionary.
        """
        
        """
        " Splits query into by whitespaces and removes empty items.
        """
        query_split = re.split("\s+",pquery)
        query_split = list(filter(None, query_split))
        index = 0

        if(len(query_split) < 3):
            print("Error: Invalid query", file=sys.stderr)
            sys.exit(80)

        
        """
        " SELECT
        """
        if(query_split[index] != "SELECT"):
            print("Error: Invalid query", file=sys.stderr)
            sys.exit(80)
        
        index += 1

        """
        " element
        """
        self.query['element'] = query_split[index]
        if(self.query['element'] == "FROM"):
            print("Error: Invalid query", file=sys.stderr)
            sys.exit(80)


        index += 1

        """
        " FROM
        """
        if(query_split[index] != "FROM"):
            print("Error: Invalid query", file=sys.stderr)
            sys.exit(80)
        
        index += 1

        while(index < len(query_split)):
            """
            " from_element
            """
            if(index == 3 and query_split[index] not in ("WHERE","ORDER","LIMIT")):
                from_sep = re.split("\.",query_split[index])
                self.query["from_el"] = from_sep[0]
                if(len(from_sep) != 1):
                    self.query["from_attr"] = from_sep[1]
                index += 1

            """
            " WHERE
            """
            elif(query_split[index] == "WHERE"):
                index += self.parse_condition(query_split[index+1:])

            """
            " ORDER
            """
            elif(query_split[index] == "ORDER"):
                index += self.parse_order(query_split[index+1:])

            """
            " LIMIT
            """
            elif(query_split[index] == "LIMIT"):
                index += 1
                if(index < len(query_split)):
                    try:
                        self.query["limit"] = int(query_split[index])   # Converting limit
                    except ValueError:
                        print("Error: Invalid query")
                        sys.exit(80)
                    if(self.query["limit"] < 0):
                        print("Error: Limit cannot be negative", file=sys.stderr)
                        sys.exit(80)

                    index += 1
            else:
                print("Error: Invalid query", file=sys.stderr)
                sys.exit(80)

    def parse_condition(self,condition):
        """
        " Parses the condition into parts in the dictionary
        """

        """
        " Individual flags for FSM
        """
        index = 0
        take_not = True
        after_not = False
        cond_op = False
        cond_right = False
        condition = list(filter(None, condition))
        elem = ""

        """
        " FSM
        """
        while(index < len(condition)):
            """
            " NOT
            """
            if(take_not and condition[index] == "NOT"):
                self.query["cond_not"] = not self.query["cond_not"]
                index += 1
                after_not = True

            """
            " first operand
            """
            elif(index == 0 or after_not):
                elem = condition[index]
                after_not = False
                take_not = False
                op = []
                op.append(elem.find("<"))
                op.append(elem.find(">"))
                op.append(elem.find("="))
                op.append(elem.find("CONTAINS"))

                if(op[0] != -1 or op[1] != -1 or op[2] != -1 or op[3] != -1 ):
                    rest = ""
                    for o in op:
                        if(o != -1):
                            operator = o
                            break
                    if(o == -1):
                        pass
                    elif(elem[o] in ("<",">","=")):
                        self.query["cond_op"] = elem[o]
                        rest = elem[o+1:]
                    else:
                        self.query["cond_op"] = "CONTAINS"
                        rest = elem[o+8:]
                    if(rest):
                        self.query["cond_right"] = rest
                    else:
                        cond_right = True
                    elem = elem[:o]
                else:
                    cond_op = True
                index += 1

            """
            " OPERATION
            """
            elif(cond_op):
                if(condition[index][0] in ("<",">","=")):
                    self.query["cond_op"] = condition[index][0]
                    rest = condition[index][1:]
                    index += 1
                    cond_op = False
                elif(condition[index][0:8] == "CONTAINS"):
                    self.query["cond_op"] = "CONTAINS"
                    rest = condition[index][8:]
                    index += 1
                    cond_op = False
                else:
                    print("Error: Icorrect query", file=sys.stderr)
                    sys.exit(80)
                if(not rest):
                    cond_right = True
                else:
                    self.query["cond_right"] = rest

            """
            " second operand
            """
            elif(cond_right):
                self.query["cond_right"] = condition[index]
                index += 1
                cond_right = False

            """
            " END
            """
            elif(condition[index] in ("ORDER","LIMIT")):
                break
            else:
                print("Error: Invalid query", file=sys.stderr)
                sys.exit(80)
        if(not elem):
            print("Error: Icorrect query", file=sys.stderr)
            sys.exit(80)

        """
        " separates element and attribute
        """
        sep = re.split("\.",elem)
        self.query["cond_el"] = sep[0]
        if(len(sep) != 1):
            self.query["cond_attr"] = sep[1]


        if(not self.query["cond_right"]):
            print("Error: Icorrect query", file=sys.stderr)
            sys.exit(80)

        """
        " Decides if it is string or number
        """
        if(self.query["cond_right"][0] == "\"" and self.query["cond_right"][-1] == "\""):
            self.query["cond_right"] = self.query["cond_right"][1:-1]
            self.query["cond_right_type"] = "string"
        else:
            
            self.query["cond_right_type"] = "number"

        """
        " Invalid operation for number
        """
        if(self.query["cond_right_type"] == "number" and self.query["cond_op"] == "CONTAINS"):
            print("Error: Invalid operation in query", file=sys.stderr)
            sys.exit(80)


        if(take_not or after_not or cond_op or cond_right):
            print("Error: Icorrect query", file=sys.stderr)
            sys.exit(80)
        return index + 1
    
    def parse_order(self,order):
        return 1

        

def main():
    
    """
    " Parsing arguments
    """
    try:
        opts, args = getopt.getopt(sys.argv[1:], "n", ["help", "output=", "input=", "query=", "qf=", "root="])
    except getopt.GetoptError as err:
        # print help information and exit:
        print(str(err), file=sys.stderr)  # will print something like "option -a not recognized"
        print()
        usage()
        sys.exit(1)

    # arguments
    output_path = None
    input_path = None
    query = None
    qf = None
    root = None
    no_head = False
    help_par = False
    for o, a in opts:
        if o == "-n":
            no_head = True
        elif o == "--help":
            help_par = True
        elif o == "--output":
            output_path = a
            if(not output_path):
                print("Error: Invalid argument", file=sys.stderr)
                sys.exit(1)
        elif o == "--input":
            input_path = a;
            if(not input_path):
                print("Error: Invalid argument", file=sys.stderr)
                sys.exit(1)
        elif o == "--query" and qf is None:
            query = a;
            if(not query):
                print("Error: Invalid argument", file=sys.stderr)
                sys.exit(80)
        elif o == "--qf" and query is None:
            qf = a;
            if(not qf):
                print("Error: Invalid argument", file=sys.stderr)
                sys.exit(80)
        elif o in ("--query","--qf"):
            usage()
            sys.exit(1)
        elif o == "--root":
            root = a;
        else:
            assert False, "unhandled option"
    
    """
    " qf and query cant be together
    """
    if(qf is None and query is None and not help_par):
        print("Error: Missing query", file=sys.stderr)
        sys.exit(80)

    """
    " No other argument with help.
    """
    if(help_par and (no_head or not input_path is None or not output_path is None or not query is None or not qf is None or not root is None)):
        print("Error: Invalid combination of arguments", file=sys.stderr)
        sys.exit(1)
    elif(help_par):
        usage()
        sys.exit(0)

    """
    " from --query
    """
    if(qf is None):
        que = Query()
        que.parse(query)

        xml = XML(input_path)
        xml.process_xml(que)
        xml.generate_output(output_path,root,no_head)

    """
    " from --qf
    """
    else:
        try:
            with open(qf,"r") as query_file:
                query_file_text = query_file.read()
        except Exception as error: 
            print("Error: Query file not found", file=sys.stderr)
            sys.exit(80)


        query_arr = query_file_text.split("SELECT")
        query_arr = list(filter(None, query_arr))
        query_arr = ["SELECT" + s for s in query_arr]
        count = 0
        
        # Individual queries
        for q in query_arr:
            if(count == 1):
                print("Error: Invalid query", file=sys.stderr)
                sys.exit(80)

            que = Query()
            que.parse(q)

            xml = XML(input_path)
            count += 1
            xml.process_xml(que)
        
        xml.generate_output(output_path,root,no_head)


def usage():
    """
    " Prints out help info
    """
    print("--help\t\t\tprints out help description\n"
          "--input=filename\tinput file in XML format\n"
          "--output=filename\toutput file in XML format\n"
          "--query='request'\tgiven request\n"
          "--qf=filename\t\tfile containing requests (cannot be combined with --query)\n"
          "-n\t\t\tdo not generate XML header on th output\n"
          "--root=element\t\tname of root element wrapping result (if no root element\n"
          "              \t\tis classified, the result are not wrapped up and so XML is not valid)")

if __name__ == "__main__":
    main()

