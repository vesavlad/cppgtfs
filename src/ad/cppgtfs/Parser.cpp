// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#include <boost/filesystem.hpp>
#include <fstream>
#include <map>
#include <string>
#include <utility>
#include "Parser.h"
#include "ad/util/CsvParser.h"
#include "gtfs/Agency.h"
#include "gtfs/Frequency.h"
#include "gtfs/Route.h"
#include "gtfs/Stop.h"

using ad::util::CsvParser;
using ad::util::CsvParserException;
using ad::cppgtfs::Parser;
using ad::cppgtfs::gtfs::Agency;
using ad::cppgtfs::gtfs::Stop;
using ad::cppgtfs::gtfs::Route;
using ad::cppgtfs::gtfs::Trip;
using ad::cppgtfs::gtfs::Service;
using ad::cppgtfs::gtfs::ServiceDate;
using ad::cppgtfs::gtfs::StopTime;
using ad::cppgtfs::gtfs::Shape;
using ad::cppgtfs::gtfs::ShapePoint;
using ad::cppgtfs::gtfs::Time;
using ad::cppgtfs::gtfs::Frequency;

// ____________________________________________________________________________
bool Parser::parse(gtfs::Feed* targetFeed, std::string path) const {
  std::ifstream fs;
  boost::filesystem::path gtfsPath(path);
  boost::filesystem::path curFile;

  try {
    curFile = gtfsPath / "feed_info.txt";
    fs.open(curFile.c_str());
    if (fs.good()) {
      parseFeedInfo(targetFeed, &fs);
      fs.close();
    }

    curFile = gtfsPath / "agency.txt";
    fs.open(curFile.c_str());
    if (!fs.good()) fileNotFound(curFile);
    parseAgencies(targetFeed, &fs);
    fs.close();

    curFile = gtfsPath / "stops.txt";
    fs.open(curFile.c_str());
    if (!fs.good()) fileNotFound(curFile);
    parseStops(targetFeed, &fs);
    fs.close();

    curFile = gtfsPath / "routes.txt";
    fs.open(curFile.c_str());
    if (!fs.good()) fileNotFound(curFile);
    parseRoutes(targetFeed, &fs);
    fs.close();

    curFile = gtfsPath / "calendar.txt";
    fs.open(curFile.c_str());
    if (fs.good()) {
      parseCalendar(targetFeed, &fs);
      fs.close();
    }

    curFile = gtfsPath / "calendar_dates.txt";
    fs.open(curFile.c_str());
    if (fs.good()) {
      parseCalendarDates(targetFeed, &fs);
      fs.close();
    }

    curFile = gtfsPath / "shapes.txt";
    fs.open(curFile.c_str());
    if (fs.good()) {
      parseShapes(targetFeed, &fs);
      fs.close();
    }

    curFile = gtfsPath / "trips.txt";
    fs.open(curFile.c_str());
    if (!fs.good()) fileNotFound(curFile);
    parseTrips(targetFeed, &fs);
    fs.close();

    curFile = gtfsPath / "stop_times.txt";
    fs.open(curFile.c_str());
    if (!fs.good()) fileNotFound(curFile);
    parseStopTimes(targetFeed, &fs);
    fs.close();

    curFile = gtfsPath / "frequencies.txt";
    fs.open(curFile.c_str());
    if (fs.good()) {
      parseFrequencies(targetFeed, &fs);
      fs.close();
    }

    curFile = gtfsPath / "transfers.txt";
    fs.open(curFile.c_str());
    if (fs.good()) {
      parseTransfers(targetFeed, &fs);
      fs.close();
    }

    curFile = gtfsPath / "fare_attributes.txt";
    fs.open(curFile.c_str());
    if (fs.good()) {
      parseTransfers(targetFeed, &fs);
      fs.close();
    }

    curFile = gtfsPath / "fare_rules.txt";
    fs.open(curFile.c_str());
    if (fs.good()) {
      parseTransfers(targetFeed, &fs);
      fs.close();
    }
  } catch (const CsvParserException& e) {
    throw ParserException(e.getMsg(), e.getFieldName(), e.getLine(),
                          curFile.c_str());
  } catch (const ParserException& e) {
    // augment with file namoe
    ParserException fe = e;
    fe.setFileName(curFile.c_str());
    throw fe;
  }

  return true;
}

