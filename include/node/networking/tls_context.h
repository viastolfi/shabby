#ifndef TLS_CONTEXT_H
#define TLS_CONTEXT_H

#include <openssl/ssl.h>

namespace Shabby::Node {

class TlsContext {
public:
  ~TlsContext();

  TlsContext(TlsContext&&) noexcept;
  TlsContext& operator=(TlsContext&&) noexcept;

  TlsContext(const TlsContext&)            = delete;
  TlsContext& operator=(const TlsContext&) = delete;

  static TlsContext MakeServer();

  static TlsContext MakeClient();

  SSL* CreateSSL(int fd) const;

private:
  TlsContext() = default;
  SSL_CTX* _ctx = nullptr;
};

} // namespace Shabby::Node

#endif // TLS_CONTEXT_H
