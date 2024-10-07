#ifndef PORTSCANNER_HPP
#define PORTSCANNER_HPP

#include <iostream>
#include <SFML/Network.hpp>
#include <string>
#include <cstdio>
#include <memory>

using namespace std;

enum OS_Type {WINDOWS,LINUX,MACOS,UNKNOWN};

OS_Type detect_OS()
{
     #ifdef _WIN32
        return  WINDOWS; 
    #elif __linux__
        return LINUX;
    #elif __APPLE__
        return MACOS;
    #endif
    return UNKNOWN;
}

string os_type_to_string(OS_Type os)
{
    switch(os)
    {
        case WINDOWS: return "Windows";
        case LINUX: return "Linux";
        case MACOS: return "MacOS";
        case UNKNOWN: return "Unknown";
        default: return " Invalid Operating System Type";
    }
}

bool port_is_open(const string& address, int port)
{
    return(sf::TcpSocket().connect(address,port)==sf::Socket::Done);
}

string linux_get_application_for_port(int port)
{
    string command = "lsof -i :" + to_string(port);
    char buffer[128];
    string result ="";

    shared_ptr<FILE> pipe(popen(command.c_str(),"r"),pclose);

    if(!pipe)
    {
        return "Error executing comand";
    }
    while(fgets(buffer,sizeof(buffer),pipe.get())!=nullptr)
    {
        result+=buffer;
    }
    return result.empty() ? " No application found " : result; 

}
//g++ port_scanner.cpp -o port_scanner -lsfml-network -lsfml-system

void ports_scan(OS_Type platform)
{
     for(int i=0;i<65536;i++)
    {
        if(port_is_open("localhost", i))
        {
            cout<<"Port "<<i<<" : ";
            switch(platform)
            {
                case WINDOWS:
                {
                    string app_info = linux_get_application_for_port(i);
                }
                case LINUX:
                break;
                case UNKNOWN:
                break;
                default:
                break;

            }
            string app_info = linux_get_application_for_port(i);
            cout<<"OPEN"<<" ---> application : \n"<<app_info<<endl;
        }
   }
}

void port_scan_2 (OS_Type platform)
{
    
}

#endif