// ____________________________________________________________________________
void Parser::parseTransfers(gtfs::Feed* targetFeed, std::istream* s) const {
  CsvParser csvp(s);

  size_t fromStopIdFld = csvp.getFieldIndex("from_stop_id");
  size_t toStopIdFld = csvp.getFieldIndex("to_stop_id");
  size_t transferTypeFld = csvp.getFieldIndex("transfer_type");
  size_t minTransferTimeFld = csvp.getOptFieldIndex("min_transfer_time");
}

// ____________________________________________________________________________
void Parser::parseFrequencies(gtfs::Feed* targetFeed, std::istream* s) const {
  CsvParser csvp(s);

  size_t tripIdFld = csvp.getFieldIndex("trip_id");
  size_t startTimeFld = csvp.getFieldIndex("start_time");
  size_t endTimeFld = csvp.getFieldIndex("end_time");
  size_t headwaySecsFld = csvp.getFieldIndex("headway_secs");
  size_t exactTimesFld = csvp.getOptFieldIndex("exact_times");

  while (csvp.readNextLine()) {
    std::string tid = getString(csvp, tripIdFld);
    gtfs::Frequency f(getTime(csvp, startTimeFld), getTime(csvp, endTimeFld),
                      getRangeInteger(csvp, headwaySecsFld, 0, UINT16_MAX),
                      getRangeInteger(csvp, exactTimesFld, 0, 1, 0));

    gtfs::Trip* trip = targetFeed->getTrips().get(tid);
    if (!trip) {
      std::stringstream msg;
      msg << "trip '" << tid << "' not found.";
      throw ParserException(msg.str(), "trip_id", csvp.getCurLine());
    }

    trip->addFrequency(f);
  }
}

// ____________________________________________________________________________
void Parser::parseFareAttributes(gtfs::Feed* targetFeed,
                                 std::istream* s) const {
  CsvParser csvp(s);

  size_t fareIdFld = csvp.getFieldIndex("fare_id");
  size_t routeIdFld = csvp.getOptFieldIndex("route_id");
  size_t originIdFld = csvp.getOptFieldIndex("origin_id");
  size_t destinationIdFld = csvp.getOptFieldIndex("destination_id");
  size_t containsIdFld = csvp.getOptFieldIndex("contains_id");
}

// ____________________________________________________________________________
void Parser::parseFareRules(gtfs::Feed* targetFeed, std::istream* s) const {
  CsvParser csvp(s);

  size_t fareIdFld = csvp.getFieldIndex("fare_id");
}

// ____________________________________________________________________________
void Parser::parseFeedInfo(gtfs::Feed* targetFeed, std::istream* s) const {
  CsvParser csvp(s);

  size_t feedPublisherNameFld = csvp.getFieldIndex("feed_publisher_name");
  size_t feedPublisherUrlFld = csvp.getFieldIndex("feed_publisher_url");
  size_t feedLangFld = csvp.getOptFieldIndex("feed_lang");
  size_t feedStartDateFld = csvp.getOptFieldIndex("feed_start_date");
  size_t feedEndDateFld = csvp.getOptFieldIndex("feed_end_date");
  size_t feedVersionFld = csvp.getOptFieldIndex("feed_version");

  while (csvp.readNextLine()) {
    targetFeed->setPublisherName(getString(csvp, feedPublisherNameFld));
    targetFeed->setPublisherUrl(getString(csvp, feedPublisherUrlFld));
    targetFeed->setLang(getString(csvp, feedLangFld, ""));
    targetFeed->setVersion(getString(csvp, feedVersionFld, ""));
    targetFeed->setStartDate(getServiceDate(csvp, feedStartDateFld, false));
    targetFeed->setEndDate(getServiceDate(csvp, feedEndDateFld, false));
  }
}

