#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>

using namespace std;
string Ack = "ACK";

void receiveTransmission();

int main(int argc, int argv[])
{
	receiveTransmission();
	return 0;
}

void receiveTransmission()
{
	try
	{
		ClientSocket clientListener("localhost", 30000);
		ClientSocket clientSender("localhost", 30001);
		string reply, request;
		request = "Request";
		clientSender << request;
		clientListener >> reply;
		clientSender << Ack;
		while(reply!="EOF")
		{
			try
			{
				cout<<reply<<"\n";
				clientListener >> reply;
				clientSender << Ack;
			}
			catch(SocketException&){}
		}
	}
	catch(SocketException& e){cout<<"Exception was caught:"<<e.description()<<"\n";}
}