#include "TransformationMatrix.hpp"

#include <cmath>

#include <CGAL/Aff_transformation_3.h>
#include <CGAL/Kernel/interface_macros.h>
#include <CGAL/Kernel/Type_equality_wrapper.h>
#include <CGAL/Simple_cartesian.h>

#include "Angle.hpp"
#include "CGALBaseTypesForVrml.hpp"
#include "Quaternion.hpp"
#include "Transformation.hpp"

/**
 * @brief Creates a scale matrix based on the given scale vector.
 *
 * @param scaleVector scale vector
 * @returns scale matrix
 */
static vrml_proc::math::TransformationMatrix CreateScaleMatrix(const vrml_proc::parser::Vec3f& scaleVector) {
  return vrml_proc::math::TransformationMatrix(scaleVector.x, 0, 0, 0, scaleVector.y, 0, 0, 0, scaleVector.z);
}

/**
 * @brief Creates rotation matrix based on the given quaternion.
 *
 * @param quaternion quaternion
 * @returns rotation matrix
 *
 * @note Link for the algorithm is https://www.cprogramming.com/tutorial/3d/quaternions.html.
 */
static vrml_proc::math::TransformationMatrix CreateRotationMatrix(const vrml_proc::math::Quaternion& quaternion) {
  double x = quaternion.x;
  double y = quaternion.y;
  double z = quaternion.z;
  double w = quaternion.w;

  return vrml_proc::math::TransformationMatrix(
      float(1.0 - 2.0 * (y * y + z * z)), float(2.0 * (x * y - w * z)), float(2.0 * (x * z + w * y)),
      float(2.0 * (x * y + w * z)), float(1.0 - 2.0 * (x * x + z * z)), float(2.0 * (y * z - w * x)),
      float(2.0 * (x * z - w * y)), float(2.0 * (y * z + w * x)), float(1.0 - 2.0 * (x * x + y * y)));
}

vrml_proc::math::TransformationMatrix vrml_proc::math::UpdateTransformationMatrix(
    const vrml_proc::math::TransformationMatrix& currentMatrix,
    const vrml_proc::math::Transformation& transformationData) {
  using vrml_proc::math::Angle;
  using vrml_proc::math::Quaternion;
  using vrml_proc::math::TransformationMatrix;

  // Create a copy of a matrix.
  TransformationMatrix matrix = currentMatrix;

  // Subtract center.
  matrix =
      TransformationMatrix(CGAL::TRANSLATION, -vrml_proc::math::cgal::Vec3fToCGALVector3(transformationData.center)) *
      matrix;

  // Compute scale orientation quaternion.
  Quaternion scaleQuaternion = Quaternion(transformationData.scaleOrientation.x, transformationData.scaleOrientation.y,
                                          transformationData.scaleOrientation.z,
                                          Angle(Angle::AngleUnit::Radians, transformationData.scaleOrientation.w));

  // Subtract scale orientation.
  Quaternion inversedScaleQuaternion = scaleQuaternion;
  inversedScaleQuaternion.Inverse();
  TransformationMatrix localScaleOrientationMatrix = CreateRotationMatrix(inversedScaleQuaternion);
  matrix = localScaleOrientationMatrix * matrix;

  // Scale orientation.
  TransformationMatrix localScaleMatrix = CreateScaleMatrix(transformationData.scale);
  matrix = localScaleMatrix * matrix;

  // Scale orientation.
  localScaleOrientationMatrix = CreateRotationMatrix(scaleQuaternion);
  matrix = localScaleOrientationMatrix * matrix;

  // Rotation.
  Quaternion rotationQuaternion =
      Quaternion(transformationData.rotation.x, transformationData.rotation.y, transformationData.rotation.z,
                 Angle(Angle::AngleUnit::Radians, transformationData.rotation.w));
  TransformationMatrix localRotationMatrix = CreateRotationMatrix(rotationQuaternion);
  matrix = localRotationMatrix * matrix;

  // Center.
  matrix = vrml_proc::math::TransformationMatrix(CGAL::TRANSLATION,
                                                 vrml_proc::math::cgal::Vec3fToCGALVector3(transformationData.center)) *
           matrix;

  // Translation.
  matrix = vrml_proc::math::TransformationMatrix(
               CGAL::TRANSLATION, vrml_proc::math::cgal::Vec3fToCGALVector3(transformationData.translation)) *
           matrix;

  return matrix;
}
