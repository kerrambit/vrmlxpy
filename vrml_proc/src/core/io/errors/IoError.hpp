#pragma once

#include "Error.hpp"

namespace vrml_proc::core::io::error {

    class IoError : public vrml_proc::core::error::Error {
    protected:
        virtual std::string GetMessageInternal() const {
            return "[IoError]";
        }
    };

    class EmptyFilePathError : public IoError {
    public:
        EmptyFilePathError(const std::string& filepath) : m_filepath(filepath) {}
    protected:
        std::string GetMessageInternal() const override {
            std::ostringstream oss;
            oss << IoError::GetMessageInternal() << "[EmptyFilePathError]: filepath <" << m_filepath << "> is empty and invalid!\n";
            return oss.str();
        }
    private:
        std::string m_filepath;
    };

    class DirectoryNotFoundError : public IoError {
    public:
        DirectoryNotFoundError(const std::string& directoryPath) : m_directoryPath(directoryPath) {}
    protected:
        std::string GetMessageInternal() const override {
            std::ostringstream oss;
            oss << IoError::GetMessageInternal() << "[DirectoryNotFoundError]: directory <" << m_directoryPath << "> was not found!\n";
            return oss.str();
        }
    private:
        std::string m_directoryPath;
    };

    class GeneralWriteError : public IoError {
    public:
        GeneralWriteError(const std::string& filepath) : m_filepath(filepath) {}
    protected:
        std::string GetMessageInternal() const override {
            std::ostringstream oss;
            oss << IoError::GetMessageInternal() << "[GeneralWriteError]: when writing data into <" << m_filepath << ">, an unknown error occured!\n";
            return oss.str();
        }
    private:
        std::string m_filepath;
    };

}
