PROJECT(darner)

SET(DARNER_VERSION 0.2.0)

SET(DARNER_DB ROCKSDB)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

CMAKE_POLICY(SET CMP0005 NEW) # don't complain about escaping the double-quotes below around DARNER_VERSION

ADD_DEFINITIONS(-Wall -Wno-return-type -O2 -std=c++11 -g -DNDEBUG -DDARNER_VERSION="${DARNER_VERSION}")

IF(DARNER_DB)
    ADD_DEFINITIONS(-D${DARNER_DB})
    IF(DARNER_DB STREQUAL "ROCKSDB")
        SET(DB_LIBRARIES rocksdb snappy)
    ENDIF(DARNER_DB STREQUAL "ROCKSDB")
ENDIF(DARNER_DB)

INCLUDE_DIRECTORIES(include)

IF(NOT DB_LIBRARIES)
    SET(DB_LIBRARIES leveldb snappy)
ENDIF(NOT DB_LIBRARIES)


ADD_EXECUTABLE(darner
               src/net/handler
               src/net/request
               src/util/log
               src/queue/iqstream
               src/queue/oqstream
               src/queue/queue
               src/main
               )

ADD_EXECUTABLE(db
               bench/db
               )

ADD_EXECUTABLE(test
               src/net/request
               src/queue/iqstream
               src/queue/oqstream
               src/queue/queue
               src/util/log
               tests/queue
               tests/request
               tests/main
               )

FIND_PACKAGE(Boost 1.40.0 COMPONENTS thread system program_options unit_test_framework filesystem)

TARGET_LINK_LIBRARIES(darner pthread jemalloc ${Boost_LIBRARIES} ${DB_LIBRARIES})
TARGET_LINK_LIBRARIES(db ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(test pthread jemalloc ${Boost_LIBRARIES} ${DB_LIBRARIES})

INSTALL(TARGETS darner DESTINATION bin)
