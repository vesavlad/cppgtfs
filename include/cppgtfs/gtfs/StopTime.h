// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef CPPGTFS_GTFS_STOPTIME_H
#define CPPGTFS_GTFS_STOPTIME_H

#include <cppgtfs/gtfs/Stop.h>
#include <cppgtfs/gtfs/flat/StopTime.h>

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace ad
{
    namespace cppgtfs
    {
        namespace gtfs
        {

            using Time = flat::Time;

            template<typename StopT>
            class StopTime
            {
            public:
                using PU_DO_TYPE = flat::StopTime::PU_DO_TYPE;

                StopTime(const Time &at, const Time &dt, typename StopT::Ref s, uint32_t seq,
                         const std::string &hs, PU_DO_TYPE put, PU_DO_TYPE dot,
                         float distTrav, bool isTp)
                        : _at(at),
                          _dt(dt),
                          _s(s),
                          _sequence(seq),
                          _headsign(hs),
                          _pickupType(put),
                          _dropOffType(dot),
                          _isTimepoint(isTp),
                          _shapeDistTravelled(distTrav) {}

                const Time &getArrivalTime() const { return _at; }

                const Time &getDepartureTime() const { return _dt; }

                const typename StopT::Ref getStop() const { return _s; }

                typename StopT::Ref getStop() { return _s; }

                const std::string &getHeadsign() const { return _headsign; }

                PU_DO_TYPE getPickupType() const
                {
                    return static_cast<PU_DO_TYPE>(_pickupType);
                }

                PU_DO_TYPE getDropOffType() const
                {
                    return static_cast<PU_DO_TYPE>(_dropOffType);
                }

                float getShapeDistanceTravelled() const { return _shapeDistTravelled; }

                void setShapeDistanceTravelled(float d) { _shapeDistTravelled = d; }

                bool isTimepoint() const { return _isTimepoint; }

                uint16_t getSeq() const { return _sequence; }

            private:
                Time _at;
                Time _dt;

                typename StopT::Ref _s;
                uint32_t _sequence;
                std::string _headsign;
                uint8_t _pickupType: 2;
                uint8_t _dropOffType: 2;
                bool _isTimepoint: 1;
                float _shapeDistTravelled;
            };

            template<typename StopTimeT>
            struct StopTimeCompare
            {
                bool operator()(const StopTimeT &lh, const StopTimeT &rh) const
                {
                    return lh.getSeq() < rh.getSeq();
                }
            };

        }  // namespace gtfs
    }  // namespace cppgtfs
}  // namespace ad

#endif // CPPGTFS_GTFS_STOPTIME_H
