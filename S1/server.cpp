#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <pthread.h>
#include <stdio.h>     
#include <stdlib.h>     
#include <time.h>

using namespace std;

//to check the ack from the client
std::string Ack2 = "ACK";   

//5 threads
#define NTHREADS 1
//the thread id and current thread
int cThread=0;

void *sendTransmission(void *);
vector<string> frameBuilder(ifstream&);
vector<string> parityGetter(vector<string>);
string swapParity(string);
string convert(int);
vector<string> openFile(string);

//initializing the ports once 
//and done globally
ServerSocket serverSender(30000);
ServerSocket serverListener(30001);

int main()
{
	//initializing main and passing a 
	//dummy ptr into the function
	int *x;
	cout<<"Initializing..."<<endl;
	sendTransmission(x);

	return 0;
}

//send transmission function to send a frame 
//with correct parity and checks for acks and
//naks. Resends if nak detected
void *sendTransmission(void *args)
{
	//thread id is printed and current thread is running	
	std::cout << "Running thread: "<<cThread<<endl;

	//new data socket and ack socket 
	//for each thread
	ServerSocket data_sock;
	ServerSocket ack_sock;

	//the actual data string that sends the frames
	string data;

	//to ensure we have the server running
	while(true)
	{
	   	try{
	   		//accept the sockets
			serverSender.accept(data_sock);
			serverListener.accept(ack_sock);

			//for the file request
			std::string file;

			//wait for the filepath
			ack_sock >> file;

			//setup the thread
			pthread_t thread_id[NTHREADS];

			//to ensure that we do not have more than 5 threads
			//running, so we wait for the while to break out once
			//we finish processing the threads
			while(!(cThread<NTHREADS))
			{
				//takes too long so we comment out the statement
				//cout<<"Waiting for free thread..."<<endl;
			}
			//if the thread is less than 5 then we create a thread
			if(cThread<NTHREADS)
			{
				pthread_create(&thread_id[cThread], NULL, &sendTransmission ,NULL);
				cThread++;
			}

			//create the vector of frames through the openfile function 
			//with the filepath passed in
			vector<string> frames = openFile(file);

			//sleeping to view the thread id and file request on server side
			//uncomment if you want to have delay for the messages
			//sleep(3);

			//for sending the frames 
			for(int i = 0; i<frames.size(); i++)
			{
				//data(string) is the frame
				data = frames[i];
				try{
					//for ack
			    	std::string ack;
			    
			    	//send out the frame
			        data_sock << data;
			        //wait for ack
			        ack_sock >> ack;

			        //check if the ack is correct 
			        //and if it is then print out 
			        //got ack msg
			        if(ack.compare(Ack2) == 0)
			        {
			        	cout<<"Got ACK!"<<endl;
			        }
			        //else we have a nak and we
			        //retransmit the frame
			        else if(ack.compare(Ack2)!=0)
			        {
			        	while (ack.compare(Ack2)!=0)
			        	{
			        		cout<<"Got NAK! Retransmitting..."<<endl;
			        		//we swap the parity,resend 
			        		//and wait for the ack
			     			data = swapParity(data);
			        		data_sock << data;
			       			ack_sock >> ack;
			       		}
			        }
			    }
			    catch(SocketException&){}
			}
		}catch (SocketException& e){ std::cout << "Exception was caught:" << e.description() << "\nExiting.\n"; }
		//EOF string added to end of data to ensure that we end the transmission
		//it is a flag string 
		data="EOF";
		data_sock << data;
		//decrement the thread once done to allow for more threads to be 
		//created once we reach less than 5 threads
		cThread--;
	}
}

