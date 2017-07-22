// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#include <string>
#include <unordered_map>
#include <utility>
#include "Feed.h"

using ad::cppgtfs::gtfs::Feed;
using ad::cppgtfs::gtfs::Agencies;
using ad::cppgtfs::gtfs::Stops;
using ad::cppgtfs::gtfs::Trips;
using ad::cppgtfs::gtfs::Routes;
using ad::cppgtfs::gtfs::Shapes;
using ad::cppgtfs::gtfs::Services;

// ____________________________________________________________________________
const Agencies& Feed::getAgencies() const { return _agencies; }

// ____________________________________________________________________________
Agencies& Feed::getAgencies() { return _agencies; }

// ____________________________________________________________________________
const Stops& Feed::getStops() const { return _stops; }

// ____________________________________________________________________________
Stops& Feed::getStops() { return _stops; }

// ____________________________________________________________________________
const Routes& Feed::getRoutes() const { return _routes; }

// ____________________________________________________________________________
Routes& Feed::getRoutes() { return _routes; }

// ____________________________________________________________________________
const Trips& Feed::getTrips() const { return _trips; }

// ____________________________________________________________________________
Trips& Feed::getTrips() { return _trips; }

// ____________________________________________________________________________
const Shapes& Feed::getShapes() const { return _shapes; }

// ____________________________________________________________________________
Shapes& Feed::getShapes() { return _shapes; }

// ____________________________________________________________________________
const Services& Feed::getServices() const { return _services; }

// ____________________________________________________________________________
Services& Feed::getServices() { return _services; }
