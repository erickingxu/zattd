// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#include "zaresd/model/zares/zares_message_manager_impl.h"

#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/stringprintf.h"
#include "db/conn_pool_manager.h"

#include "zaresd/const_zaresd_defines.h"
#include "zaresd/model/relationship_manager.h"

size_t ZAresMessageManagerImpl::GetMessages(uint32 from_user_id, uint32 to_user_id, uint32 offset, int count, std::vector<ServerMsg*>* messages) {

//  List<Message> messageList = new ArrayList<Message>();

  // ȡ�������˵Ĺ�ϵId
  uint32 relate_id = relationship_manager_->GetRelateId(from_user_id, to_user_id);
  if (relate_id == (uint32)-1) {
    return 0;
  }
  db::ScopedPtr_DatabaseConnection db_conn(db_conn_pool_);

  std::string sql = StringPrintf(
    "SELECT id,relateId,type,content,status,created,updated FROM IMMessage WHERE "
    "relateId=%d AND fromUserId = %d AND toUserId = %d AND status = 0 ORDER BY created DESC, id DESC LIMIT %d, %d",
    relate_id,from_user_id,to_user_id,offset,count);

  scoped_ptr<db::QueryAnswer> answ(db_conn->Query(sql));
  if (answ.get() != NULL) {
    while (answ->FetchRow()) {
      ServerMsg* server_msg = new ServerMsg();
      server_msg->ParseFromDatabase(*answ);
    }
  }

//     // ����û���Ϣ
//     messageList = fillUserInfoIntoMessage(messageList);
//     messageList = fillAudioData(messageList);
  return messages->size();
}

size_t ZAresMessageManagerImpl::GetDialogMessages(uint32 from_user_id, uint32 to_user_id, uint32 offset, int count, std::vector<ServerMsg*>* messages) {
#if 0
        if (relateId <= 0 || offset < 0 || count <= 0) {
            return new Message[0];
        }

        List<Message> messageList = new ArrayList<Message>();

        DBManager dbManager = DBManager.getInstance();
        Connection conn = dbManager.getConnection(DBPoolName.macim_slave);
        PreparedStatement statement = null;
        ResultSet rs = null;
        try {
            String sql = "select * from IMMessage where relateId = ? and "
                    + "status = 0 order by created desc, id desc limit ?, ?";
            statement = conn.prepareStatement(sql);
            int index = 1;
            statement.setObject(index++, relateId);
            statement.setObject(index++, offset);
            statement.setObject(index++, count);
            rs = statement.executeQuery();

            int msgFromUserId;
            int msgToUserId;
            Message message;
            while (rs.next()) {
                message = new Message();
                message.setId(rs.getInt("id"));
                message.setRelateId(rs.getInt("relateId"));
                msgFromUserId = rs.getInt("fromUserId");
                message.setFromUserId(msgFromUserId);
                msgToUserId = rs.getInt("toUserId");
                message.setToUserId(msgToUserId);
                message.setType(rs.getInt("type"));
                message.setContent(rs.getString("content")); // ������Ҫ��ͼƬ������ռλ��ת��һ��
                message.setIsDeleted(rs.getInt("status"));
                message.setCreated(rs.getInt("created"));
                message.setUpdated(rs.getInt("updated"));
                messageList.add(message);
            }
            // ����û���Ϣ
            messageList = fillUserInfoIntoMessage(messageList);
            messageList = fillAudioData(messageList);
        } catch (SQLException e) {
            throw e;
        } finally {
            dbManager.release(DBPoolName.macim_slave, conn, statement, rs);
        }

        Message[] message = new Message[messageList.size()];
        messageList.toArray(message);

        return message;

#endif
  return 0;
}

//////////////////////////////////////////////////////////////////////////
size_t ZAresMessageManagerImpl::GetGroupMessages(uint32 group_id, uint32 offset, int count, std::vector<GroupMsg*>* messages) {
  return 0;
}

size_t ZAresMessageManagerImpl::GetGroupMessagesFromId(uint32 user_id, uint32 last_msg_id, int count, std::vector<GroupMsg*>* group_msgs) {
  return 0;
}

//////////////////////////////////////////////////////////////////////////
bool ZAresMessageManagerImpl::SendAudioMessage(uint32 from_user_id, uint32 to_user_id, const std::string& content, uint32 time) {
  LOG(INFO) << "Send audio: " << from_user_id << " -> " << to_user_id << ": length = " << content.length();
  uint32 type = MESSAGE_TYPE_IM_AUDIO; // ������Ϣ

  bool is_sucess = false;
//   AudioModel audioModel = AudioModel.getInstance();
//   Audio audio = audioModel.parseAudio(byteContent);
//  if(audio != null) {
//    audio = audioModel.saveAudio(fromUserId, toUserId, audio, time);
    // logger.info("��������:" + fromUserId + " -> " + toUserId + ", time = " + audio.getCostTime() + ", contentLength=" + audio.getFileSize());
    //if(audio != null) { // �����ļ��ɹ�
      // ziye ��������Ϣ���ͣ�������ʾ����
    //  sendIMMessage(fromUserId, toUserId, type, String.valueOf(audio.getId()), time);
    //  is_sucess = true;
    // }
//  } else {
//    LOG(ERROR) << "file format valid: " << from_user_id << " -> " << to_user_id << ": length = " << content.length();
//  }

  return is_sucess;
}

bool ZAresMessageManagerImpl::SendIMGroupMessage(uint32 userId, uint32 to_group_id, const std::string& content, uint32 time) {
  return true;
}

bool ZAresMessageManagerImpl::SendIMMessage(uint32 from_user_id, uint32 to_user_id, uint32 type, const std::string& content, uint32 time) {
  return true;
}