//function to open the file, the process the frames and build 
//them and finally return a vector of string that contains all
//of the frames for that line in the file
vector<string> openFile(string file)
{
	vector<string> frames;
	vector<string> frames2;
	ifstream infile;

	//check filepath and since it is 
	//local we do not need the filepath
	if(file == "small.txt")
   	{
   		//output a msg and open the file
		cout<<"Got request for small file."<<endl;
		infile.open("small.txt");
		//build all of the frames through
		//frame builder builds all of the frames 
		frames2 = frameBuilder(infile);
		//attach the parity for those frames 
		//through parity getter function
		frames = parityGetter(frames2);
	}
	//similar to above case but for big.txt file
	else if(file == "big.txt")
   	{
		cout<<"Got request for big file."<<endl;
		infile.open("big.txt");
		frames2 = frameBuilder(infile);
		frames = parityGetter(frames2);
	}
	//similar to above case but for medium.txt file
	else if(file == "medium.txt")
   	{
		cout<<"Got request for medium file."<<endl;
		infile.open("medium.txt");
		frames2 = frameBuilder(infile);
		frames = parityGetter(frames2);
	}
	//default case if the filepath is not given
	else
   	{
		cout<<"Got invalid filepath. Processing default..."<<endl;
		infile.open("small.txt");
		frames2 = frameBuilder(infile);
		frames = parityGetter(frames2);
	}
	return frames;
}

//function to swap the parity in case 
//the data transmitted has an error(done every 5 frames)
//and a nak is returned
string swapParity(string data)
{
	string temp;
	//remove the 0 and append 1 for the correct parity
	if(data.at(data.length()-1) == '0')
	{
		temp = data.substr(0,data.size()-1);
		temp.append("1");
	}
	//remove the 1 and append 0 for the correct parity
	if(data.at(data.length()-1) == '1')
	{
		temp = data.substr(0,data.size()-1);
		temp.append("0");
	}
	return temp;
}

//function to convert a int to a string
string convert(int v)
{
	std::ostringstream oss;
    oss << v;
    return oss.str();
}

//function to build all of the frames
//it takes in the file and uses getline to get
//of the data and then adds to the vector in increments 
//of 64. If its less than 64 we pushback from i to the length
//of the line in order to get frames with characters less
//than 64. If the line is a empty line we add a \n to the vector
vector<string> frameBuilder(ifstream& infile)
{
	vector<string> f;
	string data;
	//get the line and until we are finished we process each line
	while(getline(infile, data))
	{
		//add a \n if the data is an empty line
		if(data.length()==0)
		{
			f.push_back("\n");
		}

		//loop for the length of the line and increment
		//by 64 to move onto the next set of characters in the line
		//because if there are more than 64 characters we do that 
		for(int i=0; i<data.length(); i+=64)
		{
			//case where there are less than 64 characters
			//we push back from length to length-i
			if((data.length()-i) < 64)
			{
				f.push_back(data.substr(i, (data.length()-i)));
			}
			//case where are are 64 characters or more than 64 characters
			//we pushback from i to 64
			else
			{
				f.push_back(data.substr(i,64));	
			}
		}
		//EOL flag to determine if we are finished with the line
		//because there can be multiple frames in a line
		f.push_back("EOL");
	}
	return f;
}

//function that takes in the vector of frames built 
//from framebuilder and adds the parity bit for each frame
vector<string> parityGetter(vector<string> f)
{
	int randi;
	time_t t;
	srand((unsigned) time(&t));
	for(int i=0; i<f.size(); i++)
	{
		if(i%5==0)
		{
			randi=rand();
		}
		
		if(randi%5 != 0)
		{
			int temp=0;
			//loop through the frame and add it up
			for(int j=0; j<f[i].length(); j++)
			{
				temp+=f[i].at(j);
			}
			//for even parity we append a 0 to the vector
			if(temp %2 ==0)
			{
				f[i].append("0");
			}
			//for odd parity we append a 1 to the vector
			else
			{
				f[i].append("1");
			}
		}

		//case for simulating bad parity
		else
		{
			int temp=0;
			//loop through the frame and add it up
			for(int j=0; j<f[i].length(); j++)
			{
				temp+=f[i].at(j);
			}

			//for even parity we append a 1 to the vector
			if(temp %2 ==0)
			{
				f[i].append("1");
			}
			//for odd parity we append a 0 to the vector
			else
			{
				f[i].append("0");
			}
		}
		randi++;
	}
	return f;
}

