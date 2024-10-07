#include <iostream>
#include "port_scanner.hpp"
#include <string>
#include "design.hpp"
#include "filtrare_porturi.hpp"
#include <csignal>
#include <limits>

bool keep_running = true;

void signal_handler(int signal_num)
{
    cout << "\n Ctrl+C detected. getline() still needs input so input smth and u good to go \n>>";
    keep_running=false;
    // reseteaza fluxul de intrare cin dupa Ctrl+C
    // in caz in care am resurse dealocare / deconectare
    exit(0);


}

int main()
{
    signal(SIGINT,signal_handler);
    print_banner();
    print_table();
    cout << BLUE << "Welcome to My Firewall! " <<"\nSupport only for linux at this moment" RESET << std::endl;
    OS_Type OS_option = detect_OS();
    cout << YELLOW << "Your Operating System is : "<<os_type_to_string(OS_option)<< RESET << std::endl;
    string command="";
    while(keep_running)
    {
        cout<<">>";
        getline(cin,command);
        interpret_command(command,OS_option);
        // transformat codul in functie si integrata intr-un header  || void fct(string command)
       

    }

    return 0;
}