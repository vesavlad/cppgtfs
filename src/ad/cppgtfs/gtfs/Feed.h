// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_CPPGTFS_GTFS_FEED_H_
#define AD_CPPGTFS_GTFS_FEED_H_

#include <iterator>
#include <string>
#include <unordered_map>
#include "Agency.h"
#include "Container.h"
#include "Route.h"
#include "Service.h"
#include "Shape.h"
#include "Stop.h"
#include "Trip.h"

namespace ad {
namespace cppgtfs {
namespace gtfs {

typedef Container<Agency> Agencies;
typedef Container<Stop> Stops;
typedef Container<Route> Routes;
typedef Container<Trip> Trips;
typedef Container<Shape> Shapes;
typedef Container<Service> Services;

class Feed {
 public:
  Feed() {}

  const Agencies& getAgencies() const;
  Agencies& getAgencies();

  const Stops& getStops() const;
  Stops& getStops();

  const Routes& getRoutes() const;
  Routes& getRoutes();

  const Trips& getTrips() const;
  Trips& getTrips();

  const Shapes& getShapes() const;
  Shapes& getShapes();

  const Services& getServices() const;
  Services& getServices();

 private:
  Agencies _agencies;
  Stops _stops;
  Routes _routes;
  Trips _trips;
  Shapes _shapes;
  Services _services;
};
}  // namespace gtfs
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_GTFS_FEED_H_
