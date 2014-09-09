#include "protocol/action_respond.pb.h"
#include "protocol/base.pb.h"

using namespace std;
using namespace network;

/*
目前的消息结构：
0~15bit  | 16~31bit | 32~?bit
消息长度 | 协议序号 | 协议串化字符串
*/
struct MsgHeader {
    unsigned short len;
    unsigned short type;
};
const int SIZEOF_HEADER = sizeof(MsgHeader);

bool proto_encoder(unsigned short type, google::protobuf::Message* body, string& msg);

/*
msg：接收到的信息
type：信息类型（返回值）

return：反串化后的协议指针（请记得删除）
*/
void* proto_decoder(const char* msg, unsigned short& type);
