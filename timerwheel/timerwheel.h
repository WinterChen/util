#ifndef TIMER_WHEEL_H
#define TIMER_WHEEL_H
#include <iostream>
#include <string>
#include <list>
#include <stdio.h>
using namespace std;
class Timer;
typedef list<Timer*> List;
typedef list<Timer*>::iterator ListIter;

class Timer{
public:
    Timer(int timeoutMs,long long start):mTimeoutMs(timeoutMs),mStartTime(start),mEndTime(0){};
    virtual ~Timer(){}
    virtual void TimeOut(long long now) = 0;
    virtual string ToString(){
        char str[128];
        sprintf(str, "solt:%d,round:%d,timout:%d,start:%lld,end:%lld,d:%lld",mSolt,mRound,mTimeoutMs,mStartTime,mEndTime,mEndTime-mStartTime);
        return str;
    }
public:
    bool mAdded;
    ListIter mListIter;
    int mSolt;
    int mRound;
    int mTimeoutMs;
    long long mStartTime;
    long long mEndTime;
};
class TimerWheel{
public:
    //TimerWheel():mCurrentSolt(0),mMsPerTick(2000){}
    TimerWheel(int solt, unsigned ms):mSolt(solt),mCurrentSolt(0),mCurrentRound(0),mTotalTimer(0),mMsPerTick(ms){
        mWheel = new List[mSolt];
    }
    ~TimerWheel(){}
    int AddTimer(Timer *entry);
    int UpdateTimer(Timer *entry);
    int DeleteTimer(Timer *entry);
    int TimeOut(long long now);//ms
private:
    List *mWheel;
    int mSolt;
    int mCurrentSolt;
    int mCurrentRound;
    int mTotalTimer;
    unsigned int mMsPerTick;
};

#endif      






