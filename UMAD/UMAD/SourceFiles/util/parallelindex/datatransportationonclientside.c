#include "clienttransport.h"

void
s_pipfun(int signo)
{
	cerr << "signo:" << signo << ", the server does not listen on the target port." << endl;
	return;
}

//send vector data set to every server
int
sendvectorrawdata(int *serverlist, int servernum, char *filename, int size)
{
	char buffer[MAXLINE];
	
	char *nfilename = "../data/";
	joinCharArray(nfilename, filename);

	fstream ifs(nfilename);
	if (!ifs)
	{
		cout << "Failed to open file " << filename << endl;
		return -1;
	}

	bzero(buffer, MAXLINE);
	ifs.getline(buffer, MAXLINE);
	//cout << "first line:" << buffer << endl;
	
	for (int i=0; i<servernum; i++)
	{
		bzero(buffer, MAXLINE);
		read(serverlist[i], buffer, MAXLINE);

		if (strcmp(buffer, filename) != 0)
		{
			cout << "There is no file names " << buffer << " located in this client" << endl;
			return -1;
		}

		//sleep(5);

		int nsize = 0;
		if (i == servernum - 1)
		{
			nsize = size;
		}
		else
		{
			nsize = (i + 1) * (size / servernum);
		}

		//TODO: 每一次循环结束都记录当前的location，供下次使用。
		//for (int j=0; j<i * (size / servernum); j++)
		//{
		//	ifs.getline(buffer, MAXLINE);
		//}

		//cout << "server[" << i << "] start:" << i * (size / servernum) << " end:" << nsize << endl;
		
		int senddn = 0;
		for (int j=i * (size / servernum); j<nsize; j++)
		{
			bzero(buffer, MAXLINE);
			ifs.getline(buffer, MAXLINE);

			if (strlen(buffer) >= MAXLINE)
			{
				cout << "buffer overflow" << endl;
				return -1;
			}
			else
			{
				buffer[strlen(buffer)] = '\n';
			}

			if (strlen(buffer) > 0)
			{
				int len = write(serverlist[i], buffer, strlen(buffer));
				if (len != strlen(buffer))
				{
					cout << "Failed to write data to server " << i << endl;
					return -1;
				}
				senddn++;
			}
			else
			{
				j--;
				continue;
			}
		}
		cout << "Number of data line sent to server " << i << " is: " << senddn << ". File location now is:" << ifs.tellg() << endl;

		sleep(1);
		bzero(buffer, MAXLINE);
		strcpy(buffer, "over");
		write(serverlist[i], buffer, strlen(buffer));
	}
	
	ifs.close();
}


///send vector data set to every server
int
sendvectorquerydata(int *serverlist, int servernum, char *filename, int num)
{
	char buffer[MAXLINE];

	char *nfilename = "../data/";
	joinCharArray(nfilename, filename);
	
	fstream ifs(nfilename);
	if (!ifs)
	{
		cout << "Failed to open file " << filename << endl;
		return -1;
	}


	bzero(buffer, MAXLINE);
	ifs.getline(buffer, MAXLINE);
	//cout << "first line:" << buffer << endl;

	long flength = ifs.tellg();

	for (int i=0; i<servernum; i++)
	{
		ifs.seekg(flength, ios::beg);

		bzero(buffer, MAXLINE);
		read(serverlist[i], buffer, MAXLINE);

		if (strcmp(buffer, filename) != 0)
		{
			cout << "Filename from server is not equal to the one send to it" << endl;
			return -1;
		}
		
		//cout << "server[" << i << "] query object:" << num << endl;

		int sendqn = 0;
		for (int j=0; j<num; j++)
		{
			bzero(buffer, MAXLINE);
			ifs.getline(buffer, MAXLINE);

			if (strlen(buffer) >= MAXLINE)
			{
				cout << "buffer overflow" << endl;
				return -1;
			}
			else
			{
				buffer[strlen(buffer)] = '\n';
			}

			if (strlen(buffer) > 0)
			{
				int len = write(serverlist[i], buffer, strlen(buffer));
				if (len != strlen(buffer))
				{
					cout << "Failed to write data to server " << i << endl;
					return -1;
				}
				sendqn++;
			}
		}

		cout << "Number of query data line sent to server " << i << " is: " << sendqn << endl;

		bzero(buffer, MAXLINE);
		strcpy(buffer, "over");
		write(serverlist[i], buffer, strlen(buffer));
	}

	ifs.close();
}
