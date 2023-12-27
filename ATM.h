#pragma once
#ifndef ATM_H
#define ATM_H
#define _CRT_SECURE_NO_WARNINGS
#define MAXLEN 20
#define MAX_CREDENTIALS 100 // 假设最多有100个用户名和密码对
#define IDC_EDITBOX 101
#define IDC_SUBMIT_BUTTON 102
#define EXIT 5
#define MAIN_USER 6
#define MAIN_ADMIN 7;
#define LOG_SUC 8
#define LOG_FAIL 9
#define U_TO_F 10
#define F_TO_U 11

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

typedef struct button
{
    int left;       //左上角点的横坐标
    int top;        //左上角点的纵坐标
    int right;      //右下角点的横坐标
    int bottom;     //右下角点的纵坐标
    char text[MAXLEN];
}Button;

typedef struct CardRecord   //卡片操作记录
{
    char op_time[20];       //操作时间
    char op_type[10];       //操作名称
    float money;              //操作金额
    struct CardRecord *next;
}Record;

typedef struct User         //用户个人信息
{
    char username[10];     //用户名
    char idcard[19];      //用户身份证号
    char phone[12];       //用户手机号码
}USER;

typedef struct _simple_list_node
{
    int id;                 //账户卡号
    char password[MAXLEN];   //密码
    float money;              //账户余额
    int islocked;           //是否被锁定，1为是，0为否
    char create_time[20];   //开户时间
    USER userinfo;          //用户个人信息
    Record* rec;         //卡片操作记录
    int recordnum;          //卡片记录个数
    int error_count;        //密码连续输入错误次数，若大于5则锁定账户
    struct _simple_list_node* next;
}DATA;

typedef struct _simple_list
{
    DATA* first;
    DATA* last;
} simple_list;

typedef struct _simple_list_iterator
{
    DATA* current;
    simple_list* list;
} simple_list_iterator;

typedef struct INFO_A
{
    int id;                 //账户卡号
    int password;           //密码
    char admin_name[10];  //用户名
    char IDcard_A[18];    //用户身份证号
    char phone_A[12];     //用户手机号码
    struct INFO_A* next;
}ADATA;

typedef struct adlist
{
    ADATA* first;
    ADATA* last;
} adlist;

void drawButton(Button btn);
int isMouseOnButton(int x, int y, Button btn);
int main_interface();
int user_interface(simple_list* cardlist, int* id);
int user_function(simple_list* cardlist, int id);
LRESULT CALLBACK WndProcC(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcH(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc_Float(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void CreateInputWindow(char* Text, const char* windowname, const char* className, WNDPROC wndproc);

void getCurrentTimeAsString(char* buffer, size_t bufferSize);
simple_list* create_userlist();
DATA* createNode();
void addNode(simple_list* list, DATA* node);
void saveListToFile(simple_list* list, const char* filename);
simple_list* loadListFromFile(simple_list* list, const char* filename);
void card_list_destroy(simple_list* cardlist); 

adlist* admin_list_create(void);
void admin_list_destroy(adlist* alist);

int addRecordToFile(int id, const char* opTime, const char* opType, float money);
Record* loadRecordsFromFile(int id);
int OpenRequest(const char* filename, simple_list* cardlist);
int ForgetRequest_U(const char* filename, simple_list* cardlist);

int id_assignment(simple_list* cardlist, const char* filename);
int login_U(char* str1, char* str2, simple_list* cardlist, int* id);
DATA* search(simple_list* cardlist, int id);
int isLocked(simple_list* cardlist, int id);

#endif