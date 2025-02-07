#include <string>


std::string validWorldInfoNode = R"(

	#VRML V2.0 utf8

	WorldInfo {

	   title "cube.wrl"

	   info "VRML file."

	}
)";

std::string invalidWorldInfoNodeWrongDataType = R"(

	#VRML V2.0 utf8

	WorldInfo {

	   title Node { size 5.505 }

	   info "VRML file."

	}
)";

std::string invalidWorldInfoNodeWrongFieldName = R"(

	#VRML V2.0 utf8

	WorldInfo {

	   titles "cube.wrl"

	   info "VRML file."

	}
)";

std::string validBoxNode = R"(

	#VRML V2.0 utf8

	Box {
        size 58.58 0.45 558.05
	}
)";

std::string validBoxNodeEmpty = R"(

	#VRML V2.0 utf8

	Box {
        
	}
)";

std::string invalidBoxNodeWrongDataType = R"(

	#VRML V2.0 utf8

	Box {
        size [58.58 0.45 558.05]
	}
)";

std::string invalidBoxNodeWrongFieldName = R"(

	#VRML V2.0 utf8

	Box {
        center 58.58 0.45 558.05
	}
)";

std::string invalidGroupUnknownNode = R"(

    #VRML V2.0 utf8

    WorldInfo {
        title "cube.wrl"
        info "VRML file."
    }

    Group {
        bboxCenter 4.5 10.47 -10.0
        bboxSize 0 0 0
        children [
            NoNode {
                size 58.58 0.45 558.05
            }
        ]
    }
)";

std::string validGroup = R"(

    #VRML V2.0 utf8

    WorldInfo {
        title "cube.wrl"
        info "VRML file."
    }

    Group {
        bboxCenter 4.5 10.47 -10.0
        bboxSize 0 0 0
        children [
            Box {
                size 1 1 1 
            },
            Box {
                size 2 2 2
            }
        ]
    }
)";

std::string validShape = R"(

    #VRML V2.0 utf8

    WorldInfo {
        title "cube.wrl"
        info "VRML file."
    }

    Group {
        bboxCenter 4.5 10.47 -10.0
        bboxSize 0 0 0
        children [
            DEF id Box {
                size 58.58 0.45 558.05
            },
            Shape {
                geometry USE id
            }
        ]
    }

)";

std::string validShapeMerge = R"(

    #VRML V2.0 utf8

    WorldInfo {
        title "cube.wrl"
        info "VRML file."
    }

    Group {
        bboxCenter 4.5 10.47 -10.0
        bboxSize 0 0 0
        children [
            DEF id Box {
                size 58.58 0.45 558.05
            },
            Shape {
                geometry USE id
            },
            Shape {
                geometry USE id
            },
            Shape {
                geometry USE id
            }
        ]
    }

)";

std::string validShapeEmpty = R"(

    #VRML V2.0 utf8

    WorldInfo {
        title "cube.wrl"
        info "VRML file."
    }

    Group {
        bboxCenter 4.5 10.47 -10.0
        bboxSize 0 0 0
        children [
            DEF id Box {
                size 58.58 0.45 558.05
            },
            Shape {}
        ]
    }

)";

std::string invalidShapeWrongNodeForGeometryField = R"(

    #VRML V2.0 utf8

    WorldInfo {
        title "cube.wrl"
        info "VRML file."
    }

    Group {
        bboxCenter 4.5 10.47 -10.0
        bboxSize 0 0 0
        children [
            DEF id WorldInfo {
                title "cube.wrl"
                info "VRML file."
            },
            Shape {
                geometry USE id
            }
        ]
    }

)";

std::string validIndexedFaceSetNode = R"(

    #VRML V2.0 utf8

    WorldInfo {
        title "cube.wrl"
        info "VRML file."
    }

    Group {
        bboxCenter 0 0 0
        bboxSize 0 0 0
        children [
            Shape {
                geometry IndexedFaceSet {
                    coord Coordinate {
                       point [
                           285.5 434.2858 8.5,
                           284.83615 434.5 8.5,
                           285.5 434.5 8.3120184,
                           283.5 436.35297 7.5,
                           283.28903 436.5 7.5,
                           283.5 436.5 7.3894577,
                           283.5 435.24545 8.5,
                           283.22855 435.5 8.5,
                           283.5 435.5 8.2797327
                       ]
                    }
                    coordIndex [2, 0, 1, -1, 5, 3, 4, -1]
                    ccw TRUE
                    solid FALSE
                }
            }
        ]
    }
)";
