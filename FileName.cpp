#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <Windows.h>
#define STR_LEN 128
typedef struct _tContacts
{
    /*a:姓名 b : 分组 c : 办公电话
    d : 家庭电话 e : 手机 f : 邮箱*/
    char a[STR_LEN];
    char b[STR_LEN];
    char c[STR_LEN];
    char d[STR_LEN];
    char e[STR_LEN];
    char f[STR_LEN];
} Contacts, * pContacts;
/* 联系人列表：存储多个联系人 */
typedef struct _tContactsList
{
    int count;         // 联系人数量
    Contacts* contactsArray;   // 联系人数组指针
} ContactsList;

/* 分组结构体：存储单个组信息 */
typedef struct _tGroup
{
    char groupName[STR_LEN]; // 群组名称
} Group, * pGroup;

/* 群组列表：存储多个组 */
typedef struct _tGroupList
{
    int _tcount;     // 群组数量
    Group* groupsArray; // 群组数组指针
} GroupList;

/* 向群组列表添加新群组的函数 */
void addGroupList(GroupList* list, pGroup group)
{
    // 将新群组添加到数组末尾
    list->groupsArray[list->_tcount] = *group; // 复制群组数据到列表中
    list->_tcount++; // 增加群组计数器（确保数组有足够空间）
}
/* 从群组列表中删除指定位置的群组 */
void removeGroupList(GroupList* list, int position)
{
    // 检查位置是否合法（0 ≤ position < 当前群组数量）
    if (position >= 0 && position < list->_tcount)
    {
        int index;
        // 从目标位置后一个元素开始，前移覆盖要删除的元素
        for (index = position + 1; index < list->_tcount; ++index)
        {
            list->groupsArray[index - 1] = list->groupsArray[index];// 元素前移操作   
        }
        --list->_tcount;// 减少群组数量
    }
}
/* 通过群组名称查找群组索引 */
int findGroupListByName(GroupList* list, char* name)
{
    int index;
    // 遍历所有群组
    for (index = 0; index < list->_tcount; ++index)
    {
        // 比较群组名称
        if (strcmp(list->groupsArray[index].groupName, name) == 0)
        {
            return index;// 找到则返回索引（从0开始
        }
    }
    // 未找到返回-1
    return -1;
}
/* 将群组列表保存到文件 */
void saveGroupFile(GroupList* list)
{
    // 以写入模式打开文件
    FILE* output = fopen("groups.txt", "w");
    if (output)
    {
        // 遍历所有群组   
        int index;
        for (index = 0; index < list->_tcount; ++index)
        {
            pGroup group = &list->groupsArray[index]; // 获取当前群组指针
            fprintf(output, "%-10s ", group->groupName); // 格式：左对齐10字符宽度
            fprintf(output, "\n");// 换行分隔不同群组
        }
        fclose(output); //关闭文件
    }
    else
    {
        printf("写文件失败！\n");
    }
}
/* 从文件加载群组列表 */
void loadGroupFile(GroupList* L)
{
    FILE* I = fopen("groups.txt", "r"); // 以读取模式打开文件    
    if (I)
    {
        Group i = { 0 };// 临时存储读取的群组
        L->_tcount = 0;// 重置计数器
            while (1)
            {
                // 读取群组名称（遇到空格/换行停止）
                if (fscanf(I, "%s ", i.groupName) != 1)
                    break;
                addGroupList(L, &i);// 添加到列表
            }
            fclose(I);
    }
    else
    {
        const char* l[] =
        {
        "未知",
        "同事",
        "亲戚",
        "朋友",
        "家人",
        "同学",
        };
        int j = (int)(sizeof(l) / sizeof(l[0]));// 计算数组长度
        int k;
        for (k = 0; k < j; ++k)
        {
            Group o = { 0 };
            strcpy(o.groupName, l[k]);// 复制名称   
            addGroupList(L, &o);// 添加默认群组
        }
    }
}
void showGroupList(GroupList* list)
{
    int i;
    // 左对齐16字符宽度
    printf("%-16s", "序号");
    printf("%-16s", "名称");
    printf("\n");
    for (i = 0; i < list->_tcount; ++i)
    {
        pGroup group = &list->groupsArray[i];
        printf("%-16d", i + 1);// 显示从1开始的序号
        printf("%-16s", group->groupName);// 显示群组名称
        printf("\n");
    }
}
// 添加分组到分组列表
void addGroup(GroupList* list)
{
    // 初始化一个空分组结构体
    Group group = { 0 };
    printf(" $ 添加分组 $ \n");
    printf("输入名称：");
    // 获取用户输入的分组名
    scanf("%s", group.groupName);
    // 检查分组是否已存在（通过名称查找）
    if (findGroupListByName(list, group.groupName) == -1)
    {
        // 将新分组添加到列表
        addGroupList(list, &group);
        // 将更新后的列表保存到文件
        saveGroupFile(list);
        printf("添加成功！\n");
    }
    // 分组已存在的情况
    else
    {
        printf("该名称已存在，添加失败！\n");
    }
}
// 从分组列表删除分组
void removeGroup(GroupList* list)
{
    // 存储要删除的分组名
    char name[STR_LEN] = { 0 };
    // 分组在列表中的位置
    int position = -1;
    printf(" $ 删除分组 $ \n");
    printf("输入名称：");
    // 获取要删除的分组名
    scanf("%s", name);
    // 查找分组位置
    position = findGroupListByName(list, name);
    // 找到分组的情况
    if (position != -1)
    {
        // 从列表中移除分组
        removeGroupList(list, position);
        // 保存更新后的列表到文件
        saveGroupFile(list);
        printf("删除成功!\n");
    }
    // 分组不存在的情况
    else
    {
        printf("没有该名称!\n");
    }
}
// 选择分组
void selectGroup(GroupList* list, char* name)
{
    // 检查分组列表是否为空
    if (list->_tcount)
    {
        // 循环直到选择有效分组
        while (1)
        {
            int index;
            // 用户选择的序号
            int option;
            printf("\n");
            printf("选择分组：\n");
            printf("---------------------\n");
            // 左对齐16字符宽度
            printf("%-16s", "序号");
            printf("%-16s", "名称");
            printf("\n");
            // 遍历所有分组显示信息
            for (index = 0; index < list->_tcount; ++index)
            {
                // 获取分组指针
                pGroup group = &list->groupsArray[index];
                // 显示从1开始的序号
                printf("%-16d", index + 1);
                // 显示分组名称
                printf("%-16s", group->groupName);
                printf("\n");
            }
            printf("---------------------\n");
            // 获取用户输入
            printf("      请输入分组序号：");
            scanf("%d", &option);
            if (option > 0 && option <= list->_tcount)
            {
                // 将选择的分组名称复制到输出参数
                strcpy(name, list->groupsArray[option - 1].groupName);
                break;
            }
        }
    }
    // 分组列表为空的情况
    else
    {
        strcpy(name, "未知");
    }
}
// 分组管理主菜单
void manageGroup(GroupList* list)
{
    // 循环显示菜单
    while (1)
    {
        // 用户选择的菜单项
        int option;
        // 显示管理菜单界面
        printf("╔----------------------------╗\n");
        printf("  $ 分组管理 $ \n");
        printf(" 【1】浏览分组信息\n");
        printf(" 【2】添加分组\n");
        printf(" 【3】删除分组\n");
        printf(" 【0】返回\n");
        printf("╚----------------------------╝\n");
        printf("       请选择：");
        scanf("%d", &option);
        if (option == 0)
            break;
        switch (option)
        {
        case 1:
            // 显示分组列表
            showGroupList(list);
            break;
        case 2:
            // 添加分组
            addGroup(list);
            break;
        case 3:
            // 删除分组
            removeGroup(list);
            break;
        }
    }
}

