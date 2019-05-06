#ifndef CONSISTENT_HASH_H
#define CONSISTENT_HASH_H
#include <stdint.h>
#include <string>
#include <vector>

using namespace std;
class VirtualNode{
public:
    uint32_t mPoint;  // point on circle
    string mIp;
    uint16_t mPort;
};

class ServerInfo{
public:
    ServerInfo(){}
    ServerInfo(uint32_t weight, string ip, uint16_t port): mWeight(weight), mIp(ip), mPort(port){}
    uint32_t mWeight;//权重
    string mIp;
    uint16_t mPort;
};


class ConsistentHash{
public:
    //建议pointPerServer >= 160
    ConsistentHash(uint32_t pointPerServer):mPointPerServer(pointPerServer){}
    void Init(vector<ServerInfo> &servers, uint32_t totalWeight);
    int Dispatch(string &key, string &ip, uint16_t &port);
    //return the info of the ConsistentHash
    string GetInfo();
private:
    uint32_t mPointPerServer;//每一个server的虚拟节点数
    uint32_t mNumPoints;//总节点数
    vector<VirtualNode> mContinuum;//hash环

};
#endif // !CONSISTENT_HASH_H