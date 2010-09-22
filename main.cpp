#include <iostream>
#include <string>

#include "Config.hpp"
#include "Network.hpp"

using namespace std;
using namespace sf;


map<double, Socket* > SocketList;
map<double, Socket* >::iterator SocketListIt;
typedef pair<double, Socket* > SocketListPair;
pair<map<double, Socket* >::iterator, bool> SocketListRet;
double SocketIncrement = 0;

map<double, Packet* > PacketList;
map<double, Packet* >::iterator PacketIt;
typedef pair<double, Packet* > PacketListPair;
pair<map<double, Packet* >::iterator, bool> PacketListRet;
double PacketIncrement = 0;

double insertSock(Socket* sock)
{
    SocketList.insert(SocketListPair(++SocketIncrement, sock));

    return SocketIncrement;
}

double insertPacket(Packet* sock)
{
    PacketList.insert(PacketListPair(++PacketIncrement, sock));

    return SocketIncrement;
}

double net_tcp_connect(string ip, unsigned short port, bool blocking);
double net_tcp_connect_timeout(string ip, unsigned short port, bool blocking, float seconds);
double net_tcp_listen(unsigned short port, bool blocking);
double net_tcp_accept(double listener_id, bool blocking);

double net_tcp_connect(string ip, unsigned short port, bool blocking)
{
    return net_tcp_connect_timeout(ip, port, blocking, 0.0f);
}

double net_tcp_connect_timeout(string ip, unsigned short port, bool blocking, float seconds)
{
    TcpSocket* sock = new TcpSocket();
    sock->SetBlocking(blocking);

    sock->Connect(IpAddress(ip), port, seconds);

    return insertSock(sock);
}

double net_tcp_listen(unsigned short port)
{
    TcpListener* sock = new TcpListener();

    sock->Listen(port);

    return insertSock(sock);
}

//Accepts an incoming connection
//Waits until it receives a connection if blocking
//Returns the newly created socket
//Returns 0 if non-blocking and no connections are available
double net_tcp_accept(double listener_id, bool blocking)
{
    TcpSocket* client = new TcpSocket();
    do
    {
        TcpListener* listener = dynamic_cast<TcpListener* >(SocketList[listener_id]);
        if(listener->Accept(*client) == Socket::Done)
        {
            return insertSock(client);
        }
    }
    while(blocking);

    delete client;
    return 0;
}

int main()
{
    double sockId = net_tcp_listen(1080);
    sockId = net_tcp_accept(sockId, true);



    return 0;
}

