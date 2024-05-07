TAPTOOLS
By Timothy Place
http://74objects.com/

Version 4 beta 2
26 April 2013


Distribution of TapTools is governed by the New BSD License ( http://opensource.org/licenses/BSD-3-Clause )  


INSTALLING

1. The TapTools package is a folder that can be placed into either of the following locations:

	- /Applications/Max 6.1/packages/
	- ~/Documents/Max/Packages
	
	(where ~ means your home folder)


SYSTEM REQUIREMENTS

Max 6.1.1 or higher for Mac OS


CONTACT

Please submit bug reports and suggestions to https://github.com/tap/TapTools/issues


SPECIAL THANKS

TapTools stands on the shoulders of some excellent open source software projects, including Jamoma and ObjectiveMax.  Additional thanks to the following.

- Jeremy Bernstein for help with tap.applescript, tap.myip, tap.windowdrag, and others.
- Joshua Kit Clayton for help with numerous objects.
- David Zicarelli for help with the original tap.comb~, tap.buildassist, and many others.
- Darwin Grosse for various suggestions including the tap.plug.* series.
- Ali Momeni, for whom tap.jit.ali is named, provided interpolation algorithms, feedback, and testing.
- Stephan Moore for help with the tap.anticlick~ object.
- Russell Pinkston for the idea of making tap.random~ and tap.counter~.
- jhno for his support of the augmenting of his limi~ algorithm, and help with tap.windowdrag.
- Richard Dudas for feedback on filter externs, color conversion, and buffer binding objects as well as providing the code and permission to use the code for the list.join and list.slice externals.
- Trond Lossius for various contributions and feedback.
- Dave Watson and Rob Sussman for tremendous amounts of help in assisting with the Windows version of TapTools.
- Andrew Pask for various testing and feedback.
- Nils Peters for extensive feedback and assistance.


KNOWN ISSUES

- tap.myip does not as reliably return your IP address as it once did.
- tap.jit.ali harmlessly posts "warning: attempting to allocate matrix with unknown type" to the Max window the first time it is created.


CHANGE LOG

New In TapTools 4 beta 2
- Compatibility with Max 6.1
- Distributed as a Max Package
- Jamoma dependencies, where applicable, are provided as a part of the package mechanism
- Lots of bug fixes from beta 1

New In TapTools 4 beta 1
- All code is now open source and available via GitHub
- It is no longer required that you enter a registration number for authorization
  (TapTools now operates on the honor system -- if you derive benefit from TapTools then please support it with a donation)
- An installer is no longer provided
- Requisite Jamoma dependencies, where applicable, are no longer provided (you must install Jamoma to use these objects)


New In TapTools 3.6.4
- Fixed problems with TapTools 3.6.x not functioning in Max for Live
- Fixed problems with conflicts between TapTools 3.6 and Jamoma 0.5.6 if both are installed
- tap.noise~: fix bugs with listing of modes in attrui and the inspector

New In TapTools 3.6.3
- Fixed problem loading some objects such as tap.adsr~

New In TapTools 3.6.2
- installer is now code-signed for Apple Gate Keeper compatibility
- tap.filter~: now has a default 'mode' defined (lowpass)
- tap.noise~: new 'gaussian' noise option available
- tap.limiter~: fixed ancient reference to tap.auto_thru~
- tap.folder: fixed problems with including the external in standalone applications

New In TapTools 3.6.1
- Fixed problems loading objects if Jamoma was not also installed

New In TapTools 3.6
- Audio objects updated to use new native 64-bit audio samples in Max 6
- Installer updated to install for Max 6
- tap.noise~: improved algorithm quality
- Some ancient objects dropped from the distribution:
	- tap.average~ (use average~)
	- tap.degrade~ (use degrade~)
- Some objects deprecated  (not recommended for use, they will eventually be discontinued)
	- tap.xml.sax (use the XmlParse class for the mxj object, which is now a part of Max 6)

New In TapTools 3.2
- Added an "Uninstall" script to make it easy to remove TapTools
- Fixes problems with TapTools in the Max-for-Live environment
- tap.filter~: added a myriad of new filter types

New In TapTools 3.1
- Installs of TapTools are now completely untethered from Jamoma, resolving all dependency issues
- Object modularization: rather than a monolithic "tap.tools" extension, each object is defined as a separate Max external binary:
	- eases standalone building and dependencies
	- reduces problems in the event of unexpected binary/library conflict
	- more easily supports Max-For-Live
- Some objects deprecated  (not recommended for use, they will eventually be discontinued)
	- tap.myip
	- tap.typecheck (you can use tap.typecheck~ instead, even if you don't have MSP)
	- tap.fourpole~ (use tap.filter~)
	- tap.twopole~ (use tap.filter~)
	- tap.onepole~ (use tap.filter~)
	- tap.average~ (use average~)
- tap.svn: object dropped from the distribution
- Dropped support for PPC processors
- Dropped support for Windows
- Documentation fixes and updates
- tap.applescript: fix for bug where a bang was not properly sent from the second outlet when the file read was complete.
- tap.allpass~: now supports multichannel processing and internal 64-bit operation

New In TapTools 3.0.2
- Compatibility with Jamoma 0.5

New In TapTools 3.0.1
- Performance and reliability improvements to all objects referencing buffers (e.g. tap.buffer.peak~, tap.buffer.snap~, etc.).
- Eliminated external dependency on the MaxObject framework (now linked internally)
- Updated to use a version of the Jamoma ../../../../../Jamoma/Core/DSP/library/build/JamomaDSP.dylib Library that is compatible with the latest versions of Jamoma
- tap.jit.ali help patcher fixes, now fits on a MacBook screen (submitted by Nils Peters)
- various help patcher and documentation fixes and updates (including fixed usage of named-default colors)
- tap.sustain~: now documents the ‘buffer clear’ message
- tap.sustain~: fixed divide by zero error when created with no arguments.
- tap.sustain references now indicate the time units required for the object's attributes.
- tap.svf~: fixed the ‘clear’ message (was missing in the initial 3.0 release)
- tap.svf~: internally limits the range of the frequencies used to control the lfo so that it doesn't blow up.
- object descriptions are now displayed in the file browser (requires Max 5.0.6 or higher)
- tap.jit.delay: no longer posts ugly (but harmless) error messages to the Max window upon instantiation
- tap.jit.delay help patcher: fixed bug where delay times were not calculated (thanks to Nils Peters for submitting the fix)
- taptools will now set the default state of sortpatcherdictonsave feature of Max to true (requires Max 5.0.6 or higher)
- tap.filter: a list of available filters is now in a drop-down menu when using the object's inspector.
- tap.crossfade is now multichannel capable - the first argument to the object defines the number of channels on which to operate.
- multichannel objects are now identified (and searchable) in the file browser with the 'Multichannel' tag.
- added missing documentation for the tap.rotate object
- new object: tap.inquisitor - accesses the attribute values of another named object

New In TapTools 3.0
- New feature that helps guarantee Max will load the correct version of an object, and not find old/conflicting versions the object(s) in your searchpath.
- New TapTools Builder (in the Extras menu) to make it easier to build standalones (standalone building requires a Pro license)
- Many audio objects are now using 64-bit processing internally
- Many MSP objects are capable of flexible multichannel processing:
	tap.dcblock~
	tap.filter~
	tap.overdrive~
	etc.
- All objects now have full reference pages, fully integrated with Max 5's searchable documentation system.
- All objects are integrated with Max 5's filebrowser and metadata support.
- All objects are integrated and make use of Max 5's advanced inspector features.
- All help patchers and abstractions re-crafted
- All external objects re-structured into a single extension that is loaded by Max at launch time.
- Where possible, all Java and JavaScript objects have been re-written as proper Max objects.  This eliminates the dependency on Java which is not standard on Windows.
- Integration with the new Max Window in Max 5
- Integration with the new way the buffer~ object works in Max 5
- Improved object load time performance
- Various optimizations
- New Objects
	tap.filter~: flexible multichannel filter with dynamic filter types
	tap.filecontainer: create a custom file format that bundles together many files into a single file.
	tap.folder: perform operations such as creating, deleting, copying, etc. folders in the filesystem.
	tap.svn: subversion source control management client (see http://collab.net/subversion/) for more information.
- tap.applescript rewritten to be much more reliable and flexible; has a new script method for specifying new scripts on the fly.
- tap.windowdrag re-written to be Max 5-savvy and is also now available on Windows.
- new feature: tap.applescript is no longer copy protected. It will work after the demo expires, and will work in standalones without a ‘pro’ license
- Miscellaneous bug fixes, memory leaks, etc.
- Fixed exponential mode not working in tap.pulsesub~
- Misc bugs fixed in tap.decay_calc
- Fixed crashes in tap.phasor~
- Fixed problems where tap.limi~ didn't actually limit the signal
- Fixed crashes in tap.pulsesub~
- The tap.adapt~ object no longer posts spurious (and harmless) messages to the Max window
- Fixed crash in tap.delay~  when setting the delay time higher than the buffer size.
- Added the "T" key command when patching, creates a new object box with "tap." already entered.
- All objects: the colored ring around the inlet where the mouse is hovering is now colored blue if the inlet is cold (doesn't trigger output).
- All objects: the inspectors now have switches where an attribute is toggle, or menus where the attribute is one of a list of choices
- fixed the broken backwards-compatibility alias for tap.lp-comb -> tap.comb
- fixed outlet assistance for tap.sieve
- various stability and efficiency improvements

_______________________

Copyright © 1999-2013
74 Objects LLC
