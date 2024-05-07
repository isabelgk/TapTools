{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 6,
			"minor" : 0,
			"revision" : 7
		}
,
		"rect" : [ 100.0, 100.0, 537.0, 465.0 ],
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 11.0,
		"default_fontface" : 0,
		"default_fontname" : "Verdana",
		"gridonopen" : 0,
		"gridsize" : [ 5.0, 5.0 ],
		"gridsnaponopen" : 0,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"boxes" : [ 			{
				"box" : 				{
					"id" : "obj-69",
					"maxclass" : "bpatcher",
					"name" : "tap.badge.maxpat",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patching_rect" : [ 223.0, 381.0, 225.0, 67.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"frozen_object_attributes" : 					{
						"helpfontname" : "Verdana",
						"helpfontsize" : 10.0
					}
,
					"hidden" : 1,
					"id" : "obj-52",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 15.0, 389.0, 54.0, 19.0 ],
					"text" : "autohelp"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 1,
					"fontname" : "Arial",
					"fontsize" : 11.595187,
					"frgb" : 0.0,
					"id" : "obj-66",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 20.0, 393.0, 100.0, 19.0 ],
					"text" : "See Also:",
					"varname" : "autohelp_see_title"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 11.595187,
					"id" : "obj-67",
					"items" : [ "(Objects:)", ",", "pfft~", ",", "<separator>" ],
					"maxclass" : "umenu",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 20.0, 413.0, 130.0, 19.0 ],
					"varname" : "autohelp_see_menu"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"hidden" : 1,
					"id" : "obj-8",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 267.0, 246.0, 54.0, 19.0 ],
					"text" : "loadbang"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"frgb" : 0.0,
					"id" : "obj-9",
					"linecount" : 9,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 4.0, 133.0, 140.0, 116.0 ],
					"text" : "This object remaps spectra information from various FFT bins to others. It takes some experimenting to find the different effects on differing sound inputs. Start with the presets below..."
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"frgb" : 0.0,
					"id" : "obj-10",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 392.0, 166.0, 63.0, 19.0 ],
					"text" : "Dest. Low"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"frgb" : 0.0,
					"id" : "obj-11",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 460.0, 166.0, 61.0, 19.0 ],
					"text" : "Dest. High"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"frgb" : 0.0,
					"id" : "obj-12",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 324.0, 166.0, 74.0, 19.0 ],
					"text" : "Source High"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"id" : "obj-13",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 194.0, 345.0, 70.0, 17.0 ],
					"text" : "startwindow"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"frgb" : 0.0,
					"id" : "obj-14",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 255.0, 166.0, 97.0, 19.0 ],
					"text" : "Source Low"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.407843, 0.407843, 0.407843, 1.0 ],
					"coldcolor" : [ 0.0, 0.658824, 0.0, 1.0 ],
					"id" : "obj-15",
					"interval" : 100,
					"maxclass" : "meter~",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"patching_rect" : [ 233.0, 225.0, 14.0, 100.0 ],
					"tepidcolor" : [ 0.6, 0.729412, 0.0, 1.0 ],
					"warmcolor" : [ 0.85098, 0.85098, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-16",
					"maxclass" : "gain~",
					"numinlets" : 2,
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 145.0, 225.0, 22.0, 100.0 ]
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.407843, 0.407843, 0.407843, 1.0 ],
					"coldcolor" : [ 0.0, 0.658824, 0.0, 1.0 ],
					"id" : "obj-17",
					"interval" : 100,
					"maxclass" : "meter~",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"patching_rect" : [ 211.0, 225.0, 14.0, 100.0 ],
					"tepidcolor" : [ 0.6, 0.729412, 0.0, 1.0 ],
					"warmcolor" : [ 0.85098, 0.85098, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.407843, 0.407843, 0.407843, 1.0 ],
					"coldcolor" : [ 0.0, 0.658824, 0.0, 1.0 ],
					"id" : "obj-18",
					"interval" : 100,
					"maxclass" : "meter~",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"patching_rect" : [ 167.0, 225.0, 14.0, 100.0 ],
					"tepidcolor" : [ 0.6, 0.729412, 0.0, 1.0 ],
					"warmcolor" : [ 0.85098, 0.85098, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"id" : "obj-19",
					"items" : [ 0, "-", "No", "Preset", ",", 1, "-", "Unmodified", "Spectrum", ",", 2, "-", "compressed", "Spectrum", ",", "3-", "Expanded", "Spectrum", ",", 4, "-", "Spectrum", "shifted", "up", ",", 5, "-", "Inverted", "Spectrum", "shifted", "way", "way", "down", ",", 6, "-", "Not", "Really", "sure", "what", "this", "is...", ",", 7, "-", "Inverted", "Spectrum", "shifted", "down", ",", 8, "-", "Inverted", "Spectrum", ",", 9, "-", "goofing", "around", 1, ",", 10, "-", "goofing", "around", 2, ",", 11, "-", "goofing", "around", 3, ",", 12, "-", "goofing", "around", 4 ],
					"labelclick" : 1,
					"maxclass" : "umenu",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 255.0, 226.0, 253.0, 19.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-20",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 205.0, 135.0, 15.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"id" : "obj-21",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 205.0, 155.0, 46.0, 17.0 ],
					"text" : "loop $1"
				}

			}
