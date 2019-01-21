#include <iostream>
#include <map>
#include "consistenthash.h"
#include "common/common.h"
using namespace std;
using namespace utilcommon;
const char chs[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
                'A','B','C','D','E','F','G','H','I','J','K','l','M','N','O','P','Q','R','D','T','U','V','w','x','Y','Z',
                '0','1','2','3','4','5','6','7','8','9'};
int main(int argc, char** argv) {
    if(argc < 4){
        cout << "usage: argv[0] <virturl node per server> <count of server> <count of dispatch request>" << endl;
        return 0;
    }
    int vnodePerServer = atoi(argv[1]);
    int cntServer = atoi(argv[2]);
    int dispatchRequest = atoi(argv[3]);
    ConsistentHash hash(vnodePerServer);
    //servers
    vector<ServerInfo> servers;
    for(int i = 0;i < cntServer; i++){
        servers.push_back(ServerInfo(1000, "127.0.0.1", i));
    }
    long long time1 = NowMs();
    hash.Init(servers, 1000*cntServer);
    long long time2 = NowMs();
    cout << "timeuse of consitent hash init:" << time2 - time1 << endl;
    cout << hash.GetInfo() << endl;
    time1 = time2;
    //随机产生16 byte的字符串
    vector<string> requests;
    for(int i = 0; i < dispatchRequest; i++){
        char str[16];
        for(int j = 0;j < 16;j++){
            str[j] = chs[random()%62];
        }
        requests.push_back(str);
    }
    time2 = NowMs();
    cout << "timeuse of requests init:" << time2 - time1 << endl;
    time1 = time2;
    string ip;
    uint16_t port;
    map<string, int> resultMap;
    for(int i = 0; i < dispatchRequest; i++){
        if(hash.Dispatch(requests[i], ip, port)){
            cout << "dispatch fail!request=" << requests[i] << endl;
            continue;
        }
        string key = ip + ":" + NumToString<uint16_t>(port);
        if(resultMap.find(key) == resultMap.end()){
            resultMap[key] = 0;
        }
        resultMap[key]++;
    }
    time2 = NowMs();
    cout << "timeuse of dispatch:" << time2 - time1 << endl;
    map<string, int>::iterator it = resultMap.begin();
    while(it != resultMap.end()){
        cout << "server:" << it->first << ",cnt:" << it->second << ",perent:" << float(it->second)/dispatchRequest << endl; 
        it++;
    }
    
    return 0;
}