// ____________________________________________________________________________
void Parser::parseAgencies(gtfs::Feed* targetFeed, std::istream* s) const {
  CsvParser csvp(s);
  Agency* a = 0;

  size_t agencyNameFld = csvp.getFieldIndex("agency_name");
  size_t agencyUrlFld = csvp.getFieldIndex("agency_url");
  size_t agencyTimezoneFld = csvp.getFieldIndex("agency_timezone");
  size_t agencyEmailFld = csvp.getOptFieldIndex("agency_email");
  size_t agencyFareUrlFld = csvp.getOptFieldIndex("agency_fare_url");
  size_t agencyLangFld = csvp.getOptFieldIndex("agency_lang");
  size_t agencyPhoneFld = csvp.getOptFieldIndex("agency_phone");
  size_t agencyIdFld = csvp.getOptFieldIndex("agency_id");

  while (csvp.readNextLine()) {
    a = new gtfs::Agency(
        getString(csvp, agencyIdFld, ""), getString(csvp, agencyNameFld),
        getString(csvp, agencyUrlFld), getString(csvp, agencyTimezoneFld),
        getString(csvp, agencyLangFld, ""), getString(csvp, agencyPhoneFld, ""),
        getString(csvp, agencyFareUrlFld, ""),
        getString(csvp, agencyEmailFld, ""));

    if (!targetFeed->getAgencies().add(a)) {
      std::stringstream msg;
      msg << "'agency_id' must be dataset unique. Collision with id '"
          << a->getId() << "')";
      throw ParserException(msg.str(), "agency_id", csvp.getCurLine());
    }
  }

  if (!a) {
    throw ParserException(
        "the feed has no agency defined."
        " This is a required field.",
        "", 1);
  }
}

// ____________________________________________________________________________
void Parser::parseStops(gtfs::Feed* targetFeed, std::istream* s) const {
  CsvParser csvp(s);

  std::map<Stop*, std::pair<size_t, std::string> > parentStations;

  size_t stopIdFld = csvp.getFieldIndex("stop_id");
  size_t stopNameFld = csvp.getFieldIndex("stop_name");
  size_t stopLatFld = csvp.getFieldIndex("stop_lat");
  size_t stopLonFld = csvp.getFieldIndex("stop_lon");
  size_t parentStationFld = csvp.getOptFieldIndex("parent_station");
  size_t stopCodeFld = csvp.getOptFieldIndex("stop_code");
  size_t stopDescFld = csvp.getOptFieldIndex("stop_desc");
  size_t zoneIdFld = csvp.getOptFieldIndex("zone_id");
  size_t stopUrlFld = csvp.getOptFieldIndex("stop_url");
  size_t stopTimezoneFld = csvp.getOptFieldIndex("stop_timezone");
  size_t wheelchairBoardingFld = csvp.getOptFieldIndex("wheelchair_boarding");
  size_t locationTypeFld = csvp.getOptFieldIndex("location_type");
  size_t platformCodeFld = csvp.getOptFieldIndex("platform_code");

  while (csvp.readNextLine()) {
    Stop::LOCATION_TYPE locType = static_cast<Stop::LOCATION_TYPE>(
        getRangeInteger(csvp, locationTypeFld, 0, 2, 0));

    double lat = getDouble(csvp, stopLatFld);
    double lon = getDouble(csvp, stopLonFld);
    targetFeed->updateBox(lat, lon);

    Stop* s = new Stop(
        getString(csvp, stopIdFld), getString(csvp, stopCodeFld, ""),
        getString(csvp, stopNameFld), getString(csvp, stopDescFld, ""), lat,
        lon, getString(csvp, zoneIdFld, ""), getString(csvp, stopUrlFld, ""),
        locType, 0, getString(csvp, stopTimezoneFld, ""),
        static_cast<Stop::WHEELCHAIR_BOARDING>(
            getRangeInteger(csvp, wheelchairBoardingFld, 0, 2, 0)),
        getString(csvp, platformCodeFld, ""));

    const std::string& parentStatId = getString(csvp, parentStationFld, "");
    if (!parentStatId.empty()) {
      if (locType == Stop::LOCATION_TYPE::STATION) {
        throw ParserException(
            "a stop with location_type 'station' (1) cannot"
            " have a parent station",
            "parent_station", csvp.getCurLine());
      }

      parentStations[s] =
          std::pair<size_t, std::string>(csvp.getCurLine(), parentStatId);
    }

    if (!targetFeed->getStops().add(s)) {
      std::stringstream msg;
      msg << "'stop_id' must be dataset unique. Collision with id '"
          << s->getId() << "')";
      throw ParserException(msg.str(), "stop_id", csvp.getCurLine());
    }
  }

  // second pass to resolve parentStation pointers
  for (const auto& ps : parentStations) {
    Stop* parentStation = 0;
    parentStation = targetFeed->getStops().get(ps.second.second);
    if (!parentStation) {
      std::stringstream msg;
      msg << "no stop with id '" << ps.second.second << "' defined, cannot "
          << "reference here.";
      throw ParserException(msg.str(), "parent_station", ps.second.first);
    } else {
      ps.first->setParentStation(parentStation);
    }
  }
}

