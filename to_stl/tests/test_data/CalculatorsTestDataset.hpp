#include <string>

std::string validIndexedFaceSetPyramid = R"(

    #VRML V2.0 utf8

    Group {
        bboxCenter 0 0 0
        bboxSize 0 0 0
        children [
            Shape {
                geometry IndexedFaceSet {
                
                    coord Coordinate {
                        point [
                            0 0 0, 1 0 0, 1 1 0, 0 1 0, // Base square (0-3)
                            0.5 0.5 1 // Pyramid peak (4)
                        ]
                    }
                    coordIndex [
                        0, 1, 2, -1, 0, 2, 3, -1,  # Base square
                        0, 1, 4, -1,      # Side 1
                        1, 2, 4, -1,      # Side 2
                        2, 3, 4, -1,      # Side 3
                        3, 0, 4, -1       # Side 4
                    ]

                    convex FALSE
                }
            }
        ]
    }
)";

std::string validIndexedFaceSetPyramidEmptyCoordIndex = R"(

    #VRML V2.0 utf8

    Group {
        bboxCenter 0 0 0
        bboxSize 0 0 0
        children [
            Shape {
                geometry IndexedFaceSet {
                
                    coord Coordinate {
                        point [
                            0 0 0, 1 0 0, 1 1 0, 0 1 0, // Base square (0-3)
                            0.5 0.5 1 // Pyramid peak (4)
                        ]
                    }

                    convex FALSE
                }
            }
        ]
    }
)";