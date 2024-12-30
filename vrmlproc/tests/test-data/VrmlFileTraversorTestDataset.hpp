﻿#include <string>


std::string onlyWorldInfo = R"(

	#VRML V2.0 utf8

	WorldInfo {

	   title "cube.wrl"

	   info "VRML file."

	}
)";


std::string validGroup = R"(

	#VRML V2.0 utf8

	WorldInfo {

	   title "cube.wrl"

	   info "VRML file."

	}

    Group {

        bboxCenter 4.5 10.47 -1.0E1

        bboxSize -2 -2 -2

        children [
            Spotlight {
                ambientIntensity  -1.0
            }
        ]
    }

)";

std::string nodeInNode = R"(

	#VRML V2.0 utf8

    Shape {
		appearance Appearence  {
			ambientIntensity  0.2E-1
		}
	}

)";

std::string quiteDeepRecursiveNode = R"(

	#VRML V2.0 utf8

    Shape {
		appearance Appearence  {
			ambientIntensity  0.2E-1
			appearance Appearence {
				lightExposure 42
			}
		}
       geometry [ 58.5 458.02 68.78 ,
                  100.001001 5.55317 -3.05561 ]
	}

    Group {
		bboxCenter 58.7E-3 0.0 15
		bboxSize   -1.0 -1.0 -1.0
	}

)";

std::string groupWithNodesArray = R"(

	#VRML V2.0 utf8

    Group {
		children [

					Group {
                           bboxCenter 58.7E-3 0.0 15
                      },

                    Group {
                           bboxSize   -1.0 -1.0 -1.0
                      }

				 ]
	}

)";

std::string simpleDefNode = R"(

	#VRML V2.0 utf8

    Group {
		children [
					DEF id Group {
                           bboxCenter 58.7E-3 0.0 15
                      }
				 ]
	}

)";

std::string simpleUseNode = R"(

	#VRML V2.0 utf8

    Group {
		children [
					DEF id Group {
                           bboxCenter 58.7E-3 0.0 15
                      }
				 ]
	}

	Group {

		instance USE id 
    }

)";

std::string nodeWithSwitch = R"(

	#VRML V2.0 utf8

    Group {
          children [ 
            DEF A1 Switch {
                whichChoice 0
                choice DEF C2  Empty { }
              }
           ]
         }
)";

std::string nodeWithBoolean = R"(

	#VRML V2.0 utf8

    Group {
      shape Shape {

          geometry 
              IndexedFaceSet {
                  coordIndex [ 944, 942, 943, -1, 947, 945, 946, -1,
                      947, 949, 948, -1, 945, 947, 948, -1,
                      946, 942, 944, -1, 946, 944, 947, -1,
                      108889, 108856, 182463, -1, 108863, 108433, 182449, -1 ]
                  ccw TRUE
                  solid FALSE
          }
      }
    }
)";

std::string nodeWithUtf8 = R"(

	#VRML V2.0 utf8
    # Tento dokument je skutečně v kódování UTF-8.

    ČeskáSkupina {
      políčko Tvar {

          ß 
              开儿艾诶开伊艾艾西吉艾艾伊娜伊 {
                  koordináty [ 944, 942, 943, -1, 947, 945, 946, -1,
                      947, 949, 948, -1, 945, 947, 948, -1,
                      946, 942, 944, -1, 946, 944, 947, -1,
                      108889, 108856, 182463, -1, 108863, 108433, 182449, -1 ]
          }
      }
    }
)";

std::string emptyNode = R"(

    Group {
       subgroup Group {
           subsubgroup Group {}
        }
    }

)";

