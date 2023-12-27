#include "ATM.h"


int addRecordToFile(int id, const char* opTime, const char* opType, float money) 
{
    char filename[25];
    sprintf(filename, "%d_record.txt", id); 
    // ��idת��Ϊ�ַ����������ļ���

    FILE* file = fopen(filename, "a"); 
    // ���ļ��Ա�׷��
    if (!file) 
    {
        perror("Unable to open file for appending");
        return 0;
    }

    fprintf(file, "%s,%s,%f\n", opTime, opType, money); // д���¼�¼

    fclose(file); // �ر��ļ�
    return 1;
}

Record* loadRecordsFromFile(int id)
{
    char filename[25];
    sprintf(filename, "%d_record.txt", id); 
    // ��idת��Ϊ�ַ����������ļ���

    FILE* file = fopen(filename, "r"); 
    // ���ļ��Ա��ȡ
    if (!file)
    {
        perror("Unable to open file for reading");
        return NULL;
    }

    Record* head = NULL;
    Record* current = NULL;
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        Record* newRecord = (Record*)malloc(sizeof(Record));
        if (newRecord)
        {
            sscanf(line, "%[^,],%[^,],%f",
                newRecord->op_time,
                newRecord->op_type,
                &newRecord->money);
            newRecord->next = NULL;

            if (current)
            {
                current->next = newRecord;
            }
            else
            {
                head = newRecord;
            }
            current = newRecord;
        }
    }

    fclose(file); // �ر��ļ�
    return head;
}