// ____________________________________________________________________________
void Parser::parseRoutes(gtfs::Feed* targetFeed, std::istream* s) const {
  CsvParser csvp(s);

  size_t routeIdFld = csvp.getFieldIndex("route_id");
  size_t routeLongNameFld = csvp.getOptFieldIndex("route_long_name");
  size_t routeShortNameFld = csvp.getOptFieldIndex("route_short_name");
  size_t routeTypeFld = csvp.getFieldIndex("route_type");
  size_t routeUrlFld = csvp.getOptFieldIndex("route_url");
  size_t routeDescFld = csvp.getOptFieldIndex("route_desc");
  size_t agencyIdFld = csvp.getOptFieldIndex("agency_id");
  size_t routeColorFld = csvp.getOptFieldIndex("route_color");
  size_t routeTextColorFld = csvp.getOptFieldIndex("route_text_color");

  while (csvp.readNextLine()) {
    const std::string& agencyId = getString(csvp, agencyIdFld, "");
    Agency* routeAgency = 0;

    if (!agencyId.empty()) {
      routeAgency = targetFeed->getAgencies().get(agencyId);
      if (!routeAgency) {
        std::stringstream msg;
        msg << "no agency with id '" << agencyId << "' defined, cannot "
            << "reference here.";
        throw ParserException(msg.str(), "agency_id", csvp.getCurLine());
      }
    }

    Route* r =
        new Route(getString(csvp, routeIdFld), routeAgency,
                  getString(csvp, routeShortNameFld, ""),
                  getString(csvp, routeLongNameFld, ""),
                  getString(csvp, routeDescFld, ""),
                  getRouteType(csvp, routeTypeFld,
                               getRangeInteger(csvp, routeTypeFld, 0, 1702)),
                  getString(csvp, routeUrlFld, ""),
                  getColorFromHexString(csvp, routeColorFld, "FFFFFF"),
                  getColorFromHexString(csvp, routeTextColorFld, "000000"));

    if (!targetFeed->getRoutes().add(r)) {
      std::stringstream msg;
      msg << "'route_id' must be dataset unique. Collision with id '"
          << r->getId() << "')";
      throw ParserException(msg.str(), "route_id", csvp.getCurLine());
    }
  }
}

// ____________________________________________________________________________
void Parser::parseCalendar(gtfs::Feed* targetFeed, std::istream* s) const {
  CsvParser csvp(s);

  size_t serviceIdFld = csvp.getFieldIndex("service_id");
  size_t mondayFld = csvp.getFieldIndex("monday");
  size_t tuesdayFld = csvp.getFieldIndex("tuesday");
  size_t wednesdayFld = csvp.getFieldIndex("wednesday");
  size_t thursdayFld = csvp.getFieldIndex("thursday");
  size_t fridayFld = csvp.getFieldIndex("friday");
  size_t saturdayFld = csvp.getFieldIndex("saturday");
  size_t sundayFld = csvp.getFieldIndex("sunday");
  size_t startDateFld = csvp.getFieldIndex("start_date");
  size_t endDateFld = csvp.getFieldIndex("end_date");

  while (csvp.readNextLine()) {
    const std::string& serviceId = getString(csvp, serviceIdFld);

    Service* s = new Service(
        serviceId, (getRangeInteger(csvp, mondayFld, 0, 1)) |
                       (getRangeInteger(csvp, tuesdayFld, 0, 1) << 1) |
                       (getRangeInteger(csvp, wednesdayFld, 0, 1) << 2) |
                       (getRangeInteger(csvp, thursdayFld, 0, 1) << 3) |
                       (getRangeInteger(csvp, fridayFld, 0, 1) << 4) |
                       (getRangeInteger(csvp, saturdayFld, 0, 1) << 5) |
                       (getRangeInteger(csvp, sundayFld, 0, 1) << 6),
        getServiceDate(csvp, startDateFld, true),
        getServiceDate(csvp, endDateFld, true));

    if (!targetFeed->getServices().add(s)) {
      std::stringstream msg;
      msg << "'service_id' must be unique in calendars.txt. Collision with id '"
          << s->getId() << "')";
      throw ParserException(msg.str(), "service_id", csvp.getCurLine());
    }
  }
}

