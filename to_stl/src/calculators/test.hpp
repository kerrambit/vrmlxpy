#pragma once

#include <CGAL/boost/graph/IO/STL.h>
#include <CGAL/Kernel/interface_macros.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>

#include "Mesh.hpp"

bool export_to_stl(const to_stl::core::Mesh& mesh, const std::string& filename) {
  return CGAL::IO::write_STL(filename, mesh);
}

// ------------------------------------------------------------------------------------------------------------------------

#include <future>
#include <functional>
#include <memory>

#include <result.hpp>

#include "Error.hpp"
#include "IndexedFaceSetCalculator.hpp"
#include "VectorConversionContext.hpp"

using CalculatorResult =
    cpp::result<std::shared_ptr<to_stl::core::Mesh>, std::shared_ptr<vrml_proc::core::error::Error>>;

using MeshTask = std::function<CalculatorResult()>;

using MeshTaskConversionContext = vrml_proc::conversion_context::VectorConversionContext<MeshTask>;

// Different calculators accepting various arguments
CalculatorResult createMeshFromData(std::reference_wrapper<const std::vector<float>> data, int id) {
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));  // Simulate work
  std::cout << "1 Processing MeshData ID: " << id << "\n";
  return std::make_shared<to_stl::core::Mesh>();
}

CalculatorResult createMeshFromParams(int id, const std::vector<int>& params) {
  std::this_thread::sleep_for(std::chrono::milliseconds(2500));  // Simulate work
  std::cout << "2 Processing int params for Mesh ID: " << id << "\n";
  std::vector<float> vertices(params.begin(), params.end());
  return std::make_shared<to_stl::core::Mesh>();
}

CalculatorResult createMeshFromFloat(int id, float value) {
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // Simulate work
  std::cout << "3 Processing float value " << value << " for Mesh ID: " << id << "\n";
  return std::make_shared<to_stl::core::Mesh>();
}

int taskflow_test() {
  using vrml_proc::parser::Vec3f;

  auto taskVector = std::make_unique<MeshTaskConversionContext>();

  // These indices and points are global objects alive as long as the whole program.
  vrml_proc::parser::Int32Array indices;
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(1);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(-1);
  indices.integers.emplace_back(0);
  indices.integers.emplace_back(2);
  indices.integers.emplace_back(3);
  indices.integers.emplace_back(-1);
  std::reference_wrapper<const vrml_proc::parser::Int32Array> indicesRef = std::cref(indices);

  vrml_proc::parser::Vec3fArray points;
  points.vectors.emplace_back(Vec3f(0.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 0.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(1.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.0f, 1.0f, 0.0f));
  points.vectors.emplace_back(Vec3f(0.5f, 0.5f, 1.0f));

  bool isConvex = false;

  std::cout << "---------------------" << std::endl;

  auto task1 = std::make_unique<MeshTaskConversionContext>();
  {
    // Matrix is taked by a copy.
    vrml_proc::math::TransformationMatrix matrix;
    std::cout << &(indicesRef.get()) << std::endl;

    task1->Add([=]() {
      std::cout << &(indicesRef.get()) << std::endl;
      to_stl::calculator::IndexedFaceSetCalculator calculator = to_stl::calculator::IndexedFaceSetCalculator();
      return calculator.Generate3DMesh(indicesRef, points, std::cref(isConvex), matrix);
    });
  }

  taskVector->Merge(task1.get());

  {
    std::vector<float> meshInput1 = {0.1f, 0.2f, 0.3f};
    taskVector->Add([=]() { return createMeshFromData(std::cref(meshInput1), 1); });
  }

  {
    std::vector<int> params = {10, 20, 30};
    taskVector->Add([=]() { return createMeshFromParams(2, params); });
  }

  {
    float singleValue = 5.5f;
    taskVector->Add([=]() { return createMeshFromFloat(3, singleValue); });
  }

  std::vector<std::future<CalculatorResult>> results;

  for (const auto& task : taskVector->GetData()) {
    if (task) {
      results.emplace_back(std::async(std::launch::async, task));
    }
  }

  to_stl::core::Mesh mesh;
  for (auto& future : results) {
    auto meshResult = future.get();
    if (meshResult.has_value()) {
      mesh.join(*(meshResult.value()));
    }
  };

  export_to_stl(mesh, R"(C:\Users\marek\Documents\FI_MUNI\sem_05\SBAPR\vrmlxpy\experiment.stl)");

  std::cout << "Done." << std::endl;
  return 0;
}