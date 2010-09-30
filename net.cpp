/*
Copyright (C) 2007 39ster
Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>

This file is part of EnigmaNet
EnigmaNet is free software and comes with ABSOLUTELY NO WARRANTY.
See LICENSE for details.
*/

#include "includes.h"
#include "tools.h"
#include "socket.h"
#include "buffer.h"
#include "list.h"

CList buffers;
CList sockets;
CList files;
int AddSocket(CSocket*);
int AddBuffer(CBuffer*);
int AddFile(HANDLE);

//Sockets
double net_tcp_connect(char*ip, double port, double mode)
{
	CSocket* sock = new CSocket();
	if(sock->tcpconnect(ip, (int)port, (int)mode))
		return AddSocket(sock);
	delete sock;
	return -1;
}

double net_tcp_listen(double port, double max, double mode)
{
	CSocket* sock = new CSocket();
	if(sock->tcplisten((int)port, (int) max,(int)mode))
		return AddSocket(sock);
	delete sock;
	return -1;
}

double net_tcp_accept(double sockid, double mode)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	if(sock == NULL)return -1;
	CSocket*sock2 = sock->tcpaccept((int)mode);
	if(sock2 != NULL)return AddSocket(sock2);
	return -1;
}

char* net_tcp_ip(double sockid)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	if(sock == NULL)return NULL;
	return sock->tcpip();
}

double net_set_nagle(double sockid, double value)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	if(sock == NULL)return 0;
	if(value)
		sock->setnagle(true);
	else sock->setnagle(false);
	return 1;
}
double net_tcp_connected(double sockid)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	if(sock == NULL)return 0;
	if(sock->tcpconnected())return 1;
	else return 0;
}
double net_udp_connect(double port, double mode)
{
	CSocket* sock = new CSocket();
	if(sock->udpconnect((int)port, (int)mode))
		return AddSocket(sock);
	delete sock;
	return -1;
}
double net_message_send(double sockid, char*ip, double port, double buffid)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(sock == NULL)return -1;
	if(buff == NULL)return -2;
	int size = sock->sendmessage(ip, (int)port, buff);
	if(size < 0)return -sock->lasterror();
	return size;
}

double net_message_receive(double sockid, double len, double buffid)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(sock == NULL)return -1;
	if(buff == NULL)return -2;
	int size = sock->receivemessage((int)len, buff);
	if(size < 0)
	{
		int error = sock->lasterror();
		if(error == 10054)return 0;
		return -error;
	}
	return size;
}
double net_message_peek(double sockid, double len, double buffid)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(sock == NULL)return -1;
	if(buff == NULL)return -2;
	int size = sock->peekmessage((int)len, buff);
	if(size < 0)
	{
		int error = sock->lasterror();
		if(error == 10054)return 0;
		return -error;
	}
	return size;
}

double net_set_format(double sockid, double mode, char*sep)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	if(sock == NULL)return -1;
	return sock->SetFormat((int)mode, sep);
}

char* net_sender_ip()
{
	return CSocket::lastinIP();
}

double net_sender_port()
{
	return CSocket::lastinPort();
}

double net_set_sync(double sockid, double mode)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	if(sock == NULL)return -1;
	sock->setsync((int)mode);
	return 1;
}
double net_close(double sockid)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	if(sock == NULL)return -1;
	delete sock;
	sockets.set((int)sockid, NULL);
	return 1;
}

double net_last_error(double sockid)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	if(sock == NULL)return -1;
	return -sock->lasterror();
}

char* net_my_host()
{
	return CSocket::myhost();
}

double net_compare_ip(char*ip, char*mask)
{
	return CTools::comparematch(ip, mask);
}

double net_exit()
{
	CSocket::SockExit();
	return 1;
}

double net_start()
{
	CSocket::SockStart();
	return 1;
}

char* net_host_ip(char*host)
{
	return CSocket::GetIp(host);
}


double net_get_socket_id(double sockid)
{
	CSocket*sock = (CSocket*)sockets.item((int)sockid);
	if(sock == NULL)return -1;
	return sock->sockid;
}


#define DLLEXPORT
//Buffers
DLLEXPORT double writebyte(double val, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return buff->writebyte((unsigned char)val);
}
DLLEXPORT double writeshort(double val, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return buff->writeshort((short)val);
}
DLLEXPORT double writeushort(double val, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return buff->writeushort((unsigned short)val);
}
DLLEXPORT double writeint(double val, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return buff->writeint((int)val);
}
DLLEXPORT double writeuint(double val, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return buff->writeuint((unsigned int)val);
}
DLLEXPORT double writefloat(double val, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return buff->writefloat((float)val);
}
DLLEXPORT double writedouble(double val, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return buff->writedouble(val);
}
DLLEXPORT double writechars(char*str, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return buff->writechars(str);
}
DLLEXPORT double writestring(char*str, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return buff->writestring(str);
}

DLLEXPORT double readbyte(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return buff->readbyte();
}

DLLEXPORT double readshort(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return buff->readshort();
}

DLLEXPORT double readushort(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return buff->readushort();
}

DLLEXPORT double readint(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return buff->readint();
}

DLLEXPORT double readuint(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return buff->readuint();
}
DLLEXPORT double readfloat(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return buff->readfloat();
}
DLLEXPORT double readdouble(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return buff->readdouble();
}
DLLEXPORT char* readchars(double len, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return NULL;
	return buff->readchars((int)len);
}

DLLEXPORT char* readstring(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return NULL;
	return buff->readstring();
}

DLLEXPORT char* readsep(char*sep, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return NULL;
	return buff->readsep(sep);
}
DLLEXPORT double getpos(double pos, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	if(pos == 0)return buff->writepos;
	else return buff->readpos;
}

