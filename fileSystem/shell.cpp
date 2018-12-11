//#include 
#include "filesys.h"
#define CLEN 20
#define CNUM 20
//enum ctype
char commands[CNUM][CLEN] = {
	"exit",
	"dir",
	"mkdir",
	"cd",
	"mkfile",
	"del",
	"write",
	"read",
	"user",
	"passwd",
    "who"
};
int getcid(char *command){
	int i;
	if(command == NULL)
		return -1;
	for(i=0;i<CNUM;i++){
		if(strcmp(command,commands[i]) == 0)
			return i;
	}
	return -1;
}
int shell(int user_id,char *str){
	char seps[] =" \t\n\0";
	char* token,*tstr,*buf;
	short mode;
	short fd;
	int cid,size;
	token = strtok(str,seps);
	if(token == NULL)
		return 1;
	cid = getcid(token);
	switch(cid){
	case 1:
		_dir();
		break;
	case 2:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("mkdir命令的正确格式为mkdir dirname，请检查命令!\n");
			break;
		}
		mkdir(token);
		break;
	case 3:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("cd命令的正确格式为cd dirname，请检查命令!\n");
			break;
		}
		chdir(token);
		break;
	case 4:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("mkfile 命令的正确格式为mkfile filename [mode]，请检查命令!\n");
			break;
		}
		tstr =token;
		mode = DEFAULTMODE;
		token = strtok(NULL,seps);
		if(token != NULL){
			sscanf(token,"%o",&mode);
		}
		mode = mode|DIFILE|0700;
		fd = creat(user_id,tstr,mode);
		if(fd == -1){
			printf("创建文件失败！\n");
			break;
		}
		close(user_id,fd);
		break;
	case 5:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("del 命令的正确格式为del filename，请检查命令!\n");
			break;
		}
		delete(token);
		break;
	case 6:
		mode = WRITE;
		token = strtok(NULL,seps);
		tstr = token;
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("write 命令的正确格式为write filename bytes，请检查命令!\n");
			break;
		}
		if(token[0] == '-'){
			if(token[1] == 'a')
				mode = FAPPEND;
		}else{
			sscanf(token,"%d",&size);
		}
		fd = open(user_id,tstr,char(mode));
		if(fd<0) break;
		buf = (char*)malloc(size);
		size = write(fd,buf,size);
		printf("%d bytes have been writed in file %s.\n",size,tstr);
		free(buf);
		close(user_id,fd);
		break;
	case 7:
		token = strtok(NULL,seps);
		tstr = token;
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("read 命令的正确格式为read filename bytes，请检查命令!\n");
			break;
		}
		sscanf(token,"%d",&size);
		fd = open(user_id,tstr,READ);
		if(fd<0) {
			if(fd==-3){
				mode = mode|DIFILE|0700;
				fd = creat(user_id,tstr,mode);
				mode = WRITE;
				if(fd == -1){
					printf("创建文件失败！\n");
					break;
				}
			}
			else break;
		}
		buf = (char*)malloc(size+1);
		size = read(fd,buf,size);
		printf("%d bytes have been read in buf from file %s.\n",size,tstr);
		free(buf);
		close(user_id,fd);
		break;
	case 8:
		//用户操作
		token = strtok(NULL,seps);
		if (token == NULL){
			//user显示所有用户
			usershow(user_id);
		} else if (!strcmp(token,"add")) {
			//user add 添加用户，[username]不能为0（atoi函数限制）
			token = strtok(NULL,seps);
			if (token == NULL|| atoi(token)==0) {
				printf("user add 命令的正确格式为user add [username] (username不能为0)\n请检查命令!\n");
				break;
			}
			unsigned short username = (unsigned short) atoi(token);
			useradd(user_id,username);
		} else if (!strcmp(token, "del")) {
			//user del 添加用户，[username]不能为0（atoi函数限制）
			token = strtok(NULL,seps);
			if (token == NULL|| atoi(token)==0){
				printf("user del 命令的正确格式为user del [username] (username不能为0)\n请检查命令!\n");
				break;
			}
			unsigned short username = (unsigned short) atoi(token);
			userdel(user_id,username);
		} else if (!strcmp(token,"auth")) {
			tstr = strtok(NULL,seps);
			if (tstr == NULL|| atoi(tstr)==0) {
				printf("user auth 命令的正确格式为user auth [username] [g_id]\n请检查命令!\n");
				break;
			}
			unsigned short username = (unsigned short) atoi(tstr);
			buf = strtok(NULL,seps);
			if (buf == NULL|| atoi(buf)==0){
				printf("user auth 命令的正确格式为user auth [username] [g_id](g_id不能为0)\n请检查命令!\n");
				break;
			}
			unsigned short auth = (unsigned short) atoi(buf);
			userauth(user_id,username,auth);
		}
		else
			printf("user 命令的正确格式为\n1.\tuser\n2.\tuser [add/del] [username]\n3.\tuser auth [username] [g_id]\n请检查命令!\n");
		break;
	case 9:
		token = strtok(NULL, seps);
		if (token == NULL) {
			printf("passwd 命令的正确格式为passwd [your_new_passwd]，请检查命令!\n");
			break;
		}
		passwd(user_id,token);
		break;
    case 10:
        self_show(user_id);
        break;
	case 0:
		return 0;
	default:
		printf("错误:没有命令%s！\n",token);
		break;
	}
	return 1;
}
