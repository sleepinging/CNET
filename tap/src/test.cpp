/*
 * @Description: Do not edit
 * @Author: taowentao
 * @Date: 2019-01-06 11:43:48
 * @LastEditors: taowentao
 * @LastEditTime: 2019-01-06 12:36:02
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

using namespace std;

/**
 * @description: 测试
 * @param {type} 
 * @return: 
 */
int test(int argc, char const *argv[]){
    int r = 0;
    //去除警告
    argc = argc, argv = argv;
    r = MySocket::InitLib();
    // r = test_connect();
    r = test_reconnect();
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
        try{
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
        }catch(const char* msg){
            cout << msg << endl;
            this_thread::sleep_for(chrono::seconds(1));
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