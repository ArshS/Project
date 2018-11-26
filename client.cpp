#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>

using namespace std;
string Ack = "ACK";

int main(int argc, int argv[])
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
	return 0;

}
/*
int main(int argc, int argv[])
{
   try{
		// Replace "localhost" with the hostname
		// that you're running your server.
			//for data
		ClientSocket clientListener("localhost", 30000);
		//for acks and naks
		ClientSocket clientSender("localhost" , 30001);
		string reply;
		string request;
		request= "gimmee";
		// Usually in real applications, the following
		// will be put into a loop. 
		clientSender << request;
		clientListener >> reply;
		clientSender << Ack;
		while(reply!="EOF"){
			try {
		 		//client_socket << "Test message.";
		  		cout << reply << "\n";
		 		clientListener >> reply;
		 		clientSender << Ack;
		 		//clientListener >> reply;
		  	}
		  	catch(SocketException&){}
		}
	}
  	catch(SocketException& e){
  	std::cout << "Exception was caught:" << e.description() << "\n";}
  return 0;
}
*/