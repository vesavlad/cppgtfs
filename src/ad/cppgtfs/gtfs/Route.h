// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_CPPGTFS_GTFS_ROUTE_H_
#define AD_CPPGTFS_GTFS_ROUTE_H_

#include <stdint.h>
#include <algorithm>
#include <iomanip>
#include <set>
#include <sstream>
#include <string>
#include "Agency.h"

using std::exception;
using std::string;

namespace ad {
namespace cppgtfs {
namespace gtfs {

class Route {
 public:
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

  Route() {}

  Route(const string& id, Agency* agency, const string& short_name,
        const string& long_name, const string& desc, Route::TYPE type,
        const string& url, uint32_t color, uint32_t text_color)
      : _id(id),
        _agency(agency),
        _short_name(short_name),
        _long_name(long_name),
        _desc(desc),
        _type(type),
        _url(url),
        _color(color),
        _text_color(text_color) {}

  const std::string& getId() const { return _id; }

  const Agency* getAgency() const { return _agency; }

  Agency* getAgency() { return _agency; }

  const std::string& getShortName() const { return _short_name; }

  const std::string& getLongName() const { return _long_name; }

  const std::string& getDesc() const { return _desc; }

  Route::TYPE getType() const { return _type; }

  const std::string& getUrl() const { return _url; }

  uint32_t getColor() const { return _color; }

  std::string getColorString() const { return getHexColorString(_color); }

  uint32_t getTextColor() const { return _text_color; }

  std::string getTextColorString() const {
    return getHexColorString(_text_color);
  }

  static std::string getTypeString(Route::TYPE t) {
    std::string names[8] = {"tram",  "subway",   "rail",    "bus",
                            "ferry", "cablecar", "gondola", "funicular"};
    return names[t];
  }

  static std::set<Route::TYPE> getTypesFromString(std::string name) {
    std::set<Route::TYPE> ret;

    char* rem;
    uint64_t num = strtol(name.c_str(), &rem, 10);
    if (!*rem && num <= 7) {
      ret.insert((Route::TYPE)num);
      return ret;
    }

    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    if (name == "all") {
      ret.insert(Route::TYPE::TRAM);
      ret.insert(Route::TYPE::SUBWAY);
      ret.insert(Route::TYPE::RAIL);
      ret.insert(Route::TYPE::BUS);
      ret.insert(Route::TYPE::FERRY);
      ret.insert(Route::TYPE::CABLE_CAR);
      ret.insert(Route::TYPE::GONDOLA);
      ret.insert(Route::TYPE::FUNICULAR);
      return ret;
    }

    if (name == "bus") {
      ret.insert(Route::TYPE::BUS);
      return ret;
    }

    if (name == "tram" || name == "streetcar" || name == "light_rail" ||
        name == "lightrail" || name == "light-rail") {
      ret.insert(Route::TYPE::TRAM);
      return ret;
    }

    if (name == "train" || name == "rail") {
      ret.insert(Route::TYPE::RAIL);
      return ret;
    }

    if (name == "ferry" || name == "boat" || name == "ship") {
      ret.insert(Route::TYPE::FERRY);
      return ret;
    }

    if (name == "subway" || name == "metro") {
      ret.insert(Route::TYPE::SUBWAY);
      return ret;
    }

    if (name == "cablecar" || name == "cable_car" || name == "cable-car") {
      ret.insert(Route::TYPE::CABLE_CAR);
      return ret;
    }

    if (name == "gondola") {
      ret.insert(Route::TYPE::GONDOLA);
      return ret;
    }

    if (name == "funicular") {
      ret.insert(Route::TYPE::FUNICULAR);
      return ret;
    }

    return ret;
  }

  // TODO(patrick): implement setters

 private:
  string _id;
  Agency* _agency;
  string _short_name;
  string _long_name;
  string _desc;
  Route::TYPE _type;
  string _url;
  uint32_t _color;
  uint32_t _text_color;

  std::string getHexColorString(uint32_t color) const {
    // using stringstream here, because it doesnt add "0x" to the front
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(6) << color;
    return ss.str();
  }
};

}  // namespace gtfs
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_GTFS_ROUTE_H_
