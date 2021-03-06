// Copyright (C) 2014 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef ZARESD_MODEL_ZARES_COUNTER_MANAGER_IMPL_H_
#define ZARESD_MODEL_ZARES_COUNTER_MANAGER_IMPL_H_

#include "base/synchronization/lock.h"

#include "zaresd/model/counter_manager.h"

// struct UnreadCountAllType {
//   uint32* GetByClientType(uint32 client_type) {
//     if (client_type == 17) {
//       return &ios;
//     } else if (client_type == 18) {
//       return &android;
//     } else {
//       return &other;
//     }
//   }
// 
//   uint32 ios;
//   uint32 android;
//   uint32 other;
// };

struct UserUnreadCount {
  std::map<uint32, uint32>* GetUnreadCountByClientType(uint32 client_type) {
    if (client_type == 17) {
      return &user_unread_count_ios;
    } else if (client_type == 18) {
      return &user_unread_count_android;
    } else {
      return &user_unread_count_other;
    }
  }

  std::map<uint32, uint32> user_unread_count_ios;
  std::map<uint32, uint32> user_unread_count_android;
  std::map<uint32, uint32> user_unread_count_other;
};

typedef std::map<uint32, UserUnreadCount> UserUnreadCountMap;

struct ReadCountAndLastID {
  uint32 read_count;
  uint32 last_id;
};

typedef std::map<uint32, ReadCountAndLastID> ReadCountAndLastIDMap;

// struct CounterAndLastIDAllType {
//   CounterAndLastID* GetByClientType(uint32 client_type) {
//     if (client_type == 17) {
//       return &ios;
//     } else if (client_type == 18) {
//       return &android;
//     } else {
//       return &other;
//     }
//   }
// 
//   CounterAndLastID ios;
//   CounterAndLastID android;
//   CounterAndLastID other;
// };

struct GroupCounterInfo {
  ReadCountAndLastIDMap* GetReadCountAndLastIDMap(uint32 client_type) {
    if (client_type == 17) {
      return &user_group_count_ios;
    } else if (client_type == 18) {
      return &user_group_count_android;
    } else {
      return &user_group_count_other;
    }
  }

  uint32 group_total_count;

  ReadCountAndLastIDMap user_group_count_ios;
  ReadCountAndLastIDMap user_group_count_android;
  ReadCountAndLastIDMap user_group_count_other;
  // std::map<uint32, CounterAndLastIDAllType> user_group_count;
};

typedef std::map<uint32, GroupCounterInfo> GroupCounterMap;

typedef std::map<uint32, std::map<uint32, uint32> > CounterMap;

class CacheManager {
public:
  CacheManager() {}

