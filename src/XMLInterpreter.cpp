#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/XMLInterpreter.h"
#include "../include/rapidxml.hpp"
#include "../include/CMSClient.h"

using namespace rapidxml;

XMLInterpreter::XMLInterpreter()
{
    //ctor
}

XMLInterpreter::~XMLInterpreter()
{
    //dtor
}

void XMLInterpreter::StringToTrans(const std::string& src)
{
    std::vector<char> xml_copy(src.begin(), src.end());
    xml_copy.push_back('\0');

    currentDoc.parse<0>(&xml_copy[0]);

}

void XMLInterpreter::parseForSystemInfo()
{
    bool isSystemID = false;
    xml_node<> *pRoot = currentDoc.first_node();

    for(xml_node<> *pNode=pRoot->first_node(); pNode; pNode=pNode->next_sibling())
    {
        for (xml_attribute<> *attr = pNode->first_attribute(); attr; attr = attr->next_attribute())
        {
            std::string curAttr = attr->name();
            std::string curValue = attr->value();
            std::cout << "curAttr: " << curAttr << " curValue: " << curValue << std::endl;
            if (curAttr.compare("id") == 0)
            {
                std::cout << "THIS IS A SYSTEM ID PACKET!" << std::endl;
                isSystemID = true;
            }

        }
        if (isSystemID)
        {
            //Assign ID
            std::string value = pNode->value();
            std::cout << "This node has value: " << pNode->value() << std::endl;
            info.nodeID = pNode->value();
        }

    }
}

std::string XMLInterpreter::getNodeID()
{
    return info.nodeID;
}
