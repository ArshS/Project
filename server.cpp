#include "ServerSocket.h"
#include "SocketException.h"
#include <string>

using namespace std;
std::string ack2 = "ACK";   
 
int main(int argc, int argv[])
{

   std::cout << "running....\n";

   try{
	      // Create the socket
   		//data
	   	ServerSocket serverSender(30000);
	   	//acks and naks
	   	ServerSocket serverListener(30001);


	    while (true){
		 ServerSocket data_sock;
		 ServerSocket ack_sock;
		 serverSender.accept(data_sock);
		 serverListener.accept(ack_sock);
	
		 // For multiple threading, you need to create
		 // a new thread here and pass data_sock to it.
		 // The thread will use data_sock to communicate
		 std::string request;
		 ack_sock >> request;
		 // with the client.
		 if(request == "gimmee")
		 {
		 	cout<<"Got request"<<endl;
		 	//server.accept(ack_sock);
		 
		 try{
		    while (true){
		       std::string data ="Hello";
		       std::string ack;
		     
		       data_sock << data;
		
		       ack_sock >> ack;
		       if(ack.compare(ack2) == 0)
		       {
		        	//data_sock << data;
		        	cout<<"GOT ack!"<<endl;
		       }
		    }
		 }
		 catch(SocketException&){
		 }
	      }
	   } 
	}
	catch (SocketException& e){
	std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
   }
  return 0;
}

/*
#include "ServerSocket.h"
#include "SocketException.h"
#include <string>

using namespace std;
 
int main(int argc, int argv[])
{
   std::cout << "running....\n";
   try{
      // Create the socket
     ServerSocket server(30002);

     while (true){
	 	ServerSocket data_sock;
	 	server.accept(data_sock);

		 // For multiple threading, you need to create
		 // a new thread here and pass data_sock to it.
		 // The thread will use data_sock to communicate
		 // with the client.
		try{
		    while (true){
		       		string data="poopface";
		       		data_sock >> data;
		    	   data_sock << data;
			    }
			}
	 		catch(SocketException&){
		}
      }
      //server.closesocket();
   }
   catch (SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
   }

  return 0;
}
*/