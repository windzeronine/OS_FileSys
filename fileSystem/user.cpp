//
// Created by RudolfLee on 16/12/19.
//

#include "filesys.h"

int usernum() {
    int i;
    for (i = 0; i < PWDNUM; i++) {
        if(pwd[i].p_gid==0)
            return i;
    }
    return -1;
}


void useradd(int user_id,unsigned short username) {
    //组号等于4才能增加用户
    if (user[user_id].u_gid==4) {
        int num = usernum(),i;
        for (i = 0; i < PWDNUM; i++) {
            if(pwd[i].p_uid==username)
                break;
        }
        if (i==PWDNUM) {
            if (num >= 0) {
                pwd[num].p_uid = username;
                pwd[num].p_gid = 2;
                strcpy(pwd[num].password, "1234");
                printf("用户%hu创建成功，初始密码为：1234，初始组号为：2\n", username);
            } else
                printf("用户数量超出上限，不能继续添加\n");
        }
        else
            printf("该用户名已被使用，请重新创建\n");
    }
    else
        printf("权限不足，请联系管理员更改权限\n");
}

void userdel(int user_id,unsigned short username) {
    //组号等于4才能删除用户
    if (user[user_id].u_gid==4) {
        int i;
        if(user[user_id].u_uid==username) {
            printf("你不能删除自己！\n");
            return;
        }
        for (i = 0; i < PWDNUM; i++) {
            if (username == pwd[i].p_uid) {
                //pwd
                for (int j = i; j < PWDNUM; j++) {
                    pwd[j].p_uid = pwd[j + 1].p_uid;
                    pwd[j].p_gid = pwd[j + 1].p_gid;
                    strcpy(pwd[j].password, pwd[j + 1].password);
                }
                printf("删除完毕\n");
                break;
            }
        }
        if (i == PWDNUM)
            printf("未找到该用户\n");
    } else
        printf("权限不足，请联系管理员更改权限\n");
}

void userauth(int user_id,unsigned short username,unsigned short auth){
    if (user[user_id].u_gid==4) {
        int i;
        for (i = 0; i < PWDNUM; i++) {
            if (username == pwd[i].p_uid) {
                pwd[i].p_gid = auth;
                printf("修改组成功\n");
                break;
            }
        }
        if (i == PWDNUM)
            printf("未找到该用户\n");
    } else
        printf("权限不足，请联系管理员更改权限\n");
}


void passwd(int user_id,char* passwd) {
    //修改当前用户密码，每个用户只能改自己的密码
    int i;
    for (i = 0; i < PWDNUM; ++i) {
        if(pwd[i].p_uid==user[user_id].u_uid) {
            printf("请再次输入您的密码：\n");
            char repasswd[PWDSIZ];
            scanf("%s", repasswd);
            while (strcmp(repasswd, passwd)) {
                printf("密码与第一次不同，请再次输入您的密码：\n");
                scanf("%s", repasswd);
            };
            printf("密码修改成功!\n");
            break;
        }
    }
    if (i==PWDNUM) {
        printf("系统错误，请联系运营商\n");
    }
}

void usershow(int user_id) {
    //组号等于4才能显示所有用户
    if (user[user_id].u_gid==4) {
        printf("用户名\t密码\t\t权限\n");
        for(int i = 0;i<usernum();i++){
            printf("%hu\t%s\t%hu\n",pwd[i].p_uid,pwd[i].password,pwd[i].p_gid);
        }
    }
    else
        printf("权限不足，请联系管理员更改权限\n");
}

void self_show(int user_id) {
    printf("用户名\t密码\t\t用户组\n");
    for(int i = 0;i<usernum();i++){
        if (pwd[i].p_uid==user[user_id].u_uid) {
            printf("%hu\t%s\t%hu\n",pwd[i].p_uid,pwd[i].password,pwd[i].p_gid);
            break;
        }
    }
}
