#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
string Ack = "ACK";

void receiveTransmission();
string AckAndNakker(string);

int main(int argc, int argv[])
{
	receiveTransmission();
	return 0;
}

void receiveTransmission()
{
	int lineNo = 0;
	//vector<string> lines;
	string currentLine;
	try
	{
		ClientSocket clientListener("localhost", 30000);
		ClientSocket clientSender("localhost", 30001);
		string reply, request;
		request = "Request";
		clientSender << request;
		clientListener >> reply;
		clientSender << AckAndNakker(reply);

		while(reply!="EOF")
		{
			try
			{
				if(AckAndNakker(reply) == "ACK")
				{
					//cout<<currentLine;
					//cout<<lineNo<<": "<< "\t" << reply << "\n";
					if(reply!="EOL0")
					{
						//lines.push_back(reply);
						reply.erase(reply.length()-1, reply.length());
						currentLine = currentLine+reply;
					}
					else if(reply=="EOL0")
					{
						//cout<<lineNo<<": "<< "\t" << currentLine << "\n";
						//currentLine + '\n';
						cout<<lineNo<<": "<< "\t" << currentLine << "\n";
						lineNo++;
						currentLine="";
					}
					//cout<<lineNo<<": "<< "\t" << currentLine << "\n";
					//lineNo++;
				}
				clientListener >> reply;
				clientSender << AckAndNakker(reply);
			}
			catch(SocketException&){}
		}
	}
	catch(SocketException& e){cout<<"Exception was caught:"<<e.description()<<"\n";}
}

string AckAndNakker(string reply)
{	
	int temp=0;
	int parity = 0;
	for(int i = 0; i<reply.length()-1;i++)
	{
		temp += reply.at(i);
	}
	if(temp % 2 == 0 && reply.at(reply.length()-1)=='0')
	{
		//cout<<"ACK!";
		return "ACK";
	}
	else if(temp % 2 == 1 && reply.at(reply.length()-1)=='1')
	{
		//cout<<"ACK!";
		return "ACK";
	}
	else
	{
		//cout<<"NAK!";
		return "NAK";//"NAK";
	}
}