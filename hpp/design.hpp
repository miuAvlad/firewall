#ifndef DESIGN_HPP
#define DESIGN_HPP

#include <iomanip>
#include <iostream>
// Coduri de culori ANSI
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

void print_banner() {
    std::cout << GREEN<<R"(
    ____  _        _ _      _          
   |  _ \| |      (_) |    (_)         
   | |_) | |_ _ __ _| | ___ _  ___  ___ 
   |  __/| __| '__| | |/ _ \ |/ _ \/ __|
   | |   | |_| |  | | |  __/ |  __/\__ \
   |_|    \__|_|  |_|_|\___| |\___||___/
                                 
    
    )" << RESET<<std::endl;
}


void print_table() {
    std::cout << std::left << std::setw(10) << "Port"
              << std::setw(20) << "Status"
              << std::setw(30) << "Application" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(10) << "22"
              << std::setw(20) << "OPEN"
              << std::setw(30) << "SSH" << std::endl;
    std::cout << std::left << std::setw(10) << "80"
              << std::setw(20) << "OPEN"
              << std::setw(30) << "HTTP" << std::endl;
    std::cout << std::left << std::setw(10) << "443"
              << std::setw(20) << "OPEN"
              << std::setw(30) << "HTTPS" << std::endl;
}


#endif