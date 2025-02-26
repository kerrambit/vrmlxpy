#pragma once

#include <algorithm>
#include <filesystem>
#include <future>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <result.hpp>

#include <BoxAction.hpp>
#include <BufferView.hpp>
#include <CalculatorResult.hpp>
#include <ConversionContextActionMap.hpp>
#include <FormatString.hpp>
#include <JsonFileReader.hpp>
#include <Logger.hpp>
#include <MemoryMappedFileReader.hpp>
#include <Mesh.hpp>
#include <MeshTaskConversionContext.hpp>
#include <ParserResult.hpp>
#include <StlActionMap.hpp>
#include <StlFileWriter.hpp>
#include <VrmlFile.hpp>
#include <VrmlFileTraversor.hpp>
#include <VrmlNodeManager.hpp>
#include <VrmlParser.hpp>

/**
 * @brief Reads error and logs the error into logging file.
 * @param error error to log
 */
static void HandleRootLevelError(std::shared_ptr<vrml_proc::core::error::Error> error) {
    vrml_proc::core::logger::LogUnformattedText("caught application error", error->GetMessage(), vrml_proc::core::logger::Level::Error, LOGGING_INFO);
}

/**
 * @brief Reads error message and logs it into logging file.
 * @param error error to log
 */
static void HandleRootLevelError(const std::string& error) {
    vrml_proc::core::logger::LogUnformattedText("caught application error", error, vrml_proc::core::logger::Level::Error, LOGGING_INFO);
}

/**
 * @brief Reads result and checks if it contains error, if yes, the error is logged.
 * @tparam T parameter of value inside result
 * @param result result to check
 */
template<typename T>
static void HandleRootLevelError(const cpp::result<std::shared_ptr<T>, std::shared_ptr<vrml_proc::core::error::Error>>& result) {
    if (result.has_error()) {
        HandleRootLevelError(result.error());
    }
}

/**
 * @brief Contains all data loaded from test configuration file.
 */
struct TestInfo {
    std::string baseInputPath;
    std::string baseOutputPath;
    std::string baseExpectedOutputPath;
    std::map<std::string, std::string> testFiles;
};

/**
 * @brief Static global variable holding the test information.
 */
static TestInfo g_testInfo;

/**
 * @brief Initialize testing.
 * @param filepath Path to test configuration file.
 * @return true if file was loaded succesfully, else false
 */
static bool InitTesting(const std::filesystem::path& filepath) {

    vrml_proc::core::io::JsonFileReader reader;
    auto json = reader.Read(std::filesystem::path(filepath));

    if (json.has_value()) {
        g_testInfo.baseInputPath = (json.value())["baseInputPath"];
        g_testInfo.baseOutputPath = (json.value())["baseOutputPath"];
        g_testInfo.baseExpectedOutputPath = (json.value())["baseExpectedOutputPath"];

        for (auto& element : (json.value())["testFiles"].items()) {
            g_testInfo.testFiles[element.key()] = element.value();
        }
        return true;
    }
    else {
        HandleRootLevelError(json.error());
        return false;
    }
}

/**
 * @brief Retrieves read-only reference to global test information structure.
 * @return  test information structure
 */
static const TestInfo& ReadTestInfo() {
    return g_testInfo;
}

/**
* @brief Generates file (with .stl extension) name based on test case name.
*/
#define GENERATE_TEST_OUTPUT_FILENAME(filepath) \
    std::string testName = Catch::getResultCapture().getCurrentTestName(); \
    std::replace(testName.begin(), testName.end(), ' ', '_'); \
    std::filesystem::path filepath = std::filesystem::path((testName + ".stl"))

/**
 * @brief Checks if the most inner error is equal to expected type.
 * @tparam ExpectedErrorType expected type
 * @param error error to check
 * @return true if the comparison was successfull, else false
 */
template <typename ExpectedErrorType>
static bool CheckInnermostError(std::shared_ptr<vrml_proc::core::error::Error> error) {
    return std::dynamic_pointer_cast<ExpectedErrorType>(vrml_proc::core::error::Error::GetInnermostError(error)) != nullptr;
}

/**
 * @brief Retrieves the most inner error.
 * @tparam WantedErrorType type of error to retrieve
 * @param error error to get inner error from
 * @return shared pointer owning the inner error
 * 
 * @warning nullptr can be returned if WantedErrorType is not the most inner error!
 */
template <typename WantedErrorType>
static std::shared_ptr<WantedErrorType> GetInnermostError(std::shared_ptr<vrml_proc::core::error::Error> error) {
    return std::dynamic_pointer_cast<WantedErrorType>(vrml_proc::core::error::Error::GetInnermostError(error));
}

/**
 * @brief Checks if two files have similiar size in bytes.
 * @param filepath1 first path to check
 * @param filepath2 second path to check
 * @param toleranceInBytes ± tolerance between sizes in bytes 
 * @return true if the sizes are similiar else false
 */
bool HaveSimiliarSizes(const std::filesystem::path& filepath1, const std::filesystem::path& filepath2, std::size_t toleranceInBytes) {
    auto size1 = std::filesystem::file_size(filepath1);
    auto size2 = std::filesystem::file_size(filepath2);
    bool result = std::abs(static_cast<std::ptrdiff_t>(size1) - static_cast<std::ptrdiff_t>(size2)) <= static_cast<std::ptrdiff_t>(toleranceInBytes);
    if (!result) {
        HandleRootLevelError(vrml_proc::core::utils::FormatString("Filesizes are not in the tolerance: ", size1, " B != ", size2, " B (tolerance is +-", toleranceInBytes, " B)."));
    }
    return result;
}

