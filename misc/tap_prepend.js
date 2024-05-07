/* A whole bank of prepend objects in one
 * Written by Tim Place
 * Copyright © 2004, Electrotap L.L.C.
 */


/*******************************************************************
 * SETUP
 *******************************************************************/

// GLOBALS
var prepend_items = new Array();	// the items to prepend to the input


// CONFIGURATION
outlets = 1;


// INITIALIZATION
function init()
{
	inlets = jsarguments.length - 1;	// count arguments to see how many inlets to create

	for(var i=1; i<jsarguments.length; i++){
		setinletassist(i-1, "input to prepend with "+ jsarguments[i]);	// set up inlet assistance strings
		prepend_items[i-1] = jsarguments[i];
	}
	setoutletassist(0,"assembled output");
}
init.local = 1;		// hide the init function
init();				// run the init function


/*******************************************************************
 * METHODS
 *******************************************************************/
 
// Add a set() method here...
 
// process generic input
function anything()
{
	var value;
	
	value = arrayfromargs(messagename, arguments);
	value.unshift(prepend_items[inlet]);
	
	outlet(0, value);							// send it
}

