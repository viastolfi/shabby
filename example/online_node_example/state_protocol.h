#ifndef STATE_PROTOCOL_H
#define STATE_PROTOCOL_H

#include <string>
#include <vector>
#include <sstream>
#include <cstdio>

// Isolated serialisation helpers for the game state message.
// Format: "P{id},{x},{y},{dx},{dy};...#E{id},{x},{y};..."
namespace NetProtocol {

struct PlayerState { int id; float x, y, dx, dy; };
struct EnemyState  { int id; float x, y; };

inline std::string EncodeState(
    const std::vector<PlayerState>& players,
    const std::vector<EnemyState>&  enemies)
{
  std::ostringstream oss;
  bool first = true;
  for (auto& p : players) {
    if (!first) oss << ';';
    char buf[128];
    std::snprintf(buf, sizeof(buf), "P%d,%.1f,%.1f,%.0f,%.0f",
        p.id, p.x, p.y, p.dx, p.dy);
    oss << buf;
    first = false;
  }
  oss << '#';
  first = true;
  for (auto& e : enemies) {
    if (!first) oss << ';';
    char buf[64];
    std::snprintf(buf, sizeof(buf), "E%d,%.1f,%.1f", e.id, e.x, e.y);
    oss << buf;
    first = false;
  }
  return oss.str();
}

inline bool DecodeState(
    const std::string&          msg,
    std::vector<PlayerState>&   players,
    std::vector<EnemyState>&    enemies)
{
  players.clear();
  enemies.clear();

  auto sep = msg.find('#');
  std::string players_part = (sep != std::string::npos) ? msg.substr(0, sep) : msg;
  std::string enemies_part = (sep != std::string::npos) ? msg.substr(sep + 1) : "";

  if (!players_part.empty()) {
    std::istringstream ps(players_part);
    std::string entry;
    while (std::getline(ps, entry, ';')) {
      PlayerState p{};
      if (std::sscanf(entry.c_str(), "P%d,%f,%f,%f,%f",
              &p.id, &p.x, &p.y, &p.dx, &p.dy) == 5)
        players.push_back(p);
    }
  }

  if (!enemies_part.empty()) {
    std::istringstream es(enemies_part);
    std::string entry;
    while (std::getline(es, entry, ';')) {
      EnemyState e{};
      if (std::sscanf(entry.c_str(), "E%d,%f,%f", &e.id, &e.x, &e.y) == 3)
        enemies.push_back(e);
    }
  }

  return true;
}

} // namespace NetProtocol

#endif // STATE_PROTOCOL_H
