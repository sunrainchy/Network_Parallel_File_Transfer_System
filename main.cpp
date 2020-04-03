#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "global_fun.h"
#include "oss_show.h"
using namespace std;
static current_dir curDir;
static user_info userInfo("wHt3iB4nqia2Bh7y", "your-access-key", "oss-cn-hangzhou.aliyuncs.com");//oss-cn-hangzhou-internal.aliyuncs.com

static oss_client_t *client = client_initialize_with_endpoint(userInfo.getAccessId().c_str(),\
        userInfo.getAccessKey().c_str(), userInfo.getEndPoint().c_str());

int main()
{
    const char *retInfo = NULL;
    string order;
    unsigned short retCode;
    //curDir.setBucketName("bucket-haha");
    //curDir.setDirName("bucket-haha/haha/");
    DrawOssPrompt(curDir);
    while(getline(cin, order)){
		retCode = OK;
        if("ls" == order){
            retCode = OssLs(client, curDir);
        }else if("cd" == order){
            string dirName;
            printf("Please input dirName: ");
            cin>>dirName;
            OssCd(client, curDir, dirName);
        }else if("mkdir" == order){
            string dirName;
            printf("Please input dirName:");
            cin>>dirName;
            retCode = OssMkdir(client, dirName, curDir);
        }else if("exit" == order){
            return 0;
        }else if("touch" == order){
			string objectName;
			printf("Please input objectName:");
			cin>>objectName;
			retCode = OssTouch(client, curDir, objectName);
		}else if("put" == order){
			string objectName,localFileName;
			printf("Please input objectName:");
			cin>>objectName;
			printf("Please input localFileName:");
			cin>>localFileName;
			retCode = OssPut(client, curDir, objectName, localFileName);
		}
        if (retCode == OK) {
            printf("+Done.\n");
        } else {
            retInfo = oss_get_error_message_from_retcode(retCode);
            printf("%s\n", retInfo);
        }
        DrawOssPrompt(curDir);
    }
    return 0;
}
