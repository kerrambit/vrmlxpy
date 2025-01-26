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
                size 58.58 0.45 558.05
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
