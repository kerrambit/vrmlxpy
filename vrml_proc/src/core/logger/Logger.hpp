#pragma once

#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>

#include "VrmlProcessingExport.hpp"

BOOST_LOG_GLOBAL_LOGGER(Logger, boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>)

#define LOG_TRACE() \
    BOOST_LOG_SEV(Logger::get(), boost::log::trivial::trace) \
        << "[" << __FILE__ << ":" << __LINE__ << "] [" << __FUNCTION__ << "]: "

#define LOG_DEBUG() \
    BOOST_LOG_SEV(Logger::get(), boost::log::trivial::debug) \
        << "[" << __FILE__ << ":" << __LINE__ << "] [" << __FUNCTION__ << "]: "

#define LOG_INFO() \
    BOOST_LOG_SEV(Logger::get(), boost::log::trivial::info) \
        << "[" << __FILE__ << ":" << __LINE__ << "] [" << __FUNCTION__ << "]: "

#define LOG_WARNING() \
    BOOST_LOG_SEV(Logger::get(), boost::log::trivial::warning) \
        << "[" << __FILE__ << ":" << __LINE__ << "] [" << __FUNCTION__ << "]: "

#define LOG_ERROR() \
    BOOST_LOG_SEV(Logger::get(), boost::log::trivial::error) \
        << "[" << __FILE__ << ":" << __LINE__ << "] [" << __FUNCTION__ << "]: "

#define LOG_FATAL() \
    BOOST_LOG_SEV(Logger::get(), boost::log::trivial::fatal) \
        << "[" << __FILE__ << ":" << __LINE__ << "] [" << __FUNCTION__ << "]: "

namespace vrml_proc {
    namespace core {
        namespace logger {
            VRMLPROCESSING_API void InitLogging();
        }
    }
}