, 			{
				"box" : 				{
					"bubblesize" : 8,
					"fontsize" : 12.754706,
					"hidden" : 1,
					"id" : "obj-22",
					"margin" : 4,
					"maxclass" : "preset",
					"numinlets" : 1,
					"numoutlets" : 4,
					"outlettype" : [ "preset", "int", "preset", "int" ],
					"patching_rect" : [ 313.0, 281.0, 66.0, 27.0 ],
					"preset_data" : [ 						{
							"number" : 1,
							"data" : [ 5, "obj-29", "toggle", "int", 1, 6, "obj-27", "gain~", "list", 118, 10.0, 5, "obj-26", "flonum", "float", 0.0, 5, "obj-25", "flonum", "float", 1024.0, 5, "obj-24", "flonum", "float", 0.0, 5, "obj-23", "flonum", "float", 1024.0, 5, "obj-20", "toggle", "int", 1 ]
						}
, 						{
							"number" : 2,
							"data" : [ 5, "obj-29", "toggle", "int", 1, 6, "obj-27", "gain~", "list", 157, 10.0, 5, "obj-26", "flonum", "float", 0.0, 5, "obj-25", "flonum", "float", 512.0, 5, "obj-24", "flonum", "float", 0.0, 5, "obj-23", "flonum", "float", 108.0, 5, "obj-20", "toggle", "int", 1 ]
						}
, 						{
							"number" : 3,
							"data" : [ 5, "obj-29", "toggle", "int", 1, 6, "obj-27", "gain~", "list", 148, 10.0, 5, "obj-26", "flonum", "float", 0.0, 5, "obj-25", "flonum", "float", 512.0, 5, "obj-24", "flonum", "float", 0.0, 5, "obj-23", "flonum", "float", 8196.0, 5, "obj-20", "toggle", "int", 1 ]
						}
, 						{
							"number" : 4,
							"data" : [ 5, "obj-29", "toggle", "int", 1, 6, "obj-27", "gain~", "list", 123, 10.0, 5, "obj-26", "flonum", "float", 0.0, 5, "obj-25", "flonum", "float", 512.0, 5, "obj-24", "flonum", "float", 100.0, 5, "obj-23", "flonum", "float", 612.0, 5, "obj-20", "toggle", "int", 1 ]
						}
, 						{
							"number" : 5,
							"data" : [ 5, "obj-29", "toggle", "int", 1, 6, "obj-27", "gain~", "list", 145, 10.0, 5, "obj-26", "flonum", "float", 0.0, 5, "obj-25", "flonum", "float", 512.0, 5, "obj-24", "flonum", "float", 12.0, 5, "obj-23", "flonum", "float", -500.0, 5, "obj-20", "toggle", "int", 1 ]
						}
, 						{
							"number" : 6,
							"data" : [ 5, "obj-29", "toggle", "int", 1, 6, "obj-27", "gain~", "list", 141, 10.0, 5, "obj-26", "flonum", "float", 32.0, 5, "obj-25", "flonum", "float", 0.0, 5, "obj-24", "flonum", "float", 0.0, 5, "obj-23", "flonum", "float", 32.0, 5, "obj-20", "toggle", "int", 1 ]
						}
, 						{
							"number" : 7,
							"data" : [ 5, "obj-29", "toggle", "int", 1, 6, "obj-27", "gain~", "list", 133, 10.0, 5, "obj-26", "flonum", "float", 0.0, 5, "obj-25", "flonum", "float", 512.0, 5, "obj-24", "flonum", "float", 256.0, 5, "obj-23", "flonum", "float", -256.0, 5, "obj-20", "toggle", "int", 1 ]
						}
, 						{
							"number" : 8,
							"data" : [ 5, "obj-29", "toggle", "int", 1, 6, "obj-27", "gain~", "list", 138, 10.0, 5, "obj-26", "flonum", "float", 0.0, 5, "obj-25", "flonum", "float", 512.0, 5, "obj-24", "flonum", "float", 512.0, 5, "obj-23", "flonum", "float", 0.0, 5, "obj-20", "toggle", "int", 1 ]
						}
, 						{
							"number" : 9,
							"data" : [ 5, "obj-29", "toggle", "int", 1, 6, "obj-27", "gain~", "list", 157, 10.0, 5, "obj-26", "flonum", "float", 184.0, 5, "obj-25", "flonum", "float", 151.0, 5, "obj-24", "flonum", "float", 11.039988, 5, "obj-23", "flonum", "float", 11.57997, 5, "obj-20", "toggle", "int", 1 ]
						}
, 						{
							"number" : 10,
							"data" : [ 5, "obj-29", "toggle", "int", 1, 6, "obj-27", "gain~", "list", 157, 10.0, 5, "obj-26", "flonum", "float", 184.0, 5, "obj-25", "flonum", "float", 293.0, 5, "obj-24", "flonum", "float", 9.0, 5, "obj-23", "flonum", "float", 10.0, 5, "obj-20", "toggle", "int", 1 ]
						}
, 						{
							"number" : 11,
							"data" : [ 5, "obj-29", "toggle", "int", 1, 6, "obj-27", "gain~", "list", 157, 10.0, 5, "obj-26", "flonum", "float", 222.0, 5, "obj-25", "flonum", "float", 226.0, 5, "obj-24", "flonum", "float", 55.0, 5, "obj-23", "flonum", "float", 239.0, 5, "obj-20", "toggle", "int", 1 ]
						}
, 						{
							"number" : 12,
							"data" : [ 5, "obj-29", "toggle", "int", 1, 6, "obj-27", "gain~", "list", 157, 10.0, 5, "obj-26", "flonum", "float", 304.0, 5, "obj-25", "flonum", "float", 226.0, 5, "obj-24", "flonum", "float", 55.0, 5, "obj-23", "flonum", "float", 239.0, 5, "obj-20", "toggle", "int", 1 ]
						}
 ],
					"spacing" : 2
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"id" : "obj-23",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 460.0, 179.0, 47.0, 19.0 ],
					"triscale" : 0.9
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"id" : "obj-24",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 392.0, 179.0, 47.0, 19.0 ],
					"triscale" : 0.9
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"id" : "obj-25",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 324.0, 179.0, 47.0, 19.0 ],
					"triscale" : 0.9
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"id" : "obj-26",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 256.0, 179.0, 47.0, 19.0 ],
					"triscale" : 0.9
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-27",
					"maxclass" : "gain~",
					"numinlets" : 2,
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 189.0, 225.0, 22.0, 100.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-28",
					"local" : 1,
					"maxclass" : "ezdac~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 149.0, 337.0, 33.0, 33.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-29",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 180.0, 140.0, 15.0, 15.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"id" : "obj-30",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 145.0, 110.0, 102.0, 17.0 ],
					"text" : "open drumloop.aif"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"id" : "obj-31",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "bang" ],
					"patching_rect" : [ 188.0, 175.0, 46.0, 19.0 ],
					"save" : [ "#N", "sfplay~", "", 1, 40320, 0, "", ";" ],
					"text" : "sfplay~"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"id" : "obj-32",
					"maxclass" : "newobj",
					"numinlets" : 5,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 188.0, 200.0, 284.0, 19.0 ],
					"text" : "tap.spectra~"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"frgb" : 0.0,
					"id" : "obj-33",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 322.0, 249.0, 100.0, 19.0 ],
					"text" : "Try some presets!"
				}

			}
