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

    return vrml_proc::math::TransformationMatrix(
        scaleVector.x,
        0,
        0,
        0,
        scaleVector.y,
        0,
        0,
        0,
        scaleVector.z
    );
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
        float(1.0 - 2.0 * (y * y + z * z)),
        float(2.0 * (x * y - w * z)),
        float(2.0 * (x * z + w * y)),
        float(2.0 * (x * y + w * z)),
        float(1.0 - 2.0 * (x * x + z * z)),
        float(2.0 * (y * z - w * x)),
        float(2.0 * (x * z - w * y)),
        float(2.0 * (y * z + w * x)),
        float(1.0 - 2.0 * (x * x + y * y))
    );
}

void vrml_proc::math::UpdateTransformationMatrix(vrml_proc::math::TransformationMatrix& matrix, const vrml_proc::math::Transformation& transformationData) {

    // Translation.
    matrix = vrml_proc::math::TransformationMatrix(CGAL::TRANSLATION, vrml_proc::math::cgal::Vec3fToCGALVector3(transformationData.translation)) * matrix;

    // Center.
    matrix = vrml_proc::math::TransformationMatrix(CGAL::TRANSLATION, vrml_proc::math::cgal::Vec3fToCGALVector3(transformationData.center)) * matrix;

    // Rotation.
    vrml_proc::math::Quaternion rotationQuaternion = vrml_proc::math::Quaternion(transformationData.rotation.x, transformationData.rotation.y, transformationData.rotation.z, vrml_proc::math::Angle(vrml_proc::math::Angle::AngleUnit::Radians, transformationData.rotation.w));
    vrml_proc::math::TransformationMatrix localRotationMatrix = CreateRotationMatrix(rotationQuaternion);
    matrix = localRotationMatrix * matrix;

    // Scale orientation.
    vrml_proc::math::Quaternion scaleQuaternion = vrml_proc::math::Quaternion(transformationData.scaleOrientation.x, transformationData.scaleOrientation.y, transformationData.scaleOrientation.z, vrml_proc::math::Angle(vrml_proc::math::Angle::AngleUnit::Radians, transformationData.scaleOrientation.w));
    vrml_proc::math::TransformationMatrix localScaleOrientationMatrix = CreateRotationMatrix(scaleQuaternion);
    matrix = localScaleOrientationMatrix * matrix;

    // Scale.
    vrml_proc::math::TransformationMatrix localScaleMatrix = CreateScaleMatrix(transformationData.scale);
    matrix = localScaleMatrix * matrix;

    // Remove scale orientation.
    scaleQuaternion.Inverse();
    localScaleOrientationMatrix = CreateRotationMatrix(scaleQuaternion);
    matrix = localScaleOrientationMatrix * matrix;
    
    // Remove center.
    matrix = vrml_proc::math::TransformationMatrix(CGAL::TRANSLATION, -vrml_proc::math::cgal::Vec3fToCGALVector3(transformationData.center)) * matrix;
}

