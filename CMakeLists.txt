PROJECT(darner)

SET(DARNER_VERSION 0.2.0)

SET(DARNER_DB ROCKSDB)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

CMAKE_POLICY(SET CMP0005 NEW) # don't complain about escaping the double-quotes below around DARNER_VERSION

#May be needed on Mac OS -stdlib=libstdc++
SET(OPT_FLAGS "-O2 -march=core2 -mtune=core2 -fno-omit-frame-pointer -momit-leaf-frame-pointer -fno-builtin-memcmp")
ADD_DEFINITIONS(-Wall -Wno-return-type -std=c++11 ${OPT_FLAGS} -g -DNDEBUG -DDARNER_VERSION="${DARNER_VERSION}" -DBOOST_ALL_NO_LIB -DBOOST_SYSTEM_NO_LIB)

IF(DARNER_DB)
    ADD_DEFINITIONS(-D${DARNER_DB})
    IF(DARNER_DB STREQUAL "ROCKSDB")
        SET(DB_LIBRARIES rocksdb.a snappy.a)
    ENDIF(DARNER_DB STREQUAL "ROCKSDB")
ENDIF(DARNER_DB)

INCLUDE_DIRECTORIES(include)

IF(NOT DB_LIBRARIES)
    SET(DB_LIBRARIES leveldb.a snappy.a)
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

#To set Boost_UNIT_TEST_FRAMEWORK_LIBRARY for test
FIND_PACKAGE(Boost 1.40.0 COMPONENTS unit_test_framework)

#Static link prebuilt boost libs
SET(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost 1.40.0 COMPONENTS thread system program_options filesystem)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(darner jemalloc ${Boost_LIBRARIES} ${DB_LIBRARIES})
TARGET_LINK_LIBRARIES(db ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(test jemalloc ${Boost_LIBRARIES} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${DB_LIBRARIES})

INSTALL(TARGETS darner DESTINATION bin)
MESSAGE(STATUS "Use 'make install/strip' when installing to strip all debug info")