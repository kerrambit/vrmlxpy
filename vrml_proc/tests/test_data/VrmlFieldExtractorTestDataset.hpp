std::string first = R"(

	#VRML V2.0 utf8

	Test {
		
		field1 7.5 0.0 69.9
		field2 TRUE
	}
)";

std::string nodesArray = R"(

	#VRML V2.0 utf8

    Test {
       shape Shape {
          geometry IndexedFaceSet {
                  coordIndex [ 944, 942, 943, -1, 947, 945, 946, -1,
                      947, 949, 948, -1, 945, 947, 948, -1,
                      946, 942, 944, -1, 946, 944, 947, -1,
                      108889, 108856, 182463, -1, 108863, 108433, 182449, -1 ]
          }
       }
    }
)";

std::string children = R"(

	#VRML V2.0 utf8

    Group {
       children [
            IndexedFaceSet {
                  coordIndex [ 944, 942, 943, -1, 947, 945, 946, -1,
                      947, 949, 948, -1, 945, 947, 948, -1,
                      946, 942, 944, -1, 946, 944, 947, -1,
                      108889, 108856, 182463, -1, 108863, 108433, 182449, -1 ]
          }
       ]
    }
)";