#include "Logger.hpp"

#include <ios>

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/attr.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/expressions/formatters/stream.hpp>
#include <boost/log/expressions/message.hpp>
#include <boost/log/keywords/auto_flush.hpp>
#include <boost/log/keywords/file_name.hpp>
#include <boost/log/keywords/format.hpp>
#include <boost/log/keywords/open_mode.hpp>
#include <boost/log/keywords/time_based_rotation.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace sinks = boost::log::sinks;

BOOST_LOG_GLOBAL_LOGGER_INIT(Logger, boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>) {
    return boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>();
}

void vrml_proc::core::logger::InitLogging() {

    logging::add_file_log(
        keywords::file_name = "vrmlproc_%Y-%m-%d.log",
        keywords::open_mode = std::ios_base::app,
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
        keywords::auto_flush = true,
        keywords::format = expr::stream
        << "[" << expr::format_date_time<boost::posix_time::ptime>(expr::attr<boost::posix_time::ptime>("TimeStamp"), "%Y-%m-%d %H:%M:%S") << "]"
        << " [" << expr::attr<boost::log::trivial::severity_level>("Severity") << "] "
        << expr::smessage
    );

    logging::add_common_attributes();

#ifdef RELEASE
    logging::core::get()->set_filter(
        logging::trivial::severity >= logging::trivial::info
    );
#endif

#ifdef DEBUG
    logging::core::get()->set_filter(
        logging::trivial::severity >= logging::trivial::trace
    );
#endif
}
