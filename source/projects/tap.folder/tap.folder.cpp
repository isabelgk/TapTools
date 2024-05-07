/* 
 *	External object for Max/MSP
 *	Copyright © 2008 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */
 
#include "TTClassWrapperMax.h"

typedef struct _folder {
	t_object	obj;
	t_object	*applescript;
} t_folder;

// Prototypes
void*	folder_new(t_symbol *msg, short argc, t_atom *argv);
void	folder_free(t_folder *x);
void	tap_folder_assist(t_folder *x, void *b, long msg, long arg, char *dst);
void	tap_folder_makedir(t_folder *x, t_symbol *path);
void	tap_folder_deletedir(t_folder *x, t_symbol *path);
void	tap_folder_copy(t_folder *x, t_symbol *srcpath, t_symbol *dstpath);
void	tap_folder_move(t_folder *x, t_symbol *srcpath, t_symbol *dstpath);
void	tap_folder_unzip(t_folder *x, t_symbol *path);
void	tap_folder_fileusage(t_folder *x, void *w);

// Class Globals
static t_class *s_folder_class;


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c = class_new("tap.folder",(method)folder_new, (method)folder_free, sizeof(t_folder), (method)0L, A_GIMME, 0);
	
	common_symbols_init();

	class_addmethod(c, (method)tap_folder_makedir,		"make",		A_SYM, 0);
	class_addmethod(c, (method)tap_folder_deletedir,	"delete",	A_SYM, 0);
	class_addmethod(c, (method)tap_folder_copy,			"copy",		A_SYM, A_SYM, 0);
#ifdef MAC_VERSION
	class_addmethod(c, (method)tap_folder_unzip,		"unzip",	A_SYM, 0);
#endif
	class_addmethod(c, (method)tap_folder_fileusage,	"fileusage", A_CANT, 0L);

	// TODO: if desired, we can add this feature later:
	//class_addmethod(c, (method)tap_folder_move, 		"move",		A_SYM, A_SYM, 0);
	
    class_addmethod(c, (method)tap_folder_assist, 		"assist",	A_CANT, 0);
	class_addmethod(c, (method)stdinletinfo,			"inletinfo",A_CANT, 0);

	class_register(_sym_box, c);
	s_folder_class = c;
}


/************************************************************************************/
// Object Creation Method

void *folder_new(t_symbol *msg, short argc, t_atom *argv)
{
	t_folder *x = (t_folder *)object_alloc(s_folder_class);;
	if(x){
		object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));
#ifdef MAC_VERSION
		x->applescript = (t_object*)object_new_typed(_sym_box, gensym("tap.applescript"), 0, NULL);
#endif
    	attr_args_process(x,argc,argv);
    }
 	return (x);
}


void folder_free(t_folder *x)
{
#ifdef MAC_VERSION
	object_free(x->applescript);
#endif
}


/************************************************************************************/
// Methods bound to input/inlets

// Method for Assistance Messages
void tap_folder_assist(t_folder *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1) 		// Inlets
		strcpy(dst, "(int/symbol/list) Input");
	else if(msg==2){ 	// Outlets
		switch(arg){
			case 0: strcpy(dst, "dumpout"); break;
		}
	}
}



void tap_folder_domakedir(t_folder *x, t_symbol *s, long argc, t_atom *argv)
{
	short	path = 0;				// parent folder, which we supply
	short	createdPath = 0;		// the new folder after it is created
	char	*folderName;			// the name of the new folder
	char	fullpath[4096];
	short	err = 0;
	char	temp[256];
	
	path_nameconform(s->s_name, fullpath, PATH_STYLE_MAX, PATH_TYPE_ABSOLUTE);
	folderName = strrchr(fullpath, '/');
	
	if(folderName){
		*folderName = 0;
		folderName++;
		
		err = path_frompathname(fullpath, &path, temp);
		if(!err)
			err = path_createfolder(path, folderName, &createdPath);
		if(err)
			object_error((t_object*)x, "error %hd trying to create folder", err);
	}
	object_obex_dumpout(x, _sym_bang, 0, NULL);
}

void tap_folder_makedir(t_folder *x, t_symbol *path)
{
	defer(x, (method)tap_folder_domakedir, path, 0, 0);
}


