#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global_fun.h"
#include "oss_show.h"

static string prompt = "OSS$";
static current_dir curDir;
static user_info userInfo("wHt3iB4nqia2Bh7y", "zzyzBuoBpU5eZKwP9mnzZD1vCpQhAj", "oss-cn-hangzhou-internal.aliyuncs.com");

static oss_client_t *client = client_initialize_with_endpoint(userInfo.getAccessId().c_str(),\
        userInfo.getAccessKey().c_str(), userInfo.getEndPoint().c_str());
//access_id : wHt3iB4nqia2Bh7y
//access_key: zzyzBuoBpU5eZKwP9mnzZD1vCpQhAj
int main()
{
    const char *bu[] = {"nihao", "nihaohao", "bushiba"};
    const char *retInfo = NULL;
    int cols = 0, lines = 0;
    getTerminalSize(cols, lines);
    printf("%d %d\n", cols, lines);
    unsigned short retCode;
    printf("This is chy\n");
    ossShowObject(bu, cols, lines, L_BLUE);
    printf("This is chy\n");
    /*retCode = OssLs(client, curDir);
    if (retCode == OK) {
        printf("put object from file successfully.\n");
    } else {
        retInfo = oss_get_error_message_from_retcode(retCode);
        printf("%s\n", retInfo);
    }*/
    return 0;
}
