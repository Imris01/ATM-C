#pragma once
#ifndef ATM_H
#define ATM_H
#define _CRT_SECURE_NO_WARNINGS
#define MAXLEN 20
#define MAX_CREDENTIALS 100 // ���������100���û����������
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
    int left;       //���Ͻǵ�ĺ�����
    int top;        //���Ͻǵ��������
    int right;      //���½ǵ�ĺ�����
    int bottom;     //���½ǵ��������
    char text[MAXLEN];
}Button;

typedef struct CardRecord   //��Ƭ������¼
{
    char op_time[20];       //����ʱ��
    char op_type[10];       //��������
    float money;              //�������
    struct CardRecord *next;
}Record;

typedef struct User         //�û�������Ϣ
{
    char username[10];     //�û���
    char idcard[19];      //�û����֤��
    char phone[12];       //�û��ֻ�����
}USER;

typedef struct _simple_list_node
{
    int id;                 //�˻�����
    char password[MAXLEN];   //����
    float money;              //�˻����
    int islocked;           //�Ƿ�������1Ϊ�ǣ�0Ϊ��
    char create_time[20];   //����ʱ��
    USER userinfo;          //�û�������Ϣ
    Record* rec;         //��Ƭ������¼
    int recordnum;          //��Ƭ��¼����
    int error_count;        //��������������������������5�������˻�
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
    int id;                 //�˻�����
    int password;           //����
    char admin_name[10];  //�û���
    char IDcard_A[18];    //�û����֤��
    char phone_A[12];     //�û��ֻ�����
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