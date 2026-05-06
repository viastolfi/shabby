#include "node/networking/client.h"

namespace Shabby::Node {

Client::~Client()
{
  Disconnect();
}

void Client::EnableTLS()
{
  _tls_ctx = TlsContext::MakeClient();
}

void Client::Connect(const std::string& ip, int port)
{
  if (_connected.load())
    return;

  _fd = socket(AF_INET, SOCK_STREAM, 0);
  if (_fd < 0)
    return;

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(static_cast<uint16_t>(port));
  inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

  if (connect(_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
    close(_fd);
    _fd = -1;
    return;
  }

  int nodelay = 1;
  setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay));

  if (_tls_ctx) {
    _ssl = _tls_ctx->CreateSSL(_fd);
    if (SSL_connect(_ssl) <= 0) {
      SSL_free(_ssl);
      _ssl = nullptr;
      close(_fd);
      _fd = -1;
      return;
    }
  }

  _connected.store(true);

  {
    std::lock_guard<std::mutex> lock(_queue_mutex);
    _incoming.push({"__connected", ""});
  }

  _recv_thread = std::thread(&Client::RecvLoop, this);
}

void Client::Disconnect()
{
  if (!_connected.load())
    return;

  _connected.store(false);

  if (_ssl) {
    SSL_shutdown(_ssl);
    SSL_free(_ssl);
    _ssl = nullptr;
  }

  if (_fd >= 0) {
    shutdown(_fd, SHUT_RDWR);
    close(_fd);
    _fd = -1;
  }

  if (_recv_thread.joinable())
    _recv_thread.join();
}

void Client::Send(const std::string& topic, const std::string& msg)
{
  if (!_connected.load())
    return;

  std::lock_guard<std::mutex> lock(_send_mutex);
  if (_ssl)
    NetProtocol::SendMessage(_ssl, topic, msg);
  else
    NetProtocol::SendMessage(_fd, topic, msg);
}

void Client::Poll()
{
  std::queue<InternalMessage> local;
  {
    std::lock_guard<std::mutex> lock(_queue_mutex);
    std::swap(local, _incoming);
  }

  while (!local.empty()) {
    auto& m = local.front();
    if (m.topic == "__connected") {
      if (OnConnected) OnConnected();
    } else if (m.topic == "__disconnected") {
      if (OnDisconnected) OnDisconnected();
    } else {
      if (OnMessage) OnMessage(m.topic, m.message);
    }
    local.pop();
  }
}

bool Client::IsConnected() const
{
  return _connected.load();
}

void Client::RecvLoop()
{
  while (_connected.load()) {
    std::string topic, message;
    bool ok = _ssl
      ? NetProtocol::RecvMessage(_ssl, topic, message)
      : NetProtocol::RecvMessage(_fd,  topic, message);
    if (!ok) break;

    std::lock_guard<std::mutex> lock(_queue_mutex);
    _incoming.push({topic, message});
  }

  if (_connected.load()) {
    _connected.store(false);
    std::lock_guard<std::mutex> lock(_queue_mutex);
    _incoming.push({"__disconnected", ""});
  }
}

} // namespace Shabby::Node
