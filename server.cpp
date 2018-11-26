#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <fstream>
#include <vector>

using namespace std;
std::string Ack2 = "ACK";   

void sendTransmission(ifstream& infile);
vector<string> lineReader(ifstream&);

int main()
{	
	ifstream infile;
	infile.open("/home/sina3615/CPSC3780Project/Project/file.txt");
   	sendTransmission(infile);
	return 0;
}

void sendTransmission(ifstream& infile)
{
	std::cout << "running....\n";
	//ifstream infile;
   	//infile.open("/home/sina3615/CPSC3780Project/Project/file.txt");
	ServerSocket data_sock;
	ServerSocket ack_sock;
	vector<string> frames;
	frames = lineReader(infile);
	string data;
   	try{
  		//data
   		ServerSocket serverSender(30000);
   		//ack
		ServerSocket serverListener(30001);
		serverSender.accept(data_sock);
		serverListener.accept(ack_sock);

	 	// For multiple threading, you need to create
	 	// a new thread here and pass data_sock to it.
	 	// The thread will use data_sock to communicate
		std::string request;
		ack_sock >> request;
	 	// with the client.
   		if(request == "Request"){// "gimmee"){
			cout<<"Got request"<<endl;
			//while(getline(infile, data)){
			//for(vector<string>::iterator it = frames.begin(); it != frames.end(); ++it)
			for(int i = 0; i<frames.size(); i++)
			{
				//data += '\n';
				data = frames[i];
				try{
			    //while (true){
			       //data ="Hello";
			    	std::string ack;
			     	//data ="Hello\n";
			        data_sock << data;
			
			        ack_sock >> ack;

			        if(ack.compare(Ack2) == 0)
			        {
			        	//data_sock << data;
			        	cout<<"Got ack!"<<endl;
			        }
			    }
			    catch(SocketException&){}
		}
	}
	}catch (SocketException& e){
		std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
	}
	data="EOF";
	data_sock << data;
}

vector<string> lineReader(ifstream& infile)
{
	vector<string> frames;
	string data;
	int step=0;
	while(getline(infile, data))
	{
		for(int i=0; i<data.length(); i+65)
		{
			if((data.length()-i)<64)
			{
				frames.push_back(data.substr(i, (data.length()-i)));
			}
			else
			{
				frames.push_back(data.substr(i,64));
			}
		}
		//data+='\n';
		//frames.push_back(data.substr(i, 64));

		//step=step+65;
	}

	return frames;
}

