#ifndef FILTRARE_HPP
#define FILTRARE_HPP

#include <iostream>
#include <bits/stdc++.h>
#include "port_scanner.hpp"

using namespace std;
// to be implemented
vector<string> decode_command(string command)
{   
    vector<string> result;
    string word;
    for(int i=0;i<command.size();i++)
    {
        if(isalnum(command[i]) || command[i]=='-' )
        {
            word+=command[i];
        }
        else if(command[i]==' ' && word.size()>1 )
        {
            result.push_back(word);
            word="";
        
        }
        else continue;
    }
    return result;
}
void afisare(vector<string> list)
{
    for(int i=0;i<list.size();i++)
    {
        cout<<list[i]<<"/";
    }
}
    
void interpret_command(string command,OS_Type OS_option)
{
   // inlocuire cu un switch cu enum;
        if(command == "exit")
        {
            cout<<"Exiting program..."<<endl;
            // dealocare resurse in caz de ceva
            exit(0);
        }
        if(command == "scan")
        {
            ports_scan(OS_option);
        }
        if(command == "help")
        {
            cout<<"only implemented functionalities: \n --> scan \n --> exit\n";
        }

}

// to be implemented
void modify_list(vector<int>*list,string command)
{
    if(command == "-add")
    {
        list->push_back(1);
    }
    else if(command == "-rm")
    {

    }
}

bool is_port_whitelisted(vector<int>whitelist,int port)
{
    return find(whitelist.begin(),whitelist.end(),port) != whitelist.end();
}

bool is_port_blacklisted(vector<int>blacklist,int port)
{
    return find(blacklist.begin(),blacklist.end(),port) != blacklist.end();
}

#endif