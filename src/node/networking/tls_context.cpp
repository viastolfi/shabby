#include "node/networking/tls_context.h"

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/asn1.h>

#include <stdexcept>

namespace Shabby::Node {

static EVP_PKEY* GenerateRsaKey()
{
  EVP_PKEY* pkey = nullptr;
  EVP_PKEY_CTX* pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
  if (!pctx)
    throw std::runtime_error("EVP_PKEY_CTX_new_id failed");

  if (EVP_PKEY_keygen_init(pctx) <= 0 ||
      EVP_PKEY_CTX_set_rsa_keygen_bits(pctx, 2048) <= 0 ||
      EVP_PKEY_keygen(pctx, &pkey) <= 0)
  {
    EVP_PKEY_CTX_free(pctx);
    throw std::runtime_error("RSA key generation failed");
  }

  EVP_PKEY_CTX_free(pctx);
  return pkey;
}

static void ConfigureServerCert(SSL_CTX* ctx)
{
  EVP_PKEY* pkey = GenerateRsaKey();

  X509* cert = X509_new();
  if (!cert) {
    EVP_PKEY_free(pkey);
    throw std::runtime_error("X509_new failed");
  }

  X509_set_version(cert, 2);
  ASN1_INTEGER_set(X509_get_serialNumber(cert), 1);
  X509_gmtime_adj(X509_get_notBefore(cert), 0);
  X509_gmtime_adj(X509_get_notAfter(cert), 365L * 24 * 3600);

  X509_set_pubkey(cert, pkey);

  X509_NAME* name = X509_get_subject_name(cert);
  X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC,
      reinterpret_cast<const unsigned char*>("shabby-server"), -1, -1, 0);
  X509_set_issuer_name(cert, name); 

  if (X509_sign(cert, pkey, EVP_sha256()) == 0) {
    X509_free(cert);
    EVP_PKEY_free(pkey);
    throw std::runtime_error("X509_sign failed");
  }

  SSL_CTX_use_certificate(ctx, cert);
  SSL_CTX_use_PrivateKey(ctx, pkey);

  X509_free(cert);
  EVP_PKEY_free(pkey);
}

TlsContext::~TlsContext()
{
  if (_ctx) {
    SSL_CTX_free(_ctx);
    _ctx = nullptr;
  }
}

TlsContext::TlsContext(TlsContext&& other) noexcept
  : _ctx(other._ctx)
{
  other._ctx = nullptr;
}

TlsContext& TlsContext::operator=(TlsContext&& other) noexcept
{
  if (this != &other) {
    if (_ctx) SSL_CTX_free(_ctx);
    _ctx = other._ctx;
    other._ctx = nullptr;
  }
  return *this;
}

TlsContext TlsContext::MakeServer()
{
  TlsContext tls;
  tls._ctx = SSL_CTX_new(TLS_server_method());
  if (!tls._ctx)
    throw std::runtime_error("SSL_CTX_new (server) failed");

  SSL_CTX_set_min_proto_version(tls._ctx, TLS1_2_VERSION);
  ConfigureServerCert(tls._ctx);
  return tls;
}

TlsContext TlsContext::MakeClient()
{
  TlsContext tls;
  tls._ctx = SSL_CTX_new(TLS_client_method());
  if (!tls._ctx)
    throw std::runtime_error("SSL_CTX_new (client) failed");

  SSL_CTX_set_min_proto_version(tls._ctx, TLS1_2_VERSION);
  SSL_CTX_set_verify(tls._ctx, SSL_VERIFY_NONE, nullptr);
  return tls;
}

SSL* TlsContext::CreateSSL(int fd) const
{
  SSL* ssl = SSL_new(_ctx);
  if (!ssl)
    throw std::runtime_error("SSL_new failed");
  SSL_set_fd(ssl, fd);
  return ssl;
}

} // namespace Shabby::Node
