/* 
 *	External object for Max/MSP
 *	Copyright © 2009 by Timothy Place
 * 
 *	License: This code is licensed under the terms of the "New BSD License"
 *	http://creativecommons.org/licenses/BSD/
 */

// Query the values of another object

#include "TTClassWrapperMax.h"
#include "jpatcher_api.h"


typedef struct _inquisitor{
	t_object	ob;
	void*		outlet;
	void*		outlet_names;
	t_object*	patcher;
	t_symbol*	name;
	t_object*	subject;
} t_inquisitor;


// Prototypes for methods: need a method for each incoming message type:
void*		inquisitor_new(t_symbol *msg, long argc, t_atom *argv);
void		inquisitor_free(t_inquisitor *x);
void		inquisitor_assist(t_inquisitor *x, void *b, long msg, long arg, char *dst);
void		inquisitor_get(t_inquisitor *x, t_symbol *name);
void		inquisitor_attributes(t_inquisitor* x);
t_max_err	inquisitor_set_name(t_inquisitor* x, void* attr, long ac, t_atom* av);


// Class Globals
static t_class*	s_inquisitor_class;


/************************************************************************************/
// Main() Function

extern "C" int C74_EXPORT main(void)
{
	t_class *c;

	c = class_new("tap.inquisitor",(method)inquisitor_new, (method)inquisitor_free, sizeof(t_inquisitor), (method)0L, A_GIMME, 0);

		common_symbols_init();
	class_addmethod(c, (method)inquisitor_get,			"get", A_SYM, 0L);
	class_addmethod(c, (method)inquisitor_attributes,	"attributes", 0);
    class_addmethod(c, (method)inquisitor_assist,		"assist", A_CANT, 0L); 

	CLASS_ATTR_SYM(c,		"name", 0, t_inquisitor, name);
	CLASS_ATTR_ACCESSORS(c,	"name", NULL, inquisitor_set_name);
	
class_register(_sym_box, c); 	s_inquisitor_class = c;
}


/************************************************************************************/
// Object Creation Method

void *inquisitor_new(t_symbol *msg, long argc, t_atom *argv)
{
	t_inquisitor *x;
	 
	x = (t_inquisitor*)object_alloc(s_inquisitor_class);;
	if(x){
		object_obex_lookup(x, _sym_pound_P, &x->patcher);
		object_obex_store((void *)x, _sym_dumpout, (object *)outlet_new(x,NULL));	
		x->outlet_names = outlet_new(x, NULL);
		x->outlet = outlet_new(x, NULL);
		
    	attr_args_process(x, argc, argv); 
    }
 	return (x);
}


void inquisitor_free(t_inquisitor *x)
{
	;
}


/************************************************************************************/
// Methods bound to input/inlets

void inquisitor_assist(t_inquisitor *x, void *b, long msg, long arg, char *dst)
{
	if(msg==1){ 		// Inlets
		switch(arg){
			case 0: strcpy(dst, "get message will get a named attribute value from the subject"); break;
		}
	}
	else if(msg==2){ 	// Outlets
		switch(arg){
			case 0: strcpy(dst, "attribute values from the subject"); break;
			case 1: strcpy(dst, "attributes message will dump attr names from the subject here"); break;
			default: strcpy(dst, "dumpout"); break;
		}
	}
}


void inquisitor_get(t_inquisitor *x, t_symbol *name)
{
	if(!x->subject){
		t_object* b = jpatcher_get_firstobject(x->patcher);

		while(b){			
			if(x->name == jbox_get_varname(b)){
				x->subject = jbox_get_object(b);
				break;
			}
			b = jbox_get_nextobject(b);
		}
	}
	
	if(x->subject && !NOGOOD(x->subject)){
		t_atom*		av = NULL;
		long		ac = 0;
		t_max_err	err;
		
		err = object_attr_getvalueof(x->subject, name, &ac, &av);
		if(!err && ac && av){
			for(long i=0; i<ac; i++){
				if(atom_gettype(av+i) != A_LONG && atom_gettype(av+i) != A_FLOAT && atom_gettype(av+i) != A_SYM){
					object_error((t_object*)x, "The type of data returned for this attribute value is not usable by tap.inquisitor: %s", name->s_name);
					err = MAX_ERR_GENERIC;
					break;
				}
			}
			if(!err)
				outlet_anything(x->outlet, name, ac, av);
		}
		else{
			object_error((t_object*)x, "problem getting attribute value for %s", name->s_name);
		}
		
		if(ac && av)
			sysmem_freeptr(av);
	}
}


void inquisitor_attributes(t_inquisitor* x)
{
	t_symbol**	names = NULL;
	long		count = 0;
	t_atom*		av = NULL;
	
	if(!x->subject){
		t_object* b = jpatcher_get_firstobject(x->patcher);
		
		while(b){			
			if(x->name == jbox_get_varname(b)){
				x->subject = jbox_get_object(b);
				break;
			}
			b = jbox_get_nextobject(b);
		}
	}

	if(x->subject){
		object_attr_getnames(x->subject, &count, (t_symbol***)&names);
		if(count && names){
			av = (t_atom*)sysmem_newptr(sizeof(t_atom) * count);
			for(long i=0; i<count; i++)
				atom_setsym(av+i, names[i]);
			outlet_anything(x->outlet_names, atom_getsym(av), count-1, av+1);
			
			sysmem_freeptr(av);
			sysmem_freeptr(names);
		}
	}
}


t_max_err inquisitor_set_name(t_inquisitor* x, void* attr, long ac, t_atom* av)
{
	if(ac && av){
		x->name = atom_getsym(av);
		x->subject = NULL;
	}
	return MAX_ERR_NONE;
}


