#ifndef INPUT_COMMAND_HANDLER_H
#define INPUT_COMMAND_HANDLER_H

#include "networking/protocol/ipacket_handler.h"

namespace Shabby {

class InputCommandHandler : public IPacketHandler {
public:
  NetworkPacket* HandleFromClient(
      int client_id,
      NetworkPacket& packet,
      Scene* scene) override;

  void HandleFromServer(
      NetworkPacket& packet,
      Scene* scene) override;
};

} // namespace Shabby

#endif // INPUT_COMMAND_HANDLER_H
