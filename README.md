# Shabby

Shabby is a small 2D game engine written in C++, built on top of [raylib](https://www.raylib.com/). It was developed as a school project and supports both standalone and multiplayer games.

## Architecture

The engine is organized around a node system inspired by Godot. Every object in the game inherits from `INode` and can hold child nodes, forming a tree rooted at a `Scene`. On each frame, the engine walks the tree and calls `Update` on every node.

Concrete node types include sprites, animated sprites, tilemaps, hitboxes, rigid bodies, cameras, timers, and network nodes. This lets you compose complex behaviors by nesting simple nodes rather than relying on deep inheritance.

**Assets** are managed through an `AssetRegistry`. Textures are loaded once at startup and referenced by an integer ID. Spritesheets are described with a column/row count so that frames can be addressed directly.

**Animations** are driven by `AnimatedSprite` (a single looping strip) and `AnimationPlayer` (multiple named clips that can be switched at runtime).

**Networking** is handled by `NetworkNode`, which can act as either a server or a client. Communication relies on topics and string messages dispatched through a signal system. All connections are encrypted with TLS via OpenSSL, so game data is never sent in plain text.

## TLS and network security

Every connection goes through a TLS layer built on top of OpenSSL. TLS is enabled unconditionally by `NetworkNode` for both the server and client sides.

**Certificate generation at runtime.** The server does not rely on any external certificate file. When it starts, it generates a 2048-bit RSA key pair and a self-signed X.509 certificate (SHA-256, CN `shabby-server`, validity one year) entirely in memory using the OpenSSL EVP and X509 APIs. This keeps deployment simple: no certificate management is needed to run the game.

**Protocol version.** Both sides enforce a minimum of TLS 1.2 (`SSL_CTX_set_min_proto_version`), so older and weaker protocol versions are rejected at the handshake.

**Client verification.** The client connects with `SSL_VERIFY_NONE`, meaning it does not authenticate the server certificate. Since the certificate is self-signed and generated on every server launch, there is no trusted anchor to verify against. The goal of the TLS layer is to encrypt the channel, not to provide mutual authentication.

**Transparency.** `NetProtocol::SendMessage` and `RecvMessage` have overloads for both a raw file descriptor and an `SSL*` pointer, so the rest of the codebase — nodes, scenes, game logic — never deals with OpenSSL directly. Switching a connection between plain TCP and TLS only requires toggling `EnableTLS()` on the underlying `Server` or `Client` object before the connection is established.

## Dependencies

- g++ with C++17
- make
- OpenSSL (`libssl-dev` on Debian/Ubuntu)
- raylib 5.5 (downloaded automatically on first build)

## Build

```bash
# Build the static library
make

# Build both example games
make examples
```

Build artifacts land in `build/`, the static library in `lib/libshabby.a`.

To wipe everything including raylib:

```bash
make clean-complete
```

## Run the multiplayer example

Open two terminals from the project root.

**Server:**
```bash
./build/online_node_example server
```

**Client (local):**
```bash
./build/online_node_example client
```

**Client (remote machine):**
```bash
./build/online_node_example client <server-ip>
```

The server listens on port 7777. The client connects to `127.0.0.1` by default if no IP is given.
