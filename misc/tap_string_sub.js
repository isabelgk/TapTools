/* 
 * String Substitution
 * written by Tim Place
 * Copyright © 2005, Electrotap L.L.C.
 *
 */


/*******************************************************************
 * SETUP
 *******************************************************************/

// GLOBALS
var attr_sub = "none";
var attr_search = "none";
var sub = new Array();
var search = new Array();
var re = new RegExp(attr_sub, "g");


// CONFIGURATION
inlets = 1;
outlets = 1;


// INITIALIZATION
function init()
{
	// set up assistance strings
	setinletassist(0, "input");
	setoutletassist(0,"resulting symbol");	
}
init.local = 1;		// hide the init function
init();				// run the init function


/*******************************************************************
 * METHODS
 *******************************************************************/
 

function searchstring()
{
	search = arrayfromargs(arguments);
	attr_search = search.join(" ");
	re.compile(attr_search, "g");
}


function substring()
{
	sub = arrayfromargs(arguments);
	attr_sub = sub.join(" ");
}
 

function anything()
{
	var input = new Array();
	var mystring;
	
	input[0] = messagename;
	for(var i = 0; i < arguments.length; i++){
		input[i+1] = arguments[i];
	}
	mystring = input.join(" ");
	
	var out = mystring.replace(re, attr_sub);
	outlet(0, out);
}


