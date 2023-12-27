#include "ATM.h"


// 创建链表
simple_list* create_userlist() 
{
    simple_list* list = (simple_list*)malloc(sizeof(simple_list));
    if (list) 
    {
        list->first = NULL;
        list->last = NULL;
    }
    return list;
}

// 创建新节点
DATA* createNode() 
{
    DATA* node = (DATA*)malloc(sizeof(DATA));
    if (node) 
    {
        memset(node, 0, sizeof(DATA)); 
        // 初始化节点
    }
    return node;
}

// 将节点添加到链表末尾
void addNode(simple_list* list, DATA* node)
{
    node->next = NULL;

    if (list->first == NULL)
    {
        // 若为空表，头尾皆为此节点
        list->first = node;
        list->last = node;
    }
    else
    {
        // 否则加新节点
        list->last->next = node;
        list->last = node;
    }
}

// 将链表保存到文件
void saveListToFile(simple_list* list, const char* filename) 
{
    FILE* file = fopen(filename, "w");
    if (!file) 
    {
        perror("Unable to open file for writing");
        return;
    }

    DATA* current = list->first;
    while (current) 
    {
        // 格式化字符串
        fprintf(file, "%d,%s,%f,%d,%s,%s,%s,%s\n",
            current->id, 
            current->password, 
            current->money, 
            current->islocked,
            current->create_time, 
            current->userinfo.username, 
            current->userinfo.idcard,
            current->userinfo.phone);

        current = current->next;
    }

    fclose(file);
}

// 从文件加载链表

simple_list* loadListFromFile(simple_list* list, const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        return NULL;
    }

    DATA* node;
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        node = createNode();
        if (node == NULL)
        {
            fclose(file);
            return NULL;
        }

        sscanf(line, "%d,%19[^,],%f,%d,%19[^,],%9[^,],%18[^,],%11[^,]",
            &node->id, 
            node->password, 
            &node->money, 
            &node->islocked,
            node->create_time, 
            node->userinfo.username,
            node->userinfo.idcard,
            node->userinfo.phone);

        addNode(list, node);
    }

    fclose(file);
    return list;
}

//id分配函数

int id_assignment(simple_list* cardlist, const char* filename) {
    FILE* file = fopen(filename, "r"); 
    if (!file) {
        perror("Unable to open file for reading");
        return -1; 
    }

    DATA* card = cardlist->first;
    int index = 10000;
    while (card != NULL) {
        DATA* next = card->next;
        index++;
        card = next;
    }

    char line[256];
    int idmax = index;
    while (fgets(line, sizeof(line), file)) {
        int type = 0;
        int node_id = 10000;
        if (sscanf(line, "%d,%*[^,],%*[^,],%*[^,],%*[^,],%d", &node_id, &type) == 2) {
            if (type != 1 && type != 2) {
                fclose(file);
                return -1; 
            }

            if (type == 1 && node_id > idmax)
                idmax = node_id;
        }
    }
    fclose(file);
    return idmax + 1;
    //返回下一个可能的id
}

//发送开户请求

int OpenRequest(const char* filename, simple_list* cardlist)
{
    int assigned_id = id_assignment(cardlist, filename);
    FILE* file = fopen(filename, "a");
    if (!file)
    {
        perror("Unable to open file for reading");
        return NULL;
    }

    char username[10];
    char idcard[19]; 
    char phone[12];
    char password1[20];
    char password2[20];

    CreateInputWindow(username, "输入你的名字", "CWindowClass", WndProcC);
    CreateInputWindow(idcard, "输入你的身份证号", "CWindowClass", WndProcC);
    CreateInputWindow(phone, "输入你的手机号", "CWindowClass", WndProcC);
PW:
    CreateInputWindow(password1, "输入密码", "HWindowClass", WndProcH);
    CreateInputWindow(password2, "确认密码", "HWindowClass", WndProcH);
    if (strcmp(password1, password2) != 0)
        goto PW;

    fprintf(file, "%d,%s,%s,%s,%s,%d\n",
            assigned_id,
            username,
            idcard,
            phone,
            password2,
            1);

    fclose(file);
    return assigned_id;
}

//发送修改密码请求

int ForgetRequest_U(const char* filename, simple_list* cardlist)
{
    FILE* file = fopen(filename, "a");
    if (!file)
    {
        perror("Unable to open file for reading");
        return NULL;
    }

    char username[10];
    char idcard[19];
    char phone[12];
    char passwordF1[20];
    char passwordF2[20];
    CreateInputWindow(username, "输入你的名字", "CWindowClass", WndProcC);
    CreateInputWindow(idcard, "输入你的身份证号", "CWindowClass", WndProcC);
    CreateInputWindow(phone, "输入你的手机号", "CWindowClass", WndProcC);

    DATA* dst = cardlist->first;
    int isfound = 0;
    while (dst != NULL)
    {
        if (
            strcmp(dst->userinfo.idcard, idcard)==0 &&
            strcmp(dst->userinfo.phone, phone)==0 &&
            strcmp(dst->userinfo.username, username)==0
            )
        {
            isfound = 1;
            while (1)
            {
                CreateInputWindow(passwordF1, "输入密码", "HWindowClass", WndProcH);
                CreateInputWindow(passwordF2, "确认密码", "HWindowClass", WndProcH);

                if (strcmp(passwordF1, passwordF2) == 0) 
                {
                    break;
                }
            }
            break;
        }
        dst = dst->next;
    }

    if (isfound == 0)
    {
        fclose(file);
        return 3;
    }

    FP:fprintf(file, "%d,%s,%s,%s,%s,%d\n",
        0,
        username,
        idcard,
        phone,
        passwordF2,
        2);

    fclose(file);
    return 1;
}

