#include "timerwheel.h"
#include <unistd.h>
#include <cstdlib>
const int SUCC = 0;
const int TIMERHADADDED = -10001;
const int TIMERNOTADDED = -10002;
int TimerWheel::AddTimer(Timer *entry){
    if(entry->mAdded)
        return TIMERHADADDED;
    //计算solt和round
    int solt = (mCurrentSolt + (entry->mTimeoutMs/mMsPerTick))%mSolt;
    int round = (mCurrentSolt + (entry->mTimeoutMs/mMsPerTick))/mSolt;
    entry->mListIter = mWheel[solt].insert(mWheel[solt].end(),entry);
    entry->mAdded = true;
    entry->mSolt = solt;
    entry->mRound = round;
    mTotalTimer++;
    cout << "AddTimer|timeoutMS=" << entry->mTimeoutMs << ", solt=" << solt << ", round=" << round << endl;
    return SUCC;
}
int TimerWheel::UpdateTimer(Timer *entry){
    //delete first
    DeleteTimer(entry);
    return AddTimer(entry);
}
int TimerWheel::DeleteTimer(Timer *entry){
    if(!entry->mAdded)
        return TIMERNOTADDED;
    mWheel[entry->mSolt].erase(entry->mListIter);
    entry->mAdded = false;
    mTotalTimer--;
    return SUCC;
}
int TimerWheel::TimeOut(long long now){
    cout << "[timeout], current solt:" << mCurrentSolt  << ",current round: " << mCurrentRound << ",total timer: " << mTotalTimer << endl;
    ListIter iter = mWheel[mCurrentSolt].begin();
    while(iter != mWheel[mCurrentSolt].end()){
        ListIter tmp = iter;
        iter++;
        if((*tmp)->mRound <= mCurrentRound){
            (*tmp)->TimeOut(now);
            DeleteTimer(*tmp);
            
        }
        
       
    }
    mCurrentSolt = (mCurrentSolt+1)%(mSolt);
    if(mCurrentSolt == 0)
        mCurrentRound++;
    
    return SUCC;
}

