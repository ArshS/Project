#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <pthread.h>
//#include <thread>

using namespace std;
std::string Ack2 = "ACK";   

struct Arg
{
 	ServerSocket dataptr;
 	ServerSocket ackptr;
};

//#define NTHREADS 1 
#define NTHREADS 5
int p=0;

void *sendTransmission(void *);
vector<string> frameBuilder(ifstream&);
vector<string> parityGetter(vector<string>);
string swapParity(string);
string convert(int);
vector<string> openFile(string);

//ServerSocket data_sock;
//ServerSocket ack_sock;
//data
ServerSocket serverSender(30000);
//ack
ServerSocket serverListener(30001);
//serverSender.accept(data_sock);
//		serverListener.accept(ack_sock);

int main()
{
	int *x;

	sendTransmission(x);

	return 0;
}

void *sendTransmission(void *args)
{
	//ifstream infile;
	//infile.open("/home/sina3615/CPSC3780Project/Project/file.txt");
	
	std::cout << "running....\n";
	//Arg a1;
	
	//serverSender.accept(data_sock);
		//serverListener.accept(ack_sock);
	ServerSocket data_sock;
	ServerSocket ack_sock;

	//data_sock = &args.dataptr;
	//ack_sock = &args.ackptr;

	//vector<string> frames;
	//vector<string> frames2;
	//frames2 = frameBuilder(infile);
	//frames = parityGetter(frames2);

	cout<<"before loop"<<endl;
	string data;
	while(true)
	{
   	try{
  		//data
   		//ServerSocket serverSender(30000);
   		//ack
		//ServerSocket serverListener(30001);
		serverSender.accept(data_sock);
		serverListener.accept(ack_sock);

	 	// For multiple threading, you need to create
	 	// a new thread here and pass data_sock to it.
	 	// The thread will use data_sock to communicate
		std::string request, file;
		//ack_sock >> file;
		cout<<"getting file"<<endl;
		ack_sock >> file;
		cout<<"got file"<<endl;

		pthread_t thread_id[NTHREADS];
		//int p=0;
		while(!(p<NTHREADS))
		{
			cout<<"waiting for free thread..."<<endl;
		}
		if(p<NTHREADS)
		{
			pthread_create(&thread_id[p], NULL, &sendTransmission ,NULL);
			p++;
		}

		vector<string> frames = openFile(file);
		//send(frames, data_sock, ack_sock, data);
		for(int i = 0; i<frames.size(); i++)
		{
			data = frames[i];
			try{
		    	std::string ack;
		    
		        data_sock << data;
		        //arg->data_sock << data; 
		
		        ack_sock >> ack;
		        //arg->ack_sock >> ack;

		        if(ack.compare(Ack2) == 0)
		        {
		        	//cout<<"Got ack!"<<endl;
		        }
		        else if(ack.compare(Ack2)!=0)
		        {
		        	while (ack.compare(Ack2)!=0)
		        	{
		        		//cout<<"Got NAK! Retransmitting..."<<endl;
		     			data = swapParity(data);
		        		data_sock << data;
		        		//arg->data_sock << data; 
		       			ack_sock >> ack;
		       			//arg->ack_sock >> ack;
		       		}
		        }
		    }
		    catch(SocketException&){}
		}
	}catch (SocketException& e){std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";}
	data="EOF";
	data_sock << data;
	p--;
	//arg->data_sock << data; 
}
}

vector<string> openFile(string file)
{
	vector<string> frames;
	vector<string> frames2;
	ifstream infile;

	char filepath[file.size()];
	for(int i=0; i<file.size(); i++)
		filepath[i]=file[i];

	if(file == "/home/sina3615/CPSC3780Project/Project/file.txt")
   	{
		cout<<"Got request for file 1"<<endl;
		infile.open(filepath);
		// /home/sina3615/CPSC3780Project/Project/file.txt
		frames2 = frameBuilder(infile);
		frames = parityGetter(frames2);
	}
	else if(file == "/home/sina3615/CPSC3780Project/Project/big.txt")
   	{
		cout<<"Got request for file 2"<<endl;
		infile.open(filepath);
		frames2 = frameBuilder(infile);
		frames = parityGetter(frames2);
	}
	else//(file == "/home/sina3615/CPSC3780Project/Project/file.txt")
   	{
		cout<<"Got invalid filepath. Processing default..."<<endl;
		infile.open("/home/sina3615/CPSC3780Project/Project/file.txt");
		frames2 = frameBuilder(infile);
		frames = parityGetter(frames2);
	}
	return frames;
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

string convert(int v)
{
	std::ostringstream oss;
    oss << v;
    return oss.str();
}

vector<string> frameBuilder(ifstream& infile)
{
	vector<string> f;
	string data;
	while(getline(infile, data))
	{
		if(data.length()==0)
		{
			f.push_back("\n");
		}

		for(int i=0; i<data.length(); i+=64)
		{
			if((data.length()-i) < 64)
			{
				f.push_back(data.substr(i, (data.length()-i)));
			}
			else
			{
				f.push_back(data.substr(i,64));	
			}
		}
		f.push_back("EOL");
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