, 			{
				"box" : 				{
					"background" : 1,
					"bgcolor" : [ 0.85, 0.85, 0.85, 0.75 ],
					"border" : 2,
					"bordercolor" : [ 0.5, 0.5, 0.5, 0.75 ],
					"id" : "obj-68",
					"maxclass" : "panel",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 15.0, 389.0, 140.0, 50.0 ],
					"varname" : "autohelp_see_panel"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 3,
					"fontname" : "Arial",
					"fontsize" : 20.871338,
					"frgb" : 0.0,
					"id" : "obj-2",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 10.0, 8.0, 485.0, 30.0 ],
					"text" : "tap.spectra~",
					"textcolor" : [ 0.2, 0.2, 0.2, 1.0 ],
					"varname" : "autohelp_top_title"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.754705,
					"frgb" : 0.0,
					"id" : "obj-3",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 10.0, 36.0, 485.0, 21.0 ],
					"text" : "Spectral remapping",
					"textcolor" : [ 0.2, 0.2, 0.2, 1.0 ],
					"varname" : "autohelp_top_digest"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"frgb" : 0.0,
					"id" : "obj-4",
					"linecount" : 3,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 10.0, 57.0, 477.0, 43.0 ],
					"text" : "The tap.spectra~ object performs a remapping of spectral data by reorienting the bins of an FFT to different bins of an IFFT. The result is an ultra-non-linear effect that can yield unexpected results.",
					"varname" : "autohelp_top_description"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"bgoncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"bgovercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"bgoveroncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"bordercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontface" : 3,
					"fontlink" : 1,
					"fontname" : "Arial",
					"fontsize" : 12.754705,
					"id" : "obj-5",
					"maxclass" : "textbutton",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "", "", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 321.511688, 22.0, 178.488312, 14.249397 ],
					"presentation_rect" : [ 0.0, 0.0, 178.488312, 14.249397 ],
					"spacing_x" : 0.0,
					"spacing_y" : 0.0,
					"text" : "open tap.spectra~ reference",
					"textcolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"textoncolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"textovercolor" : [ 0.4, 0.5, 0.65, 1.0 ],
					"underline" : 1,
					"varname" : "autohelp_top_ref"
				}

			}
