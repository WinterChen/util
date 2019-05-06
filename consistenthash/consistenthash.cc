/*
为了减少重复造轮子，本文件代码中有直接从ketama源码（https://github.com/RJ/ketama/tree/master/libketama）中copy过来的一些函数，
所以包含了ketama的lisence.
*/
/*
* Copyright (c) 2007, Last.fm, All rights reserved.
* Richard Jones <rj@last.fm>
* Christian Muehlhaeuser <muesli@gmail.com>
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the Last.fm Limited nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY Last.fm ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL Last.fm BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "consistenthash.h"
#include "md5.h"
#include <math.h>           /* floor & floorf                       */
#include <string.h>
#include <algorithm>
using namespace std;

void ketama_md5_digest(const char* inString, unsigned char md5pword[16] )
{
    md5_state_t md5state;

    md5_init( &md5state );
    md5_append( &md5state, (unsigned char *)inString, strlen( inString ) );
    md5_finish( &md5state, md5pword );
}

unsigned int ketama_hashi(const char* inString )
{
    unsigned char digest[16];

    ketama_md5_digest( inString, digest );
    return (unsigned int)(( digest[3] << 24 )
                        | ( digest[2] << 16 )
                        | ( digest[1] <<  8 )
                        |   digest[0] );
}

static bool Compare(const VirtualNode &n1, const VirtualNode &n2){
    return n1.mPoint < n2.mPoint;
}

static bool Compare1(const VirtualNode &n1, const uint32_t v){
    return n1.mPoint < v;
}
void ConsistentHash::Init(vector<ServerInfo> &servers, uint32_t totalWeight){
    for(vector<ServerInfo>::size_type i = 0; i < servers.size(); i++ )
    {
        float pct = (float)servers[i].mWeight / (float)totalWeight;
        uint32_t ks = floorf( pct * (mPointPerServer/4) * (float)servers.size() );//4 numbers per hash

        for(uint32_t k = 0; k < ks; k++ )
        {
            
            char ss[30];
            unsigned char digest[16];

            sprintf( ss, "%s:%d-%d", servers[i].mIp.c_str(), servers[i].mPort, k );
            ketama_md5_digest( ss, digest );

            /* Use successive 4-bytes from hash as numbers
             * for the points on the circle: */
        
            for(int h = 0; h < 4; h++ )
            {
                VirtualNode node;
                node.mIp = servers[i].mIp;
                node.mPort = servers[i].mPort;
                node.mPoint = ( digest[3+h*4] << 24 )
                                      | ( digest[2+h*4] << 16 )
                                      | ( digest[1+h*4] <<  8 )
                                      |   digest[h*4];
                mContinuum.push_back(node);
                mNumPoints++;
            }
        }
        //sort
        sort(mContinuum.begin(), mContinuum.end(), Compare);
    }
}
int ConsistentHash::Dispatch(string &key, string &ip, uint16_t &port){
    if(mContinuum.empty())
        return 1;
    uint32_t h = ketama_hashi(key.c_str());
    vector<VirtualNode>::iterator it = lower_bound(mContinuum.begin(),mContinuum.end(),h,Compare1);
    if(it == mContinuum.end()){
        it = mContinuum.begin();
    }
    ip = it->mIp;
    port = it->mPort;
    return 0;
}

string ConsistentHash::GetInfo(){
    char str[200] = {0};
    sprintf(str, "pointPerServer:%u,numPoints:%u,continuumSize:%zu", mPointPerServer, mNumPoints, mContinuum.size());
    return string(str);
}
