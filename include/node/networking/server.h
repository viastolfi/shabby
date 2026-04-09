#ifndef SERVER_H
#define SERVER_H

#include "node/inode.h"

namespace Shabby::Node {

class Server : public INode {
public:
  Server(); 
  ~Server() = default;

  void Update(float dt) override;
private:
  int _server_socket;

  std::vector<int> _connected_client;
};

} // namespace Shabby::Node

#endif // SERVER_H