/* 联系人列表操作模块 */

// 添加联系人到列表
void addContactsList(ContactsList* list, pContacts contacts)
{
    // 直接将联系人对象存入数组末尾
    list->contactsArray[list->count++] = *contacts;
}
// 从列表中移除指定位置的联系人   
void removeContactsList(ContactsList* list, int position)
{
    if (position >= 0 && position < list->count) {
        int index;
        // 通过元素前移实现删除操作
        for (index = position + 1; index < list->count; ++index)
        {
            // 将后续元素逐个前移覆盖
            list->contactsArray[index - 1] = list->contactsArray[index];
        }
        // 更新列表元素计数
        --list->count;
    }
}
// 按姓名查找联系人
int findContactsListByName(ContactsList* list, char* name, int begin)
{
    int index;
    // 遍历列表进行精确匹配查找
    for (index = begin; index < list->count; ++index)
    {
        // 结构体字段a存储姓名
        if (strcmp(list->contactsArray[index].a, name) == 0)
        {
            return index;
        }
    }
    return -1;
}
// 按分类查找联系人
int findContactsListByCategory(ContactsList* list, char* category, int begin)
{
    int index;
    for (index = begin; index < list->count; ++index)
    {
        // 结构体字段b存储分类信息
        if (strcmp(list->contactsArray[index].b, category) == 0)
        {
            return index;
        }
    }
    return -1;
}
// 按电话号码查找联系人
int findContactsListByPhone(ContactsList* list, char* phone, int begin)
{
    int index;
    for (index = begin; index < list->count; ++index)
    {
        // 字段c/d/e分别存储不同电话号码
       // 检查三个电话字段中的任意一个匹配
        if (strcmp(list->contactsArray[index].c, phone) == 0)
        {
            return index;
        }
        if (strcmp(list->contactsArray[index].d, phone) == 0)
        {
            return index;
        }
        if (strcmp(list->contactsArray[index].e, phone) == 0)
        {
            return index;
        }
    }
    return -1;
}
// 按关键词模糊查找联系人
int findContactsListByKeyWord(ContactsList* list, char* keyword, int begin)
{
    int index;
    // 检查多个字段是否包含关键词（使用strstr进行子串匹配）
    for (index = begin; index < list->count; ++index)
    {
        if (strstr(list->contactsArray[index].a, keyword))
        {
            return index;
        }
        if (strstr(list->contactsArray[index].b, keyword))
        {
            return index;
        }
        if (strstr(list->contactsArray[index].c, keyword))
        {
            return index;
        }
        if (strstr(list->contactsArray[index].d, keyword))
        {
            return index;
        }
        if (strstr(list->contactsArray[index].e, keyword))
        {
            return index;
        }
        if (strstr(list->contactsArray[index].f, keyword))
        {
            return index;
        }
    }
    return -1;
}

