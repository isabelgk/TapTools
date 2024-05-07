/* 
 *	External object for Max/MSP
 *	Copyright © 2008 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

/*	TO DO
	- Implement the 'sync' method so that files that are updated in the temp folder
		are then updated in the container.
	- Should adding files and extracting them be run in another thread? Probably...
	- Doesn't currently handle mxo externs (i.e. bundles)
*/

#include "TTClassWrapperMax.h"

#define TABLEDEF_FILES \
	"CREATE TABLE files ( \
		file_id			INTEGER PRIMARY KEY NOT NULL, \
		filename		VARCHAR(256), \
		moddate			DATETIME, \
		content			BLOB,\
		valid			INTEGER\
	)"
					
#define TABLEDEF_ATTRS \
	"CREATE TABLE attrs (\
		attr_id			INTEGER PRIMARY KEY NOT NULL, \
		file_id_ext		INTEGER,\
		attr_name		VARCHAR(256), \
		attr_value		VARCHAR(256) \
	)"

#define SQL0(sql,result) \
	object_method(x->sqlite, _sym_execstring, sql, result);
#define SQL1(sql, arg, result) \
	snprintf(s_tempstr, 512, sql, arg); \
	object_method(x->sqlite, _sym_execstring, s_tempstr, result);
#define SQL2(sql, arg1, arg2, result) \
	snprintf(s_tempstr, 512, sql, arg1, arg2); \
	object_method(x->sqlite, _sym_execstring, s_tempstr, result);
#define SQL3(sql, arg1, arg2, arg3, result) \
	snprintf(s_tempstr, 512, sql, arg1, arg2, arg3); \
	object_method(x->sqlite, _sym_execstring, s_tempstr, result);
		
	
/*	Attribute Examples
	attr_name = platform, attr_value = Windows		// don't unpack this file on the Mac
	attr_name = filetype, attr_value = PICT			// set the type correctly when unpacking
*/	


// Data Structure for this object
typedef struct _filecontainer{
	t_object		ob;
	void			*dumpout;		// outlet
	void			*sqlite;		// sqlite object instance
	t_symbol		*name;			// name of the database file
	short			temp_path;		// path to the unpacked files for this container
	t_symbol		*temp_fullpath;	//  ...	
} t_filecontainer;


// Prototypes for methods: need a method for each incoming message type:
void *filecontainer_new(t_symbol *msg, short argc, t_atom *argv);
void filecontainer_free(t_filecontainer *x);
void filecontainer_gettemppath(t_filecontainer *x);
void filecontainer_assist(t_filecontainer *x, void *b, long msg, long arg, char *dst);

void filecontainer_sql(t_filecontainer *x, t_symbol *arg);
void filecontainer_open(t_filecontainer *x, t_symbol *arg);
void filecontainer_doopen(t_filecontainer *x, t_symbol *arg);
void filecontainer_close(t_filecontainer *x);

void filecontainer_addfile(t_filecontainer *x, t_symbol *arg);
void filecontainer_doaddfile(t_filecontainer *x, t_symbol *arg);
void filecontainer_getfilenames(t_filecontainer *x);
void filecontainer_getnumfiles(t_filecontainer *x);
void filecontainer_removefile(t_filecontainer *x, t_symbol *arg);
void filecontainer_getfilepath(t_filecontainer *x, t_symbol *arg);

void filecontainer_addfileattr(t_filecontainer *x, t_symbol *msg, long argc, t_atom *argv);
void filecontainer_removefileattr(t_filecontainer *x, t_symbol *msg, long argc, t_atom *argv);
void filecontainer_getnumfileattrs(t_filecontainer *x, t_symbol *arg);
void filecontainer_getfileattrnames(t_filecontainer *x, t_symbol *arg);
void filecontainer_getfileattrvalue(t_filecontainer *x, t_symbol *msg, long argc, t_atom *argv);


// Class Globals
static t_class 	*s_filecontainer_class;
static t_symbol	*ps_getblob,
				*ps_setblob,
				*ps_getlastinsertid,
				*ps_starttransaction,
				*ps_endtransaction;