// ____________________________________________________________________________
void Parser::parseCalendarDates(gtfs::Feed* targetFeed, std::istream* s) const {
  CsvParser csvp(s);

  size_t serviceIdFld = csvp.getFieldIndex("service_id");
  size_t exceptionTypeFld = csvp.getFieldIndex("exception_type");
  size_t dateFld = csvp.getFieldIndex("date");

  while (csvp.readNextLine()) {
    const std::string& serviceId = getString(csvp, serviceIdFld);
    const ServiceDate& d = getServiceDate(csvp, dateFld, true);
    Service::EXCEPTION_TYPE t = static_cast<Service::EXCEPTION_TYPE>(
        getRangeInteger(csvp, exceptionTypeFld, 1, 2));

    Service* e = targetFeed->getServices().get(serviceId);

    if (!e) {
      e = new Service(serviceId);
      targetFeed->getServices().add(e);
    }

    e->addException(d, t);
  }
}

// ____________________________________________________________________________
void Parser::parseTrips(gtfs::Feed* targetFeed, std::istream* s) const {
  CsvParser csvp(s);

  size_t shapeIdFld = csvp.getOptFieldIndex("shape_id");
  size_t tripIdFld = csvp.getFieldIndex("trip_id");
  size_t serviceIdFld = csvp.getFieldIndex("service_id");
  size_t routeIdFld = csvp.getFieldIndex("route_id");
  size_t blockIdFld = csvp.getOptFieldIndex("block_id");
  size_t tripHeadsignFld = csvp.getOptFieldIndex("trip_headsign");
  size_t tripShortNameFld = csvp.getOptFieldIndex("trip_short_name");
  size_t bikesAllowedFld = csvp.getOptFieldIndex("bikes_allowed");
  size_t wheelchairAccessibleFld =
      csvp.getOptFieldIndex("wheelchair_accessible");
  size_t directionIdFld = csvp.getOptFieldIndex("direction_id");

  while (csvp.readNextLine()) {
    const std::string& routeId = getString(csvp, routeIdFld);
    Route* tripRoute = 0;

    tripRoute = targetFeed->getRoutes().get(routeId);
    if (!tripRoute) {
      std::stringstream msg;
      msg << "no route with id '" << routeId << "' defined, cannot "
          << "reference here.";
      throw ParserException(msg.str(), "route_id", csvp.getCurLine());
    }

    const std::string& shapeId = getString(csvp, shapeIdFld, "");
    Shape* tripShape = 0;

    if (!shapeId.empty()) {
      tripShape = targetFeed->getShapes().get(shapeId);
      if (!tripShape) {
        std::stringstream msg;
        msg << "no shape with id '" << shapeId << "' defined, cannot "
            << "reference here.";
        throw ParserException(msg.str(), "shape_id", csvp.getCurLine());
      }
    }

    const std::string& serviceId = getString(csvp, serviceIdFld);
    Service* tripService = targetFeed->getServices().get(serviceId);
    if (!tripService) {
      std::stringstream msg;
      msg << "no service with id '" << serviceId << "' defined, cannot "
          << "reference here.";
      throw ParserException(msg.str(), "service_id", csvp.getCurLine());
    }

    Trip* t = new Trip(getString(csvp, tripIdFld), tripRoute, tripService,
                       getString(csvp, tripHeadsignFld, ""),
                       getString(csvp, tripShortNameFld, ""),
                       static_cast<Trip::DIRECTION>(
                           getRangeInteger(csvp, directionIdFld, 0, 1, 2)),
                       getString(csvp, blockIdFld, ""), tripShape,
                       static_cast<Trip::WC_BIKE_ACCESSIBLE>(getRangeInteger(
                           csvp, wheelchairAccessibleFld, 0, 2, 0)),
                       static_cast<Trip::WC_BIKE_ACCESSIBLE>(
                           getRangeInteger(csvp, bikesAllowedFld, 0, 2, 0)));

    if (!targetFeed->getTrips().add(t)) {
      std::stringstream msg;
      msg << "'trip_id' must be dataset unique. Collision with id '"
          << t->getId() << "')";
      throw ParserException(msg.str(), "trip_id", csvp.getCurLine());
    }
  }
}

