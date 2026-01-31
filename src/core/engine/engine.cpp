#include "core/engine/engine.h"

namespace engine {

Engine::Engine(const EngineConfig& config) 
  : _config(config), 
    _initialized(false) 
{
  InitWindow(_config.width, _config.height, _config.title);
  SetTargetFPS(60);

  if (_config.mode == CLIENT) {
    _network_manager = std::make_unique<NetworkManager>(); 
    _network_manager->InitClient();
  }
  _initialized = true;
}

Engine::Engine(
    const ServerConf& server_config, 
    std::unique_ptr<ServerLogic> logic)
  : _config(EngineConfig{0, 0, "", EngineMode::SERVER}),
    _initialized(false)
{
  _network_manager = std::make_unique<NetworkManager>();
  _network_manager->InitServer(
      server_config, 
      std::move(logic),
      std::make_unique<Scene>());
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
  if (s) {
    s->SetSpriteFactory(&_sprite_factory);
    s->SetAssetRegistryPtr(_assets_registry.get(), _assets_registry_type);
  }
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
    _network_manager->RunServer();
}

} // namespace engine
