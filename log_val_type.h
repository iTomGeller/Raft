#ifndef _LOG_VALUE_TYPE_H_
#define _LOG_VALUE_TYPE_H_

namespace raft
{
enum log_val_type {
  app_log = 0x1,
  conf,
  cluster_server,
  log_pack,
  snp_sync_req
};

}
#endif // _LOG_VALUE_TYPE_H_