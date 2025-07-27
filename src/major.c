    
    #include <stdio.h>
    #include <string.h>
    #include <errno.h>
    #include <time.h>

    #include "../include/context.h"
    #include "../include/major.h"
    #include "../include/student.h" 
    #include "../include/major_persist.h"
    #include "../include/utils.h"

    // ��ϣ���������� majorId �����ϣֵ
    int hash_func_int(const void *key, int bucketCount)
    {
        if (!key || bucketCount <= 0) 
        {
            printf("��ϣ������������key=%p, bucketCount=%d\n", 
                key, bucketCount);
            return 0;
        }
        
        //��ȫ����ת��
        int id = *(const int *)key;
        // 2. ������ID�������Ľ���
        id = (id < 0) ? -(id) : id;

        // 3. ��ֹ����Ϊ0
        if (bucketCount == 0) 
        {
            bucketCount = 1;
        }

        //�����ϣֵ
        int index = id % bucketCount;
        return index < 0 ? -index : index;
    }

    // �ַ�����ϣ������DJB2�㷨��
    unsigned int hash_func_str(const void *key, int bucketCount) {
        if (!key || bucketCount <= 0) {
            printf("��ϣ������������key=%p, bucketCount=%d\n", 
                key, bucketCount);
            return 0;
        }
        
        unsigned int hash = 5381;
        const char *str = (const char *)key;
        
        int c;
        // DJB2�㷨���ģ�ʹ��unsigned long���������
        while ((c = *str++)) {
            hash = ((hash << 5) + hash) + c;  // hash * 33 + c
        }
        
        // ����Ͱ����Ϊ0�����
        if (bucketCount == 0) {
            bucketCount = 1;
        }
        
        // ת��Ϊint��ȷ����������Ч��Χ
        int index = hash % bucketCount;
        return index < 0 ? -index : index;
    }


    //��ʼ��רҵ��ϣ�ڵ�
    MajorNode *init_major_node(void *key, void *value)
    { //����ڵ��ڴ�
        MajorNode *node = (MajorNode *)malloc(sizeof(MajorNode));
        if (!node)
        {
            printf("�ڴ����ʧ�ܣ��޷���ʼ���ڵ�\n");
            return NULL;
        }
        //�����������룺Ϊkey(�洢��majorId)��������һ���ڴ�
        int *copiedKey =malloc(sizeof(int));
        if (!copiedKey)
        {
            printf("Ϊ key �����ڴ�ʧ�ܣ�\n");
            free(node);
            return NULL;
        }
        *copiedKey = *(int *)key;
        //��ʼ���ڵ��Ա
        node->key = copiedKey;        // ��ֵ��ͨ��ΪmajorId�ĵ�ַ�� ע�����ڵ�key�Ƕ���������int*
        node->value = value;    // ����ֵ��ͨ��ΪMajor�ṹ��ĵ�ַ��
        node->prev = NULL;      // ǰ���ڵ��ʼ��ΪNULL
        node->next = NULL;      // ��̽ڵ��ʼ��ΪNULL
        return node;
    }

    //��ʼ������
    bool initMajorHashTable(MajorHashTable *table, int bucketCount, HashFunc func)
    {
        if (!table || bucketCount <= 0 || !func) return false;
        //�����ϣ��Ͱ���飬ÿһ��Ͱ����MajorNode* ָ�룬��ʼΪNull
        table->buckets = (MajorNode  **)calloc(bucketCount, sizeof(MajorNode  *));
        if (!table->buckets) 
        {
            printf("��ϣ���ʼ��ʧ�ܣ��ڴ治�㡣(�����룺%d)\n", errno);
            perror("��ϣ���ʼ��ʧ�ܣ�\n");
            return false;
        } 
        table->bucketCount = bucketCount;  // ��¼Ͱ������
        table->size = 0;                    // ��ʼ��Чרҵ����Ϊ0
        table->hashFunc = func;             // �洢��ϣ����
        printf("��ϣ���ʼ���ɹ���Ͱ����: %d���ڴ��ַ: %p\n", 
        bucketCount, (void*)table->buckets);
        return true;
    }



    //����ڵ�
    bool insertMajor(MajorHashTable *table, Major m)
    {
        // ��ǿ�氲ȫ���
        if (!table) {
            printf("[���ش���] tableָ��ΪNULL���޷�����\n");
            return false;
        }
        
        // ���table�Ƿ����ͷţ�ͨ��buckets�Ƿ�ΪNULL�жϣ�
        if (!table->buckets) 
        {
            printf("[����] ��ϣ��δ��ʼ�������ͷţ�table��ַ: %p\n", (void*)table);
            return false;
        }
        
        if (table->bucketCount <= 0) {
            printf("[����] ��ϣ��Ͱ������Ч: %d\n", table->bucketCount);
            return false;
        }
        
        if (!table->hashFunc) {
            printf("[����] ��ϣ����δ����\n");
            return false;
        }
    
        
        // ΪMajor�ṹ�������ڴ�
        Major *newMajor = malloc(sizeof(Major));
        if (!newMajor) {
            printf("�ڴ����ʧ�ܣ�\n");
            return false;
        }
        *newMajor = m;  // ��������

        int index = table->hashFunc(&newMajor->majorId, table->bucketCount);
        if (index < 0 || index >= table->bucketCount)
        {
            printf("��ϣ�������طǷ�������%d\n", index);
            return false;
        }
        

        //1.����Ƿ������ͬ��majorid,�����ظ����� 
        MajorNode *current = table->buckets[index];
        while (current) 
        {
            printf("[�������] ��ǰͰ������ majorId = %d\n", *(int*)current->key);
            if (*(int*)current->key  == m.majorId)
            {
                printf("�����ظ�רҵ(����ʧ��)��majorId %d �Ѵ��ڡ�\n", m.majorId);
                return true; //���ؽ׶��������������Ǵ���ֻ�������ظ���Ŀ
            }
            current = current->next;
        }

        //2ʹ�ó�ʼ����ϣ�ڵ㺯�����������½ڵ�
        MajorNode *newNode = init_major_node(&newMajor->majorId, newMajor);
        if (!newNode) 
        {
            printf("�ڴ����ʧ�ܣ�\n");
            return false;
        }
        
        newNode->next = table->buckets[index];   //�½ڵ��nextָ�룬ָ��ԭ��ͷ    ͷ�巨
        
        //˫���������ͷ�巨
        if (table->buckets[index])
        {
            table->buckets[index]->prev = newNode;
        }
        table->buckets[index] = newNode;
        
        table->size++;   
        printf("������ɣ�majorId=%d����ǰ size=%d����ϣ����=%d\n", m.majorId, table->size, index);
 
        return true;
    }


    //����
    Major *findMajor(MajorHashTable *table,  int majorId)     //����ֵָ���NULL
    {
        printf("���Բ���רҵID=%d����ϣ����=%d\n", majorId, table->hashFunc(&majorId, table->bucketCount));
        if (!table || !table->buckets || !table->hashFunc) return NULL;
        
        int index =table->hashFunc(&majorId, table->bucketCount);
        MajorNode *current = table->buckets[index];

        while (current)
        {
            if (*(int *)current->key == majorId)
            {
                return (Major *)current->value;
            }
            current = current->next;
        }
        return NULL; 
    }


    //ɾ��
    bool deleteMajor(MajorHashTable *table, int majorId)
    { 
        if (!table || !table->buckets || !table->hashFunc) return false;
        printf("����ɾ��ID=%d����ϣ����=%d\n", majorId, table->hashFunc(&majorId, table->bucketCount)); 
        int index = table->hashFunc(&majorId, table->bucketCount);
        MajorNode *current = table->buckets[index];

        while (current)
        {
            if (*(int *)current->key == majorId)
            {
                //��������ָ��
                if (current->prev )
                {
                    current->prev->next = current->next;
                }
                else
                {
                    table->buckets[index] = current->next;
                }
                if (current->next)
                {
                    current->next->prev = current->prev;
                }
                free(current);
                table->size--;
                return true; 
            }
            current = current->next;
        } 
        //�־û����� ���רҵ�ĳ־û�����
        if (saveMajors(table))
        {
            printf("[�־û�]רҵ��Ϣ����ɹ�\n");
        }else
        {
            printf("[�־û�]����ʧ�ܣ�����/dataĿ¼Ȩ�ޡ�\n");
        } 
        return true;
    }

    //����չʾ����רҵ��Ϣ
    void displayAllMajors(MajorHashTable *table)
    {
        if ( !table || !table->buckets)
        {
            printf("��ϣ��Ϊ�� \n");
            return;
        } 

        printf("\n----------------��ӡ����רҵ��Ϣ-------------------\n");
        int printted = 0;
        for (size_t i = 0; i < table->bucketCount; i++)
        {
            MajorNode *node =table->buckets[i];
            while (node)
            {
                Major *m =(Major *)node->value;    //
                printf("רҵID:%d, | רҵ����:%s, |רҵԺϵ:%d, |רҵѧ������:%d, |רҵ״̬:%d\n",
                m->majorId, m->majorName, m->departmentId, m->studentCount, m->status);
                node = node->next;
                printted++;
            }
            
        }
        if (printted == 0)
        {
            printf("�����κ�רҵ��Ϣ��(��Ч�ڵ�0)\n");
        } 
        printf("רҵ��������%d����ϣ��Ͱ����%d��ƽ��ÿͰ %.2f ��\n",
            table->size, table->bucketCount,
            (float)table->size / table->bucketCount);
    }


    bool updateMajor(MajorHashTable *table, int majorId)
    {
        Major *major = findMajor(table, majorId);
        if (!major) return false;

        printf("�������µ�רҵ����: ");
        scanf("%s", major->majorName);
        printf("�������µ�רҵ����: ");
        scanf(" %s", major->majorDesc);
        printf("�������µ�����ԺϵID: ");
        scanf("%d", &major->departmentId);
        printf("�������µ�Ժϵ����: ");
        scanf("%s", major->departmentName);
        printf("�������µ�רҵ����: ");
        scanf("%s", major->majorCode);
        printf("�������µ������������: ");
        scanf("%d", &major->maxStudentCapacity);
        printf("�������µ���������ƻ�: ");
        scanf("%d", &major->annualEnrollment);
        
        major->updateTime = getCurrentDate();    // ���滻Ϊ��ǰʱ��
        //�־û����� �����רҵ�ĳ־û�����
        if (saveMajors(table))
        {
            printf("[�־û�]רҵ��Ϣ����ɹ�\n");
        }else
        {
            printf("[�־û�]����ʧ�ܣ�����/dataĿ¼Ȩ�ޡ�\n");
        } 
        return true;
    }



    //�ͷŹ�ϣ����Դ
    void freeMajorHashTable(MajorHashTable *table)
    {
        if (!table || !table->buckets) return;
        
        for (size_t i = 0; i < table->bucketCount; ++i)
        {
            MajorNode *node =table->buckets[i];
            while (node)
            {
                MajorNode *temp =node;
                free(temp->value);  // �ͷ�Major�ṹ��
                node =node->next;
                free(temp);
            } 
        }
        
        free(table->buckets);
        table->buckets = NULL;
        table->size = 0;
        table->bucketCount = 0;
        table->hashFunc =NULL;
    }



    // ��ʾרҵ����ģ��˵�
    void showMajorManagementMenu() 
    {
        printf("\n--- רҵ����ģ��˵� ---\n");
        printf("%d. ����רҵ\n", MAJOR_MENU_ADD);
        printf("%d. ɾ��רҵ\n", MAJOR_MENU_DELETE);
        printf("%d. ��ѯרҵ\n", MAJOR_MENU_FIND);
        printf("%d. �޸�רҵ\n", MAJOR_MENU_UPDATE);
        printf("%d. ��ʾ����רҵ\n", MAJOR_MENU_DISPLAY_ALL);
        printf("%d. �������˵�\n", MAJOR_MENU_BACK);
        printf("��ѡ��:");
    }

    // רҵID���ɺ����� ����ʱ������ϣ��
    int generateUniqueMajorId() 
    {
        static int currentId = 0;
        //��һ�ε���ʱ������ҳ�ʼ��
        if (currentId == 0)
        {
            time_t now = time(NULL);    //�ɹ������ص�ǰʱ������� 1970-01-01 ������������
            //ȡ�ļ�������λ+ƫ��
            currentId =(now % 100000) + 1000;
        }
        
        return currentId++;
    }

    //��ӡ��ѯ��רҵID������Ϣ
    void printMajorDetails(const Major *major)
    {
        if (major == NULL)
        {
            printf("����רҵ��ϢΪ��\n");
            return;
        }

        printf("\n================== רҵ����  ==================  \n");
        printf("רҵID: %d\n", major->majorId);
        printf("רҵ����: %s\n", major->majorName);
        printf("רҵ����: %s\n", major->majorDesc);
        printf("����ԺϵID: %d\n", major->departmentId);
        printf("Ժϵ����: %s\n", major->departmentName);
        printf("רҵ����: %s\n", major->majorCode);

        //��ӡ״̬(�ı�����ʾ)
        const char *statusTexts[] = {"ͣ��", "����", "������"};
        printf("רҵ״̬��%s (%d)\n",
                (major->status >= 0 && major->status < 3)?
                statusTexts[major->status] : "δ֪",major->status );
        printf("��ǰѧ������: %d\n", major->studentCount);
        printf("רҵ������ID: %d\n", major->responsibleTeacherId);
        printf("��ϵ�绰: %s\n", major->contactPhone);
        printf("��������: %s\n", major->email);

        //��ӡ����
        printf("����ʱ�䣺%04d-%02d-%02d\n",
                major->createTime.year, major->createTime.month, major->createTime.day);
        printf("����ʱ��: %04d-%02d-%02d\n", 
            major->updateTime.year, major->updateTime.month, major->updateTime.day);
        printf("��ǰѧ����: %d\n", major->currentStudentCount);
        printf("�����������: %d\n", major->maxStudentCapacity);
        printf("��������ƻ�: %d\n", major->annualEnrollment);
        printf("�ۼƱ�ҵ����: %d\n", major->graduatedStudentCount);

        //��ӡ��ɫ��ǩ
        printf("��ɫ��ǩ: ");
        int firstTag = 1;
        for (size_t i = 0; i < 10; i++)
        {
            if (strlen(major->tags[i])  > 0 ) 
            {
                if (!firstTag) printf(", ");
                {
                    printf("%s", major->tags[i]);
                    firstTag = 0;
                } 
            } 
        }
        printf("\n==============================================\n");    
    }

    // ����רҵ����ģ���ѡ�������ϣ��
    MajorMenuOption  handleMajorManagementChoice(MajorMenuOption choice, SystemContext *ctx) 
    {
        int id;

        switch (choice) 
        {
            case MAJOR_MENU_ADD: 
            {
                Major m;
                printf("������רҵ���ƣ�");
                scanf("%s", m.majorName);
                printf("������רҵ������");
                scanf(" %s", m.majorDesc); //  
                printf("����������ԺϵID��");
                scanf("%d", &m.departmentId);
                printf("������Ժϵ���ƣ�");
                scanf("%s", m.departmentName);
                printf("������רҵ���룺");
                scanf("%s", m.majorCode);
                printf("�������������������");
                scanf("%d", &m.maxStudentCapacity);
                printf("��������������ƻ���");
                scanf("%d", &m.annualEnrollment);

                m.majorId = generateUniqueMajorId();
                m.status = 1;
                m.studentCount = 0;
                m.currentStudentCount = 0;
                m.graduatedStudentCount = 0;
                m.responsibleTeacherId = -1;
                strcpy(m.contactPhone, "δ֪");
                strcpy(m.email, "δ֪");
                m.createTime = getCurrentDate();
                m.updateTime = m.createTime;

                if (insertMajor(ctx->majorTable, m)) 
                {
                    printf("רҵ��ӳɹ���ID: %d\n", m.majorId);
                    //ctx->majorTable->size++;
                    saveMajors(ctx->majorTable);
                    
                } else {
                    printf("���ʧ�ܣ���רҵ�����Ѵ��ڡ�\n");
                }
                break;
            }

            case MAJOR_MENU_DELETE:
                printf("������Ҫɾ����רҵID: ");
                scanf("%d", &id);
                if (deleteMajor(ctx->majorTable, id)) 
                {
                    printf("ɾ���ɹ���\n");
                    saveMajors(ctx->majorTable);
                } else {
                    printf("ɾ��ʧ�ܣ�δ�ҵ���רҵ��\n");
                }
                break;

            case MAJOR_MENU_FIND: {
                printf("������Ҫ��ѯ��רҵID: ");
                scanf("%d", &id);
                Major *found = findMajor(ctx->majorTable, id);
                if (found) 
                {
                    printMajorDetails(found);
                } else {
                    printf("δ�ҵ���רҵ��\n");
                }
                break;
            }

            case MAJOR_MENU_UPDATE:
                printf("������Ҫ�޸ĵ�רҵID: ");
                scanf("%d", &id);
                if (!updateMajor(ctx->majorTable, id)) {
                    printf("����ʧ�ܣ�δ�ҵ���רҵ��\n");
                }
                break;

            case MAJOR_MENU_DISPLAY_ALL:
                //loadMajors(ctx->majorTable);
                displayAllMajors(ctx->majorTable);
                break;

            case MAJOR_MENU_BACK:
                printf("�������˵�...\n");
                return  MAJOR_EXIT;
                //break;

            default:
                printf("��Чѡ�\n");
                break;
        }
        return choice;  // Ĭ�Ϸ���ԭѡ��
        // �������뻺����
        while (getchar() != '\n');
    }





    // // ����רҵ
    // void addMajor(Major *majors, int *count) 
    // {
    //     printf("������רҵ����: ");
    //     scanf("%s", majors[*count].majorName);

    //     printf("����������ԺϵID: ");
    //     scanf("%d", &majors[*count].departmentId);

    //     majors[*count].majorId = generateUniqueMajorId();
    //     majors[*count].studentCount = 0; // ��ʼ��Ϊ0

    //     printf("��ӳɹ���רҵIDΪ��%d\n", majors[*count].majorId);
    //     (*count)++;
    // }

    // // ��ʾ����רҵ��Ϣ
    // int displayMajor(const Major *majors, int count) 
    // {
    //     if (count == 0) {
    //         printf("����רҵ��Ϣ����ʾ��\n");
    //         return 0;
    //     }

    //     printf("\n--- ����רҵ��Ϣ ---\n");
    //     for (int i = 0; i < count; i++) {
    //         printf("ID: %d | ����: %s | ԺϵID: %d | ��ǰѧ����: %d\n",
    //                majors[i].majorId,
    //                majors[i].majorName,
    //                majors[i].departmentId,
    //                majors[i].studentCount);
    //     }

    //     return 1;
    // }

    // // ɾ��רҵ
    // void deleteMajor(Major *majors, int *count, int id) {
    //     int index = -1;
    //     for (int i = 0; i < *count; i++) {
    //         if (majors[i].majorId == id) {
    //             index = i;
    //             break;
    //         }
    //     }

    //     if (index == -1) {
    //         printf("δ�ҵ���ӦרҵID��\n");
    //         return;
    //     }

    //     for (int i = index; i < *count - 1; i++) {
    //         majors[i] = majors[i + 1];
    //     }

    //     (*count)--;
    //     printf("ɾ���ɹ���\n");
    // }

    // // ��ѯרҵ��Ϣ
    // void findMajor(const Major *majors, int count, int id) 
    // {
    //     for (int i = 0; i < count; i++) 
    //     {
    //         if (majors[i].majorId == id) 
    //         {
    //             printf("רҵID: %d\nרҵ����: %s\n����ԺϵID: %d\nѧ������: %d\n",
    //                    majors[i].majorId,
    //                    majors[i].majorName,
    //                    majors[i].departmentId,
    //                    majors[i].studentCount);
    //             return;
    //         }
    //     }
    //     printf("δ�ҵ���רҵ��Ϣ��\n");
    // }

    // // �޸�רҵ��Ϣ
    // void updateMajor(Major *majors, int count, int id) {
    //     for (int i = 0; i < count; i++) {
    //         if (majors[i].majorId == id) {
    //             printf("�������µ�רҵ����: ");
    //             scanf("%s", majors[i].majorName);
    //             printf("�������µ�ԺϵID: ");
    //             scanf("%d", &majors[i].departmentId);
    //             printf("���³ɹ���\n");
    //             return;
    //         }
    //     }
    //     printf("δ�ҵ���רҵ��Ϣ��\n");
    // }

    // // ��ʾרҵ����ģ��˵�
    // void showMajorManagementMenu() 
    // {
    //     printf("\n--- רҵ����ģ��˵� ---\n");
    //     printf("%d. ����רҵ\n", MAJOR_MENU_ADD);
    //     printf("%d. ɾ��רҵ\n", MAJOR_MENU_DELETE);
    //     printf("%d. ��ѯרҵ\n", MAJOR_MENU_FIND);
    //     printf("%d. �޸�רҵ\n", MAJOR_MENU_UPDATE);
    //     printf("%d. ��ʾ����רҵ\n", MAJOR_MENU_DISPLAY_ALL);
    //     printf("%d. �������˵�\n", MAJOR_MENU_BACK);
    //     printf("��ѡ��\n");
    // }

    // // רҵID���ɺ�������ʾ�ã�ʵ�ʿ�����ʱ������ϣ��
    // int generateUniqueMajorId() {
    //     static int currentId = 1000;
    //     return currentId++;
    // }

    // // ����רҵ����ģ���ѡ��
    // void handleMajorManagementChoice(MajorMenuOption choice, SystemContext *ctx)     //Major *majors, int *count
    // {
    //     int id;  // ���ڴ洢רҵID
        
    //     switch (choice) {
    //         case MAJOR_MENU_ADD: 
    //             addMajor(ctx->majors, ctx->majorCount);
    //             break;
    //         case MAJOR_MENU_DELETE:
    //             printf("������Ҫɾ����רҵID: ");
    //             scanf("%d", &id);
    //             deleteMajor(ctx->majors, ctx->majorCount, id);
    //             break;
    //         case MAJOR_MENU_FIND:
    //             printf("������Ҫ��ѯ��רҵID: ");
    //             scanf("%d", &id);
    //             findMajor(ctx->majors, *ctx->majorCount, id);
    //             break;
    //         case MAJOR_MENU_UPDATE:
    //             printf("������Ҫ�޸ĵ�רҵID: ");
    //             scanf("%d", &id);
    //             updateMajor(ctx->majors, *ctx->majorCount, id);
    //             break;
    //         case MAJOR_MENU_DISPLAY_ALL:
    //             displayMajor(ctx->majors, *ctx->majorCount);
    //             break;
    //         case MAJOR_MENU_BACK:
    //             printf("�������˵�...\n");
    //             break;
    //         default:
    //             printf("��Чѡ�\n");
    //             break;
    //     }

    //     // �������뻺����
    //     while (getchar() != '\n');
    // }

