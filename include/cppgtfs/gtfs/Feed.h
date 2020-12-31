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
#include <cppgtfs/gtfs/Agency.h>
#include <cppgtfs/gtfs/ContContainer.h>
#include <cppgtfs/gtfs/Container.h>
#include <cppgtfs/gtfs/Fare.h>
#include <cppgtfs/gtfs/Route.h>
#include <cppgtfs/gtfs/Service.h>
#include <cppgtfs/gtfs/Shape.h>
#include <cppgtfs/gtfs/Stop.h>
#include <cppgtfs/gtfs/Transfer.h>
#include <cppgtfs/gtfs/Trip.h>

#define FEEDTPL                                                     \
  template <typename AgencyT, typename RouteT, typename StopT,      \
            typename ServiceT, template <typename> class StopTimeT, \
            typename ShapeT, template <typename> class FareT,        \
            template <typename> class AContainerT,                  \
            template <typename> class RContainerT,                  \
            template <typename> class SContainerT,                  \
            template <typename> class StContainerT,                 \
            template <typename> class TContainerT,                  \
            template <typename> class ShContainerT,                 \
            template <typename> class FContainerT>
#define FEEDB                                                             \
  FeedB<AgencyT, RouteT, StopT, ServiceT, StopTimeT, ShapeT, FareT,       \
        AContainerT, RContainerT, SContainerT, StContainerT, TContainerT, \
        ShContainerT, FContainerT>

namespace ad {
namespace cppgtfs {
namespace gtfs {

FEEDTPL
class FeedB {
  typedef AContainerT<AgencyT> Agencies;
  typedef StContainerT<StopT> Stops;
  typedef RContainerT<RouteT> Routes;
  typedef TContainerT<TripB<StopTimeT<StopT>, ServiceT, RouteT, ShapeT>> Trips;
  typedef ShContainerT<ShapeT> Shapes;
  typedef SContainerT<ServiceT> Services;
  typedef FContainerT<FareT<RouteT>> Fares;
  typedef std::vector<Transfer> Transfers;
  typedef std::set<std::string> Zones;

 public:
  FeedB()
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

  const std::string& getPath() const { return _path; }
  void setPath(const std::string& p) { _path = p; }

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

  std::string _publisherName, _publisherUrl, _lang, _version, _path;
  ServiceDate _startDate, _endDate;
};

typedef FeedB<Agency, Route, Stop, Service, StopTime, Shape, Fare, Container,
              Container, Container, Container, Container, Container, Container>
    Feed;
typedef FeedB<Agency, Route, Stop, Service, StopTime, Shape, Fare,
              ContContainer, ContContainer, ContContainer, ContContainer,
              ContContainer, ContContainer, ContContainer>
    ContFeed;

#include <cppgtfs/gtfs/Feed.tpp>

}  // namespace gtfs
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_GTFS_FEED_H_
