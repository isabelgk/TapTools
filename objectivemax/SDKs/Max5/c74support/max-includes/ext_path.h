// ext_path.h -- path definitions

#ifndef _EXT_PATH_H_
#define _EXT_PATH_H_

#ifdef __cplusplus
extern "C" {
#endif

#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif


/**
	The size you should use when allocating strings for full paths.
	@ingroup files
*/
#define MAX_PATH_CHARS		2048

/**
	The size you should use when allocating strings for filenames.
	At the time of this writing it supports up to 256 UTF chars
	@ingroup files
*/
#define MAX_FILENAME_CHARS	512


typedef short FILE_REF;

#define PATH_SEPARATOR_CHAR			'/'
#define PATH_SEPARATOR_STRING		"/"
#define PATH_CROSS_PLAT_NAMES
#define SEPARATOR_CHAR				PATH_SEPARATOR_CHAR		// for backwards compatibility


/**
	Constants that determine the output of path_nameconform().
	@ingroup files
	@see #e_max_path_types
	@see path_nameconform()
*/
typedef enum {
	PATH_STYLE_MAX = 0,			///< use PATH_STYLE_MAX_PLAT
	PATH_STYLE_NATIVE,			///< use PATH_STYLE_NATIVE_PLAT
	PATH_STYLE_COLON,			///< ':'  sep, "vol:"   volume, ":"   relative, "^:" boot
	PATH_STYLE_SLASH,			///< '/'  sep, "vol:/"  volume, "./"  relative, "/"  boot
	PATH_STYLE_NATIVE_WIN		///< '\\' sep, "vol:\\" volume, ".\\" relative, "\\" boot
} e_max_path_styles;

#ifdef WIN_VERSION 
#define PATH_STYLE_MAX_PLAT			PATH_STYLE_SLASH
#define PATH_STYLE_NATIVE_PLAT		PATH_STYLE_NATIVE_WIN
#else
#define PATH_STYLE_MAX_PLAT			PATH_STYLE_SLASH
#define PATH_STYLE_NATIVE_PLAT		PATH_STYLE_SLASH	// PATH_STYLE_COLON path changes for Max 5-jkc
#endif

/**
	Constants that determine the output of path_nameconform().
	@ingroup files
	@see #e_max_path_styles
	@see path_nameconform()
*/
typedef enum {
	PATH_TYPE_IGNORE = 0,		///< ignore
	PATH_TYPE_ABSOLUTE,			///< absolute path
	PATH_TYPE_RELATIVE,			///< relative path
	PATH_TYPE_BOOT,				///< boot path
	PATH_TYPE_C74,				///< Cycling '74 folder
	PATH_TYPE_PATH				///< path
} e_max_path_types;


#ifdef PATH_CROSS_PLAT_NAMES
#define PATH_CHAR_IS_SEPARATOR(c) (((c) == ':') || ((c) == '/') || ((c) == '\\'))
#else
#define PATH_CHAR_IS_SEPARATOR(c) ((c) == PATH_SEPARATOR_CHAR)
#endif


/**
	Flags used to represent properties of a file in a #t_fileinfo struct.
	@ingroup files
*/
typedef enum {	
	PATH_FILEINFO_ALIAS = 1,	///< alias
	PATH_FILEINFO_FOLDER = 2,	///< folder
	PATH_FILEINFO_PACKAGE = 4	///< package (Mac-only)
} e_max_fileinfo_flags;

#define FILEINFO_ALIAS		PATH_FILEINFO_ALIAS		// for backwards compatibility
#define FILEINFO_FOLDER		PATH_FILEINFO_FOLDER	// for backwards compatibility


/** Flags used by functions such as path_foldernextfile() and path_openfolder().
	@ingroup files
*/
typedef enum {	
	PATH_REPORTPACKAGEASFOLDER = 1,	///< if not true, then a Mac OS package will be reported as a file rather than a folder.
	PATH_FOLDER_SNIFF = 2			///< sniff
} e_max_path_folder_flags;


/**
	Permissions or mode with which to open a file.
	@ingroup files
*/
typedef enum {	
	PATH_READ_PERM = 1,		///< Read mode
	PATH_WRITE_PERM = 2,	///< Write mode
	PATH_RW_PERM = 3		///< Read/Write mode
} e_max_openfile_permissions;
#define READ_PERM		PATH_READ_PERM		// for backwards compatibility
#define WRITE_PERM		PATH_WRITE_PERM		// for backwards compatibility
#define RW_PERM			PATH_RW_PERM		// for backwards compatibility

#define PATH_DEFAULT_PATHNAME_COUNT 16

// indices allow for fast access
typedef enum {	
	PATH_STARTUP_PATH = 0,	
	PATH_SEARCH_PATH,	
	PATH_ACTION_PATH,
	PATH_HELP_PATH
} e_max_path_indices;

#define STARTUP_PATH	PATH_STARTUP_PATH		// for backwards compatibility
#define SEARCH_PATH		PATH_SEARCH_PATH		// for backwards compatibility
#define ACTION_PATH		PATH_ACTION_PATH		// for backwards compatibility
#define HELP_PATH		PATH_HELP_PATH			// for backwards compatibility


#define COLLECTIVE_FILECOPY 1	// flag for copying an object-used file to support path
#define COLLECTIVE_COPYTOMADEFOLDER 2 // flag to copying to the folder you made


#define TYPELIST_SIZE 	32 //maximum number of types returned

typedef enum {
	TYPELIST_MAXFILES = 1,
	TYPELIST_EXTERNS = 2,
	TYPELIST_COLLECTIVES = 4
} e_max_typelists;


/**
	Information about a file.
	@ingroup files
*/
typedef struct _fileinfo {
	long type;			///< type (four-char-code)
	long creator;		///< Mac-only creator (four-char-code)
	long date;			///< date
	long flags;			///< One of the values defined in #e_max_fileinfo_flags
} t_fileinfo;


/** The path data structure. This struct is provided for debugging convenience, 
	but should be considered opaque and is subject to change without notice. 
	@ingroup files
*/
typedef struct _path {	
	void *p_name;		// all we have on Windows, may just be folder name on Mac	
	long p_dirID;		// directory ID on Mac	
	short p_vRefNum;	// volume on Mac	
	short p_wd;			// working directory on the Mac (compatibility mode)
} t_path;


/** The pathlink data structure. This struct is provided for debugging convenience, 
	but should be considered opaque and is subject to change without notice. 
	@ingroup files
*/
typedef struct _pathlink {
	short l_path;		// index into path table, or p_wd in compatibility mode
	short l_ancestor;	// logical parent, i.e., what path caused this path to be included
	short l_temp;		// used for temporary flag when updating folders
	struct _pathlink *l_next;
	short l_parent;		// true parent folder
	short l_recursive;	// true if is or was added from a parent recursively
	void *l_watcher;	// watcher to see if this folder changes
} t_pathlink;


// flags for t_searchpath
typedef enum {
	PATH_FLAGS_RECURSIVE = 1
} e_max_searchpath_flags;


/**
	Retrieve the Path ID of the Max application.
	@ingroup files
	@return	The path id.
*/
short path_getapppath(void);

short path_getsupportpath(void);

//#ifdef MAC_VERSION
//
//#ifndef __FILES__
//#include <Files.h>
//#endif // __FILES__
//	
//short path_tofsref(C74_CONST short path, C74_CONST char *filename, FSRef *ref);
//short path_fromfsref(FSRef *ref); 
//#endif // MAC_VERSION

void path_namefrompathname(char *pathname, char *name);


/**
	Find a Max document by name in the search path.
	This routine performs the same function as the routine path_getdefault().
	locatefile() searches through the directories specified by the user for 
	Patcher files and tables in the File Preferences dialog as well as the 
	current default path (see path_getdefault) and the directory
	containing the Max application
	
	@ingroup	files
	@param	name	A C string that is the name of the file to look for.
	@param	outvol	The Path ID containing the location of the file if it is found.
	@param	binflag	If the file found is in binary format (it's of type 'maxb') 1 is returned here; 
					if it's in text format, 0 is returned.

	@return			If a file is found with the name 
					specified by filename, locatefile returns 0, otherwise it returns non-zero. 
	
	@remark			filename and vol can then be passed to binbuf_read to read and open file the file. 
					When using MAXplay, the search path consists of all subdirectories of 
					the directory containing the MAXplay application. locatefile only 
					searches for files of type 'maxb' and 'TEXT.'
	
	@see			locatefile_extended()
*/
short locatefile(C74_CONST char *name, short *outvol, short *binflag);


/**
	Find a Max document by name in the search path.
	This function searches through the same directories as locatefile, 
	but allows you to specify a type and creator of your own.

	@ingroup	files
	@param	name		A C string that is the name of the file to look for.
	@param	outvol		The Path ID containing the location of the file if it is found.
	@param	filetype	The filetype of the file to look for. 
						If you pass 0L, files of all filetypes are considered.
	@param	creator		The creator of the file to look for. If you pass 0L, files with any creator are considered.
	
	@return			If a file is found with the name 
					specified by filename, locatefile returns 0, otherwise it returns non-zero. 

	@see			locatefile_extended()
*/
short locatefiletype(C74_CONST char *name, short *outvol, long filetype, long creator);

short locatefilelist(char *name, short *outvol, long *outtype, long *filetypelist, short numtypes);


/**
	Find a Max document by name in the search path.
	This is the preferred method for file searching since its introduction in Max version 4.
	
	This routine performs the same function as the routine path_getdefault().
	locatefile() searches through the directories specified by the user for 
	Patcher files and tables in the File Preferences dialog as well as the 
	current default path (see path_getdefault) and the directory
	containing the Max application

	@ingroup	files
	@version	4.0
	
	@param	name			The file name for the search, receives actual filename.
	@param	outvol			The Path ID of the file (if found).
	@param	outtype			The file type of the file (if found).
	@param	filetypelist	The file type(s) that you are searching for.
	@param	numtypes		The number of file types in the typelist array (1 if a single entry).

	@return			If a file is found with the name 
					specified by filename, locatefile returns 0, otherwise it returns non-zero. 

	@remark			The old file search routines locatefile() and locatefiletype() 
					are still supported in Max 4, but the use of a new routine 
					locatefile_extended() is highly recommended. However, 
					locatefile_extended() has an important difference from 
					locatefile() and locatefiletype() that may require some rewriting 
					of your code. <em>It modifies its name parameter</em> in certain cases, while 
					locatefile() and locatefiletype() do not. The two cases where it 
					could modify the incoming filename string are 1) when an alias is 
					specified, the file pointed to by the alias is returned; and 2) when a full 
					path is specified, the output is the filename plus the path number of the 
					folder it's in. 
					
					This is important because many people pass the s_name field of a 
					#t_symbol to locatefile(). If the name field of a #t_symbol were to be 
					modified, the symbol table would be corrupted. To avoid this problem, 
					use strncpy_zero() to copy the contents of a #t_symbol to a character string first, 
					as shown below: 
	@code
	char filename[MAX_FILENAME_CHARS]; 
	strncpy_zero(filename,str->s_name, MAX_FILENAME_CHARS); 
	result = locatefile_extended(filename,&path,&type,typelist,1); 
	@endcode
*/
short locatefile_extended(char *name, short *outvol, long *outtype, C74_CONST long *filetypelist, short numtypes);

short locatefile_pathlist(t_pathlink *list, char *name, short *outvol, long *outtype, long *filetypelist, short numtypes);


/**
	Resolve a Path ID plus a (possibly extended) file name 
	into a path that identifies the file's directory and a filename.
	This routine converts a name and Path ID to a standard form in which 
	the name has no path information and does not refer to an aliased file. 
	
	@ingroup files
	@param	name	A file name (which may be fully or partially qualified), 
					will contain the file name on return.
	@param	path	The Path ID to be resolved.
	@param	outpath	The Path ID of the returned file name.
	
	@return		 	Returns 0 if successful.
*/
short path_resolvefile(char *name, C74_CONST short path, short *outpath);


/**
	Retrive a #t_fileinfo structure from a file/path combination. 
	
	@ingroup files
	@param	name	The file name to be queried.
	@param	path	The Path ID of the file.
	@param	info	The address of a #t_fileinfo structure to contain the file information.
	
	@return		 	Returns 0 if successful, otherwise it returns an OS-specific error code.
*/
short path_fileinfo(C74_CONST char *name, C74_CONST short path, void *info);

short path_tempfolder();
short path_desktopfolder();
short path_createfolder(C74_CONST short path, C74_CONST char *name, short *newpath);

// internal use only -- not exported -- use path_createfolder()
short path_createnewfolder(short path, char *name, short *newpath);

short path_copyfile(short srcpath, char *srcname, short dstpath, char *dstname);
short path_copytotempfile(short srcpath, char *srcname, short *outpath, char *outname);
short path_copyfolder(short srcpath, short dstpath, char *dstname, long recurse, short *newpath);
short path_getpath(short path, char *name, short *outpath);
short path_getname(short path, char *name, short *outpath);


/**
	Create a fully qualified file name from a Path ID/file name combination.
	Unlike path_topotentialname(), this routine will only convert a 
	pathname pair to a valid path string if the path exists.
		
	@ingroup files
	@param	path	The path to be used.
	@param	file	The file name to be used.
	@param	name	Loaded with the fully extended file name on return.
	@return		 	Returns 0 if successful, otherwise it returns an OS-specific error code.	
*/
short path_topathname(C74_CONST short path, C74_CONST char *file, char *name);


/**
	Create a filename and Path ID combination from a fully qualified file name.
	Note that path_frompathname() does not require that the file actually exist. 
	In this way you can use it to convert a full path you may have received as an 
	argument to a file writing message to a form appropriate to provide to 
	a routine such as path_createfile().
		
	@ingroup files
	@param	name		The extended file path to be converted.
	@param	path		Contains the Path ID on return.
	@param	filename	Contains the file name on return.
	@return		 		Returns 0 if successful, otherwise it returns an OS-specific error code.	
*/
short path_frompathname(C74_CONST char *name, short *path, char *filename);


short path_frompotentialpathname(C74_CONST char *name, short *path, char *filename);


void path_splitnames(const char *pathname, char *foldername, char *filename);
short path_getnext(t_pathlink *list, short *val);


/**
	Install a path as the default search path.
	The default path is searched before the Max search path. For instance, 
	when loading a patcher from a directory outside the search path, the 
	patcher's directory is searched for files before the search path. 
	path_setdefault() allows you to set a path as the default.

	@ingroup files
	@param	path		The path to use as the search path.
						If path is already part of the Max Search path, it will not be added 
						(since, by default, it will be searched during file searches).
	@param	recursive	If non-zero, all subdirectories will be installed in the default search list.
						Be very careful with the use of the recursive argument.  It has the capacity to 
						slow down file searches dramatically as the list of folders is being built. 
						Max itself never creates a hierarchical default search path.
*/
void path_setdefault(short path, short recursive);


/**
	Retrieve the Path ID of the default search path. 
	@ingroup files
	@return	The path id of the default search path.
*/
short path_getdefault(void);


void path_setdefaultlist(struct _pathlink *list);


/**
	Determine the modification date of the selected path.
	
	@ingroup files
	@param	path	The Path ID of the directory to check.
	@param	date	The last modification date of the directory.
	@return			An error code.
*/
short path_getmoddate(short path, unsigned long *date);


/**
	Determine the modification date of the selected file.
	
	@ingroup files
	@param	filename	The name of the file to query.
	@param	path		The Path ID of the file.
	@param	date		The last modification date of the file upon return.
	@return				An error code.
*/
short path_getfilemoddate(C74_CONST char *filename, C74_CONST short path, unsigned long *date);


short path_getfilecreationdate(C74_CONST char *filename, C74_CONST short path, unsigned long *date);
short path_getfilesize(char *filename, short path, unsigned long *date);
long path_listcount(t_pathlink *list);

short nameinpath(char *name, short *ref);					// <-- use path_nameinpath()
short path_nameinpath(C74_CONST char *name, C74_CONST short path, short *ref);

short path_sysnameinpath(char *name, short *ref);


/**
	Prepare a directory for iteration.
	@ingroup files
	@param	path	The directory Path ID to open.
	@return			The return value of this routine is an internal "folder state" structure 
					used for further folder manipulation. It should be saved and used for 
					calls to path_foldernextfile() and path_closefolder(). 
					If the folder cannot be found or accessed, path_openfolder() returns 0.
*/
void *path_openfolder(short path);


/**
	Get the next file in the directory.
	In conjunction with path_openfolder() and path_closefolder(), 
	this routine allows you to iterate through all of the files in a path.

	@ingroup files
	@param	xx			The "folder state" value returned by path_openfolder().
	@param	filetype	Contains the file type of the file type on return.
	@param	name		Contains the file name of the next file on return.
	@param	descend		Unused.
	@return				Returns non-zero if successful, and zero when there are no more files. 
	@see				#e_max_path_folder_flags
*/
short path_foldernextfile(void *xx, long *filetype, char *name, short descend);


/**
	Complete a directory iteration.
	@ingroup files
	@param	x	The "folder state" value originally returned by path_openfolder().
*/
void path_closefolder(void *x);


short path_renamefile(C74_CONST char *name, C74_CONST short path, C74_CONST char *newname);
short path_getprefstring(short type, short index, t_symbol **s);
void path_setprefstring(short type, short index, t_symbol *s, short update);
void path_makefromsymbol(long pathtype, t_symbol *sp, short recursive);

/**
	Open a file given a filename and Path ID.
	
	@ingroup files
	@param	name	The name of the file to be opened.
	@param	path	The Path ID of the file to be opened.
	@param	ref		A #t_filehandle reference to the opened file will be returned in this parameter.
	@param	perm	The permission for the opened file as defined in #e_max_openfile_permissions.
	@return 		An error code.
*/
short path_opensysfile(C74_CONST char *name, C74_CONST short path, t_filehandle *ref, short perm);


/**
	Create a file given a type code, a filename, and a Path ID.
	
	@ingroup files
	@param	name	The name of the file to be opened.
	@param	path	The Path ID of the file to be opened.
	@param	type	The file type of the created file.
	@param	ref		A #t_filehandle reference to the opened file will be returned in this parameter.
	@return 		An error code.
*/
short path_createsysfile(C74_CONST char *name, C74_CONST short path, long type, t_filehandle *ref);


short path_createressysfile(C74_CONST char *name, C74_CONST short path, long type, t_filehandle *ref);


/**
	Convert a source path string to destination path string using the specified style and type. 

	@ingroup files
	@param	src		A pointer to source character string to be converted.
	@param	dst		A pointer to destination character string.
	@param	style	The destination filepath style, as defined in #e_max_path_styles
	@param	type	The destination filepath type, as defined in #e_max_path_types 
	@return			An error code.
	
	@see #MAX_PATH_CHARS
*/
short path_nameconform(C74_CONST char *src, char *dst, long style, long type);

short path_deletefile(C74_CONST char *name, C74_CONST short path);
short path_extendedfileinfo(char *name, short path, t_fileinfo *info, long *typelist, short numtypes, short sniff);
short path_getstyle(char *name);
char path_getseparator(char *name);
short path_fileisresource(char *name, short path);

/**
	Create a fully qualified file name from a Path ID/file name combination, 
	regardless of whether or not the file exists on disk.
	
	@ingroup files
	@param	path	The path to be used.
	@param	file	The file name to be used.
	@param	name	Loaded with the fully extended file name on return.
	@param	check	Flag to check if a file with the given path exists.
	@return		 	Returns 0 if successful, otherwise it returns an OS-specific error code.	
	
	@see path_topathname()
*/
short path_topotentialname(C74_CONST short path, C74_CONST char *file, char *name, short check);


#ifdef WIN_VERSION
short path_topotentialunicodename(short path, char *file, unsigned short **name, long *outlen, short check);
short path_fromunicodepathname(unsigned short *name, short *path, char *filename, short check);		// if check is non-zero then file must exist
#endif
void path_addsearchpath(short path, short parent);
void path_addnamed(long pathtype, char *name, short recursive, short permanent);

void path_removefromlist(t_pathlink **list, short parent);

short defvolume(void);			// <--  use path_getdefault()
short getfolder(short *vol);


/**
	Present the user with the standard open file dialog.
	This function is convenient wrapper for using Mac OS Navigation 
	Services or Standard File for opening files. 

	The mapping of extensions to types is configured in the C74:/init/max-fileformats.txt file.
	The standard types to use for Max files are 'maxb' for old-format binary files, 
	'TEXT' for text files, and 'JSON' for newer format patchers or other .json files.

	@ingroup files
	@param	name	A C-string that will receive the name of the file the user wants to open.
					The C-string should be allocated with a size of at least #MAX_FILENAME_CHARS.
	@param	volptr	Receives the Path ID of the file the user wants to open.
	@param	typeptr	The file type of the file the user wants to open.
	@param	types	A list of file types to display. This is not limited to 4 
					types as in the SFGetFile() trap. Pass NULL to display all types.
	@param	ntypes	The number of file types in typelist. Pass 0 to display all types.
	
	@return			0 if the user clicked Open in the dialog box.  
					If the user cancelled, open_dialog() returns a non-zero value.
	
	@see saveasdialog_extended()
	@see locatefile_extended()
*/
short open_dialog(char *name, short *volptr, long *typeptr, long *types, short ntypes);


/**
	Present the user with the standard save file dialog.

	The mapping of extensions to types is configured in the C74:/init/max-fileformats.txt file.
	The standard types to use for Max files are 'maxb' for old-format binary files, 
	'TEXT' for text files, and 'JSON' for newer format patchers or other .json files.

	@ingroup files
	@param	filename	A C-string containing a default name for the file to save.
						If the user decides to save a file, its name is returned here.
						The C-string should be allocated with a size of at least #MAX_FILENAME_CHARS.

	@param	path		If the user decides to save the file, the Path ID of the location chosen is returned here.

	@param	binptr		Pass NULL for this parameter.  
						This parameter was used in Max 4 to allow the choice of saving binary or text format patchers.
	
	@return				0 if the user choose to save the file.  
						If the user cancelled, returns a non-zero value.
	
	@see open_dialog()
	@see saveasdialog_extended()
	@see locatefile_extended()
*/
short saveas_dialog(char *filename, short *path, short *binptr);


/**
	Present the user with the standard save file dialog with your own list of file types.

	saveasdialog_extended() is similar to saveas_dialog(), but allows the 
	additional feature of specifying a list of possible types. These will be 
	displayed in a pop-up menu. 
	
	File types found in the typelist argument that match known Max types 
	will be displayed with descriptive text. Unmatched types will simply 
	display the type name (for example, "foXx" is not a standard type so it 
	would be shown in the pop-up menu as foXx) 
	
	Known file types include:
	- TEXT: text file 
	- maxb: Max binary patcher 
	- maxc: Max collective 
	- Midi: MIDI file 
	- Sd2f: Sound Designer II audio file 
	- NxTS: NeXT/Sun audio file 
	- WAVE: WAVE audio file. 
	- AIFF: AIFF audio file
	- mP3f: Max preference file 
	- PICT: PICT graphic file 
	- MooV: Quicktime movie file 
	- aPcs: VST plug-in 
	- AFxP: VST effect patch data file 
	- AFxB: VST effect bank data file 
	- DATA: Raw data audio file 
	- ULAW: NeXT/Sun audio file

	@ingroup files
	@param	name		A C-string containing a default name for the file to save.
						If the user decides to save a file, its name is returned here.
						The C-string should be allocated with a size of at least #MAX_FILENAME_CHARS.

	@param	vol			If the user decides to save the file, the Path ID of the location chosen is returned here.

	@param	type		Returns the type of file chosen by the user.
	@param	typelist	The list of types provided to the user. 
	@param	numtypes	The number of file types in typelist.
	
	@return				0 if the user choose to save the file.  
						If the user cancelled, returns a non-zero value.
	
	@see open_dialog()
	@see locatefile_extended()
*/
short saveasdialog_extended(char *name, short *vol, long *type, long *typelist, short numtypes);

void saveas_autoextension(char way);
void saveas_setselectedtype(long type);

void typelist_make(long *types, long include, short *numtypes);



short preferences_path(C74_CONST char *name, short create, short *path);
short preferences_subpath(C74_CONST char *name, short path, short create, short *subpath);
short textpreferences_read(C74_CONST char *filename, short path, short defaultid);
short textpreferences_default(short id);
void *textpreferences_open(void);
void textpreferences_addraw(void *p, C74_CONST char *fmt, ...);
void textpreferences_add(void *p, C74_CONST char *fmt, ...);
void textpreferences_addoption(void *p, C74_CONST char *fmt, ...);
void textpreferences_addrect(void *p, char *msg, short top, short left, short bottom, short right);
short textpreferences_close(void *p, C74_CONST char *filename, short path);



#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(pop)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack()
#endif

#ifdef __cplusplus
}
#endif


#endif // _EXT_PATH_H_