static char		s_tempstr[512];


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;
	
	c = class_new("tap.filecontainer", (method)filecontainer_new, (method)filecontainer_free, sizeof(t_filecontainer), (method)0L, A_GIMME, 0);
		
	common_symbols_init();

	// MESSAGES:
	class_addmethod(c, (method)filecontainer_addfile,			"addfile",			A_DEFSYM, 0L);
	class_addmethod(c, (method)filecontainer_removefile,		"removefile",		A_SYM, 0L);
	class_addmethod(c, (method)filecontainer_getnumfiles,		"getnumfiles",		0L);
	class_addmethod(c, (method)filecontainer_getfilenames,		"getfilenames",		0L);
	class_addmethod(c, (method)filecontainer_getfilepath,		"getfilepath",		A_SYM, 0L);

	class_addmethod(c, (method)filecontainer_addfileattr,		"addfileattr",		A_GIMME, 0L);
	class_addmethod(c, (method)filecontainer_removefileattr,	"removefileattr",	A_GIMME, 0L);
	class_addmethod(c, (method)filecontainer_getnumfileattrs,	"getnumfileattrs",	A_SYM, 0L);
	class_addmethod(c, (method)filecontainer_getfileattrnames,	"getfileattrnames",	A_SYM, 0L);
	class_addmethod(c, (method)filecontainer_getfileattrvalue,	"getfileattrvalue",	A_GIMME, 0L);

	class_addmethod(c, (method)filecontainer_sql,				"sql",				A_SYM, 0L);
	class_addmethod(c, (method)filecontainer_open,				"open",				A_DEFSYM, 0L);	//	create a cache of temp files
	class_addmethod(c, (method)filecontainer_close,				"close",			0L);			//	remove temp files
//	class_addmethod(c, (method)filecontainer_spoolfile,			"sync",				A_GIMME, 0L);	//	sync temp files and the db based on moddates

	class_addmethod(c, (method)filecontainer_assist,			"assist",			A_CANT, 0L); 
	class_addmethod(c, (method)stdinletinfo,					"inletinfo",		A_CANT, 0);

	// Finalize our class
class_register(_sym_box, c); 	s_filecontainer_class = c;

	ps_getblob				= gensym("getblob");
	ps_setblob				= gensym("setblob");
	ps_getlastinsertid		= gensym("getlastinsertid");
	ps_starttransaction		= gensym("starttransaction");
	ps_endtransaction		= gensym("endtransaction");
}
	

/************************************************************************************/
// Object Creation Method

void *filecontainer_new(t_symbol *msg, short argc, t_atom *argv)
{
	t_filecontainer		*x;

	x = (t_filecontainer *)object_alloc(s_filecontainer_class);;
	if(x){
		x->sqlite = NULL;
		x->dumpout = outlet_new(x,NULL);
		object_obex_store((void *)x, _sym_dumpout, (object *)x->dumpout);	   	
		attr_args_process(x, argc, argv);
	}
 	return (x);
}


void filecontainer_free(t_filecontainer *x)
{
	if(x->sqlite)
		filecontainer_close(x);
}


void filecontainer_gettemppath(t_filecontainer *x)
{
	char			temppath[512];
	char			outpath[512];
	t_symbol		*unique = symbol_unique();
	OSErr			err = 0;
#ifdef MAC_VERSION
	FSRef			folderref, newref;
	UniChar			uni[512];
	unsigned short	i;

	err = FSFindFolder(kUserDomain, kTemporaryFolderType, kCreateFolder, &folderref);
	err = FSRefMakePath(&folderref, (UInt8 *)temppath, 511);
	strcat(temppath, "/");
#else // WIN_VERSION
	GetTempPath(512, (LPSTR)temppath);
#endif
	strcat(temppath, unique->s_name);
	
#ifdef MAC_VERSION
	for(i=0; i<strlen(unique->s_name); i++)					// Convert from 8-bit ASCII to 16-bit Unicode
		uni[i] = unique->s_name[i];

	err = FSCreateDirectoryUnicode(&folderref, strlen(unique->s_name), uni,
			kFSCatInfoNone, NULL, &newref, NULL, NULL);
#else // WIN_VERSION
	CreateDirectory((LPCSTR)temppath, NULL);
#endif	

	path_nameconform(temppath, outpath, PATH_STYLE_MAX, PATH_TYPE_ABSOLUTE);
	x->temp_fullpath = gensym(outpath);
	path_frompathname(outpath, &x->temp_path, temppath);	// re-using temppath since we don't need it anymore
	
	// Add to the searchpath
	path_addnamed(SEARCH_PATH, outpath, 1, 0);
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void filecontainer_assist(t_filecontainer *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1){ 		// Inlets
		switch(arg){
			case 0: strcpy(dst, "in"); break;
		}
	}
	else if(msg==2){ 	// Outlets
		strcpy(dst, "dumpout");
	}
}