/**
 * @brief Parses VRML text.
 * @param text text to parse
 * @param manager VRML node manager
 * @return result containing VRMLFile object or error
 */
static vrml_proc::parser::ParserResult<vrml_proc::parser::VrmlFile> ParseVrmlFile(std::string& text, vrml_proc::parser::VrmlNodeManager& manager) {

    vrml_proc::parser::VrmlParser parser(manager);
    return parser.Parse(vrml_proc::parser::BufferView(text.c_str(), text.c_str() + text.size()));
}

/**
 * @brief Parses VRML text.
 * @param filepath path to file, which will be read and parsed
 * @param manager VRML node manager
 * @return result containing VRMLFile object or error
 */
static vrml_proc::parser::ParserResult<vrml_proc::parser::VrmlFile> ParseVrmlFile(const std::filesystem::path& filepath, vrml_proc::parser::VrmlNodeManager& manager) {

    vrml_proc::core::io::MemoryMappedFileReader reader;
    auto readResult = reader.Read(filepath);
    if (readResult.has_error()) {
        HandleRootLevelError(readResult.error());
        return {};
    }

    vrml_proc::parser::VrmlParser parser(manager);
    vrml_proc::core::io::MemoryMappedFile file = readResult.value();
    return parser.Parse(vrml_proc::parser::BufferView(file.GetBegin(), file.GetEnd()));
}

/**
 * @brief Traverses the VRML tree and creates conversion context.
 * @param parseResult VRML tree
 * @param manager VRML node manager
 * @param expectedConversionContextSize traversor creates vector of individual submeshes, this is expected count of them
 * @return false if traversing fails or if the expected count is not met, otherwise true
 */
static bool TraverseVrmlFileToMeshTask(vrml_proc::parser::ParserResult<vrml_proc::parser::VrmlFile> parseResult,
                                       const vrml_proc::parser::VrmlNodeManager& manager,
                                       size_t expectedConversionContextSize) {

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, to_stl::conversion_context::CreateActionMap());
    if (traversorResult.has_error()) {
        HandleRootLevelError(traversorResult.error());
        return false;
    }
    else {
        if (traversorResult.value()->GetData().size() != expectedConversionContextSize) {
            HandleRootLevelError(vrml_proc::core::utils::FormatString("Size of traversor result is not the same as expected: ", traversorResult.value()->GetData().size(), " != ", expectedConversionContextSize, "."));
            return false;
        }
        return true;
    }
}

/**
 * @brief Traverses the VRML tree and creates conversion context. All tasks are then run in parallel, and result mesh is exported into file.
 * @param parseResult VRML tree
 * @param manager VRML node manager
 * @param expectedConversionContextSize traversor creates vector of individual submeshes, this is expected count of them
 * @param outputFilepath output file path
 * @param expectedSubmeshesCount all tasks are run, this is a number representing how many submeshes from tasks (expectedConversionContextSize) should be converted
 * @return false if traversing fails, if the expected count of taksks/submeshes is not met or if export fails, otherwise true
 */
static bool TraverseVrmlFileToMeshTask(vrml_proc::parser::ParserResult<vrml_proc::parser::VrmlFile> parseResult,
                                       const vrml_proc::parser::VrmlNodeManager& manager,
                                       size_t expectedConversionContextSize,
                                       const std::filesystem::path& outputFilepath,
                                       size_t expectedSubmeshesCount) {

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, to_stl::conversion_context::CreateActionMap());
    if (traversorResult.has_error()) {
        HandleRootLevelError(traversorResult.error());
        return false;
    }

    if (traversorResult.value()->GetData().size() != expectedConversionContextSize) {
        HandleRootLevelError(vrml_proc::core::utils::FormatString("Size of traversor result is not the same as expected: ", traversorResult.value()->GetData().size(), " != ", expectedConversionContextSize, "."));
        return false;
    }

    std::vector<std::future<to_stl::calculator::CalculatorResult>> results;
    for (const auto& task : traversorResult.value()->GetData()) {
        if (task) {
            results.emplace_back(std::async(std::launch::async, task));
        }
    }

    to_stl::core::Mesh mesh;
    size_t currentSubmesh = 0;
    for (auto& future : results) {
        auto meshResult = future.get();
        if (meshResult.has_value()) {
            mesh.join(*(meshResult.value()));
            currentSubmesh++;
        }
        else {
            HandleRootLevelError(meshResult.error());
        }
    };

    if (currentSubmesh != expectedSubmeshesCount) {
        HandleRootLevelError(vrml_proc::core::utils::FormatString("Count of submeshes is not equal to expected count: ", currentSubmesh, " != ", expectedSubmeshesCount, "."));
        return false;
    }

    to_stl::core::io::StlFileWriter writer;
    auto writeResult = writer.Write(std::filesystem::path(outputFilepath), mesh);
    if (writeResult.has_error()) {
        HandleRootLevelError(writeResult.error());
        return false;
    }

    return true;
}

/**
 * @brief Traverses the VRML tree.
 * @param parseResult VRML tree
 * @param manager VRML node manager
 * @return false if traversing fails, otherwise true
 */
static bool TraverseVrmlFileToMeshTask(vrml_proc::parser::ParserResult<vrml_proc::parser::VrmlFile> parseResult,
                                       const vrml_proc::parser::VrmlNodeManager& manager) {
    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, to_stl::conversion_context::CreateActionMap());
    if (traversorResult.has_error()) {
        HandleRootLevelError(traversorResult.error());
        return false;
    }
    else return true;
}
