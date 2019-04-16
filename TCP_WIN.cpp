// TCP Client Socket for Windows
// Connects to AIBracket.com
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int BUFFER_SIZE = 2048;
const int PORT = 8000;
// Put your private key here
const string PRIVATE_KEY = "";
const string IP_ADDRESS = "68.55.70.40";

void connectBot(SOCKET& sock);
string receivebuffer(SOCKET& sock, char buff[]);

int main()
{
	// Init socket
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	
	if (wsResult != 0)
	{
		cerr << "Error " << wsResult << ": Can't start winsocket.\n";
		return -1;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Error " << WSAGetLastError() << ": Can't create socket\n";
		WSACleanup();
		return -2;
	}

	// Fill in hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(PORT);
	inet_pton(AF_INET, IP_ADDRESS.c_str(), &hint.sin_addr);

	// Connect to server
	int connectResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connectResult == SOCKET_ERROR)
	{
		cerr << "Error " << WSAGetLastError() << ": Can't connect to server.\n";
		closesocket(sock);
		WSACleanup();
		return -3;
	}

	char buff[BUFFER_SIZE];
	string result;
	if (!connectBot(sock))
	{
		return -4;
	}
	
	cout << receivebuffer(sock, buff) << endl;

	closesocket(sock);
	WSACleanup();

	return 0;
}

bool connectBot(SOCKET& sock)
{
	cout << "Attempting to connect bot...\n";
	string input = "BOT " + PRIVATE_KEY;
	if (send(sock, input.c_str(), input.size() + 1, 0) == SOCKET_ERROR)
	{
		cerr << "Error " << WSAGetLastError() << ": Connection to bot failed\n";
		char buffer[BUFFER_SIZE];
		cerr << "Server says >" << receivebuffer(sock, buffer) << endl;
		return false;
	}
	return true;
}

string receivebuffer(SOCKET & sock, char buff[])
{
	ZeroMemory(buff, BUFFER_SIZE);

	int bytesReceived = recv(sock, buff, BUFFER_SIZE, 0);

	if (bytesReceived > 0)
	{
		return string(buff, 0, bytesReceived);
	}

	return "";
}
