#include "ATM.h"


// ��������
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

// �����½ڵ�
DATA* createNode() 
{
    DATA* node = (DATA*)malloc(sizeof(DATA));
    if (node) 
    {
        memset(node, 0, sizeof(DATA)); 
        // ��ʼ���ڵ�
    }
    return node;
}

// ���ڵ���ӵ�����ĩβ
void addNode(simple_list* list, DATA* node)
{
    node->next = NULL;

    if (list->first == NULL)
    {
        // ��Ϊ�ձ�ͷβ��Ϊ�˽ڵ�
        list->first = node;
        list->last = node;
    }
    else
    {
        // ������½ڵ�
        list->last->next = node;
        list->last = node;
    }
}

// �������浽�ļ�
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
        // ��ʽ���ַ���
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

// ���ļ���������

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

//id���亯��

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
    //������һ�����ܵ�id
}

//���Ϳ�������

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

    CreateInputWindow(username, "�����������", "CWindowClass", WndProcC);
    CreateInputWindow(idcard, "����������֤��", "CWindowClass", WndProcC);
    CreateInputWindow(phone, "��������ֻ���", "CWindowClass", WndProcC);
PW:
    CreateInputWindow(password1, "��������", "HWindowClass", WndProcH);
    CreateInputWindow(password2, "ȷ������", "HWindowClass", WndProcH);
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

//�����޸���������

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
    CreateInputWindow(username, "�����������", "CWindowClass", WndProcC);
    CreateInputWindow(idcard, "����������֤��", "CWindowClass", WndProcC);
    CreateInputWindow(phone, "��������ֻ���", "CWindowClass", WndProcC);

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
                CreateInputWindow(passwordF1, "��������", "HWindowClass", WndProcH);
                CreateInputWindow(passwordF2, "ȷ������", "HWindowClass", WndProcH);

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