// ____________________________________________________________________________
void Parser::parseShapes(gtfs::Feed* targetFeed, std::istream* s) const {
  CsvParser csvp(s);

  size_t shapeIdFld = csvp.getFieldIndex("shape_id");
  size_t shapePtSequenceFld = csvp.getFieldIndex("shape_pt_sequence");
  size_t shapePtLonFld = csvp.getFieldIndex("shape_pt_lon");
  size_t shapePtLatFld = csvp.getFieldIndex("shape_pt_lat");
  size_t shapeDistTraveledFld = csvp.getOptFieldIndex("shape_dist_traveled");

  while (csvp.readNextLine()) {
    const std::string& shapeId = getString(csvp, shapeIdFld);
    Shape* s = targetFeed->getShapes().get(shapeId);

    double lat = getDouble(csvp, shapePtLatFld);
    double lon = getDouble(csvp, shapePtLonFld);
    targetFeed->updateBox(lat, lon);

    if (!s) {
      targetFeed->getShapes().add(new Shape(shapeId));
      s = targetFeed->getShapes().get(shapeId);
    }

    double dist = -1;  // using -1 as a NULL value here

    if (shapeDistTraveledFld < csvp.getNumColumns()) {
      if (!getString(csvp, shapeDistTraveledFld, "").empty()) {
        dist = getDouble(csvp, shapeDistTraveledFld);
        if (dist < -0.01) {  // TODO(patrick): better double comp
          throw ParserException(
              "negative values not supported for distances"
              " (value was: " +
                  std::to_string(dist),
              "shape_dist_traveled", csvp.getCurLine());
        }
      }
    }

    if (!s->addPoint(ShapePoint(
            lat, lon, dist,
            getRangeInteger(csvp, shapePtSequenceFld, 0, UINT32_MAX)))) {
      throw ParserException(
          "shape_pt_sequence collision,"
          "shape_pt_sequence has "
          "to be increasing for a single shape.",
          "shape_pt_sequence", csvp.getCurLine());
    }
  }
}

// ____________________________________________________________________________
void Parser::parseStopTimes(gtfs::Feed* targetFeed, std::istream* s) const {
  CsvParser csvp(s);

  size_t stopIdFld = csvp.getFieldIndex("stop_id");
  size_t tripIdFld = csvp.getFieldIndex("trip_id");
  size_t arrivalTimeFld = csvp.getFieldIndex("arrival_time");
  size_t departureTimeFld = csvp.getFieldIndex("departure_time");
  size_t stopSequenceFld = csvp.getFieldIndex("stop_sequence");
  size_t stopHeadsignFld = csvp.getOptFieldIndex("stop_headsign");
  size_t shapeDistTraveledFld = csvp.getOptFieldIndex("shape_dist_traveled");
  size_t timepointFld = csvp.getOptFieldIndex("timepoint");
  size_t pickUpTypeFld = csvp.getOptFieldIndex("pick_up_type");
  size_t dropOffTypeFld = csvp.getOptFieldIndex("drop_off_type");

  while (csvp.readNextLine()) {
    Stop* stop = 0;
    Trip* trip = 0;

    const std::string& stopId = getString(csvp, stopIdFld);
    const std::string& tripId = getString(csvp, tripIdFld);

    stop = targetFeed->getStops().get(stopId);
    trip = targetFeed->getTrips().get(tripId);

    if (!stop) {
      std::stringstream msg;
      msg << "no stop with id '" << stopId << "' defined in stops.txt, cannot "
          << "reference here.";
      throw ParserException(msg.str(), "stop_id", csvp.getCurLine());
    }

    if (!trip) {
      std::stringstream msg;
      msg << "no trip with id '" << tripId << "' defined in trips.txt, cannot "
          << "reference here.";
      throw ParserException(msg.str(), "trip_id", csvp.getCurLine());
    }

    double dist = -1;  // using -1 as a NULL value here

    if (shapeDistTraveledFld < csvp.getNumColumns()) {
      if (!getString(csvp, shapeDistTraveledFld, "").empty()) {
        dist = getDouble(csvp, shapeDistTraveledFld);
        if (dist < -0.01) {  // TODO(patrick): better double comp
          throw ParserException(
              "negative values not supported for distances"
              " (value was: " +
                  std::to_string(dist),
              "shape_dist_traveled", csvp.getCurLine());
        }
      }
    }

    StopTime st(getTime(csvp, arrivalTimeFld), getTime(csvp, departureTimeFld),
                stop, getRangeInteger(csvp, stopSequenceFld, 0, UINT32_MAX),
                getString(csvp, stopHeadsignFld, ""),
                static_cast<StopTime::PU_DO_TYPE>(
                    getRangeInteger(csvp, dropOffTypeFld, 0, 3, 0)),
                static_cast<StopTime::PU_DO_TYPE>(
                    getRangeInteger(csvp, pickUpTypeFld, 0, 3, 0)),
                dist, getRangeInteger(csvp, timepointFld, 0, 1, 1));

    if (st.getArrivalTime() > st.getDepartureTime()) {
      throw ParserException("arrival time '" + st.getArrivalTime().toString() +
                                "' is later than departure time '" +
                                st.getDepartureTime().toString() +
                                "'. You cannot depart earlier than you arrive.",
                            "departure_time", csvp.getCurLine());
    }

    if (!trip->addStopTime(st)) {
      throw ParserException(
          "stop_sequence collision, stop_sequence has "
          "to be increasing for a single trip.",
          "stop_sequence", csvp.getCurLine());
    }
  }
}

