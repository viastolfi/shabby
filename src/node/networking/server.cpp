#include "node/networking/server.h"

namespace Shabby::Node {

Server::~Server()
{
  Stop();
}

void Server::Start(int port)
{
  if (_running.load())
    return;

  _port = port;

  _server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (_server_fd < 0)
    return;

  int opt = 1;
  setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(static_cast<uint16_t>(port));

  if (bind(_server_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
    close(_server_fd);
    _server_fd = -1;
    return;
  }

  if (listen(_server_fd, 16) < 0) {
    close(_server_fd);
    _server_fd = -1;
    return;
  }

  _running.store(true);
  _accept_thread = std::thread(&Server::AcceptLoop, this);
}

void Server::Stop()
{
  if (!_running.load())
    return;

  _running.store(false);

  if (_server_fd >= 0) {
    shutdown(_server_fd, SHUT_RDWR);
    close(_server_fd);
    _server_fd = -1;
  }

  {
    std::lock_guard<std::mutex> lock(_clients_mutex);
    for (auto& [id, fd] : _client_fds) {
      shutdown(fd, SHUT_RDWR);
      close(fd);
    }
    _client_fds.clear();
  }

  if (_accept_thread.joinable())
    _accept_thread.join();

  for (auto& t : _client_threads) {
    if (t.joinable())
      t.join();
  }
  _client_threads.clear();
}

void Server::Broadcast(const std::string& topic, const std::string& msg)
{
  std::lock_guard<std::mutex> lock(_clients_mutex);
  for (auto& [id, fd] : _client_fds)
    NetProtocol::SendMessage(fd, topic, msg);
}

void Server::SendToClient(int client_id, const std::string& topic, const std::string& msg)
{
  std::lock_guard<std::mutex> lock(_clients_mutex);
  auto it = _client_fds.find(client_id);
  if (it != _client_fds.end())
    NetProtocol::SendMessage(it->second, topic, msg);
}

void Server::Poll()
{
  std::queue<NetworkMessage> local;
  {
    std::lock_guard<std::mutex> lock(_queue_mutex);
    std::swap(local, _incoming);
  }

  while (!local.empty()) {
    auto& m = local.front();
    if (m.topic == "__connected") {
      if (OnClientConnected) OnClientConnected(m.client_id);
    } else if (m.topic == "__disconnected") {
      if (OnClientDisconnected) OnClientDisconnected(m.client_id);
    } else {
      if (OnMessage) OnMessage(m.client_id, m.topic, m.message);
    }
    local.pop();
  }
}

bool Server::IsRunning() const
{
  return _running.load();
}

void Server::AcceptLoop()
{
  while (_running.load()) {
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(_server_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);

    if (client_fd < 0)
      continue;

    int nodelay = 1;
    setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay));

    int client_id;
    {
      std::lock_guard<std::mutex> lock(_clients_mutex);
      client_id = _next_client_id++;
      _client_fds[client_id] = client_fd;
    }

    {
      std::lock_guard<std::mutex> lock(_queue_mutex);
      _incoming.push({client_id, "__connected", ""});
    }

    _client_threads.emplace_back(&Server::ClientRecvLoop, this, client_id, client_fd);
  }
}

void Server::ClientRecvLoop(int client_id, int fd)
{
  while (_running.load()) {
    std::string topic, message;
    if (!NetProtocol::RecvMessage(fd, topic, message))
      break;

    std::lock_guard<std::mutex> lock(_queue_mutex);
    _incoming.push({client_id, topic, message});
  }

  bool was_connected = false;
  {
    std::lock_guard<std::mutex> lock(_clients_mutex);
    auto it = _client_fds.find(client_id);
    if (it != _client_fds.end()) {
      close(it->second);
      _client_fds.erase(it);
      was_connected = true;
    }
  }

  if (was_connected) {
    std::lock_guard<std::mutex> lock(_queue_mutex);
    _incoming.push({client_id, "__disconnected", ""});
  }
}

} // namespace Shabby::Node
