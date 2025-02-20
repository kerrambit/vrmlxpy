#pragma once

#include <string>
#include <memory>

#include <boost/iostreams/device/mapped_file.hpp>

#include "FileReader.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc::core::io {

	struct MemoryMappedFile {

		MemoryMappedFile(const std::string& filepath)
			: m_mappedFile(filepath, boost::iostreams::mapped_file::readonly) {}

		const char* GetBegin() const { return m_mappedFile.const_data(); }
		const char* GetEnd() const { return m_mappedFile.const_data() + m_mappedFile.size(); }
		size_t GetSize() const { return m_mappedFile.size(); }

	private:
		boost::iostreams::mapped_file m_mappedFile;
	};

	class VRMLPROCESSING_API MemoryMappedFileReader : public FileReader<MemoryMappedFile> {
	public:
		LoadFileResult Read(const std::filesystem::path& filepath) override;
	};
}
