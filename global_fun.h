#ifndef GLOBAL_FUN_H
#define GLOBAL_FUN_H
#include <string>
#include <ossc/client.h>
using namespace std;
class current_dir{
    public:
        current_dir(){}
        bool isBucketNameEmpty() const
        {
            return (0 == mBucketName.length());
        }
        const string &getBucketName() const 
        {
            return mBucketName;
        }
        const string &getObjectName() const
        {
            return mObjectName;
        }
        const string &getDirName() const
        {
            return mDirName;
        }
        void setBucketName(string bucketName)
        {
            mBucketName = bucketName;
        }
        void setObjectName(string objectName)
        {
            mObjectName = objectName;
        }
        void setDirName(string dirName)
        {
            mDirName = dirName;
        }
    private:
        string mBucketName;
        string mObjectName; //   dir/dir/objectname
        string mDirName;
};

class user_info{
    public:
        user_info(string accessId = "", string accessKey = "", string endPoint = "");
        const string & getAccessId() const
        {
            return mAccessId;
        }
        const string & getAccessKey() const
        {
            return mAccessKey;
        }
        const string & getEndPoint() const
        {
            return mEndPoint;
        }
    private:
        string mAccessId;
        string mAccessKey;
        string mEndPoint;
};

unsigned short OssMkdir(oss_client_t *client, string dirName, current_dir &currentDir);
unsigned short OssLs(oss_client_t *client, current_dir &currentDir);
#endif