// ___________________________________________________________________________
void Parser::fileNotFound(boost::filesystem::path file) const {
  throw ParserException("File not found", "", -1, std::string(file.c_str()));
}

// ___________________________________________________________________________
std::string Parser::getString(const CsvParser& csv, size_t field) const {
  const char* r = csv.getTString(field);
  if (r[0] == 0) {
    throw ParserException("expected non-empty string", csv.getFieldName(field),
                          csv.getCurLine());
  }
  return r;
}

// ___________________________________________________________________________
std::string Parser::getString(const CsvParser& csv, size_t field,
                              const std::string& def) const {
  if (field < csv.getNumColumns() && !csv.fieldIsEmpty(field)) {
    return csv.getTString(field);
  }

  return def;
}

// ___________________________________________________________________________
double Parser::getDouble(const CsvParser& csv, size_t field) const {
  return csv.getDouble(field);
}

// ___________________________________________________________________________
double Parser::getDouble(const CsvParser& csv, size_t field, double ret) const {
  if (field < csv.getNumColumns() && !csv.fieldIsEmpty(field)) {
    return csv.getDouble(field);
  }

  return ret;
}

// ___________________________________________________________________________
int64_t Parser::getRangeInteger(const CsvParser& csv, size_t field,
                                int64_t minv, int64_t maxv) const {
  int64_t ret = csv.getLong(field);

  if (ret < minv || ret > maxv) {
    std::stringstream msg;
    msg << "expected integer in range [" << minv << "," << maxv << "]";
    throw ParserException(msg.str(), csv.getFieldName(field), csv.getCurLine());
  }

  return ret;
}

// ___________________________________________________________________________
int64_t Parser::getRangeInteger(const CsvParser& csv, size_t field,
                                int64_t minv, int64_t maxv, int64_t def) const {
  int64_t ret;

  if (field < csv.getNumColumns() && !csv.fieldIsEmpty(field)) {
    ret = csv.getLong(field);

    if (ret < minv || ret > maxv) {
      if (_strict) {
        std::stringstream msg;
        msg << "expected integer in range [" << minv << "," << maxv << "]";
        throw ParserException(msg.str(), csv.getFieldName(field),
                              csv.getCurLine());
      } else
        return def;
    }

    return ret;
  }

  return def;
}

// ___________________________________________________________________________
uint32_t Parser::getColorFromHexString(const CsvParser& csv, size_t field,
                                       const std::string& def) const {
  std::string color_string;

  if (field < csv.getNumColumns()) {
    color_string = csv.getTString(field);
  }

  if (color_string.empty()) color_string = def;

  size_t chars_processed = 0;
  uint32_t ret = 0;

  try {
    ret = std::stoul("0x" + color_string, &chars_processed, 16);
  } catch (std::exception e) {
    if (_strict) {
      std::stringstream msg;
      msg << "expected a 6-character hexadecimal color string, found '"
          << color_string << "' instead. (Error while parsing was: " << e.what()
          << ")";
      throw ParserException(msg.str(), csv.getFieldName(field),
                            csv.getCurLine());
    } else {
      return std::stoul("0x" + def, &chars_processed, 16);
    }
  }

  if (color_string.size() != 6 || chars_processed != 8) {
    if (_strict) {
      std::stringstream msg;
      msg << "expected a 6-character hexadecimal color string, found '"
          << color_string << "' instead.";
      throw ParserException(msg.str(), csv.getFieldName(field),
                            csv.getCurLine());
    } else {
      return std::stoul("0x" + def, &chars_processed, 16);
    }
  }

  return ret;
}

// ____________________________________________________________________________
ServiceDate Parser::getServiceDate(const CsvParser& csv, size_t field) const {
  return getServiceDate(csv, field, false);
}

