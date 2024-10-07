
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <sys/types.h>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <dirent.h>
#include <set>
#include <unordered_map>

class listPortsInfo
{
private:
    static std::set<std::tuple<int, std::string, std::string,std::string>> lista;
    static std::set<std::pair<int, std::string>> inodeList;

public:
    listPortsInfo() {}


    void addlistPortsInfo(int port, int PID, int inode)
    {

        lista.insert(std::make_tuple(port, std::to_string(PID), std::to_string(inode),""));
    }


    int isInodeInList(const std::string &inode)
    {
        for (const auto pair : inodeList)
        {
            if (pair.second == inode)
                return pair.first;
        }
        return -1;
    }


    int initializeMapping(std::string const protocol)
    {

        std::string protFile = "/proc/net/"+protocol; 
        // mapare inode cu porturi
        std::ifstream tcpFile(protFile);
        if (!tcpFile.is_open())
        {
            return 1;
        }
        std::string line;
        std::getline(tcpFile, line); // ignoram header

        while (std::getline(tcpFile, line))
        {
            std::istringstream iss(line); // transforma linia intr-un fluz de intrare string
            std::string dummy;
            std::string localAddress;
            std::string inode; // a treia linie din ultimul bloc
            int localPort;

            iss >> dummy;                                                          //                                                    // ignoram prima coloana
            iss >> localAddress;                                                   // adresa locala coloana 2
            std::string portHex = localAddress.substr(localAddress.find(":") + 1); // copiaza string dupa ":" (format ---> adresa ip:portul)
           
            for (int i = 0; i < 7; i++)
            {
                iss >> dummy;
                
            }
            iss >> inode;
           
            inodeList.insert({std::stoi(portHex, nullptr, 16), inode});
        }
        // mapam a doua oara PID-urile la inode-uri
        DIR *proc_dir = opendir("/proc");
        if (!proc_dir)
        {
            return 1;
        }
        struct dirent *entry;
        while ((entry = readdir(proc_dir)) != nullptr)
        {
            if (entry->d_type == DT_DIR && isdigit(entry->d_name[0]))
            {
                std::string pid_dir = "/proc/" + std::string(entry->d_name) + "/fd";
                DIR *fd_dir = opendir(pid_dir.c_str());
                dirent *fd_entry;

                if (!fd_dir)
                {
                    continue;
                }
                while ((fd_entry = readdir(fd_dir)) != nullptr)
                {
                    if (fd_entry->d_type == DT_LNK)
                    {
                        std::string fd_link = pid_dir + "/" + fd_entry->d_name;
                        char buf[1024];
                        ssize_t len = readlink(fd_link.c_str(), buf, sizeof(buf) - 1);
                        if (len != -1 && std::string(buf).find("socket:[") != std::string::npos)
                        {
                            std::size_t start = std::string(buf).find('[');
                            std::size_t end = std::string(buf).find(']');
                            if (start != std::string::npos && end != std::string::npos && start < end)
                            {
                                std::string processPID = std::string(entry->d_name);
                                std::string processSocket = std::string(buf).substr(start + 1, end - start - 1);
                                int processPort = isInodeInList(processSocket);
                                if (processPort != -1)
                                    lista.insert(std::make_tuple(processPort, processPID, processSocket,protocol));
                            }
                        }
                    }
                }
                closedir(fd_dir);
            }
        }
        closedir(proc_dir);
        return 0;
    }


    void afisareInodeList()
    {
        for (auto const &pair : inodeList)
        {
            std::cout << " {" << pair.first << "," << pair.second << "}\n";
        }
    }


    void afisarePortPIDInode()
    {
        for (auto const &tuple : lista)
        {
            std::cout<<" Port: "<<std::get<0>(tuple)<<" PID: "<<std::get<1>(tuple)<<" inode: "<<std::get<2>(tuple)<<" protocol: "<<std::get<3>(tuple)<<std::endl;
        }
    }


    void empty()
    {
        inodeList.clear();
    }

    ~listPortsInfo() {}
};

std::set<std::tuple<int, std::string, std::string,std::string>> listPortsInfo::lista;
std::set<std::pair<int, std::string>> listPortsInfo::inodeList;

std::string finPIDForInode(std::string &inode)
{
    DIR *proc_dir = opendir("/proc"); // creare stream director
    if (!proc_dir)
    {
        return "Error in opening stream directory to proc";
    }
    struct dirent *entry;
    while ((entry = readdir(proc_dir)) != nullptr)
    {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) // DT_DIR indică faptul că respectiva intrare dintr-un director este un subdirector; verificam daca subdirectorul este subdirector PID
        {
            std::string pid_dir = "/proc/" + std::string(entry->d_name) + "/fd";

            DIR *fd_dir = opendir(pid_dir.c_str());
            struct dirent *fd_entry;
            if (!fd_dir)
            {
                continue; // trecem la urmatorul proces daca nu gasim directorul fd
            }
            while ((fd_entry = readdir(fd_dir)) != nullptr)
            {
                // caut descriptorul de fisier pentru socket
                if (fd_entry->d_type == DT_LNK)
                {
                    std::string fd_link = pid_dir + "/" + fd_entry->d_name;
                    char buf[1024];
                    ssize_t len = readlink(fd_link.c_str(), buf, sizeof(buf) - 1); // functia returneaza numarul de bytes din descriptor

                    if (len != -1 && std::string(buf).find("socket:[" + inode + "]") != std::string::npos)
                    {
                        closedir(fd_dir);
                        closedir(proc_dir);
                        return std::string(entry->d_name); // daca socketul este gasit returnam PID-ul
                    }
                }
            }
            closedir(fd_dir);
        }
        closedir(proc_dir);
    }

    return "No proccess found";
}

std::string scanPort(int port)
{
    std::ifstream tcpFile("/proc/net/tcp6");
    if (!tcpFile.is_open())
    {
        return "Error trying to open /proc/net/tcp";
    }
    std::string line;
    std::getline(tcpFile, line); // ignoram header

    while (std::getline(tcpFile, line))
    {
        std::istringstream iss(line); // transforma linia intr-un fluz de intrare string
        std::string localAddress;
        std::string remoteAdress;
        std::string conectionState;
        std::string inode = ""; // a treia linie din ultimul bloc
        int localPort;

        iss >> localAddress;                                                   // ignoram prima coloana
        iss >> localAddress;                                                   // adresa locala coloana 2
        std::string portHex = localAddress.substr(localAddress.find(":") + 1); // copiaza string dupa ":"
        localPort = std::stoi(portHex, nullptr, 16);                           // portul este in hexadecimal in fisierul tcp il convertim in decimal

        //////////
        if (localPort == port)
        {
            std::string pidFile = "/proc/" + finPIDForInode(inode) + "/comm";
        }
        //////////
        break; // for debuging
    }
    return "";
}

int main(int argc, char *argv[])
{
    int port = 8080;
    // std::cout << scanPort(port) << std::endl;
    listPortsInfo ports;
    ports.initializeMapping("tcp");
    ports.afisarePortPIDInode();
    ports.initializeMapping("tcp6");
    std::cout<<"\n\n------------------------------------------------------\n\n";
    ports.afisarePortPIDInode();
    ports.empty();
    return 0;
}
