// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�: 
#include <windows.h>



// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <winnt.h>
#include <WinSock2.h>
#include <detours.h>  
#include <iostream>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"detours.lib")
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/thread.hpp>
using namespace std;






