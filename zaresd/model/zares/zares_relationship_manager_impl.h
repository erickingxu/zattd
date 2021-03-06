// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZARESD_MODEL_ZARES_RELATIONSHIP_MANAGER_IMPL_H_
#define ZARESD_MODEL_ZARES_RELATIONSHIP_MANAGER_IMPL_H_

#include "zaresd/model/relationship_manager.h"

namespace db {
class CdbConnPoolManager;
}

class ZAresRelationshipManagerImpl : public RelationshipManager {
public:
  ZAresRelationshipManagerImpl(db::CdbConnPoolManager* db_conn_pool)
    : db_conn_pool_(db_conn_pool) {}
  virtual ~ZAresRelationshipManagerImpl() {}

  // 获取用户的所有最近联系人列表，默认最多1000个
  virtual bool GetRecentContactByUserId(uint32 user_id, int limit, std::vector<FriendInfo*>* friends);

  //  删除用户的某个最近联系人
  virtual bool DeleteRecentContactByUserId(uint32 user_id, uint32 friend_user_id);

  // 获取两个用户之间的关系Id,加为好友的时候会有两条来回的好友关系,
  // 以小的用户Id在前的记录的relateId作为两个人之间的关系Id,查询对话的时候根据关系Id查询
  virtual uint32 GetRelateId(uint32 user_AId, uint32 user_BId);

  bool CheckAndUpdateRelation(uint32 usera_id, uint32 userb_id);
  uint32 AddFriendship(uint32 usera_id, uint32 userb_id);

private:
  db::CdbConnPoolManager* db_conn_pool_;
};

#endif