// ____________________________________________________________________________
ServiceDate Parser::getServiceDate(const CsvParser& csv, size_t field,
                                   bool req) const {
  size_t p;
  const char* val = csv.getTString(field);
  if (strlen(val) == 0 && !req) return ServiceDate();

  try {
    int32_t yyyymmdd = std::stoul(val, &p, 10);
    if (p != strlen(val) || yyyymmdd > 99999999) {
      std::stringstream msg;
      msg << "expected a date in the YYYYMMDD format, found '" << val
          << "' instead.";
      throw ParserException(msg.str(), csv.getFieldName(field),
                            csv.getCurLine());
    }
    return ServiceDate(yyyymmdd);
  } catch (const std::out_of_range& e) {
    std::stringstream msg;
    msg << "expected a date in the YYYYMMDD format, found '" << val
        << "' instead. (Integer out of range).";
    throw ParserException(msg.str(), csv.getFieldName(field), csv.getCurLine());
  } catch (const std::invalid_argument& e) {
    std::stringstream msg;
    msg << "expected a date in the YYYYMMDD format, found '" << val
        << "' instead.";
    throw ParserException(msg.str(), csv.getFieldName(field), csv.getCurLine());
  }
}

// ____________________________________________________________________________
Time Parser::getTime(const CsvParser& csv, size_t field) const {
  size_t p;
  const char* val = csv.getTString(field);

  // TODO(patrick): null value
  if (val[0] == 0) return Time(0, 0, 0);

  try {
    uint64_t h = std::stoul(val, &p, 10);
    if (h > 255)
      throw std::out_of_range(
          "only hour-values up to 255 are "
          "supported. (read " +
          std::to_string(h) + ")");
    val += p + 1;

    uint64_t m = std::stoul(val, &p, 10);
    if (p == 1) {
      if (_strict) {
        throw std::invalid_argument("one-digit minute values are not allowed.");
      }
    }
    // allow values of 60, although standard forbids it
    if (m > 60)
      throw std::out_of_range(
          "only minute-values up to 60 are "
          "allowed. (read " +
          std::to_string(m) + ")");
    val += p + 1;

    uint64_t s = std::stoul(val, &p, 10);
    if (p == 0) s = 0;  // support HH:MM format (although standard forbids it)
    if (p == 1) {
      if (_strict) {
        throw std::invalid_argument("one-digit second values are not allowed.");
      }
    }
    // allow values of 60, although standard forbids it
    if (s > 60)
      throw std::out_of_range(
          "only second-values up to 60 are "
          "allowed. (read " +
          std::to_string(s) + ")");

    return Time(h, m % 60, s % 60);
  } catch (const std::exception& e) {
    std::stringstream msg;
    msg << "expected a time in HH:MM:SS (or H:MM:SS) format, found '" << val
        << "' instead. (" << e.what() << ")";
    throw ParserException(msg.str(), csv.getFieldName(field), csv.getCurLine());
  }
}

// ____________________________________________________________________________
Route::TYPE Parser::getRouteType(const CsvParser& csv, size_t field,
                                 int64_t t) const {
  switch (t) {
    case 2:
    case 100:
    case 101:
    case 102:
    case 103:
    case 104:
    case 105:
    case 106:
    case 107:
    case 108:
    case 109:
    case 110:
    case 111:
    case 112:
    case 113:
    case 114:
    case 115:
    case 117:
    case 300:
    case 400:
    case 403:
    case 404:
    case 405:
      return Route::TYPE::RAIL;
    case 3:
    case 200:
    case 201:
    case 202:
    case 203:
    case 204:
    case 205:
    case 206:
    case 207:
    case 208:
    case 209:
    case 700:
    case 701:
    case 702:
    case 703:
    case 704:
    case 705:
    case 706:
    case 707:
    case 708:
    case 709:
    case 710:
    case 711:
    case 712:
    case 713:
    case 716:
    case 800:
      return Route::TYPE::BUS;
    case 1:
    case 401:
    case 402:
    case 500:
    case 600:
      return Route::TYPE::SUBWAY;
    case 0:
    case 900:
    case 901:
    case 902:
    case 903:
    case 904:
    case 905:
    case 906:
      return Route::TYPE::TRAM;
    // TODO(patrick): from here on not complete!
    case 4:
    case 1000:
      return Route::TYPE::FERRY;
    case 6:
    case 1300:
      return Route::TYPE::GONDOLA;
    case 7:
    case 116:
    case 1400:
      return Route::TYPE::FUNICULAR;
    case 5:
    case 1500:
      return Route::TYPE::CABLE_CAR;
    default:
      std::stringstream msg;
      msg << "route type '" << t << " not supported.";
      throw ParserException(msg.str(), csv.getFieldName(field),
                            csv.getCurLine());
  }
}
