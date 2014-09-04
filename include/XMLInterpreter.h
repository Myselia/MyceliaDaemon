#ifndef XMLINTERPRETER_H
#define XMLINTERPRETER_H

#include <iostream>		//io
#include <string>
#include "rapidxml.hpp"


struct xmlSetupInfo{
    std::string nodeID;
};

class XMLInterpreter
{
    public:
        XMLInterpreter();
        virtual ~XMLInterpreter();

        void StringToTrans(const std::string& src);
        void parseForSystemInfo();
        std::string getNodeID();
        rapidxml::xml_document<> currentDoc;
        xmlSetupInfo info;

    protected:
    private:
};

#endif // XMLINTERPRETER_H
