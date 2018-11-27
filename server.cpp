#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <fstream>
#include <vector>

using namespace std;
std::string Ack2 = "ACK";   

void sendTransmission(ifstream& infile);
vector<string> frameBuilder(ifstream&);
vector<string> parityGetter(vector<string>);
string swapParity(string);
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
	vector<string> frames2;
	frames2 = frameBuilder(infile);
	frames = parityGetter(frames2);
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
			//while(getline(infile, data))
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
			        else if(ack.compare(Ack2)!=0)
			        {
			        	cout<<"IN NAKED"<<endl;
			        	while (ack.compare(Ack2)!=0)
			        	{
			        		cout<<"Got NAK!"<<endl;
			     			data = swapParity(data);
			        		data_sock << data;
			       			ack_sock >> ack;
			       		}
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

string swapParity(string data)
{
	string temp;
	if(data.at(data.length()-1) == '0')
	{
		temp = data.substr(0,data.size()-1);
		temp.append("1");
	}
	if(data.at(data.length()-1) == '1')
	{
		temp = data.substr(0,data.size()-1);
		temp.append("0");
	}

	return temp;
}

vector<string> frameBuilder(ifstream& infile)
{
	vector<string> f;
	string data;
	int step=0;
	while(getline(infile, data))
	{
		if(data.length()==0)
		{
			f.push_back("\n");
		}

		for(int i=0; i<data.length(); i+=64)
		{
			cout<<"len"<<data.length()<<endl;
			if((data.length()-i) < 64)
			{
				f.push_back(data.substr(i, (data.length()-i)));
			}
			else
			{
				f.push_back(data.substr(i,64));
			}
		}
		cout<<"in here"<<endl;
	}
	return f;
}

vector<string> parityGetter(vector<string> f)
{
	for(int i=0; i<f.size(); i++)
	{
		//case for good parity
		if(i%5 != 0)
		{
			int temp=0;
			for(int j=0; j<f[i].length(); j++)
			{
				temp+=f[i].at(j);
			}

			//even
			if(temp %2 ==0)
			{
				f[i].append("0");
			}
			//odd
			else
			{
				f[i].append("1");
			}
		}
		//case for simulating bad parity
		else
		{
			int temp=0;
			for(int j=0; j<f[i].length(); j++)
			{
				temp+=f[i].at(j);
			}

			//even
			if(temp %2 ==0)
			{
				f[i].append("1");
			}
			//odd
			else
			{
				f[i].append("0");
			}
		}
	}
	return f;
}

