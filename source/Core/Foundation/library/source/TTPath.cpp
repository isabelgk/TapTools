/*
 * Jamoma support for interacting with paths (i.e. folders/directories) in the file system
 * Copyright © 2011, Timothy Place
 *
 * License: This code is licensed under the terms of the "New BSD License"
 * http://creativecommons.org/licenses/BSD/
 */

#include "TTPath.h"
#define BOOST_SYSTEM_NO_DEPRECATED 1
#include "boost/filesystem.hpp"
#undef BOOST_SYSTEM_NO_DEPRECATED
//using namespace boost::filesystem;

typedef std::vector<boost::filesystem::path> path_vec;

#define PATHOBJ ((boost::filesystem::path*)mPathObject)


TTPath::TTPath()
{
    mPathObject = (TTPtr) new boost::filesystem::path;
}

TTPath::TTPath(const TTString& aFolderPath)
{
    mPathObject = (TTPtr) new boost::filesystem::path(aFolderPath.c_str());
}

TTPath::TTPath(TTPtr aBoostPathObject)
{
    mPathObject = (TTPtr) new boost::filesystem::path;
    *PATHOBJ = *((boost::filesystem::path*)(aBoostPathObject));
}

TTPath::~TTPath()
{
    delete PATHOBJ;
}


// copy constructor
TTPath::TTPath(const TTPath& that)
{
    mPathObject = (TTPtr) new boost::filesystem::path;
    *PATHOBJ = *((boost::filesystem::path*)that.mPathObject);
}



TTBoolean TTPath::exists()
{
	return boost::filesystem::exists(*PATHOBJ);
}


TTBoolean TTPath::isDirectory()
{
    if (exists())
        return boost::filesystem::is_directory(*PATHOBJ);
    else
        return NO;
}


TTErr TTPath::getDirectoryListing(TTPathVector& returnedPaths)
{
    returnedPaths.clear();

    try {
        if (isDirectory()) {
            path_vec v;

            copy(boost::filesystem::directory_iterator(*PATHOBJ), boost::filesystem::directory_iterator(), back_inserter(v));
            sort(v.begin(), v.end());
            for (path_vec::const_iterator i = v.begin(); i != v.end(); ++i) {
                boost::filesystem::path		p = (*i);
                TTPath						pobj(&p);

                returnedPaths.push_back(pobj);
            }

            return kTTErrNone;
        }
        else
            return kTTErrInvalidType;
    }
    catch (const boost::filesystem::filesystem_error& ex) {
        TTLogError("TTPath::getDirectoryListing() failed %s", ex.what());
        return kTTErrGeneric;
    }
}


void TTPath::getString(TTString& pathString)
{
    pathString = (*PATHOBJ).string().c_str();
}


void TTPath::getStem(TTString& pathStemString)
{
	pathStemString = (*PATHOBJ).stem().string().c_str();
}


void TTPath::getExtension(TTString& pathExtensionString)
{
    pathExtensionString = (*PATHOBJ).extension().string().c_str();
}

