// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef CPPGTFS_GTFS_TRANSFER_H
#define CPPGTFS_GTFS_TRANSFER_H

#include <cppgtfs/gtfs/Stop.h>
#include <cppgtfs/gtfs/flat/Transfer.h>

#include <string>


namespace ad::cppgtfs::gtfs
{

    class Transfer
    {
    public:
        using TYPE = flat::Transfer::TYPE;

        Transfer() = default;

        Transfer(Stop *fromStop, Stop *toStop, TYPE type, int32_t tTime)
                : _fromStop(fromStop), _toStop(toStop), _type(type), _tTime(tTime) {}

        Stop *getFromStop() const { return _fromStop; }

        Stop *getToStop() const { return _toStop; }

        TYPE getType() const { return _type; }

        int32_t getMinTransferTime() const { return _tTime; }

        flat::Transfer getFlat() const
        {
            return flat::Transfer{getFromStop()->getId(), getToStop()->getId(),
                                  getType(), getMinTransferTime()};
        }

        // TODO(patrick): implement setters

    private:
        Stop *_fromStop{};
        Stop *_toStop{};
        TYPE _type;
        int32_t _tTime{};
    };

}  // namespace ad

#endif // CPPGTFS_GTFS_TRANSFER_H