void tap_folder_dodeletedir(t_folder *x, t_symbol *s, long argc, t_atom *argv)
{
	char	name[MAX_PATH_CHARS];

#ifdef MAC_VERSION
	char	script[MAX_PATH_CHARS + 200];
	t_atom	a;
	
	path_nameconform(s->s_name, name, PATH_STYLE_SLASH, PATH_TYPE_BOOT);
	snprintf(script, MAX_PATH_CHARS + 200, 
		"tell application \"Finder\" \ntry \nset thing to \"%s\" as POSIX file \ndelete thing \nend try \nend tell \n",
		name);
	atom_setsym(&a, gensym(script));
	object_method_typed(x->applescript, gensym("script"), 1, &a, NULL);
	object_method(x->applescript, _sym_bang);
#else // WIN_VERSION
	int				err = 0;
	char			winpath[4096];
	char			winpath2[4096];
	char*			winpathfile = NULL;
	SHFILEOPSTRUCT	fileop;

	path_nameconform(s->s_name, winpath, PATH_STYLE_NATIVE_WIN, PATH_TYPE_ABSOLUTE);
	strcat(winpath, "\\*");

	// This is exceedingly annoying: this string has to be double-terminated in order to work!
	winpath[strlen(winpath)] = 0;
	winpath[strlen(winpath)+1] = 0;

	fileop.hwnd = NULL;
	fileop.wFunc = FO_DELETE;
	fileop.pFrom = (LPCSTR)winpath;
	fileop.pTo = (LPCSTR)winpath;
	fileop.pTo = NULL;
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
	object_obex_dumpout(x, _sym_bang, 0, NULL);
}

void tap_folder_deletedir(t_folder *x, t_symbol *path)
{
	defer(x, (method)tap_folder_dodeletedir, path, 0, 0);
}


void tap_folder_docopy(t_folder *x, t_symbol *srcin, long argc, t_atom *argv)
{
	t_symbol*	dstin = atom_getsym(argv);
	char		srcname[MAX_PATH_CHARS];
	short		srcpath = 0;
	char		srcfilename[MAX_FILENAME_CHARS];
	char		dstname[MAX_PATH_CHARS];
	short		dstpath = 0;
	char		dstfilename[MAX_FILENAME_CHARS];
	short		newpath = 0;
	char*		tempstr = NULL;
	
	strncpy_zero(srcname, srcin->s_name, MAX_PATH_CHARS);
	path_frompathname(srcname, &srcpath, srcfilename);

	strncpy_zero(dstname, dstin->s_name, MAX_PATH_CHARS);
	tempstr = strrchr(dstname, '/');
	*tempstr = 0;
	tempstr++;
	path_frompathname(dstname, &dstpath, dstfilename);
	if(tempstr)
		strncpy_zero(dstfilename, tempstr, MAX_FILENAME_CHARS);

	if(!srcfilename[0])
		path_copyfolder(srcpath, dstpath, dstfilename, true, &newpath);
	else
		path_copyfile(srcpath, srcfilename, dstpath, dstfilename);
	object_obex_dumpout(x, _sym_bang, 0, NULL);
}

void tap_folder_copy(t_folder *x, t_symbol *srcpath, t_symbol *dstpath)
{
	t_atom a;
		
	atom_setsym(&a, dstpath);
	defer(x, (method)tap_folder_docopy, srcpath, 1, &a);
}


// TODO: not yet implemented
void tap_folder_domove(t_folder *x, t_symbol *srcpath, long argc, t_atom *argv)
{
//	t_symbol	*dstpath = atom_getsym(argv);
//	path_renamefile(<#char * name#>, <#short path#>, <#char * newname#>)
}

void tap_folder_move(t_folder *x, t_symbol *srcpath, t_symbol *dstpath)
{
	t_atom a;
		
	atom_setsym(&a, dstpath);
	defer(x, (method)tap_folder_docopy, srcpath, 1, &a);
}


void tap_folder_dounzip(t_folder *x, t_symbol *s, long argc, t_atom *argv)
{
	char	name[MAX_PATH_CHARS];

#ifdef MAC_VERSION
	char	script[MAX_PATH_CHARS + 200];
	char*	tempstr = NULL;
	t_atom	a;
	
	path_nameconform(s->s_name, name, PATH_STYLE_SLASH, PATH_TYPE_BOOT);
	tempstr = strrchr(name, '/');
	if(!tempstr)
		return;
		
	*tempstr = 0;
	tempstr++;
	if(tempstr){
		snprintf(script, MAX_PATH_CHARS + 200, "do shell script \"cd \\\"%s\\\"; unzip %s\"", name, tempstr);
		atom_setsym(&a, gensym(script));
		object_method_typed(x->applescript, gensym("script"), 1, &a, NULL);
		object_method(x->applescript, _sym_bang);
	}
#else // WIN_VERSION
	; // TODO: what do we do here?
#endif
	object_obex_dumpout(x, _sym_bang, 0, NULL);
}

void tap_folder_unzip(t_folder *x, t_symbol *path)
{
	defer(x, (method)tap_folder_dounzip, path, 0, 0);
}


// Method called by collective and standalone builder
void tap_folder_fileusage(t_folder *x, void *w)
{
	short	err;
	short	path_id;
	char	filename[MAX_FILENAME_CHARS];
	t_fourcc	filetype;
	
	strncpy_zero(filename, "tap.applescript.mxo", MAX_FILENAME_CHARS);
	err = locatefile_extended(filename, &path_id, &filetype, NULL, -1);		// FIND THE FILE
	if (err) {
			object_error((t_object *)x, "cannot locate file: %s", filename);
			return;
	}
	fileusage_addfile(w, 0, filename, path_id);	
}
