#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <pthread.h>

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>


#include "logtail.h"

using namespace std;

int LogTail::StartTail(int fromwhere)
{
	//读file
	FILE *fp = fopen(mFilename.c_str(), "r");
	if (fp == NULL)
	{
		printf("fopen %s failed\n", mFilename.c_str());
		return -1;
	}
	fseek(fp, 0, fromwhere);//流指向文件尾偏移量0的地方
	while(1)//loop
	{
		char * pline = NULL;
		size_t len = 0;
		ssize_t read;

		read = getline(&pline, &len, fp);

		//If end of the file
		if(read <= 0)
		{
			//printf("meet end of the file\n");
			struct stat statbuf_now;
			struct stat statubf_pre;
			stat(mFilename.c_str(), &statbuf_now);
			fstat(fileno(fp), &statubf_pre);

			//Just no data
			if( statbuf_now.st_dev == statubf_pre.st_dev 
			 && statbuf_now.st_ino == statubf_pre.st_ino)
			{
				//printf("wait for new data to write in\n" );
				usleep(mReadIntervalMs*1000);
			}
			//File has been renamed
			else
			{
				printf("Old file has been renamed\n");
				FILE *fp2 = fopen(mFilename.c_str(), "r");

				while(fp2 == NULL)
				{
					printf("fopen fp2 %s failed\n", mFilename.c_str());
					sleep(1);
				}

				fclose(fp);
				fp = fp2;
			}
		}
		//Read the line into read_result
		else {
			for(vector<LogSender*>::size_type i = 0;i < mSenders.size();i++){
				mSenders[i]->SendLog(pline);//发送完后，pline会清除
			}
			
			if(pline)
				free(pline);
		}
	}

	fclose(fp);
	return 0;        
}