// 检查联系人是否已存在（通过姓名或电话号码）
int hasContactsList(ContactsList* list, Contacts* contacts)
{
    // 通过姓名查找
    if (findContactsListByName(list, contacts->a, 0) != -1)
        return 1;
    // 检查三个电话字段
    if (strcmp(contacts->c, "-") != 0)
    {
        if (findContactsListByPhone(list, contacts->c, 0) != -1)
            return 1;
    }
    else if (strcmp(contacts->d, "-") != 0)
    {
        if (findContactsListByPhone(list, contacts->d, 0) != -1)
            return 1;
    }
    else if (strcmp(contacts->e, "-") != 0)
    {
        if (findContactsListByPhone(list, contacts->e, 0) != -1)
            return 1;
    }
    return 0;
}
// 保存联系人数据到文件   
void saveFile(ContactsList* list)
{
    // 文件名
    FILE* output = fopen("contacts.txt", "w");
    if (output)
    {
        int index;
        for (index = 0; index < list->count; ++index)
        {
            pContacts contacts = &list->contactsArray[index];
            // 格式化输出到文件
            fprintf(output, "%-10s ", contacts->a);
            fprintf(output, "%-16s ", contacts->b);
            fprintf(output, "%-16s ", contacts->c);
            fprintf(output, "%-16s ", contacts->d);
            fprintf(output, "%-16s ", contacts->e);
            fprintf(output, "%-16s ", contacts->f);
            fprintf(output, "\n");
        }
        fclose(output);
    }
    else
    {
        printf("写文件失败！\n");
    }
}
// 从文件加载联系人数据
void LF(ContactsList* L)
{
    FILE* I = fopen("contacts.txt", "r");
    if (I)
    {
        Contacts i = { 0 };
        L->count = 0;
        while (1)
        {
            // 使用fscanf按格式读取
            if (fscanf(I, "%s ", i.a) != 1)
                break;
            if (fscanf(I, "%s ", i.b) != 1)
                break;
            if (fscanf(I, "%s ", i.c) != 1)
                break;
            if (fscanf(I, "%s ", i.d) != 1)
                break;
            if (fscanf(I, "%s ", i.e) != 1)
                break;
            if (fscanf(I, "%s ", i.f) != 1)
                break;
            addContactsList(L, &i);
        }
        fclose(I);
    }
}
// 按姓名排序
void sortContactsListByName(ContactsList* list)
{
    int index, cursor;
    for (index = 0; index < list->count; ++index)
    {
        int target = index;
        // 找出最小元素位置
        for (cursor = target + 1; cursor < list->count; ++cursor)
        {
            if (strcmp(list->contactsArray[target].a, list->contactsArray[cursor].a) > 0)
            {
                target = cursor;
            }
        }
        if (target != index)
        {
            // 交换元素
            Contacts temp = list->contactsArray[index];
            list->contactsArray[index] = list->contactsArray[target];
            list->contactsArray[target] = temp;
        }
    }
}
// 按分组排序
void sortContactsListByCategory(ContactsList* list)
{
    int index, cursor;//外层索引内层索引
    for (index = 0; index < list->count; ++index)
    {
        int target = index;
        for (cursor = target + 1; cursor < list->count; ++cursor)
        {
            //比较字符串长度
            if (strcmp(list->contactsArray[target].b, list->contactsArray[cursor].b) > 0)
            {
                target = cursor;
            }
        }
        if (target != index)
        {
            Contacts temp = list->contactsArray[index];
            list->contactsArray[index] = list->contactsArray[target];
            list->contactsArray[target] = temp;
        }
    }
}
// 按手机号码排序
void sortContactsListByMobile(ContactsList* list)
{
    int index, cursor;
    for (index = 0; index < list->count; ++index)
    {
        int target = index;
        for (cursor = target + 1; cursor < list->count; ++cursor)
        {
            if (strcmp(list->contactsArray[target].e, list->contactsArray[cursor].e) > 0)
            {
                target = cursor;
            }
        }
        if (target != index)
        {
            Contacts temp = list->contactsArray[index];
            list->contactsArray[index] = list->contactsArray[target];
            list->contactsArray[target] = temp;
        }
    }
}
// 显示联系人表格标题
void showContactsTitle()
{
    printf("%-10s", "姓名");
    printf("%-16s", "分组");
    printf("%-16s", "办公电话");
    printf("%-16s", "家庭电话");
    printf("%-16s", "手机号码");
    printf("%-16s", "电子邮箱");
    printf("\n");
}
// 显示单个联系人信息
void showContacts(pContacts contacts)
{
    printf("%-10s", contacts->a);
    printf("%-16s", contacts->b);
    printf("%-16s", contacts->c);
    printf("%-16s", contacts->d);
    printf("%-16s", contacts->e);
    printf("%-16s", contacts->f);
    printf("\n");
}
// 拨号功能实现
void dialing(ContactsList* list)
{
    // 检查联系人列表是否为空
    if (list->count)
    {
        // 用户菜单选择
        int option;
        // 拨号状态信息
        char message[STR_LEN] = "正在呼出";
        // 当前联系人指针
        pContacts contacts = NULL;
        // 用户输入的位置序号
        int position;
        int index;
        printf(" $ 拨号 $ \n");
        // 获取有效联系人序号
        while (1)
        {
            printf("输入序号：");
            scanf("%d", &position);
            if (position >= 1 && position <= list->count) break;
        }
        // 获取联系人对象
        contacts = &list->contactsArray[position - 1];
        // 电话号码选择菜单
        while (1)
        {
            // 存储选择的号码
            char number[STR_LEN] = { 0 };
            printf("╔----------------------------╗\n");
            // 显示联系人姓名
            printf("  $ 拨号 - %s $ \n", contacts->a);
            printf(" 【1】办公电话\n");
            printf(" 【2】家庭电话\n");
            printf(" 【3】手机号码\n");
            printf(" 【0】取消\n");
            printf("╚----------------------------╝\n");
            printf("       请选择：");
            scanf("%d", &option);
            // 取消拨号
            if (option == 0)
                return;
            // 根据选择获取号码
            switch (option)
            {
            case 1:
                strcat(number, contacts->c);
                break;
            case 2:
                strcat(number, contacts->d);
                break;
            case 3:
                strcat(number, contacts->e);
                break;
            default:
                printf("请输入有效选项！\n");
            }
            // 检查号码有效性
            if (strcmp(number, "-") != 0)
            {
                strcat(message, number);
                break;
            }
            else
            {
                printf("号码为空，无法拨号！\n");
            }
        }
        // 模拟拨号动画
        printf("[按任意键结束拨号\n");
        strcat(message, "...");
        while (1)
        {
            // 逐字符打印动画效果
            for (index = 0; index < strlen(message); ++index)
            {
                printf("%c", message[index]);
                Sleep(250); // Windows平台延时函数
            }
            printf("\n");
            // 检测键盘输入
            //需要头文件#include <conio.h>
            if (_kbhit())//检测有没有按键，如果有按键就返回真
            {
                _getch();
                break;
            }
        }
    }
}
// 显示并管理联系人列表
void showContactsList(ContactsList* list)
{
    // 默认按姓名排序
    int option = 1;
    while (1)
    {
        int index;
        // 根据选项排序
        switch (option)
        {
        case 1:
            sortContactsListByName(list);//按姓名排序
            break;
        case 2:
            sortContactsListByCategory(list);// 按分组排序
            break;
        case 3:
            sortContactsListByMobile(list);// 按手机排序
            break;
        case 4:
            dialing(list);// 进入拨号功能
            break;
        }
        // 显示表格标题
        printf(" $ 显示联系人信息 $ \n");
        printf("%-10s", "序号");
        showContactsTitle();
        for (index = 0; index < list->count; ++index)
        {
            pContacts contacts = &list->contactsArray[index];
            printf("%-10d", index + 1);// 显示从1开始的序号
            showContacts(contacts);// 显示联系人详细信息
        }
        // 操作菜单
        printf("【1 按姓名排序 2 按分组排序 3 按手机号码排序 4 拨号 0 返回】\n");
        scanf("%d", &option);
        if (option == 0) break;
    }
}
// 电话号码格式验证
int checkPhone(const char* phone)
{
    int index;
    size_t len = strlen(phone);
    // 必须11位
    if (len != 11)
        return 0;
    // 检查是否全数字
    for (index = 0; index < len; ++index)
    {
        if (!isdigit(phone[index]))
        {
            return 0;
        }
    }
    return 1;
}
// 安全输入电话号码
void inputPhone(char* phone)
{
    do {
        scanf("%s", phone);
        // 允许输入"-"表示空值
        if (strcmp(phone, "-") == 0)
            break;
        if (checkPhone(phone))
            break;
        printf("格式错误，请重新输入！（11位数字）\n");
    } while (1);
}
// 编辑联系人信息
void editContacts(pContacts contacts, GroupList* gl)
{
    printf(" $ 录入信息 $ \n");
    if (strlen(contacts->a))
    {
        // 姓名处理（如果是修改则不重复输入）
        printf("姓名：%s\n", contacts->a);
    }
    else
    {
        printf("姓名：");
        scanf("%s", contacts->a);
    }
    printf("分组：");
    selectGroup(gl, contacts->b);
    printf("电子邮箱：");
    scanf("%s", contacts->f);
    // 电话号码输入（至少需要一个有效号码）
    while (1)
    {
        printf("以下内容如果为空则输入'-'，并至少保证一项不为空\n");
        printf("-----------------\n");
        printf("办公电话：");
        inputPhone(contacts->c);
        printf("家庭电话：");
        inputPhone(contacts->d);
        printf("手机号码：");
        inputPhone(contacts->e);
        // 验证至少有一个号码
        //比较两字符串是否相等
        if (strcmp(contacts->c, "-") != 0)
            break;
        if (strcmp(contacts->d, "-") != 0)
            break;
        if (strcmp(contacts->e, "-") != 0)
            break;
    }
}
// 添加联系人
void addContacts(ContactsList* cl, GroupList* gl)
{
    int succeed = 1;
    // 临时联系人对象
    Contacts contacts = { 0 };
    printf(" $ 添加联系人信息 $ \n");
    // 编辑信息
    editContacts(&contacts, gl);
    // 查重处理
    if (hasContactsList(cl, &contacts))
    {
        char option[STR_LEN] = { 0 };
        printf("存在重复信息，是否继续添加？(Y/N)\n");
        scanf("%s", option);
        // 确认处理
        if (strcmp(option, "Y") == 0 || strcmp(option, "y") == 0)
        {
            succeed = 1;
        }
        else
        {
            succeed = 0;
        }
    }
    if (succeed)
    {
        addContactsList(cl, &contacts);
        saveFile(cl);
        // 显示添加结果
        showContactsTitle();
        showContacts(&contacts);
        printf("成功添加以上联系人信息！\n");
    }
    else
    {
        printf("取消添加！\n");
    }
}
// 修改联系人信息
void updateContacts(ContactsList* list, GroupList* gl)
{
    char name[STR_LEN] = { 0 };
    int position = -1;
    printf(" $ 修改联系人信息 $ \n");
    printf("输入联系人姓名：");
    scanf("%s", name);
    position = findContactsListByName(list, name, 0);
    if (position != -1)
    {
        int index;
        // 显示所有同名联系人
        printf("%-10s", "序号");
        showContactsTitle();
        do
        {
            printf("%-10d", position + 1);
            showContacts(&list->contactsArray[position]);
            position = findContactsListByName(list, name, position + 1);
        } while (position != -1);
        // 选择修改目标
        printf("-------------\n");
        printf("输入要修改的联系人序号：");
        scanf("%d", &index);
        if (index > 0 && index <= list->count)
        {
            Contacts* contacts = &list->contactsArray[index - 1];
            // 二次验证姓名
            if (strcmp(contacts->a, name) == 0)
            {
                showContactsTitle();
                showContacts(contacts);
                editContacts(contacts, gl); // 执行修改
                saveFile(list);
                printf("成功修改以上联系人信息！\n");
            }
            else
            {
                printf("输入错误!\n");
            }
        }
        else
        {
            printf("输入错误!\n");
        }
    }
    else
    {
        printf("没有符合的信息!\n");
    }
}
// 删除联系人功能
void removeContacts(ContactsList* list)
{
    char name[STR_LEN] = { 0 };
    int position = -1;
    printf(" $ 删除联系人信息 $ \n");
    printf("输入联系人姓名：");
    scanf("%s", name);
    // 查找匹配项
    position = findContactsListByName(list, name, 0);
    if (position != -1)
    {
        // 用户输入的删除序号
        int index;
        // 显示所有同名联系人
        printf("%-10s", "序号");
        showContactsTitle();
        do
        {
            printf("%-10d", position + 1);
            // 显示联系人信息（序号从1开始）
            showContacts(&list->contactsArray[position]);
            // 查找下一个匹配项
            position = findContactsListByName(list, name, position + 1);
        } while (position != -1);
        printf("-------------\n");
        printf("输入要删除的联系人序号：");
        scanf("%d", &index);
        // 验证序号有效性
        if (index > 0 && index <= list->count)
        {
            Contacts* contacts = &list->contactsArray[index - 1];
            // 二次验证姓名匹配
            if (strcmp(contacts->a, name) == 0)
            {
                // 显示即将删除的信息
                showContactsTitle();
                showContacts(contacts);
                // 执行删除操作
                removeContactsList(list, index - 1);
                //文件更新
                saveFile(list);
                printf("成功删除以上联系人信息！\n");
            }
            // 姓名与序号不匹配
            else
            {
                printf("输入错误!\n");
            }
        }
        // 序号超出范围
        else
        {
            printf("输入错误!\n");
        }
    }
    // 未找到联系人
    else
    {
        printf("没有符合的信息!\n");
    }
}
// 按姓名精确查询
void searchContactsByID(ContactsList* list)
{
    char name[STR_LEN] = { 0 };
    int position = -1;
    printf(" $ 按姓名查询 $ \n");
    printf("输入联系人姓名：");
    scanf("%s", name);
    position = findContactsListByName(list, name, 0);
    if (position != -1)
    {
        showContactsTitle();
        do {
            showContacts(&list->contactsArray[position]);
            position = findContactsListByName(list, name, position + 1);
        } while (position != -1);
    }
    else
    {
        printf("未找到该联系人!\n");
    }
}
// 按分组查询
void searchContactsByCategory(ContactsList* list, GroupList* gl)
{
    char category[STR_LEN] = { 0 };
    int position = -1;
    printf(" $ 按分组查询 $ \n");
    selectGroup(gl, category);
    // 从位置0开始查找第一个匹配项
    position = findContactsListByCategory(list, category, 0);
    if (position != -1) // 找到至少一个联系人
    {
        showContactsTitle();
        do {
            showContacts(&list->contactsArray[position]);
            // 从下一个位置继续查找同分组联系人
            position = findContactsListByCategory(list, category, position + 1);
        } while (position != -1);
    }
    else
    {
        printf("未找到该联系人!\n");
    }
}
// 按电话号码精确查询
void searchContactsByPhone(ContactsList* list)
{
    char phone[STR_LEN] = { 0 };
    int position = -1;
    printf(" $ 按电话号码查询 $ \n");
    printf("输入电话号码：");
    scanf("%s", phone);
    position = findContactsListByPhone(list, phone, 0);
    if (position != -1)
    {
        showContactsTitle();
        do {
            // 显示所有匹配的联系人
            showContacts(&list->contactsArray[position]);
            position = findContactsListByPhone(list, phone, position + 1);
        } while (position != -1);
    }
    else
    {
        printf("未找到该联系人!\n");
    }
}
// 按关键词模糊查询
void searchContactsByKeyWord(ContactsList* list)
{
    char keyword[STR_LEN] = { 0 };
    int position = -1;
    printf(" $ 按关键词模糊查询 $ \n");
    printf("输入关键词：");
    scanf("%s", keyword);
    position = findContactsListByKeyWord(list, keyword, 0);
    if (position != -1)
    {
        showContactsTitle();
        do {
            showContacts(&list->contactsArray[position]);
            position = findContactsListByKeyWord(list, keyword, position + 1);
        } while (position != -1);
    }
    else
    {
        printf("未找到该联系人!\n");
    }
}
// 查询功能菜单
void searchContacts(ContactsList* list, GroupList* gl)
{
    while (1) {
        int option;
        printf("╔----------------------------╗\n");
        printf("  $ 查询联系人信息 $ \n");
        printf(" 【1】按姓名查询\n");
        printf(" 【2】按分组查询\n");
        printf(" 【3】按电话号码查询\n");
        printf(" 【4】按关键词模糊查询\n");
        printf(" 【0】返回\n");
        printf("╚----------------------------╝\n");
        printf("       请选择：");
        scanf("%d", &option);
        if (option == 0) break;
        switch (option)
        {
        case 1:
            searchContactsByID(list);// 精确姓名查询
            break;
        case 2:
            searchContactsByCategory(list, gl); // 分组查询
            break;
        case 3:
            searchContactsByPhone(list); // 电话号码查询
            break;
        case 4:
            searchContactsByKeyWord(list);// 模糊关键词查询
            break;
        }
    }
}
void process(ContactsList* cl, GroupList* gl)
{
    LF(cl); // 加载联系人数据
    loadGroupFile(gl); // 加载分组数据
    while (1) {
        int option;// 用户主菜单选择
        printf("╔----------------------------╗\n");
        printf("  $ 手机通信录系统 $ \n");
        printf(" 【1】浏览联系人信息\n");
        printf(" 【2】添加联系人信息\n");
        printf(" 【3】修改联系人信息\n");
        printf(" 【4】删除联系人信息\n");
        printf(" 【5】搜索联系人信息\n");
        printf(" 【6】分组管理\n");
        printf(" 【0】退出系统\n");
        printf("╚----------------------------╝\n");
        printf("       请选择：");
        scanf("%d", &option);
        if (option == 0)
            break;
        switch (option) {
        case 1:
            showContactsList(cl);// 显示联系人列表
            break;
        case 2:
            addContacts(cl, gl);// 添加联系人
            break;
        case 3:
            updateContacts(cl, gl);// 修改联系人
            break;
        case 4:
            removeContacts(cl);// 删除联系人
            break;
        case 5:
            searchContacts(cl, gl); // 搜索功能菜单
            break;
        case 6:
            manageGroup(gl); // 分组管理
            break;
        }
    }
}
int main()
{
    // 初始化数据结构
    ContactsList cl = { 0 };
    GroupList gl = { 0 };
    // 分配初始内存
    cl.contactsArray = (Contacts*)calloc(500, sizeof(Contacts));
    gl.groupsArray = (Group*)calloc(500, sizeof(Group));
    //设置控制台标题
    system("title 手机通信录系统");
    printf("╔----------------------------╗\n");
    printf("  $ 欢迎使用手机通信录系统 $ \n");
    printf("╚----------------------------╝\n");
    printf("按任意键继续：");
    scanf("%*[^\n]");
    process(&cl, &gl);
    return 0;
}
