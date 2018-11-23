#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>

int main(int argc, int argv[])
{
   try{
      // Replace "localhost" with the hostname
      // that you're running your server.
   	  //for data
      ClientSocket clientListener("localhost", 30000);
      //for acks and naks
      ClientSocket clientSender("localhost" , 30001);
      std::string reply;
      std::string request;
      request= "gimmee";
      // Usually in real applications, the following
      // will be put into a loop. 
      clientSender << request;
      while(request == "gimmee"){
      try {
	 	//client_socket << "Test message.";

	 	clientListener >> reply;
	 	std::string Ack;
	 	Ack = "ACK";
	 	clientSender << Ack;
	 	//clientListener >> reply;
      }
      catch(SocketException&){
      }
      std::cout << "We received this response from the server:\n\"" << reply << "\"\n";;
  	}
   }
   catch(SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\n";
   }

   return 0;
}

/*
#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
//using namespace std;

int main()
{
   try{
      // Replace "localhost" with the hostname
      // that you're running your server.
      ClientSocket client_socket("localhost", 30002);
      std::string reply;
      //std::stringstream s;
      // Usually in real applications, the following
      // will be put into a loop. 
      int num=0;
      std::string test="";
      //std::string tmp=std::to_string(num);
      while(true){

      	//s<<num;
      	//s>>tmp;
      	test="Test message # ";
      	num++;
	      try {
		 client_socket << test;
		 client_socket >> reply;
	      }
	      catch(SocketException&){
	      }
	      std::cout << "We received this response from the server:\n\"" << reply << "\"\n";;
	   }
	}
   catch(SocketException& e){
      std::cout << "Exception was caught:" << e.description() << "\n";
   }


   return 0;
}
*/