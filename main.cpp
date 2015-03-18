#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "global_fun.h"
#include "oss_show.h"
using namespace std;
static string prompt = "<->$";
static current_dir curDir;
static user_info userInfo("wHt3iB4nqia2Bh7y", "zzyzBuoBpU5eZKwP9mnzZD1vCpQhAj", "oss-cn-hangzhou-internal.aliyuncs.com");

static oss_client_t *client = client_initialize_with_endpoint(userInfo.getAccessId().c_str(),\
        userInfo.getAccessKey().c_str(), userInfo.getEndPoint().c_str());
//access_id : wHt3iB4nqia2Bh7y
//access_key: zzyzBuoBpU5eZKwP9mnzZD1vCpQhAj
int main()
{
    const char *retInfo = NULL;
    string order;
    unsigned short retCode;
    curDir.setBucketName("bucket-haha");
    //curDir.setDirName("bucket-haha/haha/");
    while(getline(cin, order)){
        cout<<prompt;
        if("ls" == order){
            retCode = OssLs(client, curDir);
        }else if("cd" == order){

        }else if("mkdir" == order){
            string dirName;
            printf("Please input dirName:");
            cin>>dirName;
            retCode = OssMkdir(client, dirName, curDir);
        }

        if (retCode == OK) {
            printf("put object from file successfully.\n");
        } else {
            retInfo = oss_get_error_message_from_retcode(retCode);
            printf("%s\n", retInfo);
        }
    }
    return 0;
}