, 			{
				"box" : 				{
					"background" : 1,
					"grad1" : [ 0.88, 0.98, 0.78, 1.0 ],
					"grad2" : [ 0.9, 0.9, 0.9, 1.0 ],
					"id" : "obj-6",
					"maxclass" : "panel",
					"mode" : 1,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 5.0, 5.0, 495.0, 52.0 ],
					"varname" : "autohelp_top_panel"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-28", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-13", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-28", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-16", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-28", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-16", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-22", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-19", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-21", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-20", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-21", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-19", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"midpoints" : [ 353.833344, 320.0, 251.0, 320.0, 251.0, 222.0, 264.5, 222.0 ],
					"source" : [ "obj-22", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 4 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-23", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 3 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-24", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 2 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-25", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-26", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"midpoints" : [ 198.0, 332.0, 230.0, 332.0, 230.0, 219.0, 219.0, 219.0 ],
					"source" : [ "obj-27", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-28", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-27", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-28", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-27", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-29", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-30", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-31", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-18", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-31", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-31", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-15", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-32", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-27", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-32", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-19", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-8", 0 ]
				}

			}
 ],
		"dependency_cache" : [ 			{
				"name" : "tap.spectra~.maxpat",
				"bootpath" : "/Applications/Max6/Cycling '74/TapTools/TapTools Abstractions",
				"patcherrelativepath" : "../TapTools Abstractions",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "tap.spectra.pfft.maxpat",
				"bootpath" : "/Applications/Max6/Cycling '74/TapTools/TapTools Help",
				"patcherrelativepath" : "",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "requirements.txt",
				"bootpath" : "/Users/tim/Library/Caches",
				"patcherrelativepath" : "../../../../../Users/tim/Library/Caches",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "tap.badge.maxpat",
				"bootpath" : "/Applications/Max6/Cycling '74/TapTools/TapTools Help",
				"patcherrelativepath" : "",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "74objects-logo128.png",
				"bootpath" : "/Applications/Max6/Cycling '74/TapTools/TapTools Help",
				"patcherrelativepath" : "",
				"type" : "PNG ",
				"implicit" : 1
			}
, 			{
				"name" : "tap.scale~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "tap.typecheck~.mxo",
				"type" : "iLaX"
			}
 ]
	}

}
