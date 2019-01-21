#ifndef LOG_TAIL_H
#define LOG_TAIL_H
#include <string>
#include <vector>
using namespace std;
class LogSender{
public:
    virtual int SendLog(const char *line) = 0;
};


class LogTail{
public:
    LogTail(string filename, int readIntervalMs):mFilename(filename),mReadIntervalMs(readIntervalMs){}
    int StartTail(int fromwhere = 2);//0(SEEK_SET)，当前位置1(SEEK_CUR)，文件尾2(SEEK_END)）
    int AddLogSender(LogSender *sender){mSenders.push_back(sender);}
private:
    string mFilename;
    int mReadIntervalMs;
    vector<LogSender*> mSenders;
};
#endif // !LOG_TAIL_H
