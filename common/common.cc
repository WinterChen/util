#include <arpa/inet.h>
#include <sys/time.h>
#include <map>
#include <string>
#include <fstream>
#include <list>
#include <stdlib.h>  
#include <time.h>  

#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>

#include <cerrno>
#include <cassert>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "common.h"

namespace utilcommon{


/////////////////////////
//common function
inline unsigned IpStringToU32(const char* pAddr)
{
	in_addr in = {0};
	if(inet_pton(AF_INET, pAddr, &in) > 0)
	{
		return in.s_addr;
	}
	return 0;
}
inline unsigned IpStringToU32(const string& strAddr)
{
	return IpStringToU32(strAddr.c_str());
}
string IpU32ToString(unsigned ipv4)
{
    char buf[INET_ADDRSTRLEN] = {0};
    struct in_addr in;
    in.s_addr = ipv4;

    if(inet_ntop(AF_INET, &in ,buf, sizeof(buf)))
    {
        return string(buf);
    }
    else
    {
        return string("");
    }

}

long long NowMs() {
	struct timeval timeNow;
	gettimeofday(&timeNow, 0);
	return ((long long)(timeNow.tv_sec) * 1000 + timeNow.tv_usec / 1000);
}
int GetTimeElapsed(long long tMs) {
	return int(NowMs() - tMs);
}
//s:源字符串，v:保存结果的向量，c:分隔符
void SplitString(const string& s, vector<std::string>& v, const string& c)
{
  std::string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  while(std::string::npos != pos2)
  {
    v.push_back(s.substr(pos1, pos2-pos1));
 
    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if(pos1 != s.length())
    v.push_back(s.substr(pos1));
}


//此处不考虑其它平台，在inet架构下测试, 输入的ip为主机字节顺序
// 0xa -- "10.0.0.0">>24; 0xc0a8--"192.168.0.0.">>16; 0x2b0--"127.17.0.1">>22
int IsInnerIP(unsigned a_ip )
{
       int bValid = -1;
       if( (a_ip>>24 == 0xa) || (a_ip>>16 == 0xc0a8) || (a_ip>>22 == 0x2b0) )
       {
          bValid = 0;
       }
       return bValid;
}

vector<string> GetLocalHosts()
{
    vector<string> result;
    int fd=socket(AF_INET,SOCK_STREAM,0);//创建本地sock描述符

    int cmd = SIOCGIFCONF;

    struct ifconf ifc;

    int numaddrs = 10;

    int old_ifc_len = 0;

    while(true)
    {
        int bufsize = numaddrs * static_cast<int>(sizeof(struct ifreq));
        ifc.ifc_len = bufsize;
        ifc.ifc_buf = (char*)malloc(bufsize);
        int rs = ioctl(fd, cmd, &ifc);

        if(rs == -1)
        {
            free(ifc.ifc_buf);
            //cout << "getLocalHosts|ioctl error="  << errno << endl;
            break;
        }
        else if(ifc.ifc_len == old_ifc_len)
        {
            break;
        }
        else
        {
            old_ifc_len = ifc.ifc_len;
        }
    
        numaddrs += 10;
        free(ifc.ifc_buf);
    }

    numaddrs = ifc.ifc_len / static_cast<int>(sizeof(struct ifreq));
    struct ifreq* ifr = ifc.ifc_req;
    for(int i = 0; i < numaddrs; ++i)
    {
        if(ifr[i].ifr_addr.sa_family == AF_INET)
        {
            struct sockaddr_in* addr = reinterpret_cast<struct sockaddr_in*>(&ifr[i].ifr_addr);
            if(addr->sin_addr.s_addr != 0)
            {
                char sAddr[INET_ADDRSTRLEN] = "\0";
                inet_ntop(AF_INET, &(*addr).sin_addr, sAddr, sizeof(sAddr));
                result.push_back(sAddr);
            }
        }
    }

    free(ifc.ifc_buf);

    return result;
}
string GetHostName(){
    char host_name[128]={0};
    gethostname(host_name, sizeof(host_name));//获取本地主机名称
    return host_name;
}

string GetInnerIp(){
    vector<string> res = GetLocalHosts();
    for(vector<string>::size_type i = 0; i < res.size(); i++){
        unsigned intIP;
        if(!(intIP = inet_addr(res[i].c_str())))
        {
            return "127.0.0.1";
        }
        
        if(res[i] != "127.0.0.1" && IsInnerIP(intIP))
            return res[i];
    }
    return "127.0.0.1";
}

//ParseKeyValue parse string such as "key=value" or "key:value"
void ParseKeyValue(const string& s, string &key, string &value,const string&c){
    string::size_type pos;
    pos = s.find(c);
    if(string::npos != pos){
        key = "";
        value = "";
    }
    key = s.substr(0, pos);
    value = s.substr(pos+c.size(), string::npos);
}


}

    