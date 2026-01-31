#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <functional>
#include <typeindex>

#include "entities/entities.h"
#include "entities/entity_manager.h"
#include "replication/snapshot/snapshot.h"
#include "networking/client.h"
#include "core/factories/sprite_factory.h"
#include "core/assets/assets_registry.h"

namespace engine {

class Scene {
public:
  explicit Scene();
  virtual ~Scene();

  Scene(const Scene&) = delete;
  Scene& operator=(const Scene&) = delete;

  Scene(Scene&&) = default;
  Scene& operator=(Scene&&) = default;

  void SetSpriteFactory(SpriteFactory* factory) { _sprite_factory = factory; }
  
  void SetAssetRegistryPtr(void* registry_ptr, std::type_index type) {
    _assets_registry_ptr = registry_ptr;
    _assets_registry_type = type;
  }
  
  template<typename T>
  AssetRegistry<T>* GetAssetRegistry() {
    if (_assets_registry_type != std::type_index(typeid(T))) {
      return nullptr;
    }
    return static_cast<AssetRegistry<T>*>(_assets_registry_ptr);
  }
  
  SpriteFactory* GetSpriteFactory() const { return _sprite_factory; }

  void AddEntity(std::unique_ptr<Entity> e);
  
  template<typename T, typename... Args>
  void AddEntity(Args&&... args) {
    AddEntity(std::make_unique<T>(std::forward<Args>(args)...));
  }
  
  template<typename T, typename... Args>
  size_t AddEntityWithServerId(Args&&... args) 
  {
    auto entity = std::make_unique<T>(std::forward<Args>(args)...);
    
    Packet request(PacketType::ENTITY_CREATE_REQUEST);
    request.Write(entity->GetPos());
    request.Write(entity->GetSpriteTextureId());
    Client::GetInstance().Send(request);
    
    Packet response = Client::GetInstance().Receive();
    size_t server_id = 0;
    if (response._type == PacketType::ENTITY_CREATE_RESPONSE) {
      response.Read(server_id);
      entity->SetId(server_id);
    }
    
    AddEntity(std::move(entity));
    return server_id;
  }

  void UpdateScene(float dt);
  void DrawScene() const;
  void ApplySnapshot(Snapshot& s);

  Packet GenerateWorldSnapshot() const;

  void ApplyWorldSnapshot(Packet& snapshot, size_t ignore_entity_id = 0);

  void ApplyNewEntitySnapshot(
      Packet& snapshot, 
      size_t ignore_entity_id,
      std::function<std::unique_ptr<Sprite>(int)> sprite_factory);
private:
  std::unique_ptr<EntityManager> _entity_manager;
  void* _assets_registry_ptr = nullptr;
  std::type_index _assets_registry_type{typeid(void)};
  SpriteFactory* _sprite_factory = nullptr;
};

} // namespace engine

#endif // SCENE_H
