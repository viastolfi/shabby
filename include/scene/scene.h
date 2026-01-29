#ifndef SCENE_H
#define SCENE_H

#include <string>

#include "entities/entities.h"
#include "entities/entity_manager.h"
#include "replication/snapshot/snapshot.h"
#include "networking/client.h"

namespace engine {

class Scene {
public:
  explicit Scene();
  virtual ~Scene();

  Scene(const Scene&) = delete;
  Scene& operator=(const Scene&) = delete;

  Scene(Scene&&) = default;
  Scene& operator=(Scene&&) = default;

  void AddEntity(std::unique_ptr<Entity> e);
  
  template<typename T, typename... Args>
  void AddEntity(Args&&... args) {
    AddEntity(std::make_unique<T>(std::forward<Args>(args)...));
  }
  
  template<typename T, typename... Args>
  size_t AddEntityWithServerId(Args&&... args) {
    auto entity = std::make_unique<T>(std::forward<Args>(args)...);
    
    Packet request(PacketType::ENTITY_CREATE_REQUEST);
    request.Write(entity->GetPos());
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
  
private:
  std::unique_ptr<EntityManager> _entity_manager;
};

} // namespace engine

#endif // SCENE_H