DLLEXPORT double clearbuffer(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	buff->clear();
	return 1;
}

DLLEXPORT double buffsize(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return buff->count;
}

DLLEXPORT double setpos(double pos, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	buff->readpos = (int)pos;
	buff->writepos = (int)pos;
	return pos;
}

DLLEXPORT double bytesleft(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return buff->bytesleft();
}

DLLEXPORT double createbuffer()
{
	CBuffer* buff = new CBuffer();
	return AddBuffer(buff);
}
DLLEXPORT double freebuffer(double buffid)
{
	if(buffid == 0)return 0; //no deleting default buffer
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	delete buff;
	buffers.set((int)buffid, NULL);
	return 1;
}
DLLEXPORT double copybuffer(double destinationid, double sourceid)
{
	CBuffer*destination = (CBuffer*)buffers.item((int)destinationid);
	if(destination == NULL)return 0;
	CBuffer*source = (CBuffer*)buffers.item((int)sourceid);
	if(source == NULL)return 0;
	destination->addBuffer(source);
	return 1;

}
DLLEXPORT double copybuffer2(double destinationid, double start, double len, double sourceid)
{
	CBuffer*destination = (CBuffer*)buffers.item((int)destinationid);
	if(destination == NULL)return 0;
	CBuffer*source = (CBuffer*)buffers.item((int)sourceid);
	if(source == NULL)return 0;
	destination->addBuffer(source->data+(int)start, (int)len);
	return 1;

}
//tools
DLLEXPORT char* getmacaddress()
{
	return CTools::getmacaddress();
}
DLLEXPORT char* md5string(char*str)
{
	return CTools::md5string(str);
}

DLLEXPORT char* md5buffer(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return NULL;
	return CTools::md5buffer(buff);
}

DLLEXPORT double bufferencrypt(char*pass, double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	CTools::encryptedbuffer(buff, pass);
	return 1;
}

DLLEXPORT double adler32(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return CTools::adler32(buff);
}

DLLEXPORT double bufferexists(double buffid)
{
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return 0;
	return 1;
}

DLLEXPORT double netconnected()
{
	return CTools::netconnected();
}

DLLEXPORT double fileopen(char*name, double mode)
{
	HANDLE file = CTools::BinOpen(name, (int)mode);
	if(file != NULL)
		return AddFile(file);
	return -1;
}

DLLEXPORT double fileclose(double fileid)
{
	HANDLE file = (HANDLE)files.item((int)fileid);
	if(file == NULL)return -1;
	files.set((int)fileid, NULL);
	return CTools::BinClose(file);
}

DLLEXPORT double filewrite(double fileid, double buffid)
{
	HANDLE file = (HANDLE)files.item((int)fileid);
	if(file == NULL)return -1;
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return -1;
	return CTools::BinWrite(file, buff);
}

DLLEXPORT double fileread(double fileid, double bytes, double buffid)
{
	HANDLE file = (HANDLE)files.item((int)fileid);
	if(file == NULL)return -1;
	CBuffer*buff = (CBuffer*)buffers.item((int)buffid);
	if(buff == NULL)return -1;
	return CTools::BinRead(file, (int)bytes, buff);
}
DLLEXPORT double filepos(double fileid)
{
	HANDLE file = (HANDLE)files.item((int)fileid);
	if(file == NULL)return -1;
	return CTools::BinGetPos(file);
}
DLLEXPORT double filesetpos(double fileid, double pos)
{
	HANDLE file = (HANDLE)files.item((int)fileid);
	if(file == NULL)return -1;
	return CTools::BinSetPos(file, (int)pos);
}
DLLEXPORT double filesize(double fileid)
{
	HANDLE file = (HANDLE)files.item((int)fileid);
	if(file == NULL)return -1;
	return CTools::BinFileSize(file);
}

DLLEXPORT double iptouint(char*ip)
{
	return CTools::ip2uint(ip);
}
DLLEXPORT char* uinttoip(double ip)
{
	return CTools::uint2ip((unsigned int)ip);
}
int AddBuffer(CBuffer*b)
{
	for(int i = 0; i < buffers.count; i ++)
	{
		if(buffers[i] == NULL)
		{
			buffers.set(i, b);
			return i;
		}
	}
	buffers.Add(b);
	return buffers.count-1;
}

int AddSocket(CSocket*b)
{
	for(int i = 0; i < sockets.count; i ++)
	{
		if(sockets[i] == NULL)
		{
			sockets.set(i, b);
			return i;
		}
	}
	sockets.Add(b);
	return sockets.count-1;
}

int AddFile(HANDLE b)
{
	for(int i = 0; i < files.count; i ++)
	{
		if(files[i] == NULL)
		{
			files.set(i, b);
			return i;
		}
	}
	files.Add(b);
	return files.count-1;
}

DLLEXPORT double dllInit()
{
    CSocket::SockStart();
    buffers.Add(new CBuffer());
    sockets.Add(new CSocket()); //dummy socket
    return 1;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
    {
        case DLL_PROCESS_DETACH:
			CSocket::SockExit();
			for(int i = 0; i < buffers.count; i++)
				if(buffers[i] != NULL)
					delete (CBuffer*)buffers.item(i);
			for(int i = 0; i < sockets.count; i++)
				if(sockets[i] != NULL)
					delete (CSocket*)sockets.item(i);
			for(int i = 0; i < files.count; i ++)
				if(files[i] != NULL)
					CTools::BinClose((HANDLE)files.item(i));
            break;
    }
    return TRUE;
}