  //////////////////////////////////////////////////////////////////////////
  void IncrUserMsgCount(uint32 from_user_id, uint32 to_user_id) {
    base::AutoLock lock(lock_);
    CounterMap::iterator it = counter_instance.find(from_user_id);
    if (it==counter_instance.end()) {
      std::map<uint32, uint32> to_user_ids;
      to_user_ids[to_user_id] = 1;
      counter_instance[from_user_id] = to_user_ids;
    } else {
      std::map<uint32, uint32>::iterator it2 = it->second.find(to_user_id);
      if (it2 == it->second.end()) {
        it->second[to_user_id] = 1;
      } else {
        it->second[to_user_id] += 1;
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////
  void IncreaseUserUnreadMsgCount(uint32 from_user_id, uint32 to_user_id) {
    base::AutoLock lock(lock_);
    UserUnreadCountMap::iterator it = unread_instance.find(from_user_id);
    if (it!=unread_instance.end()) {
      it->second.user_unread_count_ios[to_user_id] += 1;
      it->second.user_unread_count_android[to_user_id] += 1;
      it->second.user_unread_count_other[to_user_id] += 1;
    } else {
      UserUnreadCount user_unread_count;
      user_unread_count.user_unread_count_ios[to_user_id] = 1;
      user_unread_count.user_unread_count_android[to_user_id] = 1;
      user_unread_count.user_unread_count_other[to_user_id] = 1;
      unread_instance[from_user_id] = user_unread_count;
    }

  }

  std::map<uint32, uint32>* FindUnreadCount(uint32 user_id, uint32 client_type) {
    base::AutoLock lock(lock_);
    UserUnreadCountMap::iterator it = unread_instance.find(user_id);
    if (it!=unread_instance.end()) {
      return it->second.GetUnreadCountByClientType(client_type);
    } else {
      return NULL;
    }
  }

  //////////////////////////////////////////////////////////////////////////

  bool IncrGroupMsgCount(uint32 group_id, uint32 last_message_id) {
//     base::AutoLock lock(lock_);
//     CacheManager cacheManager = CacheManager.getInstance();
//     Jedis counterInstance = cacheManager
//       .getResource(CachePoolName.group_counter);
// 
//     counterInstance.hincrBy(getGroupRedisKey(groupId),
//       BizConstants.GROUP_COUNTER_SUBKEY_COUNTER, 1);
//     counterInstance.hset(getGroupRedisKey(groupId),
//       BizConstants.GROUP_COUNTER_SUBKEY_LASTID,
//       String.valueOf(lastMessageId));
// 
//     cacheManager.returnResource(CachePoolName.group_counter,
//       counterInstance);
    return true;
  }

  uint32 GetGroupTotalCount(uint32 group_id) {
    base::AutoLock lock(lock_);
    GroupCounterMap::iterator it = group_counter_instance.find(group_id);
    if (it != group_counter_instance.end()) {
      return it->second.group_total_count;
    } else {
      return 0;
    }
  }

  ReadCountAndLastID* FindCounterAndLastIDs(uint32 user_id, uint32 group_id, uint32 client_type) {
    base::AutoLock lock(lock_);
    GroupCounterMap::iterator it1 = group_counter_instance.find(group_id);
    if (it1 == group_counter_instance.end()) {
      return NULL;
    }

    ReadCountAndLastIDMap* counter_and_last_ids = it1->second.GetReadCountAndLastIDMap(client_type);
    ReadCountAndLastIDMap::iterator it2 = counter_and_last_ids->find(user_id);
    if (it2 == counter_and_last_ids->end()) {
      return NULL;
    }
    return &(it2->second);
  }
  

private:
  base::Lock lock_;
  CounterMap counter_instance;
  UserUnreadCountMap unread_instance;
  GroupCounterMap group_counter_instance;
};

class ZAresCounterManagerImpl : public CounterManager {
public:
  ZAresCounterManagerImpl() {}
  virtual ~ZAresCounterManagerImpl() {}

  // 获得用户所有的未读消息
  virtual const Counter* GetUnreadMsgCount(uint32 user_id, uint32 client_type, Counter* unread_msg_count);

  // 清除用户来自某好友的未读消息计数
  virtual bool ClearUserUnreadItemCount(uint32 user_id, uint32 friend_user_id, uint32 client_type);

  // 获得用户来自他的某好友的未读消息
  virtual size_t GetUserFriendUnreadCount(uint32 user_id, uint32 friend_user_id, uint32 client_type);

  // 返回某用户在某个群里的未读消息计数
  virtual size_t GetUserGroupUnreadCount(uint32 user_id, const std::vector<uint32>& group_ids, uint32 client_type, std::map<uint32, uint32>* unreads);

  virtual bool DeleteUserReadedDialogMessages(uint32 user_id, uint32 friend_user_id, uint32 client_type);

  //////////////////////////////////////////////////////////////////////////
  // 返回某用户在某群里的未读消息数
  virtual const GroupCounterItem* GetUserGroupCount(uint32 user_id, uint32 group_id, uint32 client_type, GroupCounterItem* item);

  // 清除用户对应的某个群组的计数
  virtual bool ClearUserGroupCounter(uint32 user_id, uint32 group_id, uint32 client_type);

  //////////////////////////////////////////////////////////////////////////
  void IncrUserMsgCount(uint32 from_user_id, uint32 to_user_id);
  void IncreaseUserUnreadMsgCount(uint32 from_user_id, uint32 to_user_id);
private:
  CacheManager cache_;
};

#endif
