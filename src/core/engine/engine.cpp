#include "core/engine/engine.h"
#include "networking/packet_handler.h"
#include "raylib.h"

namespace engine {

Engine::Engine(const EngineConfig& config) 
  : _config(config), 
    _initialized(false) 
{
  if (_config.mode != SERVER) {
    InitWindow(_config.width, _config.height, _config.title);
    SetTargetFPS(60);

    if (_config.mode == CLIENT) {
      _network_manager = std::make_unique<NetworkManager>(); 
      _network_manager->InitClient();
    }
  }
  else {
    _network_manager = std::make_unique<NetworkManager>();
    // TODO: make this choice of the user
    ServerConf conf{8080, 2};
    _network_manager->InitServer(conf);
  }
  _initialized = true;
}

Engine::~Engine() 
{
  _loaded_scene.reset();
  
  if (_initialized) 
    CloseWindow();
}

void Engine::LoadScene(std::unique_ptr<Scene> s)
{
  _loaded_scene = std::move(s);
}

void Engine::Run() 
{
  if (_config.mode != SERVER) 
    RunGame();
  else
    RunServer();
}

void Engine::RunGame() 
{
  PacketHandler packet_handler(_loaded_scene.get(), _local_player_id, &_sprite_factory);
  
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    if (_config.mode == CLIENT && _loaded_scene) {
      Packet packet = Client::GetInstance().ReceiveNonBlocking();
      if (packet._type != PacketType::NONE) {
        packet_handler.HandlePacket(packet);
      }
    }

    if (_loaded_scene) {
      _loaded_scene->UpdateScene(dt); 
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (_loaded_scene) 
      _loaded_scene->DrawScene();

    EndDrawing();
  }
}

void Engine::RunServer()
{
  if (_initialized)
    _network_manager->RunServer(std::move(_loaded_scene));
}

} // namespace engine
