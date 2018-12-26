/* dummy init routine for C++ defined symbol initialization */

void G__initcxx() 
{
}

void G__init_replacesymbol() {}
void G__add_replacesymbol(const char* s1,const char* s2) {}
const char* G__replacesymbol(const char* s) { return(s); }
int G__display_replacesymbol(FILE *fout,const char* name) { return(0); }