void filecontainer_open(t_filecontainer *x, t_symbol *arg)
{
	if(x->name == arg)		// already open
		return;
	if(x->sqlite)			// a different container is open, close it first.
		filecontainer_close(x);
		
	defer(x, (method)filecontainer_doopen, arg, 0, 0L);
}

void filecontainer_doopen(t_filecontainer *x, t_symbol *arg)
{
	t_atom			a[4];
	int				err = 0;
	char			filename[256];
	short			path;
	t_fourcc		outtype = 0;
	t_fourcc		type = 'cO0p';
#ifdef MAC_VERSION
	char			*temppath;
	FSRef			ref;
	Boolean			isDir;
	FSCatalogInfo	catalogInfo;
#else // WIN_VERSION
	char			temppath[512];
#endif	
	char			fullpath[512];
	t_object		*result = NULL;
	t_object		*result2 = NULL;
	char			**record = NULL;		// sqlite records
	char			**record2 = NULL;		// sqlite records
	t_filehandle	file_handle;
	t_ptr_size		len = 0;
	char			*blob;
	char			sql[512];
		
	if(!arg || !arg->s_name[0]){
		if(open_dialog(filename, &path, &outtype, NULL, -1))			// Returns 0 if successful
			return;										
	}
	else{
		t_fourcc typelist[1];
		typelist[0] = 'cO0p';
		strcpy(filename, arg->s_name);
		path = 0;
		locatefile_extended(filename, &path, &type, typelist, 0);
	}
	path_topotentialname(path, filename, fullpath, 0);
#ifdef MAC_VERSION
	temppath = strchr(fullpath, ':');
	temppath += 1;
#else // WIN_VERSION
	path_nameconform(fullpath, temppath, PATH_STYLE_NATIVE_WIN, PATH_TYPE_ABSOLUTE);
#endif
	x->name = gensym(temppath);
	
	// Create our temp folder for extracted files
	filecontainer_gettemppath(x);

	// Create the SQLite instance
	atom_setsym(&a[0], gensym("@rambased"));
	atom_setlong(&a[1], 0);
	atom_setsym(&a[2], gensym("@db"));
	atom_setsym(&a[3], x->name);
	x->sqlite = object_new_typed(CLASS_NOBOX, _sym_sqlite, 4, a);

	// Operate on the open DB
	if(x->sqlite){
		object_method(x->sqlite, ps_starttransaction);
		object_method(x->sqlite, _sym_execstring, TABLEDEF_FILES, NULL);
		object_method(x->sqlite, _sym_execstring, TABLEDEF_ATTRS, NULL);
		
		object_method(x->sqlite, _sym_execstring, "UPDATE files SET valid = 1", NULL);
		object_method(x->sqlite, _sym_execstring, "SELECT file_id, filename, moddate FROM files", &result);
		while(record = (char **)object_method(result, _sym_nextrecord)){
			// Here we check for the optional 'platform' attr for this file.
			// If a flag exists for the other platform, but not for the current platform, then we ignore this file.
#ifdef MAC_VERSION
			sprintf(sql, "SELECT file_id_ext FROM attrs \
							WHERE attr_name = 'platform' AND attr_value = 'windows' AND file_id_ext = %s ", record[0]);
			object_method(x->sqlite, _sym_execstring, sql, &result2);
			record2 = (char **)object_method(result2, _sym_nextrecord);
			if(record2){
				sprintf(sql, "SELECT file_id_ext FROM attrs \
								WHERE attr_name = 'platform' AND attr_value = 'mac' AND file_id_ext = %s ", record[0]);
				object_method(x->sqlite, _sym_execstring, sql, &result2);
				record2 = (char **)object_method(result2, _sym_nextrecord);
				if(!record2){
					sprintf(sql, "UPDATE files SET valid = 0 WHERE file_id = %s ", record[0]);
					object_method(x->sqlite, _sym_execstring, sql, NULL);
					continue;
				}
			}
#else // WIN_VERSION
			snprintf(sql, 512, "SELECT file_id_ext FROM attrs \
							WHERE attr_name = 'platform' AND attr_value = 'mac' AND file_id_ext = %s ", record[0]);
			object_method(x->sqlite, _sym_execstring, sql, &result2);
			record2 = (char **)object_method(result2, _sym_nextrecord);
			if(record2){
				snprintf(sql, 512, "SELECT file_id_ext FROM attrs \
								WHERE attr_name = 'platform' AND attr_value = 'windows' AND file_id_ext = %s ", record[0]);
				object_method(x->sqlite, _sym_execstring, sql, &result2);
				record2 = (char **)object_method(result2, _sym_nextrecord);
				if(!record2){
					snprintf(sql, 512, "UPDATE files SET valid = 0 WHERE file_id = %s ", record[0]);
					object_method(x->sqlite, _sym_execstring, sql, NULL);
					continue;
				}
			}
#endif
			// At this point we have a file (record[0]), and we have determined that it is indeed a file we want to cache
			// So cache it to a new file in our temp path
			err = path_createsysfile(record[1], x->temp_path, type, &file_handle);
			if(err){																// Handle any errors that occur
				object_error((t_object *)x, "%s - error %d creating file", filename, err);
			}
			else{
				snprintf(sql, 512, "SELECT content FROM files WHERE file_id = %s", record[0]);
				object_method(x->sqlite, ps_getblob, sql, &blob, &len);
				err = sysfile_write(file_handle, &len, blob);
				if(err){
					object_error((t_object *)x, "sysfile_write error (%d)", err);
				}
			}
			err = sysfile_seteof(file_handle, len);
			if(err){
				object_error((t_object *)x, "%s - error %d setting EOF", filename, err);
			}
			sysfile_close(file_handle);		// close file reference
			sysmem_freeptr(blob);
			blob = NULL;
			
			// Set the moddate
#ifdef MAC_VERSION
//			FSCatalogInfo		catalogInfo;
//			Boolean             status;
			CFGregorianDate     gdate;
			CFAbsoluteTime      abstime;
			CFTimeZoneRef		tz;
			UTCDateTime			utc;

			sscanf(record[2], "%4ld-%02hd-%02hd %02hd:%02hd:%02lf", &gdate.year, (signed short*)&gdate.month, (signed short*)&gdate.day, 
																	(signed short*)&gdate.hour, (signed short*)&gdate.minute, &gdate.second);
			tz = CFTimeZoneCopySystem();
			abstime = CFGregorianDateGetAbsoluteTime(gdate, tz);
			UCConvertCFAbsoluteTimeToUTCDateTime(abstime, &utc);
			catalogInfo.contentModDate = utc;

			strcpy(s_tempstr, x->temp_fullpath->s_name);
			temppath = strchr(s_tempstr, ':');
			temppath++;
			strcat(temppath, "/");
			strcat(temppath, record[1]);
			FSPathMakeRef((UInt8*)temppath, &ref, &isDir);
			err = FSSetCatalogInfo(&ref, kFSCatInfoContentMod, &catalogInfo);

#else // WIN_VERSION
			char				winpath[512];
			HANDLE				hFile;
			FILETIME			fileTime;
			SYSTEMTIME			systemTime;

			sscanf(record[2], "%4lu-%02lu-%02lu %02lu:%02lu:%02lu", &systemTime.wYear, &systemTime.wMonth, &systemTime.wDay, 
																	&systemTime.wHour, &systemTime.wMinute, &systemTime.wSecond);
			err = SystemTimeToFileTime(&systemTime, &fileTime);

			strcpy(s_tempstr, x->temp_fullpath->s_name);
			path_nameconform(s_tempstr, winpath, PATH_STYLE_NATIVE_WIN, PATH_TYPE_ABSOLUTE);
			strcat(winpath, "\\");
			strcat(winpath, record[1]);
			hFile = CreateFile((LPCSTR)winpath , GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
			if(hFile == INVALID_HANDLE_VALUE){
				object_error((t_object *)x, "invalid handle value");
				goto out;
			}
			err = SetFileTime(hFile, &fileTime, &fileTime, &fileTime);
			if(err == 0){
				err = GetLastError();
				object_error((t_object *)x, "Error setting date: %i", err);
			}
			CloseHandle(hFile);
out:
			;
#endif		
		}
		object_method(x->sqlite, ps_endtransaction);
	}
	object_free(result);
	object_free(result2);
}


void filecontainer_close(t_filecontainer *x)
{
	char			*fullpath = strchr(x->temp_fullpath->s_name, ':');
	int				err = 0;
	
	if(!x->sqlite)
		return;
	
	if(fullpath)
		fullpath += 1;

	// Should we remove x->temppath from the searchpath ?
	
	// Delete the cache
#ifdef MAC_VERSION
	FSRef			folderref, itemref;
	FSIterator		iterator;
	Boolean			isFolder;
	ItemCount		count;

	err = FSPathMakeRef((const UInt8 *)fullpath, &folderref, &isFolder);
	
	FSOpenIterator(&folderref, kFSIterateFlat + kFSIterateDelete, &iterator);
	while(!FSGetCatalogInfoBulk(iterator, 1, &count, NULL,
									kFSCatInfoNone, NULL,
									&itemref, NULL, NULL))
		err = FSDeleteObject(&itemref);
	FSCloseIterator(iterator);
	
	err = FSDeleteObject(&folderref);
#else // WIN_VERSION
	char			winpath[512];
	SHFILEOPSTRUCT	fileop;

	path_nameconform(fullpath, winpath, PATH_STYLE_NATIVE_WIN, PATH_TYPE_ABSOLUTE);
	strcat(winpath, "\\*");
	fileop.hwnd = NULL;
	fileop.wFunc = FO_DELETE;
	fileop.pFrom = (LPCSTR)winpath;
	fileop.pTo = (LPCSTR)winpath;
	fileop.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
	fileop.fAnyOperationsAborted = FALSE;

// !!!
// THE SUCCESS OF SHFILEOPERATION() SEEMS TO BE EXTREMELY INTERMITTENT
// MULTIPLE CALLS TO THIS METHOD WILL EVENTUALLY RESULT IN SUCCESS
// !!!
	err = SHFileOperation(&fileop);
	if(err)
		object_error((t_object *)x, "ERROR: %i deleting %s", err, winpath);
	else{
		winpath[strlen(winpath)-2] = 0;
		err = RemoveDirectory((LPCSTR)winpath);
		if(err == 0){
			err = GetLastError();
			object_error((t_object *)x, "ERROR: %i deleting the folder", err);
		}
	}
#endif

	// Free the DB
	object_free(x->sqlite);
	x->sqlite = NULL;
}


#pragma mark -
#pragma mark File Interface

void filecontainer_addfile(t_filecontainer *x, t_symbol *arg)
{
	if(!x->sqlite){
		object_error((t_object *)x, "no container open for adding file");
		return;
	}
	defer(x, (method)filecontainer_doaddfile, arg, 0, 0L);
}

void filecontainer_doaddfile(t_filecontainer *x, t_symbol *arg)
{
	int				err = 0;
	char			filename[256];
	short			path;
	t_fourcc		outtype = 0;
	t_filehandle	file_handle;
	char			*blob = NULL;
	char			sql[512];
	t_ptr_size		size;
	t_ptr_uint		moddate = 0;
	t_datetime		moddatetime;
	char			moddatetimestring[32];
		
	if(!arg || !arg->s_name[0]){
		if(open_dialog(filename, &path, &outtype, NULL, -1))			// Returns 0 if successful
			return;										
	}
	else{
		strcpy(filename, arg->s_name);
		path = 0;
	}

	err = path_opensysfile(filename, path, &file_handle, READ_PERM);
	if(err){
		object_error((t_object *)x, "error opening file");
		return;
	}
	err = sysfile_geteof(file_handle, &size);
	
	blob = sysmem_newptr(size);
	if(!blob){
		object_error((t_object *)x, "could not allocate memory", err);
	}
	err = sysfile_read(file_handle, &size, blob);
	if(err){
		object_error((t_object *)x, "sysfile_read error (%d)", err);
	}

	path_getfilemoddate(filename, path, &moddate);
	systime_secondstodate(moddate, &moddatetime);
	snprintf(moddatetimestring, 32, "%4lu-%02lu-%02lu %02lu:%02lu:%02lu", 
		moddatetime.year, moddatetime.month, moddatetime.day, moddatetime.hour, moddatetime.minute, moddatetime.second);
	snprintf(sql, 512, "INSERT INTO files ('filename', 'content', 'moddate','valid') VALUES (\"%s\", ?, '%s', 1)", 
			filename, moddatetimestring);
	object_method(x->sqlite, ps_setblob, sql, blob, size);
	
	sysmem_freeptr(blob);
	sysfile_close(file_handle);
	
	#pragma mark should also add the file immediately to the temp dir?
}


void filecontainer_getfilenames(t_filecontainer *x)
{
	t_atom		a;
	t_object*	result = NULL;
	char**		record = NULL;

	outlet_anything(x->dumpout, gensym("filenames_begin"), 0, NULL);
	SQL0("SELECT filename FROM files", &result);
	while(record = (char **)object_method(result, _sym_nextrecord)){
		atom_setsym(&a, gensym(*record));
		outlet_anything(x->dumpout, gensym("filename"), 1, &a);
	}
	outlet_anything(x->dumpout, gensym("filenames_end"), 0, NULL);
	object_free(result);
}


void filecontainer_getnumfiles(t_filecontainer *x)
{
	t_atom		a;
	t_object*	result = NULL;
	
	SQL0("SELECT filename FROM files", &result);
	atom_setlong(&a, (long)object_method(result, _sym_numrecords));
	outlet_anything(x->dumpout, gensym("numfiles"), 1, &a);
	object_free(result);
}


void filecontainer_removefile(t_filecontainer *x, t_symbol *arg)
{
	SQL1("DELETE FROM attrs WHERE file_id_ext in (SELECT file_id FROM files WHERE filename = \"%s\" )", arg->s_name, NULL);
	SQL1("DELETE FROM files WHERE filename = \"%s\" ", arg->s_name, NULL);
	#pragma mark should also remove the extracted file in the temp dir?
}


void filecontainer_getfilepath(t_filecontainer *x, t_symbol *arg)
{
	t_atom		a[2];
	t_object*	result = NULL;
	
	SQL1("SELECT file_id FROM files WHERE filename = \"%s\" ", arg->s_name, &result);
	if((long)object_method(result, _sym_numrecords)){
		atom_setsym(&a[0], arg);
		strcpy(s_tempstr, x->temp_fullpath->s_name);
#ifdef MAC_VERSION
		strcat(s_tempstr, "/");
#endif
		strcat(s_tempstr, arg->s_name);
		atom_setsym(&a[1], gensym(s_tempstr)),
		outlet_anything(x->dumpout, gensym("filepath"), 2, a);
	}
	else
		object_error((t_object *)x, "no such file (%s)", arg->s_name);
	object_free(result);
}


void filecontainer_sql(t_filecontainer *x, t_symbol *arg)
{
	char		resultstr[1024];
	short		i;
	t_atom		a;
	long		numfields;
	t_object*	result = NULL;
	char**		record = NULL;

	SQL0(arg->s_name, &result);
	while(record = (char **)object_method(result,_sym_nextrecord)){
		resultstr[0] = 0;
		numfields = (long)object_method(result, _sym_numfields);
		for(i=0; i < numfields; i++){
			strcat(resultstr, record[i]);
			strcat(resultstr, " ");
		}
		resultstr[strlen(resultstr)-1] = 0;
		atom_setsym(&a, gensym(resultstr));
		outlet_anything(x->dumpout, gensym("sqlresult"), 1, &a);
		#pragma mark safe only for small containers - otherwise may blow stack?
	}
	object_free(result);
}


#pragma mark -
#pragma mark Attribute Interface

void filecontainer_addfileattr(t_filecontainer *x, t_symbol *msg, long argc, t_atom *argv)
{
	t_symbol	*filename = atom_getsym(argv);
	t_symbol	*attrname = atom_getsym(argv+1);
	t_atom		*attrval  = argv+2;
	t_object*	result = NULL;
	char**		record = NULL;
	
	if(argc == 3){
		SQL1("SELECT file_id FROM files WHERE filename = \"%s\" ", filename->s_name, &result);
		record = (char **)object_method(result, _sym_nextrecord);
		if(record){
			switch(attrval->a_type){
				case A_LONG:
					SQL3("INSERT INTO attrs ('file_id_ext', 'attr_name', 'attr_value') \
							VALUES (%s, \"%s\", %ld)", *record, attrname->s_name, attrval->a_w.w_long, NULL);
					break;
				case A_FLOAT:
					SQL3("INSERT INTO attrs ('file_id_ext', 'attr_name', 'attr_value') \
							VALUES (%s, \"%s\", %f)", *record, attrname->s_name, attrval->a_w.w_float, NULL);
					break;
				case A_SYM:
					SQL3("INSERT INTO attrs ('file_id_ext', 'attr_name', 'attr_value') \
							VALUES (%s, \"%s\", \"%s\")", *record, attrname->s_name, attrval->a_w.w_sym->s_name, NULL);
					break;
			}
		}
	}
	else
		object_error((t_object *)x, "wrong num args for filecontainer::addfileattr()");
	object_free(result);
}


void filecontainer_removefileattr(t_filecontainer *x, t_symbol *msg, long argc, t_atom *argv)
{
	t_symbol	*filename = atom_getsym(argv);
	t_symbol	*attrname = atom_getsym(argv+1);
	
	if(argc == 2){	
		SQL2("DELETE FROM attrs WHERE attr_name = \"%s\" AND file_id_ext in (SELECT file_id FROM files WHERE filename = \"%s\" )", 
			attrname->s_name, filename->s_name, NULL);
	}
	else
		object_error((t_object *)x, "wrong num args for filecontainer::removefileattr()");
}


void filecontainer_getnumfileattrs(t_filecontainer *x, t_symbol *arg)
{
	t_atom		a;
	t_object*	result = NULL;
	
	SQL1("SELECT attr_id FROM attrs WHERE file_id_ext in (SELECT file_id FROM files WHERE filename = \"%s\") ", arg->s_name, &result);
	atom_setlong(&a, (long)object_method(result, _sym_numrecords));
	outlet_anything(x->dumpout, gensym("numfileattrs"), 1, &a);
	object_free(result);
}


void filecontainer_getfileattrnames(t_filecontainer *x, t_symbol *arg)
{
	t_atom		a;
	t_object*	result = NULL;
	char**		record = NULL;

	outlet_anything(x->dumpout, gensym("fileattrnames_begin"), 0, NULL);
	SQL1("SELECT attr_name FROM attrs WHERE file_id_ext in (SELECT file_id FROM files WHERE filename = \"%s\") ", arg->s_name, &result);
	while(record = (char **)object_method(result, _sym_nextrecord)){
		atom_setsym(&a, gensym(*record));
		outlet_anything(x->dumpout, gensym("fileattrname"), 1, &a);
	}
	outlet_anything(x->dumpout, gensym("fileattrnames_end"), 0, NULL);
	object_free(result);
}


void filecontainer_getfileattrvalue(t_filecontainer *x, t_symbol *msg, long argc, t_atom *argv)
{
	t_symbol	*filename = atom_getsym(argv);
	t_symbol	*attrname = atom_getsym(argv+1);
	t_object*	result = NULL;
	char**		record = NULL;
	t_atom		a;
	
	if(argc == 2){	
		SQL2("SELECT attr_value FROM attrs WHERE attr_name = \"%s\" AND \
				file_id_ext in (SELECT file_id FROM files WHERE filename = \"%s\")", attrname->s_name, filename->s_name, &result);
		record = (char **)object_method(result, _sym_nextrecord);
		if(record){
			atom_setsym(&a, gensym(*record));
			outlet_anything(x->dumpout, gensym("fileattrvalue"), 1, &a);
		}
	}
	else
		object_error((t_object *)x, "wrong num args for filecontainer::removefileattr()");
	object_free(result);
}

