// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef CPPGTFS_GTFS_NULLCONTAINER_H
#define CPPGTFS_GTFS_NULLCONTAINER_H

#include <string>

namespace ad
{
    namespace cppgtfs
    {
        namespace gtfs
        {

            template<typename T>
            class NullContainer
            {
            public:
                NullContainer() {}

                std::string add(const T &obj) const { return obj.getId(); }

                T *get(const std::string &id) const
                {
                    do { (void) (id); } while (0);
                    return nullptr;
                }

                std::string getRef(const std::string &id) const { return id; }

                void finalize() {};
            };

        }  // namespace gtfs
    }  // namespace cppgtfs
}  // namespace ad

#endif // CPPGTFS_GTFS_NULLCONTAINER_H
