{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 47.0, 68.0, 1387.0, 585.0 ],
		"bglocked" : 0,
		"defrect" : [ 47.0, 68.0, 1387.0, 585.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 11.0,
		"default_fontface" : 0,
		"default_fontname" : "Verdana",
		"gridonopen" : 0,
		"gridsize" : [ 5.0, 5.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "bpatcher",
					"numoutlets" : 0,
					"id" : "obj-2",
					"name" : "tap.badge.maxpat",
					"args" : [  ],
					"numinlets" : 0,
					"patching_rect" : [ 261.0, 497.0, 225.0, 67.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "autohelp",
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"hidden" : 1,
					"id" : "obj-3",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 53.0, 505.0, 54.0, 19.0 ],
					"frozen_object_attributes" : 					{
						"helpfontname" : "Verdana",
						"helpfontsize" : 10.0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_see_title",
					"text" : "See Also:",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 0,
					"fontface" : 1,
					"fontsize" : 11.595187,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-4",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 58.0, 509.0, 100.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "umenu",
					"varname" : "autohelp_see_menu",
					"numoutlets" : 3,
					"fontsize" : 11.595187,
					"items" : [ "(Objects:)", ",", "tap.jit.motion", ",", "<separator>" ],
					"outlettype" : [ "int", "", "" ],
					"id" : "obj-5",
					"fontname" : "Arial",
					"types" : [  ],
					"numinlets" : 1,
					"patching_rect" : [ 58.0, 529.0, 130.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "This is a rather unwieldly example of a 64 region analysis",
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"frgb" : [ 0.333333, 0.333333, 0.333333, 1.0 ],
					"id" : "obj-8",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 20.0, 134.0, 356.0, 21.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-9",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 14.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-10",
					"numinlets" : 1,
					"patching_rect" : [ 160.0, 461.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-11",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 1337.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-12",
					"numinlets" : 1,
					"patching_rect" : [ 139.0, 461.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-13",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 1316.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-14",
					"numinlets" : 1,
					"patching_rect" : [ 118.0, 461.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-15",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 1295.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-16",
					"numinlets" : 1,
					"patching_rect" : [ 97.0, 461.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-17",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 1274.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-18",
					"numinlets" : 1,
					"patching_rect" : [ 76.0, 461.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-19",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 1253.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-20",
					"numinlets" : 1,
					"patching_rect" : [ 55.0, 461.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-21",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 1232.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-22",
					"numinlets" : 1,
					"patching_rect" : [ 34.0, 461.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-23",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 1211.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-24",
					"numinlets" : 1,
					"patching_rect" : [ 13.0, 461.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-25",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 1190.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-26",
					"numinlets" : 1,
					"patching_rect" : [ 160.0, 443.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-27",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 1169.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-28",
					"numinlets" : 1,
					"patching_rect" : [ 139.0, 443.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-29",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 1148.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-30",
					"numinlets" : 1,
					"patching_rect" : [ 118.0, 443.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-31",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 1127.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-32",
					"numinlets" : 1,
					"patching_rect" : [ 97.0, 443.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-33",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 1106.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-34",
					"numinlets" : 1,
					"patching_rect" : [ 76.0, 443.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-35",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 1085.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-36",
					"numinlets" : 1,
					"patching_rect" : [ 55.0, 443.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-37",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 1064.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-38",
					"numinlets" : 1,
					"patching_rect" : [ 34.0, 443.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-39",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 1043.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-40",
					"numinlets" : 1,
					"patching_rect" : [ 13.0, 443.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-41",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 1022.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-42",
					"numinlets" : 1,
					"patching_rect" : [ 160.0, 425.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-43",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 1001.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-44",
					"numinlets" : 1,
					"patching_rect" : [ 139.0, 425.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-45",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 980.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-46",
					"numinlets" : 1,
					"patching_rect" : [ 118.0, 425.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-47",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 959.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-48",
					"numinlets" : 1,
					"patching_rect" : [ 97.0, 425.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-49",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 938.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-50",
					"numinlets" : 1,
					"patching_rect" : [ 76.0, 425.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-51",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 917.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-52",
					"numinlets" : 1,
					"patching_rect" : [ 55.0, 425.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-53",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 896.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-54",
					"numinlets" : 1,
					"patching_rect" : [ 34.0, 425.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-55",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 875.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-56",
					"numinlets" : 1,
					"patching_rect" : [ 13.0, 425.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-57",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 854.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-58",
					"numinlets" : 1,
					"patching_rect" : [ 160.0, 407.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-59",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 833.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-60",
					"numinlets" : 1,
					"patching_rect" : [ 139.0, 407.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-61",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 812.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-62",
					"numinlets" : 1,
					"patching_rect" : [ 118.0, 407.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-63",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 791.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-64",
					"numinlets" : 1,
					"patching_rect" : [ 97.0, 407.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-65",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 770.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-66",
					"numinlets" : 1,
					"patching_rect" : [ 76.0, 407.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-67",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 749.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-68",
					"numinlets" : 1,
					"patching_rect" : [ 55.0, 407.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-69",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 728.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-70",
					"numinlets" : 1,
					"patching_rect" : [ 34.0, 407.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-71",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 707.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-72",
					"numinlets" : 1,
					"patching_rect" : [ 13.0, 407.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-73",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 686.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-74",
					"numinlets" : 1,
					"patching_rect" : [ 160.0, 389.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-75",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 665.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-76",
					"numinlets" : 1,
					"patching_rect" : [ 139.0, 389.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-77",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 644.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-78",
					"numinlets" : 1,
					"patching_rect" : [ 118.0, 389.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-79",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 623.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-80",
					"numinlets" : 1,
					"patching_rect" : [ 97.0, 389.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-81",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 602.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-82",
					"numinlets" : 1,
					"patching_rect" : [ 76.0, 389.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-83",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 581.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-84",
					"numinlets" : 1,
					"patching_rect" : [ 55.0, 389.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-85",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 560.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-86",
					"numinlets" : 1,
					"patching_rect" : [ 34.0, 389.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-87",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 539.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-88",
					"numinlets" : 1,
					"patching_rect" : [ 13.0, 389.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-89",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 518.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-90",
					"numinlets" : 1,
					"patching_rect" : [ 160.0, 371.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-91",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 497.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-92",
					"numinlets" : 1,
					"patching_rect" : [ 139.0, 371.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-93",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 476.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-94",
					"numinlets" : 1,
					"patching_rect" : [ 118.0, 371.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-95",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 455.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-96",
					"numinlets" : 1,
					"patching_rect" : [ 97.0, 371.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-97",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 434.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-98",
					"numinlets" : 1,
					"patching_rect" : [ 76.0, 371.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-99",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 413.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-100",
					"numinlets" : 1,
					"patching_rect" : [ 55.0, 371.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-101",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 392.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-102",
					"numinlets" : 1,
					"patching_rect" : [ 34.0, 371.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-103",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 371.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-104",
					"numinlets" : 1,
					"patching_rect" : [ 13.0, 371.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-105",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 350.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-106",
					"numinlets" : 1,
					"patching_rect" : [ 160.0, 353.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-107",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 329.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-108",
					"numinlets" : 1,
					"patching_rect" : [ 139.0, 353.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-109",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 308.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-110",
					"numinlets" : 1,
					"patching_rect" : [ 118.0, 353.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-111",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 287.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-112",
					"numinlets" : 1,
					"patching_rect" : [ 97.0, 353.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-113",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 266.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-114",
					"numinlets" : 1,
					"patching_rect" : [ 76.0, 353.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-115",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 245.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-116",
					"numinlets" : 1,
					"patching_rect" : [ 55.0, 353.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-117",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 224.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-118",
					"numinlets" : 1,
					"patching_rect" : [ 34.0, 353.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-119",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 203.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-120",
					"numinlets" : 1,
					"patching_rect" : [ 13.0, 353.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-121",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 182.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-122",
					"numinlets" : 1,
					"patching_rect" : [ 160.0, 335.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-123",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 161.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-124",
					"numinlets" : 1,
					"patching_rect" : [ 139.0, 335.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-125",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 140.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-126",
					"numinlets" : 1,
					"patching_rect" : [ 118.0, 335.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-127",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 119.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-128",
					"numinlets" : 1,
					"patching_rect" : [ 97.0, 335.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-129",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 98.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-130",
					"numinlets" : 1,
					"patching_rect" : [ 76.0, 335.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-131",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 77.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-132",
					"numinlets" : 1,
					"patching_rect" : [ 55.0, 335.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-133",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 56.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-134",
					"numinlets" : 1,
					"patching_rect" : [ 34.0, 335.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.motion2",
					"linecount" : 7,
					"numoutlets" : 4,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "float", "float", "float" ],
					"id" : "obj-135",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 35.0, 230.0, 22.0, 92.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.fpsgui",
					"numoutlets" : 2,
					"fontsize" : 10.0,
					"outlettype" : [ "", "" ],
					"id" : "obj-136",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 125.0, 166.0, 60.0, 35.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.grayscale",
					"numoutlets" : 1,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix" ],
					"id" : "obj-137",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 14.0, 179.0, 89.0, 19.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tap.jit.source",
					"numoutlets" : 1,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix" ],
					"id" : "obj-138",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 14.0, 156.0, 75.0, 19.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"depthbuffer" : 0,
					"id" : "obj-139",
					"numinlets" : 1,
					"patching_rect" : [ 13.0, 335.0, 20.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.scissors @rows 8 @columns 8",
					"numoutlets" : 65,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "jit_matrix", "" ],
					"id" : "obj-140",
					"fontname" : "Verdana",
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 14.0, 205.0, 1364.0, 19.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_see_panel",
					"bgcolor" : [ 0.85, 0.85, 0.85, 0.75 ],
					"numoutlets" : 0,
					"bordercolor" : [ 0.5, 0.5, 0.5, 0.75 ],
					"border" : 2,
					"id" : "obj-6",
					"background" : 1,
					"numinlets" : 1,
					"patching_rect" : [ 53.0, 505.0, 140.0, 50.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_title",
					"text" : "tap.jit.motion2",
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"numoutlets" : 0,
					"fontface" : 3,
					"fontsize" : 20.871338,
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"id" : "obj-142",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 10.0, 8.0, 485.0, 30.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_digest",
					"text" : "Video motion detection analysis",
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 12.754705,
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"id" : "obj-143",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 10.0, 36.0, 485.0, 21.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_description",
					"text" : "The tap.jit.motion2 object This is yet another version of tap.jit.motion. The distinguishing feature is that the difference found in a region is output as a 1x1 matrix rather than as a floating-point number.",
					"linecount" : 3,
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"frgb" : [ 0.333333, 0.333333, 0.333333, 1.0 ],
					"id" : "obj-144",
					"fontname" : "Verdana",
					"numinlets" : 1,
					"patching_rect" : [ 10.0, 57.0, 477.0, 43.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "textbutton",
					"varname" : "autohelp_top_ref",
					"bgcolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"bgoveroncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"underline" : 1,
					"textcolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"numoutlets" : 3,
					"fontface" : 3,
					"fontsize" : 12.754705,
					"spacing_x" : 0.0,
					"bgoncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"outlettype" : [ "", "", "int" ],
					"bordercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"spacing_y" : 0.0,
					"textoncolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 190.140411, 14.666666 ],
					"id" : "obj-145",
					"fontlink" : 1,
					"text" : "open tap.jit.motion2 reference",
					"textovercolor" : [ 0.4, 0.5, 0.65, 1.0 ],
					"fontname" : "Arial",
					"bgovercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 309.859589, 22.0, 190.140411, 14.666666 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_top_panel",
					"grad2" : [ 0.85, 0.85, 0.85, 1.0 ],
					"numoutlets" : 0,
					"mode" : 1,
					"id" : "obj-146",
					"background" : 1,
					"grad1" : [ 0.27, 0.35, 0.47, 1.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 5.0, 5.0, 495.0, 52.0 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-138", 0 ],
					"destination" : [ "obj-137", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-137", 0 ],
					"destination" : [ "obj-140", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-139", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-121", 0 ],
					"destination" : [ "obj-120", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-105", 0 ],
					"destination" : [ "obj-104", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-89", 0 ],
					"destination" : [ "obj-88", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-73", 0 ],
					"destination" : [ "obj-72", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-57", 0 ],
					"destination" : [ "obj-56", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-40", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 1 ],
					"destination" : [ "obj-135", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-135", 0 ],
					"destination" : [ "obj-134", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-119", 0 ],
					"destination" : [ "obj-118", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-103", 0 ],
					"destination" : [ "obj-102", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-87", 0 ],
					"destination" : [ "obj-86", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-71", 0 ],
					"destination" : [ "obj-70", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-55", 0 ],
					"destination" : [ "obj-54", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 0 ],
					"destination" : [ "obj-38", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 2 ],
					"destination" : [ "obj-133", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-133", 0 ],
					"destination" : [ "obj-132", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-117", 0 ],
					"destination" : [ "obj-116", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-101", 0 ],
					"destination" : [ "obj-100", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-85", 0 ],
					"destination" : [ "obj-84", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-69", 0 ],
					"destination" : [ "obj-68", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-53", 0 ],
					"destination" : [ "obj-52", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-36", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 3 ],
					"destination" : [ "obj-131", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-131", 0 ],
					"destination" : [ "obj-130", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-115", 0 ],
					"destination" : [ "obj-114", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-99", 0 ],
					"destination" : [ "obj-98", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-83", 0 ],
					"destination" : [ "obj-82", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-67", 0 ],
					"destination" : [ "obj-66", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-51", 0 ],
					"destination" : [ "obj-50", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 4 ],
					"destination" : [ "obj-129", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-129", 0 ],
					"destination" : [ "obj-128", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-113", 0 ],
					"destination" : [ "obj-112", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-97", 0 ],
					"destination" : [ "obj-96", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-81", 0 ],
					"destination" : [ "obj-80", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-65", 0 ],
					"destination" : [ "obj-64", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-49", 0 ],
					"destination" : [ "obj-48", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-32", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 5 ],
					"destination" : [ "obj-127", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-127", 0 ],
					"destination" : [ "obj-126", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-111", 0 ],
					"destination" : [ "obj-110", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-95", 0 ],
					"destination" : [ "obj-94", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-79", 0 ],
					"destination" : [ "obj-78", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-63", 0 ],
					"destination" : [ "obj-62", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-47", 0 ],
					"destination" : [ "obj-46", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-30", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-137", 0 ],
					"destination" : [ "obj-136", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 6 ],
					"destination" : [ "obj-125", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-125", 0 ],
					"destination" : [ "obj-124", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-109", 0 ],
					"destination" : [ "obj-108", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-93", 0 ],
					"destination" : [ "obj-92", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-77", 0 ],
					"destination" : [ "obj-76", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-61", 0 ],
					"destination" : [ "obj-60", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-45", 0 ],
					"destination" : [ "obj-44", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 7 ],
					"destination" : [ "obj-123", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-123", 0 ],
					"destination" : [ "obj-122", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-107", 0 ],
					"destination" : [ "obj-106", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-91", 0 ],
					"destination" : [ "obj-90", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-75", 0 ],
					"destination" : [ "obj-74", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-59", 0 ],
					"destination" : [ "obj-58", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-42", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 8 ],
					"destination" : [ "obj-121", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 9 ],
					"destination" : [ "obj-119", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 10 ],
					"destination" : [ "obj-117", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 11 ],
					"destination" : [ "obj-115", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 12 ],
					"destination" : [ "obj-113", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 13 ],
					"destination" : [ "obj-111", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 14 ],
					"destination" : [ "obj-109", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 15 ],
					"destination" : [ "obj-107", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 16 ],
					"destination" : [ "obj-105", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 17 ],
					"destination" : [ "obj-103", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 18 ],
					"destination" : [ "obj-101", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 19 ],
					"destination" : [ "obj-99", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 20 ],
					"destination" : [ "obj-97", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 21 ],
					"destination" : [ "obj-95", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 22 ],
					"destination" : [ "obj-93", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 23 ],
					"destination" : [ "obj-91", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 24 ],
					"destination" : [ "obj-89", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 25 ],
					"destination" : [ "obj-87", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 26 ],
					"destination" : [ "obj-85", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 27 ],
					"destination" : [ "obj-83", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 28 ],
					"destination" : [ "obj-81", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 29 ],
					"destination" : [ "obj-79", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 30 ],
					"destination" : [ "obj-77", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 31 ],
					"destination" : [ "obj-75", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 32 ],
					"destination" : [ "obj-73", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 33 ],
					"destination" : [ "obj-71", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 34 ],
					"destination" : [ "obj-69", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 35 ],
					"destination" : [ "obj-67", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 36 ],
					"destination" : [ "obj-65", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 37 ],
					"destination" : [ "obj-63", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 38 ],
					"destination" : [ "obj-61", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 39 ],
					"destination" : [ "obj-59", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 40 ],
					"destination" : [ "obj-57", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 41 ],
					"destination" : [ "obj-55", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 42 ],
					"destination" : [ "obj-53", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 43 ],
					"destination" : [ "obj-51", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 44 ],
					"destination" : [ "obj-49", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 45 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 46 ],
					"destination" : [ "obj-45", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 47 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 48 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 49 ],
					"destination" : [ "obj-39", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 50 ],
					"destination" : [ "obj-37", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 51 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 52 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 53 ],
					"destination" : [ "obj-31", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 54 ],
					"destination" : [ "obj-29", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 55 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 56 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 57 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 58 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 59 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 60 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 61 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 62 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-140", 63 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
