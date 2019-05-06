#include <iostream>
#include "timerwheel.h"
#include "common/common.h"
using namespace std;
using namespace utilcommon;
class Connection : public Timer{
public:
    Connection(int idx, int timeout, long long startTime):Timer(timeout, startTime), mIndex(idx){}
    ~Connection(){cout << "delete connection: " << mIndex << endl;}
    void TimeOut(long long now){
        mEndTime = now;
        cout << "index: " << mIndex << ", timer:" << ToString() << endl;
    }
    void Close(){}
private:
    int mIndex;
};

int main(int argc, char** argv) {
    if(argc < 3){
        cout << "usage: " << argv[0] << " <solt count> <tick interval(ms)>" << endl;
        return 0;
    }
    int solt = atoi(argv[1]);
    int tickInterval = atoi(argv[2]);
    TimerWheel wheeler(solt, tickInterval);
    
    for(int i = 0; i < 100 ;i++){
        long long now = NowMs();
        Connection *conn = new Connection(i, i*tickInterval, now);
            wheeler.AddTimer(conn);
    }
    while(1){
        cout << "******************loop***********************" << endl;
        
        usleep(tickInterval*1000);
        long long now = NowMs();
        wheeler.TimeOut(now);
    }
}