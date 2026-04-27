#ifndef STATE_PROTOCOL_H
#define STATE_PROTOCOL_H

#include <string>
#include <vector>
#include <sstream>
#include <cstdio>

// Format: "P{id},{x},{y},{dx},{dy},{hp},{score};...#R{id},{x},{y};...#F{id},{x},{y};..."
namespace NetProtocol {

struct PlayerState     { int id; float x, y, dx, dy; int hp, score; };
struct ProjectileState { int id; float x, y; };
struct FoodState       { int id; float x, y; };

inline std::string EncodeState(
    const std::vector<PlayerState>&     players,
    const std::vector<ProjectileState>& projectiles,
    const std::vector<FoodState>&       foods)
{
  std::ostringstream oss;
  bool first = true;
  for (auto& p : players) {
    if (!first) oss << ';';
    char buf[128];
    std::snprintf(buf, sizeof(buf), "P%d,%.1f,%.1f,%.0f,%.0f,%d,%d",
        p.id, p.x, p.y, p.dx, p.dy, p.hp, p.score);
    oss << buf;
    first = false;
  }
  oss << '#';
  first = true;
  for (auto& r : projectiles) {
    if (!first) oss << ';';
    char buf[64];
    std::snprintf(buf, sizeof(buf), "R%d,%.1f,%.1f", r.id, r.x, r.y);
    oss << buf;
    first = false;
  }
  oss << '#';
  first = true;
  for (auto& f : foods) {
    if (!first) oss << ';';
    char buf[64];
    std::snprintf(buf, sizeof(buf), "F%d,%.1f,%.1f", f.id, f.x, f.y);
    oss << buf;
    first = false;
  }
  return oss.str();
}

inline bool DecodeState(
    const std::string&            msg,
    std::vector<PlayerState>&     players,
    std::vector<ProjectileState>& projectiles,
    std::vector<FoodState>&       foods)
{
  players.clear();
  projectiles.clear();
  foods.clear();

  auto sep1 = msg.find('#');
  auto sep2 = (sep1 != std::string::npos) ? msg.find('#', sep1 + 1) : std::string::npos;

  std::string players_part = (sep1 != std::string::npos) ? msg.substr(0, sep1) : msg;
  std::string proj_part    = (sep1 != std::string::npos && sep2 != std::string::npos)
                               ? msg.substr(sep1 + 1, sep2 - sep1 - 1) : "";
  std::string food_part    = (sep2 != std::string::npos) ? msg.substr(sep2 + 1) : "";

  if (!players_part.empty()) {
    std::istringstream ps(players_part);
    std::string entry;
    while (std::getline(ps, entry, ';')) {
      PlayerState p{};
      if (std::sscanf(entry.c_str(), "P%d,%f,%f,%f,%f,%d,%d",
              &p.id, &p.x, &p.y, &p.dx, &p.dy, &p.hp, &p.score) == 7)
        players.push_back(p);
    }
  }

  if (!proj_part.empty()) {
    std::istringstream ps(proj_part);
    std::string entry;
    while (std::getline(ps, entry, ';')) {
      ProjectileState r{};
      if (std::sscanf(entry.c_str(), "R%d,%f,%f", &r.id, &r.x, &r.y) == 3)
        projectiles.push_back(r);
    }
  }

  if (!food_part.empty()) {
    std::istringstream ps(food_part);
    std::string entry;
    while (std::getline(ps, entry, ';')) {
      FoodState f{};
      if (std::sscanf(entry.c_str(), "F%d,%f,%f", &f.id, &f.x, &f.y) == 3)
        foods.push_back(f);
    }
  }

  return true;
}

} // namespace NetProtocol

#endif // STATE_PROTOCOL_H
