#include "codec.h"

bool proto_encoder(uint16_t type, google::protobuf::Message& body, string& msg)
{
	MsgHeader header;

	string str;
	body.SerializeToString(&str);
	header.len = str.length() + SIZEOF_HEADER;
	header.type = type;
	
	msg.clear();
	msg.append((char*)&header, SIZEOF_HEADER);
	msg.append(str);
	
	return true;
}

/*
msg：接收到的信息
type：信息类型（返回值）

return：反串化后的协议指针（请记得删除）
*/
void* proto_decoder(const char* msg, uint16_t& type)
{
	MsgHeader* header_ptr = (MsgHeader*)msg;
    google::protobuf::Message *proto = NULL;

	type = header_ptr->type;
	switch (header_ptr->type)
	{
    case MSG_LOGIN_REP:
        proto = new LoginReply();
        proto->ParseFromArray(msg + SIZEOF_HEADER, header_ptr->len - SIZEOF_HEADER);
        break;
    case MSG_LOGOUT_REP:
        proto = new LogoutReply();
        proto->ParseFromArray(msg + SIZEOF_HEADER, header_ptr->len - SIZEOF_HEADER);
        break;
    case MSG_SINGLE_ROOM:
        proto = new SingleRoom();
        proto->ParseFromArray(msg + SIZEOF_HEADER, header_ptr->len - SIZEOF_HEADER);
        break;
    case MSG_ROOMS:
        proto = new Rooms();
        proto->ParseFromArray(msg + SIZEOF_HEADER, header_ptr->len - SIZEOF_HEADER);
        break;
    case MSG_READY:
        proto = new ReadyToStart();
        proto->ParseFromArray(msg + SIZEOF_HEADER, header_ptr->len - SIZEOF_HEADER);
        break;
    case MSG_SINGLE_PLAYER:
        proto = new SinglePlayerInfo();
        proto->ParseFromArray(msg + SIZEOF_HEADER, header_ptr->len - SIZEOF_HEADER);
        break;
    case MSG_GAME:
        proto = new GameInfo();
        proto->ParseFromArray(msg + SIZEOF_HEADER, header_ptr->len - SIZEOF_HEADER);
        break;
    case MSG_GOSSIP:
        proto = new Gossip();
        proto->ParseFromArray(msg + SIZEOF_HEADER, header_ptr->len - SIZEOF_HEADER);
        break;
    case MSG_PICK_REQ:
        proto = new CharactorPickRequest();
        proto->ParseFromArray(msg + SIZEOF_HEADER, header_ptr->len - SIZEOF_HEADER);
        return proto;
        break;
    case MSG_CMD_REQ:
        proto = new CommandRequest();
        proto->ParseFromArray(msg + SIZEOF_HEADER, header_ptr->len - SIZEOF_HEADER);
        break;
    case MSG_ERROR_INPUT:
        proto = new ErrorInput();
        proto->ParseFromArray(msg + SIZEOF_HEADER, header_ptr->len - SIZEOF_HEADER);
        break;
    case MSG_HIT:
		proto = new Action();
		proto->ParseFromArray(msg + SIZEOF_HEADER, header_ptr->len - SIZEOF_HEADER);
		break;
	case MSG_RESPOND:
		proto = new Respond();
		proto->ParseFromArray(msg + SIZEOF_HEADER, header_ptr->len - SIZEOF_HEADER);
		break;
	default:
		type = 0;
		return NULL;
		break;
	}
    return proto;
}
