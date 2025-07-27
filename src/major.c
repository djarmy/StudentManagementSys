    
    #include <stdio.h>
    #include <string.h>
    #include <errno.h>
    #include <time.h>

    #include "../include/context.h"
    #include "../include/major.h"
    #include "../include/student.h" 
    #include "../include/major_persist.h"
    #include "../include/utils.h"

    // 哈希函数：根据 majorId 计算哈希值
    int hash_func_int(const void *key, int bucketCount)
    {
        if (!key || bucketCount <= 0) 
        {
            printf("哈希函数参数错误：key=%p, bucketCount=%d\n", 
                key, bucketCount);
            return 0;
        }
        
        //安全类型转换
        int id = *(const int *)key;
        // 2. 处理负数ID（常见改进）
        id = (id < 0) ? -(id) : id;

        // 3. 防止除数为0
        if (bucketCount == 0) 
        {
            bucketCount = 1;
        }

        //计算哈希值
        int index = id % bucketCount;
        return index < 0 ? -index : index;
    }

    // 字符串哈希函数（DJB2算法）
    unsigned int hash_func_str(const void *key, int bucketCount) {
        if (!key || bucketCount <= 0) {
            printf("哈希函数参数错误：key=%p, bucketCount=%d\n", 
                key, bucketCount);
            return 0;
        }
        
        unsigned int hash = 5381;
        const char *str = (const char *)key;
        
        int c;
        // DJB2算法核心（使用unsigned long避免溢出）
        while ((c = *str++)) {
            hash = ((hash << 5) + hash) + c;  // hash * 33 + c
        }
        
        // 处理桶数量为0的情况
        if (bucketCount == 0) {
            bucketCount = 1;
        }
        
        // 转换为int并确保索引在有效范围
        int index = hash % bucketCount;
        return index < 0 ? -index : index;
    }


    //初始化专业哈希节点
    MajorNode *init_major_node(void *key, void *value)
    { //分配节点内存
        MajorNode *node = (MajorNode *)malloc(sizeof(MajorNode));
        if (!node)
        {
            printf("内存分配失败，无法初始化节点\n");
            return NULL;
        }
        //这里新增代码：为key(存储的majorId)单独分配一份内存
        int *copiedKey =malloc(sizeof(int));
        if (!copiedKey)
        {
            printf("为 key 分配内存失败！\n");
            free(node);
            return NULL;
        }
        *copiedKey = *(int *)key;
        //初始化节点成员
        node->key = copiedKey;        // 键值（通常为majorId的地址） 注意现在的key是独立拷贝的int*
        node->value = value;    // 数据值（通常为Major结构体的地址）
        node->prev = NULL;      // 前驱节点初始化为NULL
        node->next = NULL;      // 后继节点初始化为NULL
        return node;
    }

    //初始化函数
    bool initMajorHashTable(MajorHashTable *table, int bucketCount, HashFunc func)
    {
        if (!table || bucketCount <= 0 || !func) return false;
        //分配哈希表桶数组，每一个桶都是MajorNode* 指针，初始为Null
        table->buckets = (MajorNode  **)calloc(bucketCount, sizeof(MajorNode  *));
        if (!table->buckets) 
        {
            printf("哈希表初始化失败：内存不足。(错误码：%d)\n", errno);
            perror("哈希表初始化失败：\n");
            return false;
        } 
        table->bucketCount = bucketCount;  // 记录桶的数量
        table->size = 0;                    // 初始有效专业数量为0
        table->hashFunc = func;             // 存储哈希函数
        printf("哈希表初始化成功，桶数量: %d，内存地址: %p\n", 
        bucketCount, (void*)table->buckets);
        return true;
    }



    //插入节点
    bool insertMajor(MajorHashTable *table, Major m)
    {
        // 增强版安全检查
        if (!table) {
            printf("[严重错误] table指针为NULL，无法插入\n");
            return false;
        }
        
        // 检查table是否已释放（通过buckets是否为NULL判断）
        if (!table->buckets) 
        {
            printf("[错误] 哈希表未初始化或已释放，table地址: %p\n", (void*)table);
            return false;
        }
        
        if (table->bucketCount <= 0) {
            printf("[错误] 哈希表桶数量无效: %d\n", table->bucketCount);
            return false;
        }
        
        if (!table->hashFunc) {
            printf("[错误] 哈希函数未设置\n");
            return false;
        }
    
        
        // 为Major结构体分配堆内存
        Major *newMajor = malloc(sizeof(Major));
        if (!newMajor) {
            printf("内存分配失败！\n");
            return false;
        }
        *newMajor = m;  // 复制内容

        int index = table->hashFunc(&newMajor->majorId, table->bucketCount);
        if (index < 0 || index >= table->bucketCount)
        {
            printf("哈希函数返回非法索引：%d\n", index);
            return false;
        }
        

        //1.检查是否存在相同的majorid,避免重复插入 
        MajorNode *current = table->buckets[index];
        while (current) 
        {
            printf("[插入调试] 当前桶中已有 majorId = %d\n", *(int*)current->key);
            if (*(int*)current->key  == m.majorId)
            {
                printf("跳过重复专业(插入失败)：majorId %d 已存在。\n", m.majorId);
                return true; //加载阶段允许跳过，不是错误，只是跳过重复项目
            }
            current = current->next;
        }

        //2使用初始化哈希节点函数，来创建新节点
        MajorNode *newNode = init_major_node(&newMajor->majorId, newMajor);
        if (!newNode) 
        {
            printf("内存分配失败！\n");
            return false;
        }
        
        newNode->next = table->buckets[index];   //新节点的next指针，指向原表头    头插法
        
        //双向链表插入头插法
        if (table->buckets[index])
        {
            table->buckets[index]->prev = newNode;
        }
        table->buckets[index] = newNode;
        
        table->size++;   
        printf("插入完成：majorId=%d，当前 size=%d，哈希索引=%d\n", m.majorId, table->size, index);
 
        return true;
    }


    //查找
    Major *findMajor(MajorHashTable *table,  int majorId)     //返回值指针或NULL
    {
        printf("尝试查找专业ID=%d，哈希索引=%d\n", majorId, table->hashFunc(&majorId, table->bucketCount));
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


    //删除
    bool deleteMajor(MajorHashTable *table, int majorId)
    { 
        if (!table || !table->buckets || !table->hashFunc) return false;
        printf("尝试删除ID=%d，哈希索引=%d\n", majorId, table->hashFunc(&majorId, table->bucketCount)); 
        int index = table->hashFunc(&majorId, table->bucketCount);
        MajorNode *current = table->buckets[index];

        while (current)
        {
            if (*(int *)current->key == majorId)
            {
                //更新链表指针
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
        //持久化保存 完成专业的持久化保存
        if (saveMajors(table))
        {
            printf("[持久化]专业信息保存成功\n");
        }else
        {
            printf("[持久化]保存失败！请检查/data目录权限。\n");
        } 
        return true;
    }

    //遍历展示所有专业信息
    void displayAllMajors(MajorHashTable *table)
    {
        if ( !table || !table->buckets)
        {
            printf("哈希表为空 \n");
            return;
        } 

        printf("\n----------------打印所有专业信息-------------------\n");
        int printted = 0;
        for (size_t i = 0; i < table->bucketCount; i++)
        {
            MajorNode *node =table->buckets[i];
            while (node)
            {
                Major *m =(Major *)node->value;    //
                printf("专业ID:%d, | 专业名称:%s, |专业院系:%d, |专业学生人数:%d, |专业状态:%d\n",
                m->majorId, m->majorName, m->departmentId, m->studentCount, m->status);
                node = node->next;
                printted++;
            }
            
        }
        if (printted == 0)
        {
            printf("暂无任何专业信息！(有效节点0)\n");
        } 
        printf("专业总数量：%d，哈希表桶数：%d，平均每桶 %.2f 项\n",
            table->size, table->bucketCount,
            (float)table->size / table->bucketCount);
    }


    bool updateMajor(MajorHashTable *table, int majorId)
    {
        Major *major = findMajor(table, majorId);
        if (!major) return false;

        printf("请输入新的专业名称: ");
        scanf("%s", major->majorName);
        printf("请输入新的专业描述: ");
        scanf(" %s", major->majorDesc);
        printf("请输入新的所属院系ID: ");
        scanf("%d", &major->departmentId);
        printf("请输入新的院系名称: ");
        scanf("%s", major->departmentName);
        printf("请输入新的专业代码: ");
        scanf("%s", major->majorCode);
        printf("请输入新的最大招生容量: ");
        scanf("%d", &major->maxStudentCapacity);
        printf("请输入新的年度招生计划: ");
        scanf("%d", &major->annualEnrollment);
        
        major->updateTime = getCurrentDate();    // 可替换为当前时间
        //持久化保存 待完成专业的持久化保存
        if (saveMajors(table))
        {
            printf("[持久化]专业信息保存成功\n");
        }else
        {
            printf("[持久化]保存失败！请检查/data目录权限。\n");
        } 
        return true;
    }



    //释放哈希表资源
    void freeMajorHashTable(MajorHashTable *table)
    {
        if (!table || !table->buckets) return;
        
        for (size_t i = 0; i < table->bucketCount; ++i)
        {
            MajorNode *node =table->buckets[i];
            while (node)
            {
                MajorNode *temp =node;
                free(temp->value);  // 释放Major结构体
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



    // 显示专业管理模块菜单
    void showMajorManagementMenu() 
    {
        printf("\n--- 专业管理模块菜单 ---\n");
        printf("%d. 增加专业\n", MAJOR_MENU_ADD);
        printf("%d. 删除专业\n", MAJOR_MENU_DELETE);
        printf("%d. 查询专业\n", MAJOR_MENU_FIND);
        printf("%d. 修改专业\n", MAJOR_MENU_UPDATE);
        printf("%d. 显示所有专业\n", MAJOR_MENU_DISPLAY_ALL);
        printf("%d. 返回主菜单\n", MAJOR_MENU_BACK);
        printf("请选择:");
    }

    // 专业ID生成函数（ 引入时间戳或哈希）
    int generateUniqueMajorId() 
    {
        static int currentId = 0;
        //第一次调用时间戳并且初始化
        if (currentId == 0)
        {
            time_t now = time(NULL);    //成功：返回当前时间戳（自 1970-01-01 以来的秒数）
            //取四件戳后四位+偏移
            currentId =(now % 100000) + 1000;
        }
        
        return currentId++;
    }

    //打印查询的专业ID完整信息
    void printMajorDetails(const Major *major)
    {
        if (major == NULL)
        {
            printf("错误：专业信息为空\n");
            return;
        }

        printf("\n================== 专业详情  ==================  \n");
        printf("专业ID: %d\n", major->majorId);
        printf("专业名称: %s\n", major->majorName);
        printf("专业描述: %s\n", major->majorDesc);
        printf("所属院系ID: %d\n", major->departmentId);
        printf("院系名称: %s\n", major->departmentName);
        printf("专业代码: %s\n", major->majorCode);

        //打印状态(文本化显示)
        const char *statusTexts[] = {"停用", "启用", "招生中"};
        printf("专业状态：%s (%d)\n",
                (major->status >= 0 && major->status < 3)?
                statusTexts[major->status] : "未知",major->status );
        printf("当前学生数量: %d\n", major->studentCount);
        printf("专业负责人ID: %d\n", major->responsibleTeacherId);
        printf("联系电话: %s\n", major->contactPhone);
        printf("电子邮箱: %s\n", major->email);

        //打印日期
        printf("创建时间：%04d-%02d-%02d\n",
                major->createTime.year, major->createTime.month, major->createTime.day);
        printf("更新时间: %04d-%02d-%02d\n", 
            major->updateTime.year, major->updateTime.month, major->updateTime.day);
        printf("当前学生数: %d\n", major->currentStudentCount);
        printf("最大招生容量: %d\n", major->maxStudentCapacity);
        printf("年度招生计划: %d\n", major->annualEnrollment);
        printf("累计毕业人数: %d\n", major->graduatedStudentCount);

        //打印特色标签
        printf("特色标签: ");
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

    // 处理专业管理模块的选择（适配哈希表）
    MajorMenuOption  handleMajorManagementChoice(MajorMenuOption choice, SystemContext *ctx) 
    {
        int id;

        switch (choice) 
        {
            case MAJOR_MENU_ADD: 
            {
                Major m;
                printf("请输入专业名称：");
                scanf("%s", m.majorName);
                printf("请输入专业描述：");
                scanf(" %s", m.majorDesc); //  
                printf("请输入所属院系ID：");
                scanf("%d", &m.departmentId);
                printf("请输入院系名称：");
                scanf("%s", m.departmentName);
                printf("请输入专业代码：");
                scanf("%s", m.majorCode);
                printf("请输入最大招生容量：");
                scanf("%d", &m.maxStudentCapacity);
                printf("请输入年度招生计划：");
                scanf("%d", &m.annualEnrollment);

                m.majorId = generateUniqueMajorId();
                m.status = 1;
                m.studentCount = 0;
                m.currentStudentCount = 0;
                m.graduatedStudentCount = 0;
                m.responsibleTeacherId = -1;
                strcpy(m.contactPhone, "未知");
                strcpy(m.email, "未知");
                m.createTime = getCurrentDate();
                m.updateTime = m.createTime;

                if (insertMajor(ctx->majorTable, m)) 
                {
                    printf("专业添加成功，ID: %d\n", m.majorId);
                    //ctx->majorTable->size++;
                    saveMajors(ctx->majorTable);
                    
                } else {
                    printf("添加失败，该专业可能已存在。\n");
                }
                break;
            }

            case MAJOR_MENU_DELETE:
                printf("请输入要删除的专业ID: ");
                scanf("%d", &id);
                if (deleteMajor(ctx->majorTable, id)) 
                {
                    printf("删除成功。\n");
                    saveMajors(ctx->majorTable);
                } else {
                    printf("删除失败，未找到该专业。\n");
                }
                break;

            case MAJOR_MENU_FIND: {
                printf("请输入要查询的专业ID: ");
                scanf("%d", &id);
                Major *found = findMajor(ctx->majorTable, id);
                if (found) 
                {
                    printMajorDetails(found);
                } else {
                    printf("未找到该专业。\n");
                }
                break;
            }

            case MAJOR_MENU_UPDATE:
                printf("请输入要修改的专业ID: ");
                scanf("%d", &id);
                if (!updateMajor(ctx->majorTable, id)) {
                    printf("更新失败，未找到该专业。\n");
                }
                break;

            case MAJOR_MENU_DISPLAY_ALL:
                //loadMajors(ctx->majorTable);
                displayAllMajors(ctx->majorTable);
                break;

            case MAJOR_MENU_BACK:
                printf("返回主菜单...\n");
                return  MAJOR_EXIT;
                //break;

            default:
                printf("无效选项。\n");
                break;
        }
        return choice;  // 默认返回原选择
        // 吸收输入缓冲区
        while (getchar() != '\n');
    }





    // // 增加专业
    // void addMajor(Major *majors, int *count) 
    // {
    //     printf("请输入专业名称: ");
    //     scanf("%s", majors[*count].majorName);

    //     printf("请输入所在院系ID: ");
    //     scanf("%d", &majors[*count].departmentId);

    //     majors[*count].majorId = generateUniqueMajorId();
    //     majors[*count].studentCount = 0; // 初始化为0

    //     printf("添加成功！专业ID为：%d\n", majors[*count].majorId);
    //     (*count)++;
    // }

    // // 显示所有专业信息
    // int displayMajor(const Major *majors, int count) 
    // {
    //     if (count == 0) {
    //         printf("暂无专业信息可显示。\n");
    //         return 0;
    //     }

    //     printf("\n--- 所有专业信息 ---\n");
    //     for (int i = 0; i < count; i++) {
    //         printf("ID: %d | 名称: %s | 院系ID: %d | 当前学生数: %d\n",
    //                majors[i].majorId,
    //                majors[i].majorName,
    //                majors[i].departmentId,
    //                majors[i].studentCount);
    //     }

    //     return 1;
    // }

    // // 删除专业
    // void deleteMajor(Major *majors, int *count, int id) {
    //     int index = -1;
    //     for (int i = 0; i < *count; i++) {
    //         if (majors[i].majorId == id) {
    //             index = i;
    //             break;
    //         }
    //     }

    //     if (index == -1) {
    //         printf("未找到对应专业ID。\n");
    //         return;
    //     }

    //     for (int i = index; i < *count - 1; i++) {
    //         majors[i] = majors[i + 1];
    //     }

    //     (*count)--;
    //     printf("删除成功！\n");
    // }

    // // 查询专业信息
    // void findMajor(const Major *majors, int count, int id) 
    // {
    //     for (int i = 0; i < count; i++) 
    //     {
    //         if (majors[i].majorId == id) 
    //         {
    //             printf("专业ID: %d\n专业名称: %s\n所属院系ID: %d\n学生数量: %d\n",
    //                    majors[i].majorId,
    //                    majors[i].majorName,
    //                    majors[i].departmentId,
    //                    majors[i].studentCount);
    //             return;
    //         }
    //     }
    //     printf("未找到该专业信息。\n");
    // }

    // // 修改专业信息
    // void updateMajor(Major *majors, int count, int id) {
    //     for (int i = 0; i < count; i++) {
    //         if (majors[i].majorId == id) {
    //             printf("请输入新的专业名称: ");
    //             scanf("%s", majors[i].majorName);
    //             printf("请输入新的院系ID: ");
    //             scanf("%d", &majors[i].departmentId);
    //             printf("更新成功！\n");
    //             return;
    //         }
    //     }
    //     printf("未找到该专业信息。\n");
    // }

    // // 显示专业管理模块菜单
    // void showMajorManagementMenu() 
    // {
    //     printf("\n--- 专业管理模块菜单 ---\n");
    //     printf("%d. 增加专业\n", MAJOR_MENU_ADD);
    //     printf("%d. 删除专业\n", MAJOR_MENU_DELETE);
    //     printf("%d. 查询专业\n", MAJOR_MENU_FIND);
    //     printf("%d. 修改专业\n", MAJOR_MENU_UPDATE);
    //     printf("%d. 显示所有专业\n", MAJOR_MENU_DISPLAY_ALL);
    //     printf("%d. 返回主菜单\n", MAJOR_MENU_BACK);
    //     printf("请选择：\n");
    // }

    // // 专业ID生成函数（演示用，实际可引入时间戳或哈希）
    // int generateUniqueMajorId() {
    //     static int currentId = 1000;
    //     return currentId++;
    // }

    // // 处理专业管理模块的选择
    // void handleMajorManagementChoice(MajorMenuOption choice, SystemContext *ctx)     //Major *majors, int *count
    // {
    //     int id;  // 用于存储专业ID
        
    //     switch (choice) {
    //         case MAJOR_MENU_ADD: 
    //             addMajor(ctx->majors, ctx->majorCount);
    //             break;
    //         case MAJOR_MENU_DELETE:
    //             printf("请输入要删除的专业ID: ");
    //             scanf("%d", &id);
    //             deleteMajor(ctx->majors, ctx->majorCount, id);
    //             break;
    //         case MAJOR_MENU_FIND:
    //             printf("请输入要查询的专业ID: ");
    //             scanf("%d", &id);
    //             findMajor(ctx->majors, *ctx->majorCount, id);
    //             break;
    //         case MAJOR_MENU_UPDATE:
    //             printf("请输入要修改的专业ID: ");
    //             scanf("%d", &id);
    //             updateMajor(ctx->majors, *ctx->majorCount, id);
    //             break;
    //         case MAJOR_MENU_DISPLAY_ALL:
    //             displayMajor(ctx->majors, *ctx->majorCount);
    //             break;
    //         case MAJOR_MENU_BACK:
    //             printf("返回主菜单...\n");
    //             break;
    //         default:
    //             printf("无效选项。\n");
    //             break;
    //     }

    //     // 吸收输入缓冲区
    //     while (getchar() != '\n');
    // }

