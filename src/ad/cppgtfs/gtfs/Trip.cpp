// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#include <stdint.h>
#include <iostream>
#include <string>
#include "Route.h"
#include "Service.h"
#include "Shape.h"
#include "StopTime.h"
#include "Trip.h"

using std::exception;
using std::string;

using ad::cppgtfs::gtfs::Trip;
using ad::cppgtfs::gtfs::Route;
using ad::cppgtfs::gtfs::Service;
using ad::cppgtfs::gtfs::Shape;
using ad::cppgtfs::gtfs::StopTime;
using ad::cppgtfs::gtfs::StopTimes;

// _____________________________________________________________________________
Trip::Trip(const std::string& id, Route* r, Service* s, const std::string& hs,
           const std::string& short_name, DIRECTION dir,
           const std::string& blockid, Shape* shp, WC_BIKE_ACCESSIBLE wc,
           WC_BIKE_ACCESSIBLE ba)
    : _id(id),
      _route(r),
      _service(s),
      _headsign(hs),
      _short_name(short_name),
      _dir(dir),
      _block_id(blockid),
      _shape(shp),
      _wc(wc),
      _ba(ba) {}

// _____________________________________________________________________________
const std::string& Trip::getId() const { return _id; }

// _____________________________________________________________________________
Route* Trip::getRoute() { return _route; }

// _____________________________________________________________________________
const Route* Trip::getRoute() const { return _route; }

// _____________________________________________________________________________
Service* Trip::getService() { return _service; }

// _____________________________________________________________________________
const Service* Trip::getService() const { return _service; }

// _____________________________________________________________________________
const std::string& Trip::getHeadsign() const { return _headsign; }

// _____________________________________________________________________________
const std::string& Trip::getShortname() const { return _short_name; }

// _____________________________________________________________________________
Trip::DIRECTION Trip::getDirection() const { return _dir; }

// _____________________________________________________________________________
const std::string& Trip::getBlockId() const { return _block_id; }

// _____________________________________________________________________________
Shape* Trip::getShape() { return _shape; }

// _____________________________________________________________________________
const Shape* Trip::getShape() const { return _shape; }

// _____________________________________________________________________________
void Trip::setShape(Shape* shp) { _shape = shp; }

// _____________________________________________________________________________
Trip::WC_BIKE_ACCESSIBLE Trip::getWheelchairAccessibility() const {
  return _wc;
}

// _____________________________________________________________________________
Trip::WC_BIKE_ACCESSIBLE Trip::getBikesAllowed() const { return _ba; }

// _____________________________________________________________________________
const StopTimes& Trip::getStopTimes() const { return _stoptimes; }

// _____________________________________________________________________________
StopTimes& Trip::getStopTimes() { return _stoptimes; }

// _____________________________________________________________________________
bool Trip::addStopTime(const StopTime& t) {
  return _stoptimes.insert(t).second;
}
