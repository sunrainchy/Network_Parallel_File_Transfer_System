#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <ossc/client.h>
#include "global_fun.h"
#include "oss_show.h"
using namespace std;
user_info::user_info(string accessId, string accessKey, string endPoint):mAccessId(accessId), mAccessKey(accessKey), mEndPoint(endPoint)
{
    // Nothing was here
}



void DrawOssPrompt(current_dir &currentDir)
{
    printf("OSS:~/");
    if(!currentDir.isBucketNameEmpty())
        printf("%s\/", currentDir.getBucketName().c_str());
    if(!currentDir.isDirNameEmpty())
        printf("%s",currentDir.getDirName().c_str());
    printf("$ ");
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

static unsigned short LsBucketForOssLs(oss_client_t *client)
{
    unsigned short retCode;
    int cols, lines;
    std::vector<std::string> bucketNameList;
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
    return retCode;
}

static unsigned short LsObjectForOssLs(oss_client_t *client, current_dir &currentDir)
{ 
    std::vector<std::string> dirs;
    std::vector<std::string> objects;
    int isTruncated = 1;
    unsigned short retcode;                 //保存服务器http返回码的解析结果
    const char *retinfo;                    //保存通过retcode获得的错误信息
    int i;
    // 构造 request 参数 
    oss_list_objects_request_t *request = list_objects_request_initialize();
    request->set_bucket_name(request, currentDir.getBucketName().c_str());
    if(currentDir.getDirName().length() > 0)
        request->set_prefix(request, currentDir.getDirName().c_str());          //设置prefix
    request->set_delimiter(request, "/");      //设置delimiter
    request->set_max_keys(request, 100);      //设置max_keys
    //request->set_marker(request, "t");         //设置marker

    while(isTruncated){
        oss_object_listing_t *object_listing = client_list_objects(client, request, &retcode);
        //printf("list_objects_with_request result:\n\n");
        if(retcode == 0) {
            isTruncated = object_listing->is_truncated;
            if(strlen(object_listing->next_marker) > 0)
               request->set_marker(request, object_listing->next_marker);
            //   printf("list_objects_with_request successfully.\n");
            //   printf("bucket_name = %s\nnext_marker = %s\nprefix = %s\nmarker = %s\nmax_keys = %d\ndelimiter = %s\nis_truncated = %d\n", 
            //           object_listing->bucket_name, object_listing->next_marker, object_listing->prefix, object_listing->marker,
            //           object_listing->max_keys, object_listing->delimiter, object_listing->is_truncated);
            for(i = 0; i < object_listing->_counts_summaries; i++) {
                //       printf("****************************************************************\n");
                //       printf("object [%d] : \n", i);
                //       printf("type = %s\netag = %s\nkey = %s\nlast_modified = %s\nsize = %ld\nstorage_class = %s\nid = %s\ndisplay_name = %s\n",
                //               object_listing->summaries[i]->type, object_listing->summaries[i]->etag,
                //               object_listing->summaries[i]->key, object_listing->summaries[i]->last_modified,
                //               object_listing->summaries[i]->size, object_listing->summaries[i]->storage_class,
                //               object_listing->summaries[i]->owner->id, object_listing->summaries[i]->owner->display_name);
                objects.push_back(object_listing->summaries[i]->key);
            }
            //  printf("=============================================================\n");
            for(i = 0; i < object_listing->_counts_common_prefixes; i++) {
                //      printf("common_prefix = %s\n", (object_listing->common_prefixes)[i]);
                dirs.push_back((object_listing->common_prefixes)[i]);
            }
        } else {
            retinfo = oss_get_error_message_from_retcode(retcode);
            printf("error = %s\n", retinfo);
            break;
        }
        //释放空间 
        if(object_listing != NULL) {
            if(object_listing->summaries != NULL) {
                for(i = 0; i < object_listing->_counts_summaries; i++) {
                    owner_finalize((object_listing->summaries)[i]->owner);
                    object_summary_finalize((object_listing->summaries)[i]);
                }
                free(object_listing->summaries);
            }
            object_listing_finalize(object_listing);
        }
    }
    ossShowObject(dirs, L_BLUE);
    ossShowObject(objects, NONE);
    return retcode;
}

unsigned short OssLs(oss_client_t *client, current_dir &currentDir)
{
    unsigned short retCode;
    int cols, lines;
    std::vector<std::string> bucketNameList;
    if(currentDir.isBucketNameEmpty()){
        //ListBucket
        retCode = LsBucketForOssLs(client);
    }else{
        //List Object
        LsObjectForOssLs(client, currentDir);
    }
    return retCode;
}


unsigned short OssCd(oss_client_t *client, current_dir &currentDir, string dirName)
{
    //TODO check if the format of dirName is right or not
    unsigned short retCode = 0;
    if(dirName.empty()){
        currentDir.clearAll();
        return retCode;
    }
    int pos_1 = dirName.find_first_of(""), pos_2;
    string bucketName, objectName;
    if(0 == pos_1){ //Absolute path
        pos_2 = dirName.find_first_of("/", 1);
        if(pos_2 == -1){
            bucketName = dirName.substr(1, dirName.length() - 1);
            if(client_is_bucket_exist(client, bucketName.c_str())){
                currentDir.clearAll();
                currentDir.setBucketName(bucketName);
            }
        }
        else{
            objectName = dirName.substr(1, pos_2);
            objectName = dirName.substr(pos_2 + 1, dirName.length() - 1);
            if(isObjectExist(client, bucketName, objectName + "/")){
                currentDir.clearAll();
                currentDir.setBucketName(bucketName);
                currentDir.setDirName(objectName + "/");
            }
        }
    }else{
        if(isObjectExist(client, currentDir.getBucketName(), dirName + "/")){
            currentDir.setDirName(dirName + "/");
        }
    }
    return 0;
}
bool isObjectExist(oss_client_t *client, string bucketName, string objectName)
{
    if(0 == objectName.length()){
        return client_is_bucket_exist(client, bucketName.c_str());
    }
    bool ans = false;
    unsigned short retcode;
    oss_list_objects_request_t *request = list_objects_request_initialize();
    request->set_bucket_name(request, bucketName.c_str());
    request->set_prefix(request, objectName.c_str());
    //request->set_delimiter(request, "/");      //设置delimiter
    request->set_max_keys(request, 1);      //设置max_keys
    //request->set_marker(request, "t");         //设置marker
    oss_object_listing_t *object_listing = client_list_objects(client, request, &retcode);
    if(retcode == 0) {
        ans = object_listing->_counts_summaries > 0 || object_listing->_counts_common_prefixes > 0;
    } else {
        const char *retinfo = oss_get_error_message_from_retcode(retcode);
        printf("error = %s\n", retinfo);
    }
    //释放空间 
    if(object_listing != NULL) {
        if(object_listing->summaries != NULL) {
            for(int i = 0; i < object_listing->_counts_summaries; i++) {
                object_summary_finalize((object_listing->summaries)[i]);
            }
            free(object_listing->summaries);
        }
        object_listing_finalize(object_listing);
    }
    return ans;
}
