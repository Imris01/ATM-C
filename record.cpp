#include "ATM.h"


int addRecordToFile(int id, const char* opTime, const char* opType, float money) 
{
    char filename[25];
    sprintf(filename, "%d_record.txt", id); 
    // 将id转换为字符串并创建文件名

    FILE* file = fopen(filename, "a"); 
    // 打开文件以便追加
    if (!file) 
    {
        perror("Unable to open file for appending");
        return 0;
    }

    fprintf(file, "%s,%s,%f\n", opTime, opType, money); // 写入新记录

    fclose(file); // 关闭文件
    return 1;
}

Record* loadRecordsFromFile(int id)
{
    char filename[25];
    sprintf(filename, "%d_record.txt", id); 
    // 将id转换为字符串并创建文件名

    FILE* file = fopen(filename, "r"); 
    // 打开文件以便读取
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

    fclose(file); // 关闭文件
    return head;
}
