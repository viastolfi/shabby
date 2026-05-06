#ifndef NET_PROTOCOL_H
#define NET_PROTOCOL_H

#include <string>
#include <cstdint>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>

namespace Shabby::Node::NetProtocol {

inline bool SendRaw(int fd, const void* data, size_t len)
{
  const char* ptr = static_cast<const char*>(data);
  size_t sent = 0;
  while (sent < len) {
    ssize_t n = ::write(fd, ptr + sent, len - sent);
    if (n <= 0) return false;
    sent += static_cast<size_t>(n);
  }
  return true;
}

inline bool RecvRaw(int fd, void* data, size_t len)
{
  char* ptr = static_cast<char*>(data);
  size_t received = 0;
  while (received < len) {
    ssize_t n = ::read(fd, ptr + received, len - received);
    if (n <= 0) return false;
    received += static_cast<size_t>(n);
  }
  return true;
}

inline bool SendMessage(int fd, const std::string& topic, const std::string& message)
{
  std::string payload;
  payload.reserve(topic.size() + 1 + message.size());
  payload += topic;
  payload += '\n';
  payload += message;

  uint32_t len = htonl(static_cast<uint32_t>(payload.size()));
  if (!SendRaw(fd, &len, 4)) return false;
  return SendRaw(fd, payload.data(), payload.size());
}

inline bool RecvMessage(int fd, std::string& topic, std::string& message)
{
  uint32_t net_len;
  if (!RecvRaw(fd, &net_len, 4)) return false;

  uint32_t len = ntohl(net_len);
  if (len > 1024 * 1024) return false;

  std::string payload(len, '\0');
  if (!RecvRaw(fd, payload.data(), len)) return false;

  auto sep = payload.find('\n');
  if (sep == std::string::npos) return false;

  topic = payload.substr(0, sep);
  message = payload.substr(sep + 1);
  return true;
}

inline bool SendRaw(SSL* ssl, const void* data, size_t len)
{
  const char* ptr = static_cast<const char*>(data);
  size_t sent = 0;
  while (sent < len) {
    int n = SSL_write(ssl, ptr + sent, static_cast<int>(len - sent));
    if (n <= 0) return false;
    sent += static_cast<size_t>(n);
  }
  return true;
}

inline bool RecvRaw(SSL* ssl, void* data, size_t len)
{
  char* ptr = static_cast<char*>(data);
  size_t received = 0;
  while (received < len) {
    int n = SSL_read(ssl, ptr + received, static_cast<int>(len - received));
    if (n <= 0) return false;
    received += static_cast<size_t>(n);
  }
  return true;
}

inline bool SendMessage(SSL* ssl, const std::string& topic, const std::string& message)
{
  std::string payload;
  payload.reserve(topic.size() + 1 + message.size());
  payload += topic;
  payload += '\n';
  payload += message;

  uint32_t len = htonl(static_cast<uint32_t>(payload.size()));
  if (!SendRaw(ssl, &len, 4)) return false;
  return SendRaw(ssl, payload.data(), payload.size());
}

inline bool RecvMessage(SSL* ssl, std::string& topic, std::string& message)
{
  uint32_t net_len;
  if (!RecvRaw(ssl, &net_len, 4)) return false;

  uint32_t len = ntohl(net_len);
  if (len > 1024 * 1024) return false;

  std::string payload(len, '\0');
  if (!RecvRaw(ssl, payload.data(), len)) return false;

  auto sep = payload.find('\n');
  if (sep == std::string::npos) return false;

  topic   = payload.substr(0, sep);
  message = payload.substr(sep + 1);
  return true;
}

} // namespace Shabby::Node::NetProtocol

#endif // NET_PROTOCOL_H
