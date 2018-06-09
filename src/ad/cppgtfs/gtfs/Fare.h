// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_CPPGTFS_GTFS_FARE_H_
#define AD_CPPGTFS_GTFS_FARE_H_

#include <string>
#include <vector>
#include "Agency.h"
#include "Route.h"

using std::exception;
using std::string;

namespace ad {
namespace cppgtfs {
namespace gtfs {

class FareRule {
 public:
  FareRule() {}

  FareRule(gtfs::Route* route, const std::string& originId,
           const std::string& destId, const std::string& containsId)
      : _route(route),
        _originId(originId),
        _destId(destId),
        _containsId(containsId) {}

  gtfs::Route* getRoute() const { return _route; }
  const std::string& getOriginId() const { return _originId; }
  const std::string& getDestId() const { return _destId; }
  const std::string& getContainsId() const { return _containsId; }

 private:
  gtfs::Route* _route;
  std::string _originId;
  std::string _destId;
  std::string _containsId;
};

class Fare {
 public:
  enum PAYMENT_METHOD : bool { ON_BOARD = 0, BEFORE_BOARDING = 1 };

  enum NUM_TRANSFERS : uint8_t {
    NO_TRANSFERS = 0,
    ONCE = 1,
    TWICE = 2,
    UNLIMITED = 3
  };

  Fare() {}

  Fare(const std::string& id, double price, const std::string& currencyType,
       PAYMENT_METHOD paymentMethod, NUM_TRANSFERS numTransfers, Agency* agency,
       int64_t dur)
      : _id(id),
        _price(price),
        _currencyType(currencyType),
        _paymentMethod(paymentMethod),
        _numTransfers(numTransfers),
        _agency(agency),
        _duration(dur) {}

  const std::string& getId() const { return _id; }

  double getPrice() const { return _price; }

  const std::string& getCurrencyType() const { return _currencyType; }

  PAYMENT_METHOD getPaymentMethod() const { return _paymentMethod; }

  NUM_TRANSFERS getNumTransfers() const { return _numTransfers; }

  Agency* getAgency() const { return _agency; }

  int64_t getDuration() const { return _duration; }

  const std::vector<FareRule>& getFareRules() const { return _fareRules; }

  void addFareRule(const FareRule& rule) { _fareRules.push_back(rule); }

  // TODO(patrick): implement setters

 private:
  std::string _id;
  double _price;
  std::string _currencyType;
  PAYMENT_METHOD _paymentMethod;
  NUM_TRANSFERS _numTransfers;
  Agency* _agency;
  int64_t _duration;

  std::vector<FareRule> _fareRules;
};

}  // namespace gtfs
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_GTFS_FARE_H_
