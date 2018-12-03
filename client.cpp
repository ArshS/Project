#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//for ACK string
string Ack = "ACK";

void receiveTransmission();
string AckAndNakker(string);

int main(int argc, int argv[])
{
	//to receive transmissions
	receiveTransmission();
	return 0;
}

void receiveTransmission()
{
	//line number counter
	int lineNo = 0;
	//contains all of the frames that make up the current line
	string currentLine;
	try
	{
		//get ip
		string IP;
		cout<<"Please enter server IP Address."<<endl;
		cin>>IP;
		//setup the ports with the IP address
		ClientSocket clientListener(IP,30000);//"localhost", 30000);
		ClientSocket clientSender(IP,30001);//"localhost", 30001);

		//for reply which are the frames, and file for the initial request 
		//and the file path
		string reply, file;
		
		//ask for file input
		cout<<"Enter Datapath request for File: "<<endl;
		cin >> file;

		//send the request which is the file datapath
		clientSender << file;

		//wait for reply from server
		clientListener >> reply;
		//checks the parity bit to see if the parity is correct 
		//for sum of the ascii values
		clientSender << AckAndNakker(reply);

		//checking the EOF string we added as a flag to stop replies
		//EOF is a frame by itself
		while(reply!="EOF")
		{
			try
			{	//if the reply came back as an ACK
				if(AckAndNakker(reply) == "ACK")
				{	
					//we check the EOL string we added at the end 
					//as a flag and if it is not then we erase the 
					//last bit which is the parity bit
					//we do this for all of the frames in the line
					if(reply!="EOL0")
					{
						//remove the parity bit
						reply.erase(reply.length()-1, reply.length());
						//add to the currentline string
						currentLine = currentLine + reply;
					}
					//we the check the EOL string we added at the end
					//as a flag and if it is then we print the currentline
					//because we move onto to process the next line
					else if(reply=="EOL0")
					{
						cout<<lineNo<<": "<< "\t" << currentLine << "\n";
						lineNo++;
						//reset the line
						currentLine="";
					}
				}
				//get reply
				clientListener >> reply;
				//checking the parity bit like above
				clientSender << AckAndNakker(reply);
			}
			catch(SocketException&){}
		}
	}
	catch(SocketException& e){cout<<"Exception was caught:"<<e.description()<<"\n";}
}

//function the takes in the reply and determines if the parity bit is correct
//for the sum of ascii values of the frame
string AckAndNakker(string reply)
{	
	int temp=0;
	int parity = 0;

	//get the frame and set it to temp
	for(int i = 0; i<reply.length()-1;i++)
	{
		temp += reply.at(i);
	}
	//if the parity is correct for even parity send an ack
	if(temp % 2 == 0 && reply.at(reply.length()-1)=='0')
	{
		return "ACK";
	}
	//if the parity is correct for odd parity send an ack
	else if(temp % 2 == 1 && reply.at(reply.length()-1)=='1')
	{
		return "ACK";
	}
	//send a NAK if the above cases are not correct
	else
	{
		return "NAK";
	}
}