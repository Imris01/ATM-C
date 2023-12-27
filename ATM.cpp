#include "ATM.h"
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <graphics.h>
#include <string.h>


void getCurrentTimeAsString(char* buffer, size_t bufferSize) 
{
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime); // 获取当前时间
    timeinfo = localtime(&rawtime); // 转换为本地时间

    // 格式化时间字符串，例如 "YYYY-MM-DD HH:MM:SS"
    strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S", timeinfo);
}

void card_list_destroy(simple_list* cardlist)             //摧毁链表
{
    DATA* card = cardlist->first;               //让从第一个节点开始依次释放内存
    while (card != NULL)
    {
        DATA* next = card->next;
        free(card);
        card = next;
    }
    free(cardlist);
}

adlist* admin_list_create(void)
{
    adlist* alist = (adlist*)malloc(sizeof(adlist));    //为链表分配内存
    alist->first = NULL;                                 //初始化头指针和尾指针
    alist->last = NULL;
    return alist;
}

void admin_list_destroy(adlist* alist)             //摧毁链表
{
    ADATA* ad = alist->first;               //让从第一个节点开始依次释放内存
    while (ad != NULL)
    {
        ADATA* next = ad->next;
        free(ad);
        ad = next;
    }
    free(alist);
}



int login_U(char* str1, char* str2, simple_list* cardlist, int* id)
{
    if (str1 == NULL || str2 == NULL || cardlist == NULL) 
    {
        return LOG_FAIL; 
    }

    int id_in = atoi(str1); 
    if (id_in == 0 && strcmp(str1, "0") != 0) 
    {
        return LOG_FAIL; 
    }

    DATA* prev = cardlist->first;
    while (prev != NULL)
    {
        if (prev->id == id_in && strcmp(str2, prev->password) == 0)
        {
            *id = prev->id;
            return LOG_SUC;
        }
        prev = prev->next;
    }
    return LOG_FAIL;
}

DATA* search(simple_list* cardlist, int id)
{
    DATA* card = cardlist->first;
    while (card != NULL)
    {
        if (card->id == id) // Use == for comparison instead of =
        {
            return card;
        }
        card = card->next; // Directly use card->next
    }
    return NULL;
}

int isLocked(simple_list* cardlist, int id)
{
    DATA* data = search(cardlist, id);
    return data->islocked;
}