std::string complicatedNode = R"(

    #VRML V2.0 utf8

    WorldInfo {

    title "cube.wrl"

    info "VRML file generated by IVREAD."

  }

 # comment

    Group {
      children DEF ShowHideSep Group {
           children [ 
                DEF emptyGroup     Group {

                },
          
                DEF bpPointsInventor     Group {

                },

                DEF bpROIViewerInventorT   Group {
                  children 
                    DEF nine Switch {
                      whichChoice -1
                      choice 
                        DEF TimeSwitch10     Switch {
                          whichChoice -1
                        }
                    }
                },

                DEF bpSurfacesViewerInventor     Group {
                  children [ 
                    DEF one     Switch {
                      whichChoice 0
                      choice 
                        DEF grehgregre     Group {
                          children 
                            DEF TimeSwitch     Switch {
                              whichChoice 0
                              choice 
                                DEF bpColorSwitchSetInventor     Group {
                                  children [ 
                                    DEF htrehtr     Group {
                                      children [ 
                                        DEF hrtehteh     Shape {
                                          appearance 
                                              Appearance {
                                            material 
                                                Material {
                                              diffuseColor 0 0.80000001 0
                                            }

                                          }
                                          geometry 
                                              IndexedFaceSet {
                                            coord 
                                            DEF jnyuju     Coordinate {
                                              point [ 285.5 434.2858 8.5,
                                                  284.83615 434.5 8.5,
                                                  515.5 612.5 31.799721 ]

                                            }
                                            normal 
                                            DEF wetwt     Normal {
                                              vector [ -0.2371722 -0.7134968 -0.65929627,
                                                  0.28721604 0.440483 0.85057724 ]

                                            }
                                            coordIndex [ 2, 0, 1, -1, 5, 3, 4, -1,
                                                8, 6, 7, -1, 11, 9, 10, -1,
                                                7, 9, 11, -1, 8, 7, 11, -1,
                                                8, 11, 4, -1, 8, 4, 3, -1,
                                                14, 12, 13, -1, 5, 16, 15, -1,
                                                787, 641, 791, -1, 642, 545, 791, -1 ]
                                            ccw TRUE
                                            solid FALSE
                                            convex TRUE
                                            creaseAngle 0

                                          }

                                        },
                                  
                                        DEF seven     Shape {
                                          appearance 
                                              Appearance {
                                            material 
                                                Material {
                                              diffuseColor 0 0 0.80000001

                                            }

                                          }
                                          geometry 
                                              IndexedFaceSet {
                                            coord 
                                            USE grthg
                                            normal 
                                            USE ghhhhhyuu
                                            coordIndex [ 944, 942, 943, -1, 947, 945, 946, -1,
                                                947, 949, 948, -1, 945, 947, 948, -1,
                                                946, 942, 944, -1, 946, 944, 947, -1,
                                                108889, 108856, 182463, -1, 108863, 108433, 182449, -1 ]
                                            ccw TRUE
                                            solid FALSE
                                            convex TRUE
                                            creaseAngle 0

                                          }

                                        } ]

                                    },
                              
                                    DEF SelectionColorClass     Group {

                                    } ]

                                }

                            }

                        }

                    },
              
                    DEF ejt     Switch {
                      whichChoice -1

                    } ]

                },

               DEF bpTracksViewerInventor  Group {
                  children 
                    DEF eleven Transform {
                      translation 0 0 0
                      rotation 0 0 1 0
                    }
                }
           ]
      }
    }
)";

std::string complicatedNodeWithComplicatedDefNodeNames = R"(

#VRML V2.0 utf8
#Inventor V2.1 ascii

    WorldInfo {

    title "cube.wrl"

    info "VRML file generated by IVREAD."

  }

 # comment


    Group {
      children
    DEF ShowHideSep     Group {
      children [ 
        DEF _0     Group {

        },
          
        DEF bpPointsInventor     Group {

        },
          
        DEF bpSurfacesViewerInventor     Group {
          children [ 
            DEF _1     Switch {
              whichChoice 0
              choice 
                DEF _2     Group {
                  children 
                    DEF TimeSwitch     Switch {
                      whichChoice 0
                      choice 
                        DEF bpColorSwitchSetInventor     Group {
                          children [ 
                            DEF _3     Group {
                              children [ 
                                DEF _4     Shape {
                                  appearance 
                                      Appearance {
                                    material 
                                        Material {
                                      diffuseColor 0 0.80000001 0
                                    }

                                  }
                                  geometry 
                                      IndexedFaceSet {
                                    coord 
                                    DEF _5     Coordinate {
                                      point [ 285.5 434.2858 8.5,
                                          284.83615 434.5 8.5,
                                          515.5 612.5 31.799721 ]

                                    }
                                    normal 
                                    DEF _6     Normal {
                                      vector [ -0.2371722 -0.7134968 -0.65929627,
                                          0.28721604 0.440483 0.85057724 ]

                                    }
                                    coordIndex [ 2, 0, 1, -1, 5, 3, 4, -1,
                                        8, 6, 7, -1, 11, 9, 10, -1,
                                        7, 9, 11, -1, 8, 7, 11, -1,
                                        8, 11, 4, -1, 8, 4, 3, -1,
                                        14, 12, 13, -1, 5, 16, 15, -1,
                                        787, 641, 791, -1, 642, 545, 791, -1 ]
                                    ccw TRUE
                                    solid FALSE
                                    convex TRUE
                                    creaseAngle 0

                                  }

                                },
                                  
                                DEF _7     Shape {
                                  appearance 
                                      Appearance {
                                    material 
                                        Material {
                                      diffuseColor 0 0 0.80000001

                                    }

                                  }
                                  geometry 
                                      IndexedFaceSet {
                                    coord 
                                    USE _5
                                    normal 
                                    USE _6
                                    coordIndex [ 944, 942, 943, -1, 947, 945, 946, -1,
                                        947, 949, 948, -1, 945, 947, 948, -1,
                                        946, 942, 944, -1, 946, 944, 947, -1,
                                        108889, 108856, 182463, -1, 108863, 108433, 182449, -1 ]
                                    ccw TRUE
                                    solid FALSE
                                    convex TRUE
                                    creaseAngle 0

                                  }

                                } ]

                            },
                              
                            DEF SelectionColorClass     Group {

                            } ]

                        }

                    }

                }

            },
              
            DEF _8     Switch {
              whichChoice -1

            } ]

        },
          
        DEF bpROIViewerInventorT     Group {
          children 
            DEF _9     Switch {
              whichChoice -1
              choice 
                DEF TimeSwitch_10     Switch {
                  whichChoice -1

                }
            }
        },
          
        DEF bpTracksViewerInventor     Group {
          children 
            DEF _11  Transform {
              translation 0 0 0
              rotation 0 0 1 0
            }

        } ]

    }
}    

)";
