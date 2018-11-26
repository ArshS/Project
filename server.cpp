#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <fstream>
#include <vector>

using namespace std;
std::string Ack2 = "ACK";   

int main(){	
	std::cout << "running....\n";
	ServerSocket data_sock;
	ServerSocket ack_sock;
   	ifstream infile;
   	infile.open("/home/sina3615/CPSC3780Project/Project/file.txt");
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
			while(getline(infile, data)){
				data += '\n';
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
			        	cout<<"GOT ack!"<<endl;
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
	return 0;
}

