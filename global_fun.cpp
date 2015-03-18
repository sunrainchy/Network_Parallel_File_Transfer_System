#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <ossc/client.h>

#include "global_fun.h"
#include "oss_show.h"

user_info::user_info(string accessId, string accessKey, string endPoint):mAccessId(accessId), mAccessKey(accessKey), mEndPoint(endPoint)
{
    // Nothing was here
}


unsigned short OssMkdir(oss_client_t *client, string dirName, current_dir &currentDir)
{
    unsigned short retCode;
    if(currentDir.isBucketNameEmpty()){//CreateBucket
        client_create_bucket(client, dirName.c_str(), &retCode);
    }else{
        char buffer[8];
        memset(buffer, 0, sizeof(buffer));
        oss_object_metadata_t *metadata = object_metadata_initialize(); 
        metadata->set_content_length(metadata, 0);
        metadata->set_content_type(metadata, "application/octet-stream");
        metadata->set_cache_control(metadata, "no-cache");
        metadata->set_content_encoding(metadata, "utf-8");
        metadata->set_content_disposition(metadata, "attachment;");
        //metadata->set_expiration_time(metadata, "Thu, 13 Sep 2012 21:08:42 GMT");
        client_put_object_from_buffer(client, currentDir.getBucketName().c_str(), \
                (currentDir.getDirName() + dirName + "/").c_str(), buffer, metadata, &retCode);
    }
    return retCode;
}

unsigned short OssLs(oss_client_t *client, current_dir &currentDir)
{
    unsigned short retCode;
    int cols, lines;
    std::vector<std::string> bucketNameList;
    if(currentDir.isBucketNameEmpty()){
        oss_owner_t *owner = NULL;
        int bucketNumber = 0;
        oss_bucket_t **buckets = client_list_buckets(client, &bucketNumber, &retCode);
        //Add color
        if(retCode == 0){
            if(bucketNumber != 0){
                for(int i=0; i<bucketNumber; i++){
                    //printf("***********************************************************************\n");
                    //printf("bucket [%d] : \n", i);
                    //printf("name = %s\tcreate_date = %s\n", buckets[i]->get_name(buckets[i]), buckets[i]->get_create_date(buckets[i]));
                    //owner = buckets[i]->get_owner(buckets[i]);
                    //printf("id = %s\tdisplay_name = %s\n", owner->get_id(owner), owner->get_display_name(owner));
                    //Here is a bug for ossc
                    bucketNameList.push_back(buckets[i]->get_create_date(buckets[i]));
                }
                ossShowObject(bucketNameList, L_BLUE);
            }else{
                printf("Without any bucket\n");
            }
        }else{
            return retCode;
        }
        if(buckets != NULL) {
            for(int i = 0; i < bucketNumber; i++) {
                if(buckets[i] != NULL) {
                    bucket_finalize(buckets[i]);
                }
            }
            free(buckets);
        }
        if(owner != NULL) {
            owner_finalize(owner);
        }
    }else{
        //TODO List Object
    }
    return retCode;
}
