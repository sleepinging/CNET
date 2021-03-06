/*
 * @Description: Do not edit
 * @Author: taowentao
 * @Date: 2019-01-06 11:43:48
 * @LastEditors: taowentao
 * @LastEditTime: 2019-03-16 12:58:50
 */

#include "test.h"

#include <stdio.h>
#include <errno.h>

#include <string>
#include <iostream>
#include <thread>

#include "mytap.h"
#include "myip.h"
#include "mymac.h"
#include "mymask.h"
#include "mytool.h"
#include "mysocket.h"
#include "netdef.hpp"
#include "mybufconn.h"
#include "channel.hpp"
#include "cmdline.h"
#include "myipnet.h"
#include "mycompress.h"

using namespace std;

//测试压缩
int test_comp(){
    MyCompress::Init();
    const char str[] = "hello";
    const char unsigned *data = (const char unsigned *)str;
    const uint64_t inlen = sizeof(str);
    uint64_t outlen = inlen + inlen / 16 + 64 + 3;
    unsigned char *buf = new unsigned char[outlen];
    cout << MyCompress::Compress(data, inlen, buf, &outlen) << endl;

    {
        for (unsigned int i = 0; i < outlen;++i){
            cout << (unsigned)buf[i]<<"," << flush;
        }
    }

    unsigned char out[2048] = {0};
    uint64_t newlen;
    cout << MyCompress::DeCompress(buf, outlen, out, &newlen) << endl;
    cout << (char *)out << endl;
    return 1;
}

int test_channel(){
    Channel<BinArr> *channel_ = new Channel<BinArr>();
    auto fp_read = [&]() {
        int cc = 0;
        while (1)
        {
            channel_->pop();
            if (++cc % 100000 == 0)
            {
                cout << "pop  " << cc << " times" << endl;
            }
        }
    };
    thread(fp_read).detach();
    BinArr bs(1500, 0xff);
    int cc = 0;
    while (1)
    {
        channel_->push(bs);
        if (++cc % 100000 == 0)
        {
            cout << "push "<<cc<<" times" << endl;
        }
    }
    delete channel_;
    return -1;
}

/**
 * @description: 测试
 * @param {type} 
 * @return: 
 */
int test(int argc, char const *argv[]){
    int r = 0;
    //去除警告
    r = r;
    argc = argc, argv = argv;
    // r=test_channel();
    // r = MySocket::InitLib();
    // r = test_connect();
    // r = test_reconnect();
    // r = test_comp();
    return r;
}

/**
 * @description: 测试命令行
 * @param {type} 
 * @return: 
 */
int test_cmd(int argc, char const *argv[]){
    int r = 0;

    cmdline::parser a;
    a.add<string>("net", 'n', "ip string", true, "192.168.10.100/24");
    a.add<string>("mac", 'm', "mac string", true, "00:02:03:04:05:07");
    a.parse_check(argc, argv);

    // auto netstr = a.get<string>("net");
    // auto macstr = a.get<string>("mac");

    return r;
}

/**
 * @description: 测试重连
 * @param {type} 
 * @return: 
 */
int test_reconnect(){
    int r = 0;

    MySocket ms;
    MyIP ip("192.168.1.100");
    auto cn = ms.Connect(&ip, 8875, ConnType::TCP);
    // auto cn = ms.Connect(&ip, 8876, ConnType::TCP,12346);
    char buf[2048] = {0};
    while (1)
    {
        r = cn.Write("123", 3);
        if (r <= 0)
        {
            cn.ReConnect();
        }
        r = cn.Read(buf, 2047);
        if (r <= 0)
        {
            cn.ReConnect();
        }else{
            cout << string(buf, r) << endl;
        }
    }

    return r;
}

/**
 * @description: 测试连接
 * @param {type} 
 * @return: 
 */
int test_connect(){
    int r = 0;
    
    // MySocket ms;
    // MyIP ip("192.168.1.100");
    // auto cn = ms.Connect(&ip, 8875, ConnType::TCP);
    // // auto cn = ms.Connect(&ip, 8876, ConnType::TCP,12346);
    // r = cn.Write("123", 3);
    // r = cn.Write("456", 3);
    // r = cn.Write("798", 3);
    // if (r < 0)
    // {
    //     showerr("write");
    // }
    // char buf[2048] = {0};
    // while (1)
    // {
    //     r = cn.Read(buf, 2047);
    //     cout << string(buf, r) << endl;
    // }

    MyIP ip("192.168.1.100");
    MyBufConn bc(&ip, 8875, ConnType::TCP);
    r = bc.Write("123", 3);
    r = bc.Write("456", 3);
    r = bc.Write("798", 3);

    char buf[2048]={0};
    while (r>0){
        r = bc.Read(buf, 2047);
        cout << string(buf, r) << endl;
    }

    if (r < 0)
    {
        showerr("write");
    }

    // this_thread::sleep_for(chrono::seconds(1));
    return r;
}

/**
 * @description: 测试tap
 * @param {type} 
 * @return: 
 */
int test_tap(const MyMAC *mac, const MyIPNet *ipnet){
    int r = 0;
#ifdef _WIN32
    r = test_tap_win(mac, ipnet);
#else
    r = test_tap_linux(mac, ipnet);
#endif
    return r;
}

#ifdef _WIN32
/**
 * @description: 测试win tap
 * @param {type} 
 * @return: 
 */
int test_tap_win(const MyMAC *mac, const MyIPNet *ipnet){
    int r = 0;
    //去除警告
    r = mac->data[0];
    r = ipnet->GetIP().data[0];
    return r;
}
#else
/**
 * @description: 测试linux tap
 * @param {type} 
 * @return: 
 */
int test_tap_linux(const MyMAC *mac, const MyIPNet *ipnet){
    int r = 0;
    //去除警告
    r = mac->data[0];
    r = ipnet->GetIP().data[0];
    return r;
}
#endif