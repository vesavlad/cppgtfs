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
#include <cppgtfs/gtfs/Agency.h>
#include <cppgtfs/gtfs/flat/Route.h>

using std::exception;
using std::string;

namespace ad {
namespace cppgtfs {
namespace gtfs {

template <typename AgencyT>
class RouteB {
 public:
  typedef RouteB<AgencyT>* Ref;
  static std::string getId(Ref r) { return r->getId(); }

  typedef flat::Route::TYPE TYPE;

  RouteB() {}

  RouteB(const string& id, typename AgencyT::Ref agency,
         const string& short_name, const string& long_name, const string& desc,
         flat::Route::TYPE type, const string& url, uint32_t color,
         uint32_t text_color)
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

  const typename AgencyT::Ref getAgency() const { return _agency; }

  typename AgencyT::Ref getAgency() { return _agency; }

  const std::string& getShortName() const { return _short_name; }

  const std::string& getLongName() const { return _long_name; }

  const std::string& getDesc() const { return _desc; }

  flat::Route::TYPE getType() const { return _type; }

  const std::string& getUrl() const { return _url; }

  uint32_t getColor() const { return _color; }

  std::string getColorString() const {
    return flat::Route::getHexColorString(_color);
  }

  uint32_t getTextColor() const { return _text_color; }

  std::string getTextColorString() const {
    return flat::Route::getHexColorString(_text_color);
  }

  flat::Route getFlat() const {
    flat::Route r;
    r.id = _id;
    r.agency = _agency->getId();
    r.short_name = _short_name;
    r.long_name = _long_name;
    r.desc = _desc;
    r.type = _type;
    r.url = _url;
    r.color = _color;
    r.text_color = _text_color;
    return r;
  }

  // TODO(patrick): implement setters

 private:
  string _id;
  typename AgencyT::Ref _agency;
  string _short_name;
  string _long_name;
  string _desc;
  flat::Route::TYPE _type;
  string _url;
  uint32_t _color;
  uint32_t _text_color;
};

typedef RouteB<Agency> Route;

}  // namespace gtfs
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_GTFS_ROUTE_H_
