#include "node/networking/server.h"

namespace Shabby::Node {

Server::Server()
  :_server_socket(1)
{}

void Server::Update(float dt)
{
  (void)dt;
}

} // namespace Shabby::Node
