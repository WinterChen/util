#include <iostream>
#include "bloomfilter.h"
using namespace std;

int main(int argc, char** argv) {
    BloomFilter filter(8);
    filter.CreateFilter(100);
    filter.Add("hello");
    cout << filter.KeyMayMatch("hello") << endl;
    cout << filter.KeyMayMatch("world") << endl;
    cout << filter.GetInfo() << endl;
    return 0;
}
