// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_CPPGTFS_GTFS_FLAT_ROUTE_H_
#define AD_CPPGTFS_GTFS_FLAT_ROUTE_H_

#include <string>
#include <sstream>

namespace ad {
namespace cppgtfs {
namespace gtfs {
namespace flat {

struct RouteFlds {
  size_t routeIdFld;
  size_t routeLongNameFld;
  size_t routeShortNameFld;
  size_t routeTypeFld;
  size_t routeUrlFld;
  size_t routeDescFld;
  size_t agencyIdFld;
  size_t routeColorFld;
  size_t routeTextColorFld;
};

struct Route {
  enum TYPE : uint8_t {
    TRAM = 0,
    SUBWAY = 1,
    RAIL = 2,
    BUS = 3,
    FERRY = 4,
    CABLE_CAR = 5,
    GONDOLA = 6,
    FUNICULAR = 7
  };

  string id;
  string agency;
  string short_name;
  string long_name;
  string desc;
  Route::TYPE type;
  string url;
  uint32_t color;
  uint32_t text_color;

  static std::string getHexColorString(uint32_t color) {
    // using stringstream here, because it doesnt add "0x" to the front
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(6) << color;
    return ss.str();
  }
};

}  // namespace flat
}  // namespace gtfs
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_GTFS_ROUTE_H_
