#include "Logger.hpp"

#include <ios>
#include <sstream>
#include <vector>
#include <string>

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
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

#include "FormatString.hpp"

// --------------------------------------------------------------------------------- //

namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace sinks = boost::log::sinks;

struct BufferedMessage {
  std::string text;
  vrml_proc::core::logger::Level level;
  std::string file;
  int line;
  std::string function;

  BufferedMessage(std::string text, vrml_proc::core::logger::Level level, std::string file, int line,
                  std::string function)
      : text(text), level(level), file(file), line(line), function(function) {}
};

static std::vector<BufferedMessage> g_logBuffer;
static bool g_loggingInitialized = false;

BOOST_LOG_GLOBAL_LOGGER_INIT(Logger, boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>) {
  return boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>();
}

void LogBufferedMessages();

void vrml_proc::core::logger::InitLogging() {
  g_loggingInitialized = true;

  logging::add_file_log(
      keywords::file_name = "vrmlproc_%Y-%m-%d.log", keywords::open_mode = std::ios_base::app,
      keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), keywords::auto_flush = true,
      keywords::format = expr::stream << "["
                                      << expr::format_date_time<boost::posix_time::ptime>(
                                             expr::attr<boost::posix_time::ptime>("TimeStamp"), "%Y-%m-%d %H:%M:%S")
                                      << "]"
                                      << " [" << expr::attr<boost::log::trivial::severity_level>("Severity") << "] "
                                      << expr::smessage);

  logging::add_common_attributes();

#ifdef RELEASE
  logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
#endif

#ifdef DEBUG
  logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::trace);
#endif

  LogBufferedMessages();
}

void vrml_proc::core::logger::InitLogging(const std::string& loggingDirectory, const std::string& projectName) {
  g_loggingInitialized = true;

  if (!boost::filesystem::exists(loggingDirectory)) {
    boost::filesystem::create_directories(loggingDirectory);
  }

  logging::add_file_log(
      keywords::file_name = loggingDirectory + "/" + projectName + "_%Y-%m-%d.log",
      keywords::open_mode = std::ios_base::app,
      keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), keywords::auto_flush = true,
      keywords::format = expr::stream << "["
                                      << expr::format_date_time<boost::posix_time::ptime>(
                                             expr::attr<boost::posix_time::ptime>("TimeStamp"), "%Y-%m-%d %H:%M:%S")
                                      << "]"
                                      << " [" << expr::attr<boost::log::trivial::severity_level>("Severity") << "] "
                                      << expr::smessage);

  logging::add_common_attributes();

#ifdef RELEASE
  logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
#endif

#ifdef DEBUG
  logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::trace);
#endif

  LogBufferedMessages();
}

void vrml_proc::core::logger::LogUnformattedText(const std::string& title, const std::string& text,
                                                 vrml_proc::core::logger::Level level, const std::string& file,
                                                 int line, const std::string& function) {
  if (!g_loggingInitialized) {
    g_logBuffer.push_back(
        BufferedMessage(vrml_proc::core::utils::FormatString(title, ":\n", text), level, file, line, function));
    return;
  }

  switch (level) {
    case vrml_proc::core::logger::Level::Trace:
      BOOST_LOG_SEV(Logger::get(), boost::log::trivial::trace)
          << "[" << file << ":" << line << "] [" << function << "]: " << title << ":\n"
          << text;
      break;
    case vrml_proc::core::logger::Level::Debug:
      BOOST_LOG_SEV(Logger::get(), boost::log::trivial::debug) << title << ":\n" << text;
      break;
    case vrml_proc::core::logger::Level::Info:
      BOOST_LOG_SEV(Logger::get(), boost::log::trivial::info) << title << ":\n" << text;
      break;
    case vrml_proc::core::logger::Level::Warning:
      BOOST_LOG_SEV(Logger::get(), boost::log::trivial::warning) << title << ":\n" << text;
      break;
    case vrml_proc::core::logger::Level::Error:
      BOOST_LOG_SEV(Logger::get(), boost::log::trivial::error)
          << "[" << file << ":" << line << "] [" << function << "]: " << title << ":\n"
          << text;
      break;
    case vrml_proc::core::logger::Level::Fatal:
      BOOST_LOG_SEV(Logger::get(), boost::log::trivial::fatal) << title << ":\n" << text;
      break;
    default:
      break;
  }
}

void vrml_proc::core::logger::Log(const std::string& text, Level level, const std::string& file, int line,
                                  const std::string& function) {
  if (!g_loggingInitialized) {
    g_logBuffer.push_back(BufferedMessage(text, level, file, line, function));
    return;
  }

  switch (level) {
    case vrml_proc::core::logger::Level::Trace:
      BOOST_LOG_SEV(Logger::get(), boost::log::trivial::trace)
          << "[" << file << ":" << line << "] [" << function << "]: " << text;
      break;
    case vrml_proc::core::logger::Level::Debug:
      BOOST_LOG_SEV(Logger::get(), boost::log::trivial::debug)
          << "[" << file << ":" << line << "] [" << function << "]: " << text;
      break;
    case vrml_proc::core::logger::Level::Info:
      BOOST_LOG_SEV(Logger::get(), boost::log::trivial::info)
          << "[" << file << ":" << line << "] [" << function << "]: " << text;
      break;
    case vrml_proc::core::logger::Level::Warning:
      BOOST_LOG_SEV(Logger::get(), boost::log::trivial::warning)
          << "[" << file << ":" << line << "] [" << function << "]: " << text;
      break;
    case vrml_proc::core::logger::Level::Error:
      BOOST_LOG_SEV(Logger::get(), boost::log::trivial::error)
          << "[" << file << ":" << line << "] [" << function << "]: " << text;
      break;
    case vrml_proc::core::logger::Level::Fatal:
      BOOST_LOG_SEV(Logger::get(), boost::log::trivial::fatal)
          << "[" << file << ":" << line << "] [" << function << "]: " << text;
      break;
    default:
      break;
  }
}

void vrml_proc::core::logger::LogTrace(const std::string& text, const std::string& file, int line,
                                       const std::string& function) {
  vrml_proc::core::logger::Log(text, vrml_proc::core::logger::Level::Trace, file, line, function);
}

void vrml_proc::core::logger::LogDebug(const std::string& text, const std::string& file, int line,
                                       const std::string& function) {
  vrml_proc::core::logger::Log(text, vrml_proc::core::logger::Level::Debug, file, line, function);
}

void vrml_proc::core::logger::LogInfo(const std::string& text, const std::string& file, int line,
                                      const std::string& function) {
  vrml_proc::core::logger::Log(text, vrml_proc::core::logger::Level::Info, file, line, function);
}

void vrml_proc::core::logger::LogWarning(const std::string& text, const std::string& file, int line,
                                         const std::string& function) {
  vrml_proc::core::logger::Log(text, vrml_proc::core::logger::Level::Warning, file, line, function);
}

void vrml_proc::core::logger::LogError(const std::string& text, const std::string& file, int line,
                                       const std::string& function) {
  vrml_proc::core::logger::Log(text, vrml_proc::core::logger::Level::Error, file, line, function);
}

void vrml_proc::core::logger::LogFatal(const std::string& text, const std::string& file, int line,
                                       const std::string& function) {
  vrml_proc::core::logger::Log(text, vrml_proc::core::logger::Level::Fatal, file, line, function);
}

void LogBufferedMessages() {
  for (const auto& message : g_logBuffer) {
    vrml_proc::core::logger::Log(message.text, message.level, message.file, message.line, message.function);
  }
  g_logBuffer.clear();
}