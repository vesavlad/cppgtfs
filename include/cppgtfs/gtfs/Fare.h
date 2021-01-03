// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef CPPGTFS_GTFS_FARE_H
#define CPPGTFS_GTFS_FARE_H

#include <cppgtfs/gtfs/Agency.h>
#include <cppgtfs/gtfs/Route.h>
#include <cppgtfs/gtfs/flat/Fare.h>

#include <string>
#include <utility>
#include <vector>

namespace ad::cppgtfs::gtfs
{

    template<typename RouteT>
    class FareRule
    {
    public:
        FareRule() = default;

        FareRule(typename RouteT::Ref route, std::string originId,
                 std::string destId, std::string containsId)
                : _route(route),
                  _originId(std::move(originId)),
                  _destId(std::move(destId)),
                  _containsId(std::move(containsId)) {}

        typename RouteT::Ref getRoute() const { return _route; }

        const std::string &getOriginId() const { return _originId; }

        const std::string &getDestId() const { return _destId; }

        const std::string &getContainsId() const { return _containsId; }


    private:
        typename RouteT::Ref _route;
        std::string _originId;
        std::string _destId;
        std::string _containsId;
    };

    template<typename RouteT>
    class Fare
    {
    public:
        using Ref = Fare<RouteT> *;

        static std::string getId(Ref r) { return r->getId(); }

        using PAYMENT_METHOD = flat::Fare::PAYMENT_METHOD;
        using NUM_TRANSFERS = flat::Fare::NUM_TRANSFERS;

        Fare() = default;

        Fare(std::string id, double price, std::string currencyType,
             PAYMENT_METHOD paymentMethod, NUM_TRANSFERS numTransfers, Agency *agency,
             int64_t dur)
                : _id(std::move(id)),
                  _price(price),
                  _currencyType(std::move(currencyType)),
                  _paymentMethod(paymentMethod),
                  _numTransfers(numTransfers),
                  _agency(agency),
                  _duration(dur) {}

        const std::string &getId() const { return _id; }

        double getPrice() const { return _price; }

        const std::string &getCurrencyType() const { return _currencyType; }

        PAYMENT_METHOD getPaymentMethod() const { return _paymentMethod; }

        NUM_TRANSFERS getNumTransfers() const { return _numTransfers; }

        Agency *getAgency() const { return _agency; }

        int64_t getDuration() const { return _duration; }

        const std::vector<FareRule<RouteT>> &getFareRules() const
        {
            return _fareRules;
        }

        void addFareRule(const FareRule<RouteT> &rule) { _fareRules.push_back(rule); }

        flat::Fare getFlat() const
        {
            return flat::Fare{_id, _price,
                              _currencyType, _paymentMethod,
                              _numTransfers, _agency ? _agency->getId() : "",
                              _duration};
        }

        // TODO(patrick): implement setters

    private:
        std::string _id;
        double _price{};
        std::string _currencyType;
        PAYMENT_METHOD _paymentMethod;
        NUM_TRANSFERS _numTransfers;
        Agency *_agency{};
        int64_t _duration{};

        std::vector<FareRule<RouteT>> _fareRules;
    };

}  // namespace ad

#endif // CPPGTFS_GTFS_FARE_H
