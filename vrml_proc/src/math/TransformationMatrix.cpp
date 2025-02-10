#include "TransformationMatrix.hpp"

#include <cmath>

#include <CGAL/Aff_transformation_3.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Kernel/interface_macros.h>
#include <CGAL/Kernel/Type_equality_wrapper.h>

#include "CGALBaseTypesForVrml.hpp"
#include "VrmlUnits.hpp"
#include "Transformation.hpp"

void vrml_proc::math::UpdateTransformationMatrix(vrml_proc::math::TransformationMatrix& matrix, const vrml_proc::math::Transformation& transformationData) {

    // Apply translation (final translation first)
    matrix = vrml_proc::math::TransformationMatrix(CGAL::TRANSLATION, vrml_proc::math::cgal::Vec3fToCGALVector3(transformationData.translation)) * matrix;

    // Apply translation to the center
    matrix = vrml_proc::math::TransformationMatrix(CGAL::TRANSLATION, vrml_proc::math::cgal::Vec3fToCGALVector3(transformationData.center)) * matrix;

    //// Apply rotation (axis-angle to CGAL rotation)
    //CGAL::Rotation_3<vrml_proc::parser::float32_t> rotation = axisAngleToRotation(transformationData.rotation);
    //matrix = CGAL::ROTATE(matrix, rotation);

    //// Apply scale orientation (pre-rotation before scaling)
    //CGAL::Rotation_3<vrml_proc::parser::float32_t> scaleOrientation = axisAngleToRotation(transformationData.scaleOrientation);
    //matrix = CGAL::ROTATE(matrix, scaleOrientation);

    //// Apply scaling
    //matrix = CGAL::SCALE(matrix, transformationData.scale);

    //// Apply -scaleOrientation (undo scale orientation)
    //matrix = CGAL::ROTATE(matrix, -scaleOrientation); // Undo scaleOrientation (inverse rotation)

    //// Apply -center (undo translation to center)
    //matrix = CGAL::TRANSLATE(matrix, -transformationData.center); // Undo translation to center
}
