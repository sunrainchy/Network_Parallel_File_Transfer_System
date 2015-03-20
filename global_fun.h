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
        bool isDirNameEmpty() const
        {
            return (0 == mDirName.length());
        }
        bool isObjectNameEmpty() const
        {
            return (0 == mObjectName.length());
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
        void setBucketName(const string &bucketName)
        {
            mBucketName = bucketName;
        }
        void setObjectName(const string &objectName)
        {
            mObjectName = objectName;
        }
        void setDirName(const string &dirName)
        {
            mDirName = dirName;
        }
        void clearAll(){
            mBucketName.clear();
            mObjectName.clear();
            mDirName.clear();
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

string SplitFileName(const string &str);
string SplitDirName(const string &str);
string SplitFatherDirName(const string &dir);
void DrawOssPrompt(current_dir &currentDir);

unsigned short OssMkdir(oss_client_t *client, string dirName, current_dir &currentDir);
unsigned short OssLs(oss_client_t *client, current_dir &currentDir);
unsigned short OssCd(oss_client_t *client, current_dir &currentDir, string dirName);
bool isObjectExist(oss_client_t *client, string bucketName, string objectName = "");
#endif
