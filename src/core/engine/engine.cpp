#include "core/engine/engine.h"

namespace engine {

Engine::Engine(const EngineConfig& config) 
  : _config(config), 
    _initialized(false) 
{
  if (_config.mode == STANDALONE || _config.mode == CLIENT) {
    RenderConfig render_config;
    render_config.width = _config.width;
    render_config.height = _config.height;
    render_config.title = _config.title;
    render_config.target_fps = 60;
    _render_system = std::make_unique<RenderSystem>(render_config);
  }

  GameLoopConfig loop_config;
  loop_config.target_fps = 60.0f;
  loop_config.fixed_timestep = false;
  _game_loop = std::make_unique<GameLoop>(loop_config);

  if (_config.mode == CLIENT) {
    ClientConfig client_config;
    client_config.server_address = "127.0.0.1";
    client_config.port = 8080;
    _client = std::make_unique<Client>(client_config);
    
    _packet_registry = std::make_unique<PacketRegistry>();
    _packet_registry->RegisterHandler(
      PacketType::ENTITY_SPAWN,
      std::make_unique<EntitySpawnHandler>()
    );
    _packet_registry->RegisterHandler(
      PacketType::ENTITY_DESTROY,
      std::make_unique<EntityDestroyHandler>()
    );
    _packet_registry->RegisterHandler(
      PacketType::SNAPSHOT,
      std::make_unique<SnapshotHandler>()
    );
  }
  
  _initialized = true;
}

Engine::Engine(
    const ServerConf& server_config, 
    std::unique_ptr<ServerLogic> logic)
  : _config(EngineConfig{0, 0, "", EngineMode::SERVER}),
    _initialized(false)
{
  _loaded_scene = std::make_unique<Scene>();
  
  _server = std::make_unique<Server>(
      server_config, 
      std::move(logic),
      _loaded_scene.get(),
      _loaded_scene->GetScheduler());
  _initialized = true;
}
    
Engine::~Engine() 
{
  _loaded_scene.reset();
  _render_system.reset();
  _server.reset();
}

void Engine::LoadScene(std::unique_ptr<Scene> s)
{
  if (s) 
    _loaded_scene = std::move(s);
}

void Engine::Run() 
{
  if (_config.mode == STANDALONE) {
    if (!_game_loop || !_loaded_scene) return;
    _game_loop->Run(
      _loaded_scene.get(),
      _render_system.get(),
      [this]() { return _render_system && !_render_system->ShouldClose(); }
    );
  }
  else if (_config.mode == SERVER) {
    if (_server) {
      _server->Run();
    }
  }
  else if (_config.mode == CLIENT) {
    if (!_game_loop || !_loaded_scene) return;
    _game_loop->Run(
      _loaded_scene.get(),
      _render_system.get(),
      [this]() {
        if (_client && _client->IsConnected()) {
          NetworkPacket packet = _client->ReceiveNonBlocking();
          if (packet.GetType() != PacketType::NONE) {
            _packet_registry->RouteFromServer(packet, _loaded_scene.get());
          }
        }
        return _render_system && !_render_system->ShouldClose();
      }
    );
  }
}

} // namespace engine
