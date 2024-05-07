// Object to Interpolate between an incoming list and a stored list (the previous list)
// by Timothy Place
// Copyright © 2004, Electrotap L.L.C.


/*******************************************************************
 * SETUP
 *******************************************************************/

// GLOBALS
//var list_len = 0;			// length of the last list received
var list_out = new Array();		// the most recently transmitted list
var list_dest = new Array();	// the most recently transmitted list
var steps = new Array();	// the step size for each element of the list
var current_step;			// the current step in the interpolation process
var num_steps = 0;			// number of steps it takes to reach the destination

var ramp_time = 0;			// time in ms to ramp to the new list input
var granularity = 5.0;		// time between scheduler ticks in ms


// CONFIGURATION
inlets = 1;
outlets = 2;
var scheduler = new Task(tick, this);


// INITIALIZATION
function init()
{
	// check arguments using attribute-style parsing
	if(jsarguments.length > 2){
		if(jsarguments[1] == "@ramp_time"){
			ramp_time = jsarguments[2];
		}
	}

	// set up assistance strings
	setinletassist(0, "(list) input to interpolate to");
	setoutletassist(0,"(list) interpolated output");
	setoutletassist(1, "(toggle) flags high when interpolating, low when idle");
}
init.local = 1;		// hide the init function
init();				// run the init function


/*******************************************************************
 * METHODS
 *******************************************************************/
 
// process input (list) - this process *assumes* the list is numeric, no checking is done !!!
function list()
{
	if(scheduler.running){
		scheduler.cancel();
	}

	// First, size the last list to make it the same as the new input (if needed)
	if(arguments.length != list_out.length){
		if(arguments.length > list_out.length){
			for(var i = list_out.length; i < arguments.length; i++){
				list_out[i] = 0.0;
			}
		}
		else{	// new list must be shorter
			while(list_out.length > arguments.length){
				list_out.pop();
			}
		}
	}
	
	// Second, calculate the number of steps we have to work with
	num_steps = ramp_time / granularity;
	if(num_steps == 0){
		for(var i = 0; i<arguments.length; i++){
			list_out[i] = arguments[i];
		}
		outlet(0, list_out);
		return;	// exit the function
	}

	// Third, calculate the size of the steps to get to the new list
	for(var i = 0; i < arguments.length; i++){
		steps[i] = arguments[i] - list_out[i];
		steps[i] /= num_steps;
	}
	
	// Finally, copy the input to an array containing our destination
	for(var i = 0; i < arguments.length; i++){
		list_dest[i] = arguments[i];
	}

	// Now, we tell the scheduler to start doing the real work
	current_step = 0;					// reset our counter
	outlet(1, 1);
	scheduler.interval = granularity;	// set the time resolution
	scheduler.repeat();					// trigger the sequence of calculations
}


// Do the actual work of interpolation (scheduled)
function tick()
{
	for(var i = 0; i < list_out.length; i++){
		list_out[i] += steps[i];
	}
	outlet(0, list_out);
	current_step++;
	
	// evaluate the first item to see if we've reached our destination
	// correct for any errors when we reach this point...
	if(current_step > num_steps){
		scheduler.cancel();
		for(var i = 0; i < list_out.length; i++){
			list_out[i] = list_dest[i];
		}	
		outlet(0, list_out);
		outlet(1, 0);
	}
}
tick.local = 1;


// reset any problem areas
function clear()
{
	while(list.length){
		list.pop();
	}
	while(list_out.length){
		list_out.pop();
	}
	while(steps.length){
		steps.pop();
	}	
	var num_steps = 0;
}
