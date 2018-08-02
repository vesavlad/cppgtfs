// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_CPPGTFS_GTFS_FEED_H_
#define AD_CPPGTFS_GTFS_FEED_H_

#include <iterator>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>
#include "Agency.h"
#include "Container.h"
#include "Route.h"
#include "Service.h"
#include "Shape.h"
#include "Stop.h"
#include "Trip.h"
#include "Transfer.h"
#include "Fare.h"

namespace ad {
namespace cppgtfs {
namespace gtfs {

typedef Container<Agency> Agencies;
typedef Container<Stop> Stops;
typedef Container<Route> Routes;
typedef Container<Trip> Trips;
typedef Container<Shape> Shapes;
typedef Container<Service> Services;
typedef Container<Fare> Fares;
typedef std::vector<Transfer> Transfers;
typedef std::set<std::string> Zones;

class Feed {
 public:
  Feed()
      : _maxLat(std::numeric_limits<double>::lowest()),
        _maxLon(std::numeric_limits<double>::lowest()),
        _minLat(std::numeric_limits<double>::max()),
        _minLon(std::numeric_limits<double>::max()) {}

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

  const Transfers& getTransfers() const;
  Transfers& getTransfers();

  const Zones& getZones() const;
  Zones& getZones();

  const Fares& getFares() const;
  Fares& getFares();

  const std::string& getPublisherName() const;
  const std::string& getPublisherUrl() const;
  const std::string& getLang() const;
  const std::string& getVersion() const;
  const ServiceDate& getStartDate() const;
  const ServiceDate& getEndDate() const;

  void setPublisherName(const std::string& name);
  void setPublisherUrl(const std::string& url);
  void setLang(const std::string& lang);
  void setVersion(const std::string& version);
  void setStartDate(const ServiceDate& start);
  void setEndDate(const ServiceDate& end);

  void updateBox(double lat, double lon);
  double getMinLat() const;
  double getMinLon() const;
  double getMaxLat() const;
  double getMaxLon() const;

 private:
  Agencies _agencies;
  Stops _stops;
  Routes _routes;
  Trips _trips;
  Shapes _shapes;
  Services _services;
  Transfers _transfers;
  Zones _zones;
  Fares _fares;

  double _maxLat, _maxLon, _minLat, _minLon;

  std::string _publisherName, _publisherUrl, _lang, _version;
  ServiceDate _startDate, _endDate;
};
}  // namespace gtfs
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_GTFS_FEED_H_
