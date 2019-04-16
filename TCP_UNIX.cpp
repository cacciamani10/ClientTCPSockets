// TCP Client Socket for UNIX/Mac 
// Connects to AIBracket.com
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>

using namespace std;

const int BUFFER_SIZE = 2048;
const int PORT = 8000;
// Put your private key here
const string PRIVATE_KEY = "";
const string IP_ADDRESS = "68.55.70.40";

bool connectBot(int& sock);
string receivebuffer(int& sock, char buff[]);

int main()
{
	// Init socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		return -1;
	}

	// Create hint
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(PORT); // Big Endian
	inet_pton(AF_INET, IP_ADDRESS.c_str(), &hint.sin_addr);

	// Connect to server
	if (connect(sock, (sockaddr*)&hint, sizeof(hint)) < 0)
	{
		return -2;
	}

	// Connect to bot
	if (!connectBot(sock))
	{
		return -3;
	}

	// Receive data
	char buff[BUFFER_SIZE];
	cout << receivebuffer(sock, buff) << endl;

	// Close socket
	close(sock);
	return 0;
}

bool connectBot(int& sock)
{
	cout << "Attempting to connect bot...\n";
	string input = "BOT " + PRIVATE_KEY;
	if (send(sock, input.c_str(), input.size() + 1, 0) < 0)
	{
		return false;
	}
	return true;
}

string receivebuffer(int& sock, char buff[])
{
	memset(buff, 0, BUFFER_SIZE);

	int bytesReceived = recv(sock, buff, BUFFER_SIZE, 0);

	if (bytesReceived > 0)
	{
		return string(buff, 0, bytesReceived);
	}

	return "";
}