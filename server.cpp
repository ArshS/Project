#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <pthread.h>

using namespace std;

std::string Ack2 = "ACK";   

#define NTHREADS 5
int cThread=0;

void *sendTransmission(void *);
vector<string> frameBuilder(ifstream&);
vector<string> parityGetter(vector<string>);
string swapParity(string);
string convert(int);
vector<string> openFile(string);

ServerSocket serverSender(30000);
ServerSocket serverListener(30001);

int main()
{
	int *x;
	cout<<"Initializing..."<<endl;
	sendTransmission(x);

	return 0;
}

void *sendTransmission(void *args)
{
	
	std::cout << "Running thread: "<<cThread<<endl;

	ServerSocket data_sock;
	ServerSocket ack_sock;

	string data;
	while(true)
	{
	   	try{
			serverSender.accept(data_sock);
			serverListener.accept(ack_sock);

			std::string request, file;

			//cout<<"Getting file..."<<endl;
			ack_sock >> file;
			//cout<<"Got file."<<endl;

			pthread_t thread_id[NTHREADS];
			while(!(cThread<NTHREADS))
			{
				cout<<"Waiting for free thread..."<<endl;
			}
			if(cThread<NTHREADS)
			{
				pthread_create(&thread_id[cThread], NULL, &sendTransmission ,NULL);
				cThread++;
			}

			vector<string> frames = openFile(file);
			//sleeping to view the thread id and file request on server side
			sleep(3);
			for(int i = 0; i<frames.size(); i++)
			{
				data = frames[i];
				try{
			    	std::string ack;
			    
			        data_sock << data;
			        ack_sock >> ack;

			        if(ack.compare(Ack2) == 0)
			        {
			        	cout<<"Got ACK!"<<endl;
			        }
			        else if(ack.compare(Ack2)!=0)
			        {
			        	while (ack.compare(Ack2)!=0)
			        	{
			        		cout<<"Got NAK! Retransmitting..."<<endl;
			     			data = swapParity(data);
			        		data_sock << data;
			       			ack_sock >> ack;
			       		}
			        }
			    }
			    catch(SocketException&){}
			}
		}catch (SocketException& e){ std::cout << "Exception was caught:" << e.description() << "\nExiting.\n"; }
		data="EOF";
		data_sock << data;
		cThread--;
	}
}

vector<string> openFile(string file)
{
	vector<string> frames;
	vector<string> frames2;
	ifstream infile;

	if(file == "file.txt")
   	{
		cout<<"Got request for file 1"<<endl;
		infile.open("file.txt");
		frames2 = frameBuilder(infile);
		frames = parityGetter(frames2);
	}
	else if(file == "big.txt")
   	{
		cout<<"Got request for big file"<<endl;
		infile.open("big.txt");
		frames2 = frameBuilder(infile);
		frames = parityGetter(frames2);
	}
	else if(file == "medium.txt")
   	{
		cout<<"Got request for medium file"<<endl;
		infile.open("medium.txt");
		frames2 = frameBuilder(infile);
		frames = parityGetter(frames2);
	}
	else
   	{
		cout<<"Got invalid filepath. Processing default..."<<endl;
		infile.open("file.txt");
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

