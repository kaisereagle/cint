/********************************************************
* vcstrmold.cxx
********************************************************/
#include "vcstrmold.h"

#ifdef G__MEMTEST
#undef malloc
#undef free
#endif

extern "C" void G__cpp_reset_tagtableG__stream();

extern "C" void G__set_cpp_environmentG__stream() {
  G__add_compiledheader("iostrm.h");
  G__add_compiledheader("fstrm.h");
  G__add_compiledheader("stdiostr.h");
  G__add_compiledheader("strstrm.h");
  G__cpp_reset_tagtableG__stream();
}
static void* operator new(size_t size,void* p) {
  if(p && (long)p==G__getgvp() && G__PVOID!=G__getgvp()) return(p);
#ifndef G__ROOT
  return(malloc(size));
#else
  return new char[size];
#endif
}
#include "dllrev.h"
extern "C" int G__cpp_dllrevG__stream() { return(G__CREATEDLLREV); }

/*********************************************************
* Member function Interface Method
*********************************************************/

/* streambuf */
static int G__G__stream_0_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 1:
      G__letint(result7,105,(long)((streambuf*)(G__getstructoffset()))->overflow((int)G__int(libp->para[0])));
      break;
   case 0:
      G__letint(result7,105,(long)((streambuf*)(G__getstructoffset()))->overflow());
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_0_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((streambuf*)(G__getstructoffset()))->underflow());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_0_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((streambuf*)(G__getstructoffset()))->pbackfail((int)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_0_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((streambuf*)(G__getstructoffset()))->sync());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_0_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 3:
      G__letint(result7,108,(long)((streambuf*)(G__getstructoffset()))->seekoff((streamoff)G__int(libp->para[0]),(ios::seek_dir)G__int(libp->para[1])
,(int)G__int(libp->para[2])));
      break;
   case 2:
      G__letint(result7,108,(long)((streambuf*)(G__getstructoffset()))->seekoff((streamoff)G__int(libp->para[0]),(ios::seek_dir)G__int(libp->para[1])));
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_0_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 2:
      G__letint(result7,108,(long)((streambuf*)(G__getstructoffset()))->seekpos((streampos)G__int(libp->para[0]),(int)G__int(libp->para[1])));
      break;
   case 1:
      G__letint(result7,108,(long)((streambuf*)(G__getstructoffset()))->seekpos((streampos)G__int(libp->para[0])));
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_0_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((streambuf*)(G__getstructoffset()))->xsputn((const char*)G__int(libp->para[0]),(int)G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_0_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((streambuf*)(G__getstructoffset()))->xsgetn((char*)G__int(libp->para[0]),(int)G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_0_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((streambuf*)(G__getstructoffset()))->in_avail());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_0_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((streambuf*)(G__getstructoffset()))->out_waiting());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_0_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((streambuf*)(G__getstructoffset()))->sgetc());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_0_2_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((streambuf*)(G__getstructoffset()))->snextc());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_0_3_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((streambuf*)(G__getstructoffset()))->sbumpc());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_0_4_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((streambuf*)(G__getstructoffset()))->stossc();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_0_5_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((streambuf*)(G__getstructoffset()))->sputbackc((char)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_0_6_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((streambuf*)(G__getstructoffset()))->sputc((int)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_0_7_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((streambuf*)(G__getstructoffset()))->sputn((const char*)G__int(libp->para[0]),(int)G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_0_8_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((streambuf*)(G__getstructoffset()))->sgetn((char*)G__int(libp->para[0]),(int)G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__G__stream_0_1_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (streambuf *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((streambuf *)((G__getstructoffset())+sizeof(streambuf)*i))->~streambuf();
   else if(G__PVOID==G__getgvp()) delete (streambuf *)(G__getstructoffset());
   else ((streambuf *)(G__getstructoffset()))->~streambuf();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* ostream */
static int G__G__stream_1_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   ostream *p=NULL;
      p=new((void*)G__getgvp()) ostream((streambuf*)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_ostream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((ostream*)(G__getstructoffset()))->opfx());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((ostream*)(G__getstructoffset()))->osfx();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->flush();
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->seekp((streampos)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->seekp((streamoff)G__int(libp->para[0]),(ios::seek_dir)G__int(libp->para[1]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((ostream*)(G__getstructoffset()))->tellp());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->put((char)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->operator<<((char)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->operator<<((unsigned char)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->operator<<((const char*)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_2_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->operator<<((int)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_3_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->operator<<((long)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_4_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->operator<<((double)G__double(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_5_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->operator<<((float)G__double(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_6_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->operator<<((unsigned int)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_7_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->operator<<((unsigned long)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_8_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->operator<<((void*)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_9_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->operator<<((streambuf*)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_0_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->operator<<((short)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_1_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->operator<<((unsigned short)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_2_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->operator<<((ostream& (*)(ostream&))G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_1_3_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=((ostream*)(G__getstructoffset()))->write((const char*)G__int(libp->para[0]),(int)G__int(libp->para[1]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__G__stream_1_4_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (ostream *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((ostream *)((G__getstructoffset())+sizeof(ostream)*i))->~ostream();
   else if(G__PVOID==G__getgvp()) delete (ostream *)(G__getstructoffset());
   else ((ostream *)(G__getstructoffset()))->~ostream();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* ios */
static int G__G__stream_2_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((const ios*)(G__getstructoffset()))->flags());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((ios*)(G__getstructoffset()))->flags((long)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((ios*)(G__getstructoffset()))->setf((long)G__int(libp->para[0]),(long)G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((ios*)(G__getstructoffset()))->setf((long)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((ios*)(G__getstructoffset()))->unsetf((long)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((const ios*)(G__getstructoffset()))->width());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((ios*)(G__getstructoffset()))->width((int)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((ios*)(G__getstructoffset()))->tie((ostream*)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((ios*)(G__getstructoffset()))->tie());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,99,(long)((ios*)(G__getstructoffset()))->fill((char)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_2_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,99,(long)((const ios*)(G__getstructoffset()))->fill());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_3_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((ios*)(G__getstructoffset()))->precision((int)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_4_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((const ios*)(G__getstructoffset()))->precision());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_5_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((const ios*)(G__getstructoffset()))->rdstate());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_6_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,89,(long)((ios*)(G__getstructoffset()))->operator void*());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_7_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((const ios*)(G__getstructoffset()))->operator!());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_8_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((const ios*)(G__getstructoffset()))->eof());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_9_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((const ios*)(G__getstructoffset()))->fail());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_0_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((const ios*)(G__getstructoffset()))->bad());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_1_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((const ios*)(G__getstructoffset()))->good());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_2_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 1:
      G__setnull(result7);
      ((ios*)(G__getstructoffset()))->clear((int)G__int(libp->para[0]));
      break;
   case 0:
      G__setnull(result7);
      ((ios*)(G__getstructoffset()))->clear();
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_3_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((ios*)(G__getstructoffset()))->rdbuf());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_4_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const long& obj=((ios*)(G__getstructoffset()))->iword((int)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_5_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        void*& obj=((ios*)(G__getstructoffset()))->pword((int)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_6_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)ios::bitalloc());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_7_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)ios::xalloc());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_2_9_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ios::sync_with_stdio();
   return(1 || funcname || hash || result7 || libp) ;
}


/* istream */
static int G__G__stream_7_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   istream *p=NULL;
      p=new((void*)G__getgvp()) istream((streambuf*)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_istream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((istream*)(G__getstructoffset()))->ipfx((int)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((istream*)(G__getstructoffset()))->isfx();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->seekg((streampos)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->seekg((streamoff)G__int(libp->para[0]),(ios::seek_dir)G__int(libp->para[1]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((istream*)(G__getstructoffset()))->tellg());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->operator>>((istream& (*)(istream&))G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->operator>>((char*)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->operator>>((unsigned char*)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->operator>>(*(unsigned char*)G__UCharref(&libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->operator>>(*(char*)G__Charref(&libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_2_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->operator>>(*(short*)G__Shortref(&libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_3_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->operator>>(*(int*)G__Intref(&libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_4_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->operator>>(*(long*)G__Longref(&libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_5_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->operator>>(*(unsigned short*)G__UShortref(&libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_6_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->operator>>(*(unsigned int*)G__UIntref(&libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_7_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->operator>>(*(unsigned long*)G__ULongref(&libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_8_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->operator>>(*(float*)G__Floatref(&libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_9_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->operator>>(*(double*)G__Doubleref(&libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_0_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->operator>>((streambuf*)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_1_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 3:
      {
        const istream& obj=((istream*)(G__getstructoffset()))->get((char*)G__int(libp->para[0]),(int)G__int(libp->para[1])
,(char)G__int(libp->para[2]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
      break;
   case 2:
      {
        const istream& obj=((istream*)(G__getstructoffset()))->get((char*)G__int(libp->para[0]),(int)G__int(libp->para[1]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_2_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 3:
      {
        const istream& obj=((istream*)(G__getstructoffset()))->getline((char*)G__int(libp->para[0]),(int)G__int(libp->para[1])
,(char)G__int(libp->para[2]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
      break;
   case 2:
      {
        const istream& obj=((istream*)(G__getstructoffset()))->getline((char*)G__int(libp->para[0]),(int)G__int(libp->para[1]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_3_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 3:
      {
        const istream& obj=((istream*)(G__getstructoffset()))->getline((unsigned char*)G__int(libp->para[0]),(int)G__int(libp->para[1])
,(char)G__int(libp->para[2]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
      break;
   case 2:
      {
        const istream& obj=((istream*)(G__getstructoffset()))->getline((unsigned char*)G__int(libp->para[0]),(int)G__int(libp->para[1]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_4_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 2:
      {
        const istream& obj=((istream*)(G__getstructoffset()))->get(*(streambuf*)libp->para[0].ref,(char)G__int(libp->para[1]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
      break;
   case 1:
      {
        const istream& obj=((istream*)(G__getstructoffset()))->get(*(streambuf*)libp->para[0].ref);
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_5_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->get(*(unsigned char*)G__UCharref(&libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_6_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->get(*(char*)G__Charref(&libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_7_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((istream*)(G__getstructoffset()))->get());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_8_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((istream*)(G__getstructoffset()))->peek());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_9_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 2:
      {
        const istream& obj=((istream*)(G__getstructoffset()))->ignore((int)G__int(libp->para[0]),(int)G__int(libp->para[1]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
      break;
   case 1:
      {
        const istream& obj=((istream*)(G__getstructoffset()))->ignore((int)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
      break;
   case 0:
      {
        const istream& obj=((istream*)(G__getstructoffset()))->ignore();
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_0_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->read((char*)G__int(libp->para[0]),(int)G__int(libp->para[1]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_1_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((istream*)(G__getstructoffset()))->gcount());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_7_2_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=((istream*)(G__getstructoffset()))->putback((char)G__int(libp->para[0]));
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__G__stream_7_4_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (istream *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((istream *)((G__getstructoffset())+sizeof(istream)*i))->~istream();
   else if(G__PVOID==G__getgvp()) delete (istream *)(G__getstructoffset());
   else ((istream *)(G__getstructoffset()))->~istream();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* iostream */
static int G__G__stream_8_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   iostream *p=NULL;
      p=new((void*)G__getgvp()) iostream((streambuf*)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_iostream);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__G__stream_8_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (iostream *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((iostream *)((G__getstructoffset())+sizeof(iostream)*i))->~iostream();
   else if(G__PVOID==G__getgvp()) delete (iostream *)(G__getstructoffset());
   else ((iostream *)(G__getstructoffset()))->~iostream();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* filebuf */
static int G__G__stream_9_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   filebuf *p=NULL;
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       p=new filebuf[G__getaryconstruct()];
     else {
       for(int i=0;i<G__getaryconstruct();i++)
         p=new((void*)(G__getgvp()+sizeof(filebuf)*i)) filebuf;
       p=(filebuf*)G__getgvp();
     }
   else p=new((void*)G__getgvp()) filebuf;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_filebuf);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_9_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   filebuf *p=NULL;
      p=new((void*)G__getgvp()) filebuf((int)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_filebuf);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_9_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   filebuf *p=NULL;
      p=new((void*)G__getgvp()) filebuf(
(int)G__int(libp->para[0]),(char*)G__int(libp->para[1])
,(int)G__int(libp->para[2]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_filebuf);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_9_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((filebuf*)(G__getstructoffset()))->is_open());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_9_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((filebuf*)(G__getstructoffset()))->fd());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_9_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 3:
   G__letint(result7,85,(long)((filebuf*)(G__getstructoffset()))->open((const char*)G__int(libp->para[0]),(int)G__int(libp->para[1])
,(int)G__int(libp->para[2])));
      break;
   case 2:
   G__letint(result7,85,(long)((filebuf*)(G__getstructoffset()))->open((const char*)G__int(libp->para[0]),(int)G__int(libp->para[1])));
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_9_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((filebuf*)(G__getstructoffset()))->attach((int)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_9_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((filebuf*)(G__getstructoffset()))->close());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_9_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 1:
      G__letint(result7,105,(long)((filebuf*)(G__getstructoffset()))->overflow((int)G__int(libp->para[0])));
      break;
   case 0:
      G__letint(result7,105,(long)((filebuf*)(G__getstructoffset()))->overflow());
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_9_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((filebuf*)(G__getstructoffset()))->underflow());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_9_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((filebuf*)(G__getstructoffset()))->sync());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_9_2_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((filebuf*)(G__getstructoffset()))->seekoff((streamoff)G__int(libp->para[0]),(ios::seek_dir)G__int(libp->para[1])
,(int)G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_9_3_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((filebuf*)(G__getstructoffset()))->setbuf((char*)G__int(libp->para[0]),(int)G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__G__stream_9_4_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (filebuf *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((filebuf *)((G__getstructoffset())+sizeof(filebuf)*i))->~filebuf();
   else if(G__PVOID==G__getgvp()) delete (filebuf *)(G__getstructoffset());
   else ((filebuf *)(G__getstructoffset()))->~filebuf();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* ifstream */
static int G__G__stream_10_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   ifstream *p=NULL;
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       p=new ifstream[G__getaryconstruct()];
     else {
       for(int i=0;i<G__getaryconstruct();i++)
         p=new((void*)(G__getgvp()+sizeof(ifstream)*i)) ifstream;
       p=(ifstream*)G__getgvp();
     }
   else p=new((void*)G__getgvp()) ifstream;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_ifstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_10_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   ifstream *p=NULL;
   switch(libp->paran) {
   case 3:
      p = new((void*)G__getgvp()) ifstream(
(const char*)G__int(libp->para[0]),(int)G__int(libp->para[1])
,(int)G__int(libp->para[2]));
      break;
   case 2:
      p = new((void*)G__getgvp()) ifstream((const char*)G__int(libp->para[0]),(int)G__int(libp->para[1]));
      break;
   case 1:
      p = new((void*)G__getgvp()) ifstream((const char*)G__int(libp->para[0]));
      break;
   }
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_ifstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_10_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   ifstream *p=NULL;
      p=new((void*)G__getgvp()) ifstream((int)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_ifstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_10_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   ifstream *p=NULL;
      p=new((void*)G__getgvp()) ifstream(
(int)G__int(libp->para[0]),(char*)G__int(libp->para[1])
,(int)G__int(libp->para[2]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_ifstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_10_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((ifstream*)(G__getstructoffset()))->setbuf((char*)G__int(libp->para[0]),(int)G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_10_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((const ifstream*)(G__getstructoffset()))->rdbuf());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_10_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((ifstream*)(G__getstructoffset()))->attach((int)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_10_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((const ifstream*)(G__getstructoffset()))->fd());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_10_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((const ifstream*)(G__getstructoffset()))->is_open());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_10_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 3:
      G__setnull(result7);
      ((ifstream*)(G__getstructoffset()))->open((const char*)G__int(libp->para[0]),(int)G__int(libp->para[1])
,(int)G__int(libp->para[2]));
      break;
   case 2:
      G__setnull(result7);
      ((ifstream*)(G__getstructoffset()))->open((const char*)G__int(libp->para[0]),(int)G__int(libp->para[1]));
      break;
   case 1:
      G__setnull(result7);
      ((ifstream*)(G__getstructoffset()))->open((const char*)G__int(libp->para[0]));
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_10_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((ifstream*)(G__getstructoffset()))->close();
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__G__stream_10_2_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (ifstream *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((ifstream *)((G__getstructoffset())+sizeof(ifstream)*i))->~ifstream();
   else if(G__PVOID==G__getgvp()) delete (ifstream *)(G__getstructoffset());
   else ((ifstream *)(G__getstructoffset()))->~ifstream();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* ofstream */
static int G__G__stream_11_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   ofstream *p=NULL;
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       p=new ofstream[G__getaryconstruct()];
     else {
       for(int i=0;i<G__getaryconstruct();i++)
         p=new((void*)(G__getgvp()+sizeof(ofstream)*i)) ofstream;
       p=(ofstream*)G__getgvp();
     }
   else p=new((void*)G__getgvp()) ofstream;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_ofstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_11_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   ofstream *p=NULL;
   switch(libp->paran) {
   case 3:
      p = new((void*)G__getgvp()) ofstream(
(const char*)G__int(libp->para[0]),(int)G__int(libp->para[1])
,(int)G__int(libp->para[2]));
      break;
   case 2:
      p = new((void*)G__getgvp()) ofstream((const char*)G__int(libp->para[0]),(int)G__int(libp->para[1]));
      break;
   case 1:
      p = new((void*)G__getgvp()) ofstream((const char*)G__int(libp->para[0]));
      break;
   }
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_ofstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_11_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   ofstream *p=NULL;
      p=new((void*)G__getgvp()) ofstream((int)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_ofstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_11_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   ofstream *p=NULL;
      p=new((void*)G__getgvp()) ofstream(
(int)G__int(libp->para[0]),(char*)G__int(libp->para[1])
,(int)G__int(libp->para[2]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_ofstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_11_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((ofstream*)(G__getstructoffset()))->setbuf((char*)G__int(libp->para[0]),(int)G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_11_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((const ofstream*)(G__getstructoffset()))->rdbuf());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_11_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((ofstream*)(G__getstructoffset()))->attach((int)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_11_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((const ofstream*)(G__getstructoffset()))->fd());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_11_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((const ofstream*)(G__getstructoffset()))->is_open());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_11_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 3:
      G__setnull(result7);
      ((ofstream*)(G__getstructoffset()))->open((const char*)G__int(libp->para[0]),(int)G__int(libp->para[1])
,(int)G__int(libp->para[2]));
      break;
   case 2:
      G__setnull(result7);
      ((ofstream*)(G__getstructoffset()))->open((const char*)G__int(libp->para[0]),(int)G__int(libp->para[1]));
      break;
   case 1:
      G__setnull(result7);
      ((ofstream*)(G__getstructoffset()))->open((const char*)G__int(libp->para[0]));
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_11_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((ofstream*)(G__getstructoffset()))->close();
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__G__stream_11_2_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (ofstream *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((ofstream *)((G__getstructoffset())+sizeof(ofstream)*i))->~ofstream();
   else if(G__PVOID==G__getgvp()) delete (ofstream *)(G__getstructoffset());
   else ((ofstream *)(G__getstructoffset()))->~ofstream();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* fstream */
static int G__G__stream_12_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   fstream *p=NULL;
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       p=new fstream[G__getaryconstruct()];
     else {
       for(int i=0;i<G__getaryconstruct();i++)
         p=new((void*)(G__getgvp()+sizeof(fstream)*i)) fstream;
       p=(fstream*)G__getgvp();
     }
   else p=new((void*)G__getgvp()) fstream;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_fstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_12_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   fstream *p=NULL;
   switch(libp->paran) {
   case 3:
      p = new((void*)G__getgvp()) fstream(
(const char*)G__int(libp->para[0]),(int)G__int(libp->para[1])
,(int)G__int(libp->para[2]));
      break;
   case 2:
      p = new((void*)G__getgvp()) fstream((const char*)G__int(libp->para[0]),(int)G__int(libp->para[1]));
      break;
   }
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_fstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_12_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   fstream *p=NULL;
      p=new((void*)G__getgvp()) fstream((int)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_fstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_12_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   fstream *p=NULL;
      p=new((void*)G__getgvp()) fstream(
(int)G__int(libp->para[0]),(char*)G__int(libp->para[1])
,(int)G__int(libp->para[2]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_fstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_12_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((fstream*)(G__getstructoffset()))->setbuf((char*)G__int(libp->para[0]),(int)G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_12_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((const fstream*)(G__getstructoffset()))->rdbuf());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_12_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((fstream*)(G__getstructoffset()))->attach((int)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_12_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((const fstream*)(G__getstructoffset()))->fd());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_12_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((const fstream*)(G__getstructoffset()))->is_open());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_12_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 3:
      G__setnull(result7);
      ((fstream*)(G__getstructoffset()))->open((const char*)G__int(libp->para[0]),(int)G__int(libp->para[1])
,(int)G__int(libp->para[2]));
      break;
   case 2:
      G__setnull(result7);
      ((fstream*)(G__getstructoffset()))->open((const char*)G__int(libp->para[0]),(int)G__int(libp->para[1]));
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_12_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((fstream*)(G__getstructoffset()))->close();
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__G__stream_12_2_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (fstream *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((fstream *)((G__getstructoffset())+sizeof(fstream)*i))->~fstream();
   else if(G__PVOID==G__getgvp()) delete (fstream *)(G__getstructoffset());
   else ((fstream *)(G__getstructoffset()))->~fstream();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* strstreambuf */
static int G__G__stream_17_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   strstreambuf *p=NULL;
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       p=new strstreambuf[G__getaryconstruct()];
     else {
       for(int i=0;i<G__getaryconstruct();i++)
         p=new((void*)(G__getgvp()+sizeof(strstreambuf)*i)) strstreambuf;
       p=(strstreambuf*)G__getgvp();
     }
   else p=new((void*)G__getgvp()) strstreambuf;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_strstreambuf);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_17_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   strstreambuf *p=NULL;
      p=new((void*)G__getgvp()) strstreambuf((int)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_strstreambuf);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_17_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   strstreambuf *p=NULL;
   switch(libp->paran) {
   case 3:
      p = new((void*)G__getgvp()) strstreambuf(
(char*)G__int(libp->para[0]),(int)G__int(libp->para[1])
,(char*)G__int(libp->para[2]));
      break;
   case 2:
      p = new((void*)G__getgvp()) strstreambuf((char*)G__int(libp->para[0]),(int)G__int(libp->para[1]));
      break;
   }
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_strstreambuf);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_17_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 1:
      G__setnull(result7);
      ((strstreambuf*)(G__getstructoffset()))->freeze((int)G__int(libp->para[0]));
      break;
   case 0:
      G__setnull(result7);
      ((strstreambuf*)(G__getstructoffset()))->freeze();
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_17_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((strstreambuf*)(G__getstructoffset()))->str());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_17_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((strstreambuf*)(G__getstructoffset()))->setbuf((char*)G__int(libp->para[0]),(int)G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_17_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((strstreambuf*)(G__getstructoffset()))->seekoff((streamoff)G__int(libp->para[0]),(ios::seek_dir)G__int(libp->para[1])
,(int)G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__G__stream_17_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (strstreambuf *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((strstreambuf *)((G__getstructoffset())+sizeof(strstreambuf)*i))->~strstreambuf();
   else if(G__PVOID==G__getgvp()) delete (strstreambuf *)(G__getstructoffset());
   else ((strstreambuf *)(G__getstructoffset()))->~strstreambuf();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* istrstream */
static int G__G__stream_18_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   istrstream *p=NULL;
      p=new((void*)G__getgvp()) istrstream((char*)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_istrstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_18_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   istrstream *p=NULL;
      p=new((void*)G__getgvp()) istrstream((char*)G__int(libp->para[0]),(int)G__int(libp->para[1]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_istrstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_18_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   istrstream *p=NULL;
      p=new((void*)G__getgvp()) istrstream((const char*)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_istrstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_18_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   istrstream *p=NULL;
      p=new((void*)G__getgvp()) istrstream((const char*)G__int(libp->para[0]),(int)G__int(libp->para[1]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_istrstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_18_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((const istrstream*)(G__getstructoffset()))->rdbuf());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_18_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((istrstream*)(G__getstructoffset()))->str());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__G__stream_18_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (istrstream *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((istrstream *)((G__getstructoffset())+sizeof(istrstream)*i))->~istrstream();
   else if(G__PVOID==G__getgvp()) delete (istrstream *)(G__getstructoffset());
   else ((istrstream *)(G__getstructoffset()))->~istrstream();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* ostrstream */
static int G__G__stream_19_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   ostrstream *p=NULL;
   switch(libp->paran) {
   case 3:
      p = new((void*)G__getgvp()) ostrstream(
(char*)G__int(libp->para[0]),(int)G__int(libp->para[1])
,(int)G__int(libp->para[2]));
      break;
   case 2:
      p = new((void*)G__getgvp()) ostrstream((char*)G__int(libp->para[0]),(int)G__int(libp->para[1]));
      break;
   }
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_ostrstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_19_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   ostrstream *p=NULL;
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       p=new ostrstream[G__getaryconstruct()];
     else {
       for(int i=0;i<G__getaryconstruct();i++)
         p=new((void*)(G__getgvp()+sizeof(ostrstream)*i)) ostrstream;
       p=(ostrstream*)G__getgvp();
     }
   else p=new((void*)G__getgvp()) ostrstream;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_ostrstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_19_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((const ostrstream*)(G__getstructoffset()))->pcount());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_19_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((const ostrstream*)(G__getstructoffset()))->rdbuf());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_19_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((ostrstream*)(G__getstructoffset()))->str());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__G__stream_19_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (ostrstream *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((ostrstream *)((G__getstructoffset())+sizeof(ostrstream)*i))->~ostrstream();
   else if(G__PVOID==G__getgvp()) delete (ostrstream *)(G__getstructoffset());
   else ((ostrstream *)(G__getstructoffset()))->~ostrstream();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* strstream */
static int G__G__stream_20_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   strstream *p=NULL;
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       p=new strstream[G__getaryconstruct()];
     else {
       for(int i=0;i<G__getaryconstruct();i++)
         p=new((void*)(G__getgvp()+sizeof(strstream)*i)) strstream;
       p=(strstream*)G__getgvp();
     }
   else p=new((void*)G__getgvp()) strstream;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_strstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_20_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   strstream *p=NULL;
      p=new((void*)G__getgvp()) strstream(
(char*)G__int(libp->para[0]),(int)G__int(libp->para[1])
,(int)G__int(libp->para[2]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__streamLN_strstream);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_20_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((const strstream*)(G__getstructoffset()))->pcount());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_20_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((const strstream*)(G__getstructoffset()))->rdbuf());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream_20_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((strstream*)(G__getstructoffset()))->str());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__G__stream_20_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (strstream *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((strstream *)((G__getstructoffset())+sizeof(strstream)*i))->~strstream();
   else if(G__PVOID==G__getgvp()) delete (strstream *)(G__getstructoffset());
   else ((strstream *)(G__getstructoffset()))->~strstream();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* Setting up global function */
static int G__G__stream__0_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ios& obj=dec(*(ios*)libp->para[0].ref);
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream__1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=endl(*(ostream*)libp->para[0].ref);
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream__2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=ends(*(ostream*)libp->para[0].ref);
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream__3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ostream& obj=flush(*(ostream*)libp->para[0].ref);
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream__4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ios& obj=hex(*(ios*)libp->para[0].ref);
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream__5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const ios& obj=oct(*(ios*)libp->para[0].ref);
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__stream__6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const istream& obj=ws(*(istream*)libp->para[0].ref);
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}


/*********************************************************
* Member function Stub
*********************************************************/

/* streambuf */

/* ostream */

/* ios */

/* istream */

/* iostream */

/* filebuf */

/* ifstream */

/* ofstream */

/* fstream */

/* strstreambuf */

/* istrstream */

/* ostrstream */

/* strstream */

/*********************************************************
* Global function Stub
*********************************************************/

/*********************************************************
* Get size of pointer to member function
*********************************************************/
class G__Sizep2memfuncG__stream {
 public:
  G__Sizep2memfuncG__stream() {p=&G__Sizep2memfuncG__stream::sizep2memfunc;}
    size_t sizep2memfunc() { return(sizeof(p)); }
  private:
    size_t (G__Sizep2memfuncG__stream::*p)();
};

size_t G__get_sizep2memfuncG__stream()
{
  G__Sizep2memfuncG__stream a;
  G__setsizep2memfunc((int)a.sizep2memfunc());
  return((size_t)a.sizep2memfunc());
}


/*********************************************************
* virtual base class offset calculation interface
*********************************************************/

   /* Setting up class inheritance */
static long G__2vbo_ostream_ios_0(long pobject) {
  ostream *G__Lderived=(ostream*)pobject;
  ios *G__Lbase=G__Lderived;
  return((long)G__Lbase-(long)G__Lderived);
}

static long G__2vbo_istream_ios_0(long pobject) {
  istream *G__Lderived=(istream*)pobject;
  ios *G__Lbase=G__Lderived;
  return((long)G__Lbase-(long)G__Lderived);
}

static long G__2vbo_iostream_ios_1(long pobject) {
  iostream *G__Lderived=(iostream*)pobject;
  ios *G__Lbase=G__Lderived;
  return((long)G__Lbase-(long)G__Lderived);
}

static long G__2vbo_iostream_ios_3(long pobject) {
  iostream *G__Lderived=(iostream*)pobject;
  ios *G__Lbase=G__Lderived;
  return((long)G__Lbase-(long)G__Lderived);
}

static long G__2vbo_ifstream_ios_1(long pobject) {
  ifstream *G__Lderived=(ifstream*)pobject;
  ios *G__Lbase=G__Lderived;
  return((long)G__Lbase-(long)G__Lderived);
}

static long G__2vbo_ofstream_ios_1(long pobject) {
  ofstream *G__Lderived=(ofstream*)pobject;
  ios *G__Lbase=G__Lderived;
  return((long)G__Lbase-(long)G__Lderived);
}

static long G__2vbo_fstream_ios_2(long pobject) {
  fstream *G__Lderived=(fstream*)pobject;
  ios *G__Lbase=G__Lderived;
  return((long)G__Lbase-(long)G__Lderived);
}

static long G__2vbo_fstream_ios_4(long pobject) {
  fstream *G__Lderived=(fstream*)pobject;
  ios *G__Lbase=G__Lderived;
  return((long)G__Lbase-(long)G__Lderived);
}

static long G__2vbo_istrstream_ios_1(long pobject) {
  istrstream *G__Lderived=(istrstream*)pobject;
  ios *G__Lbase=G__Lderived;
  return((long)G__Lbase-(long)G__Lderived);
}

static long G__2vbo_ostrstream_ios_1(long pobject) {
  ostrstream *G__Lderived=(ostrstream*)pobject;
  ios *G__Lbase=G__Lderived;
  return((long)G__Lbase-(long)G__Lderived);
}

static long G__2vbo_strstream_ios_2(long pobject) {
  strstream *G__Lderived=(strstream*)pobject;
  ios *G__Lbase=G__Lderived;
  return((long)G__Lbase-(long)G__Lderived);
}

static long G__2vbo_strstream_ios_4(long pobject) {
  strstream *G__Lderived=(strstream*)pobject;
  ios *G__Lbase=G__Lderived;
  return((long)G__Lbase-(long)G__Lderived);
}


/*********************************************************
* Inheritance information setup/
*********************************************************/
extern "C" void G__cpp_setup_inheritanceG__stream() {

   /* Setting up class inheritance */
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__G__streamLN_ostream))) {
     {
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_ostream),G__get_linked_tagnum(&G__G__streamLN_ios),(long)G__2vbo_ostream_ios_0,1,3);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__G__streamLN_istream))) {
     {
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_istream),G__get_linked_tagnum(&G__G__streamLN_ios),(long)G__2vbo_istream_ios_0,1,3);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__G__streamLN_iostream))) {
     iostream *G__Lderived;
     G__Lderived=(iostream*)0x1000;
     {
       istream *G__Lpbase=(istream*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_iostream),G__get_linked_tagnum(&G__G__streamLN_istream),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
     {
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_iostream),G__get_linked_tagnum(&G__G__streamLN_ios),(long)G__2vbo_iostream_ios_1,1,2);
     }
     {
       ostream *G__Lpbase=(ostream*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_iostream),G__get_linked_tagnum(&G__G__streamLN_ostream),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
     {
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_iostream),G__get_linked_tagnum(&G__G__streamLN_ios),(long)G__2vbo_iostream_ios_3,1,2);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__G__streamLN_filebuf))) {
     filebuf *G__Lderived;
     G__Lderived=(filebuf*)0x1000;
     {
       streambuf *G__Lpbase=(streambuf*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_filebuf),G__get_linked_tagnum(&G__G__streamLN_streambuf),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__G__streamLN_ifstream))) {
     ifstream *G__Lderived;
     G__Lderived=(ifstream*)0x1000;
     {
       istream *G__Lpbase=(istream*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_ifstream),G__get_linked_tagnum(&G__G__streamLN_istream),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
     {
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_ifstream),G__get_linked_tagnum(&G__G__streamLN_ios),(long)G__2vbo_ifstream_ios_1,1,2);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__G__streamLN_ofstream))) {
     ofstream *G__Lderived;
     G__Lderived=(ofstream*)0x1000;
     {
       ostream *G__Lpbase=(ostream*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_ofstream),G__get_linked_tagnum(&G__G__streamLN_ostream),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
     {
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_ofstream),G__get_linked_tagnum(&G__G__streamLN_ios),(long)G__2vbo_ofstream_ios_1,1,2);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__G__streamLN_fstream))) {
     fstream *G__Lderived;
     G__Lderived=(fstream*)0x1000;
     {
       iostream *G__Lpbase=(iostream*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_fstream),G__get_linked_tagnum(&G__G__streamLN_iostream),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
     {
       istream *G__Lpbase=(istream*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_fstream),G__get_linked_tagnum(&G__G__streamLN_istream),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
     {
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_fstream),G__get_linked_tagnum(&G__G__streamLN_ios),(long)G__2vbo_fstream_ios_2,1,2);
     }
     {
       ostream *G__Lpbase=(ostream*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_fstream),G__get_linked_tagnum(&G__G__streamLN_ostream),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
     {
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_fstream),G__get_linked_tagnum(&G__G__streamLN_ios),(long)G__2vbo_fstream_ios_4,1,2);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__G__streamLN_strstreambuf))) {
     strstreambuf *G__Lderived;
     G__Lderived=(strstreambuf*)0x1000;
     {
       streambuf *G__Lpbase=(streambuf*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_strstreambuf),G__get_linked_tagnum(&G__G__streamLN_streambuf),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__G__streamLN_istrstream))) {
     istrstream *G__Lderived;
     G__Lderived=(istrstream*)0x1000;
     {
       istream *G__Lpbase=(istream*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_istrstream),G__get_linked_tagnum(&G__G__streamLN_istream),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
     {
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_istrstream),G__get_linked_tagnum(&G__G__streamLN_ios),(long)G__2vbo_istrstream_ios_1,1,2);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__G__streamLN_ostrstream))) {
     ostrstream *G__Lderived;
     G__Lderived=(ostrstream*)0x1000;
     {
       ostream *G__Lpbase=(ostream*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_ostrstream),G__get_linked_tagnum(&G__G__streamLN_ostream),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
     {
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_ostrstream),G__get_linked_tagnum(&G__G__streamLN_ios),(long)G__2vbo_ostrstream_ios_1,1,2);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__G__streamLN_strstream))) {
     strstream *G__Lderived;
     G__Lderived=(strstream*)0x1000;
     {
       iostream *G__Lpbase=(iostream*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_strstream),G__get_linked_tagnum(&G__G__streamLN_iostream),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
     {
       istream *G__Lpbase=(istream*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_strstream),G__get_linked_tagnum(&G__G__streamLN_istream),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
     {
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_strstream),G__get_linked_tagnum(&G__G__streamLN_ios),(long)G__2vbo_strstream_ios_2,1,2);
     }
     {
       ostream *G__Lpbase=(ostream*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_strstream),G__get_linked_tagnum(&G__G__streamLN_ostream),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
     {
       G__inheritance_setup(G__get_linked_tagnum(&G__G__streamLN_strstream),G__get_linked_tagnum(&G__G__streamLN_ios),(long)G__2vbo_strstream_ios_4,1,2);
     }
   }
}

/*********************************************************
* typedef information setup/
*********************************************************/
extern "C" void G__cpp_setup_typetableG__stream() {

   /* Setting up typedef entry */
   G__search_typename2("streampos",108,-1,0,
-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("streamoff",108,-1,0,
-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("istream& (*)(istream&)",81,-1,0,
-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("ios& (*)(ios&) ",81,-1,0,
-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("ostream& (*)(ostream&)",81,-1,0,
-1);
   G__setnewtype(-1,NULL,0);
}

/*********************************************************
* Data Member information setup/
*********************************************************/

   /* Setting up class,struct,union tag member variable */

   /* streambuf */
static void G__setup_memvarstreambuf(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__streamLN_streambuf));
   { streambuf *p; p=(streambuf*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}


   /* ostream */
static void G__setup_memvarostream(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__streamLN_ostream));
   { ostream *p; p=(ostream*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}


   /* ios */
static void G__setup_memvarios(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__streamLN_ios));
   { ios *p; p=(ios*)0x1000; if (p) { }
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLio_state),-1,-2,1,"goodbit=0",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLio_state),-1,-2,1,"eofbit=1",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLio_state),-1,-2,1,"failbit=2",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLio_state),-1,-2,1,"badbit=4",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLio_state),-1,-2,1,"hardfail=128",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLopen_mode),-1,-2,1,"in=1",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLopen_mode),-1,-2,1,"out=2",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLopen_mode),-1,-2,1,"ate=4",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLopen_mode),-1,-2,1,"app=8",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLopen_mode),-1,-2,1,"trunc=16",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLopen_mode),-1,-2,1,"nocreate=32",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLopen_mode),-1,-2,1,"noreplace=64",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLopen_mode),-1,-2,1,"binary=128",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLseek_dir),-1,-2,1,"beg=0",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLseek_dir),-1,-2,1,"cur=1",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLseek_dir),-1,-2,1,"end=2",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"skipws=1",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"left=2",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"right=4",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"internal=8",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"dec=16",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"oct=32",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"hex=64",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"showbase=128",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"showpoint=256",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"uppercase=512",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"showpos=1024",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"scientific=2048",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"fixed=4096",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"unitbuf=8192",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"stdio=16384",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"boolalpha=100000",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"adjustfield=14",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"basefield=112",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),-1,-2,1,"floatfield=6144",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* istream */
static void G__setup_memvaristream(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__streamLN_istream));
   { istream *p; p=(istream*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}


   /* iostream */
static void G__setup_memvariostream(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__streamLN_iostream));
   { iostream *p; p=(iostream*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}


   /* filebuf */
static void G__setup_memvarfilebuf(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__streamLN_filebuf));
   { filebuf *p; p=(filebuf*)0x1000; if (p) { }
   G__memvar_setup((void*)(&filebuf::openprot),105,0,1,-1,-1,-2,1,"openprot=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* ifstream */
static void G__setup_memvarifstream(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__streamLN_ifstream));
   { ifstream *p; p=(ifstream*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}


   /* ofstream */
static void G__setup_memvarofstream(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__streamLN_ofstream));
   { ofstream *p; p=(ofstream*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}


   /* fstream */
static void G__setup_memvarfstream(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__streamLN_fstream));
   { fstream *p; p=(fstream*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}


   /* strstreambuf */
static void G__setup_memvarstrstreambuf(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__streamLN_strstreambuf));
   { strstreambuf *p; p=(strstreambuf*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}


   /* istrstream */
static void G__setup_memvaristrstream(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__streamLN_istrstream));
   { istrstream *p; p=(istrstream*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}


   /* ostrstream */
static void G__setup_memvarostrstream(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__streamLN_ostrstream));
   { ostrstream *p; p=(ostrstream*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}


   /* strstream */
static void G__setup_memvarstrstream(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__streamLN_strstream));
   { strstream *p; p=(strstream*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}

extern "C" void G__cpp_setup_memvarG__stream() {
}
/***********************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
***********************************************************/

/*********************************************************
* Member function information setup for each class
*********************************************************/
static void G__setup_memfuncstreambuf(void) {
   /* streambuf */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__streamLN_streambuf));
   G__memfunc_setup("overflow",884,G__G__stream_0_1_0,105,-1,-1,0,1,1,1,0,"i - - 0 EOF c",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("underflow",982,G__G__stream_0_2_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("pbackfail",925,G__G__stream_0_3_0,105,-1,-1,0,1,1,1,0,"i - - 0 - c",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("sync",445,G__G__stream_0_4_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("seekoff",739,G__G__stream_0_5_0,108,-1,G__defined_typename("streampos"),0,3,1,1,0,
"l - 'streamoff' 0 - - i 'ios::seek_dir' - 0 - - "
"i - - 0 ios::in|ios::out -",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("seekpos",762,G__G__stream_0_6_0,108,-1,G__defined_typename("streampos"),0,2,1,1,0,
"l - 'streampos' 0 - - i - - 0 ios::in|ios::out -",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("xsputn",690,G__G__stream_0_7_0,105,-1,-1,0,2,1,1,0,
"C - - 10 - s i - - 0 - n",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("xsgetn",665,G__G__stream_0_8_0,105,-1,-1,0,2,1,1,0,
"C - - 0 - s i - - 0 - n",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("in_avail",835,G__G__stream_0_9_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("out_waiting",1194,G__G__stream_0_0_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("sgetc",534,G__G__stream_0_1_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("snextc",661,G__G__stream_0_2_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("sbumpc",650,G__G__stream_0_3_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("stossc",671,G__G__stream_0_4_1,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("sputbackc",960,G__G__stream_0_5_1,105,-1,-1,0,1,1,1,0,"c - - 0 - c",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("sputc",559,G__G__stream_0_6_1,105,-1,-1,0,1,1,1,0,"i - - 0 - c",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("sputn",570,G__G__stream_0_7_1,105,-1,-1,0,2,1,1,0,
"C - - 10 - s i - - 0 - n",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("sgetn",545,G__G__stream_0_8_1,105,-1,-1,0,2,1,1,0,
"C - - 0 - s i - - 0 - n",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("streambuf",969,(G__InterfaceMethod)NULL,105,G__get_linked_tagnum(&G__G__streamLN_streambuf),-1,0,0,1,2,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("streambuf",969,(G__InterfaceMethod)NULL,105,G__get_linked_tagnum(&G__G__streamLN_streambuf),-1,0,1,1,4,0,"u 'streambuf' - 1 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~streambuf",1095,G__G__stream_0_1_2,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncostream(void) {
   /* ostream */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__streamLN_ostream));
   G__memfunc_setup("ostream",763,G__G__stream_1_1_0,105,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,0,1,1,1,0,"U 'streambuf' - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("opfx",445,G__G__stream_1_2_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("osfx",448,G__G__stream_1_3_0,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("flush",546,G__G__stream_1_4_0,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("seekp",536,G__G__stream_1_5_0,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"l - 'streampos' 0 - p",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("seekp",536,G__G__stream_1_6_0,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,2,1,1,0,
"l - 'streamoff' 0 - o i 'ios::seek_dir' - 0 - d",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("tellp",545,G__G__stream_1_7_0,108,-1,G__defined_typename("streampos"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("put",345,G__G__stream_1_8_0,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"c - - 0 - c",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator<<",996,G__G__stream_1_9_0,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"c - - 0 - c",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator<<",996,G__G__stream_1_0_1,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"b - - 0 - c",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator<<",996,G__G__stream_1_1_1,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"C - - 10 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator<<",996,G__G__stream_1_2_1,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"i - - 0 - a",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator<<",996,G__G__stream_1_3_1,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"l - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator<<",996,G__G__stream_1_4_1,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"d - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator<<",996,G__G__stream_1_5_1,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"f - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator<<",996,G__G__stream_1_6_1,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"h - - 0 - a",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator<<",996,G__G__stream_1_7_1,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"k - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator<<",996,G__G__stream_1_8_1,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"Y - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator<<",996,G__G__stream_1_9_1,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"U 'streambuf' - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator<<",996,G__G__stream_1_0_2,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"s - - 0 - i",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator<<",996,G__G__stream_1_1_2,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"r - - 0 - i",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator<<",996,G__G__stream_1_2_2,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"Q - 'ostream& (*)(ostream&)' 0 - f",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("write",555,G__G__stream_1_3_2,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,2,1,1,0,
"C - - 10 - s i - - 0 - n",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~ostream",889,G__G__stream_1_4_2,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncios(void) {
   /* ios */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__streamLN_ios));
   G__memfunc_setup("ios",331,(G__InterfaceMethod)NULL,105,G__get_linked_tagnum(&G__G__streamLN_ios),-1,0,1,1,2,0,"U 'streambuf' - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("flags",525,G__G__stream_2_2_0,108,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("flags",525,G__G__stream_2_3_0,108,-1,-1,0,1,1,1,0,"l - - 0 - f",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("setf",434,G__G__stream_2_4_0,108,-1,-1,0,2,1,1,0,
"l - - 0 - setbits l - - 0 - field",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("setf",434,G__G__stream_2_5_0,108,-1,-1,0,1,1,1,0,"l - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("unsetf",661,G__G__stream_2_6_0,108,-1,-1,0,1,1,1,0,"l - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("width",544,G__G__stream_2_7_0,105,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("width",544,G__G__stream_2_8_0,105,-1,-1,0,1,1,1,0,"i - - 0 - w",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("tie",322,G__G__stream_2_9_0,85,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,0,1,1,1,0,"U 'ostream' - 0 - s",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("tie",322,G__G__stream_2_0_1,85,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("fill",423,G__G__stream_2_1_1,99,-1,-1,0,1,1,1,0,"c - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("fill",423,G__G__stream_2_2_1,99,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("precision",972,G__G__stream_2_3_1,105,-1,-1,0,1,1,1,0,"i - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("precision",972,G__G__stream_2_4_1,105,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("rdstate",759,G__G__stream_2_5_1,105,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator void*",1384,G__G__stream_2_6_1,89,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator!",909,G__G__stream_2_7_1,105,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("eof",314,G__G__stream_2_8_1,105,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("fail",412,G__G__stream_2_9_1,105,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("bad",295,G__G__stream_2_0_2,105,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("good",425,G__G__stream_2_1_2,105,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("clear",519,G__G__stream_2_2_2,121,-1,-1,0,1,1,1,0,"i - - 0 0 i",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("rdbuf",531,G__G__stream_2_3_2,85,G__get_linked_tagnum(&G__G__streamLN_streambuf),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("iword",549,G__G__stream_2_4_2,108,-1,-1,1,1,1,1,0,"i - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("pword",556,G__G__stream_2_5_2,89,-1,-1,1,1,1,1,0,"i - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("bitalloc",842,G__G__stream_2_6_2,108,-1,-1,0,0,3,1,0,"",(char*)NULL,(void*)(long (*)())(&ios::bitalloc),0);
   G__memfunc_setup("xalloc",643,G__G__stream_2_7_2,105,-1,-1,0,0,3,1,0,"",(char*)NULL,(void*)(int (*)())(&ios::xalloc),0);
   G__memfunc_setup("sync_with_stdio",1626,G__G__stream_2_9_2,121,-1,-1,0,0,3,1,0,"",(char*)NULL,(void*)(void (*)())(&ios::sync_with_stdio),0);
   G__memfunc_setup("ios",331,(G__InterfaceMethod)NULL,105,G__get_linked_tagnum(&G__G__streamLN_ios),-1,0,0,1,2,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ios",331,(G__InterfaceMethod)NULL,105,G__get_linked_tagnum(&G__G__streamLN_ios),-1,0,1,1,4,0,"u 'ios' - 1 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator=",937,(G__InterfaceMethod)NULL,121,-1,-1,0,1,1,4,0,"u 'ios' - 1 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~ios",457,(G__InterfaceMethod)NULL,(int)('y'),-1,-1,0,0,1,2,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncistream(void) {
   /* istream */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__streamLN_istream));
   G__memfunc_setup("istream",757,G__G__stream_7_1_0,105,G__get_linked_tagnum(&G__G__streamLN_istream),-1,0,1,1,1,0,"U 'streambuf' - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ipfx",439,G__G__stream_7_2_0,105,-1,-1,0,1,1,1,0,"i - - 0 - noskipws",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("isfx",442,G__G__stream_7_3_0,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("seekg",527,G__G__stream_7_4_0,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"l - 'streampos' 0 - p",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("seekg",527,G__G__stream_7_5_0,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,2,1,1,0,
"l - 'streamoff' 0 - o i 'ios::seek_dir' - 0 - d",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("tellg",536,G__G__stream_7_6_0,108,-1,G__defined_typename("streampos"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator>>",1000,G__G__stream_7_7_0,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"Q - 'istream& (*)(istream&)' 0 - f",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator>>",1000,G__G__stream_7_8_0,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"C - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator>>",1000,G__G__stream_7_9_0,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"B - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator>>",1000,G__G__stream_7_0_1,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"b - - 1 - c",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator>>",1000,G__G__stream_7_1_1,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"c - - 1 - c",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator>>",1000,G__G__stream_7_2_1,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"s - - 1 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator>>",1000,G__G__stream_7_3_1,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"i - - 1 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator>>",1000,G__G__stream_7_4_1,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"l - - 1 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator>>",1000,G__G__stream_7_5_1,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"r - - 1 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator>>",1000,G__G__stream_7_6_1,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"h - - 1 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator>>",1000,G__G__stream_7_7_1,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"k - - 1 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator>>",1000,G__G__stream_7_8_1,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"f - - 1 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator>>",1000,G__G__stream_7_9_1,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"d - - 1 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator>>",1000,G__G__stream_7_0_2,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"U 'streambuf' - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("get",320,G__G__stream_7_1_2,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,3,1,1,0,
"C - - 0 - - i - - 0 - lim "
"c - - 0 '\\n' delim",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getline",744,G__G__stream_7_2_2,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,3,1,1,0,
"C - - 0 - b i - - 0 - lim "
"c - - 0 '\\n' delim",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getline",744,G__G__stream_7_3_2,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,3,1,1,0,
"B - - 0 - b i - - 0 - lim "
"c - - 0 '\\n' delim",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("get",320,G__G__stream_7_4_2,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,2,1,1,0,
"u 'streambuf' - 1 - sb c - - 0 '\\n' delim",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("get",320,G__G__stream_7_5_2,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"b - - 1 - c",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("get",320,G__G__stream_7_6_2,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"c - - 1 - c",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("get",320,G__G__stream_7_7_2,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("peek",421,G__G__stream_7_8_2,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ignore",644,G__G__stream_7_9_2,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,2,1,1,0,
"i - - 0 1 n i - - 0 EOF delim",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("read",412,G__G__stream_7_0_3,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,2,1,1,0,
"C - - 0 - s i - - 0 - n",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("gcount",656,G__G__stream_7_1_3,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("putback",746,G__G__stream_7_2_3,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"c - - 0 - c",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("istream",757,(G__InterfaceMethod)NULL,105,G__get_linked_tagnum(&G__G__streamLN_istream),-1,0,0,1,2,0,"",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~istream",883,G__G__stream_7_4_3,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfunciostream(void) {
   /* iostream */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__streamLN_iostream));
   G__memfunc_setup("iostream",868,G__G__stream_8_1_0,105,G__get_linked_tagnum(&G__G__streamLN_iostream),-1,0,1,1,1,0,"U 'streambuf' - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("iostream",868,(G__InterfaceMethod)NULL,105,G__get_linked_tagnum(&G__G__streamLN_iostream),-1,0,0,1,2,0,"",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~iostream",994,G__G__stream_8_3_0,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncfilebuf(void) {
   /* filebuf */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__streamLN_filebuf));
   G__memfunc_setup("filebuf",733,G__G__stream_9_1_0,105,G__get_linked_tagnum(&G__G__streamLN_filebuf),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("filebuf",733,G__G__stream_9_2_0,105,G__get_linked_tagnum(&G__G__streamLN_filebuf),-1,0,1,1,1,0,"i - - 0 - fd",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("filebuf",733,G__G__stream_9_3_0,105,G__get_linked_tagnum(&G__G__streamLN_filebuf),-1,0,3,1,1,0,
"i - - 0 - fd C - - 0 - p "
"i - - 0 - l",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("is_open",749,G__G__stream_9_4_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("fd",202,G__G__stream_9_5_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("open",434,G__G__stream_9_6_0,85,G__get_linked_tagnum(&G__G__streamLN_filebuf),-1,0,3,1,1,0,
"C - - 10 - name i - - 0 - om "
"i - - 0 openprot prot",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("attach",629,G__G__stream_9_7_0,85,G__get_linked_tagnum(&G__G__streamLN_filebuf),-1,0,1,1,1,0,"i - - 0 - fd",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("close",534,G__G__stream_9_8_0,85,G__get_linked_tagnum(&G__G__streamLN_filebuf),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("overflow",884,G__G__stream_9_9_0,105,-1,-1,0,1,1,1,0,"i - - 0 EOF -",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("underflow",982,G__G__stream_9_0_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("sync",445,G__G__stream_9_1_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("seekoff",739,G__G__stream_9_2_1,108,-1,G__defined_typename("streampos"),0,3,1,1,0,
"l - 'streamoff' 0 - - i 'ios::seek_dir' - 0 - - "
"i - - 0 - -",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("setbuf",649,G__G__stream_9_3_1,85,G__get_linked_tagnum(&G__G__streamLN_streambuf),-1,0,2,1,1,0,
"C - - 0 - p i - - 0 - len",(char*)NULL,(void*)NULL,1);
   // automatic destructor
   G__memfunc_setup("~filebuf",859,G__G__stream_9_4_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncifstream(void) {
   /* ifstream */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__streamLN_ifstream));
   G__memfunc_setup("ifstream",859,G__G__stream_10_1_0,105,G__get_linked_tagnum(&G__G__streamLN_ifstream),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ifstream",859,G__G__stream_10_2_0,105,G__get_linked_tagnum(&G__G__streamLN_ifstream),-1,0,3,1,1,0,
"C - - 10 - name i - - 0 ios::in mode "
"i - - 0 filebuf::openprot prot",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ifstream",859,G__G__stream_10_3_0,105,G__get_linked_tagnum(&G__G__streamLN_ifstream),-1,0,1,1,1,0,"i - - 0 - fd",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ifstream",859,G__G__stream_10_4_0,105,G__get_linked_tagnum(&G__G__streamLN_ifstream),-1,0,3,1,1,0,
"i - - 0 - fd C - - 0 - p "
"i - - 0 - l",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("setbuf",649,G__G__stream_10_5_0,85,G__get_linked_tagnum(&G__G__streamLN_streambuf),-1,0,2,1,1,0,
"C - - 0 - - i - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("rdbuf",531,G__G__stream_10_6_0,85,G__get_linked_tagnum(&G__G__streamLN_filebuf),-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("attach",629,G__G__stream_10_7_0,121,-1,-1,0,1,1,1,0,"i - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("fd",202,G__G__stream_10_8_0,105,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("is_open",749,G__G__stream_10_9_0,105,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("open",434,G__G__stream_10_0_1,121,-1,-1,0,3,1,1,0,
"C - - 10 - - i - - 0 ios::in - "
"i - - 0 filebuf::openprot -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("close",534,G__G__stream_10_1_1,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~ifstream",985,G__G__stream_10_2_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncofstream(void) {
   /* ofstream */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__streamLN_ofstream));
   G__memfunc_setup("ofstream",865,G__G__stream_11_1_0,105,G__get_linked_tagnum(&G__G__streamLN_ofstream),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ofstream",865,G__G__stream_11_2_0,105,G__get_linked_tagnum(&G__G__streamLN_ofstream),-1,0,3,1,1,0,
"C - - 10 - name i - - 0 ios::out mode "
"i - - 0 filebuf::openprot prot",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ofstream",865,G__G__stream_11_3_0,105,G__get_linked_tagnum(&G__G__streamLN_ofstream),-1,0,1,1,1,0,"i - - 0 - fd",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ofstream",865,G__G__stream_11_4_0,105,G__get_linked_tagnum(&G__G__streamLN_ofstream),-1,0,3,1,1,0,
"i - - 0 - fd C - - 0 - p "
"i - - 0 - l",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("setbuf",649,G__G__stream_11_5_0,85,G__get_linked_tagnum(&G__G__streamLN_streambuf),-1,0,2,1,1,0,
"C - - 0 - - i - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("rdbuf",531,G__G__stream_11_6_0,85,G__get_linked_tagnum(&G__G__streamLN_filebuf),-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("attach",629,G__G__stream_11_7_0,121,-1,-1,0,1,1,1,0,"i - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("fd",202,G__G__stream_11_8_0,105,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("is_open",749,G__G__stream_11_9_0,105,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("open",434,G__G__stream_11_0_1,121,-1,-1,0,3,1,1,0,
"C - - 10 - - i - - 0 ios::out - "
"i - - 0 filebuf::openprot -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("close",534,G__G__stream_11_1_1,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~ofstream",991,G__G__stream_11_2_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncfstream(void) {
   /* fstream */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__streamLN_fstream));
   G__memfunc_setup("fstream",754,G__G__stream_12_1_0,105,G__get_linked_tagnum(&G__G__streamLN_fstream),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("fstream",754,G__G__stream_12_2_0,105,G__get_linked_tagnum(&G__G__streamLN_fstream),-1,0,3,1,1,0,
"C - - 10 - name i - - 0 - mode "
"i - - 0 filebuf::openprot prot",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("fstream",754,G__G__stream_12_3_0,105,G__get_linked_tagnum(&G__G__streamLN_fstream),-1,0,1,1,1,0,"i - - 0 - fd",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("fstream",754,G__G__stream_12_4_0,105,G__get_linked_tagnum(&G__G__streamLN_fstream),-1,0,3,1,1,0,
"i - - 0 - fd C - - 0 - p "
"i - - 0 - l",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("setbuf",649,G__G__stream_12_5_0,85,G__get_linked_tagnum(&G__G__streamLN_streambuf),-1,0,2,1,1,0,
"C - - 0 - - i - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("rdbuf",531,G__G__stream_12_6_0,85,G__get_linked_tagnum(&G__G__streamLN_filebuf),-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("attach",629,G__G__stream_12_7_0,121,-1,-1,0,1,1,1,0,"i - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("fd",202,G__G__stream_12_8_0,105,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("is_open",749,G__G__stream_12_9_0,105,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("open",434,G__G__stream_12_0_1,121,-1,-1,0,3,1,1,0,
"C - - 10 - - i - - 0 - - "
"i - - 0 filebuf::openprot -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("close",534,G__G__stream_12_1_1,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~fstream",880,G__G__stream_12_2_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncstrstreambuf(void) {
   /* strstreambuf */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__streamLN_strstreambuf));
   G__memfunc_setup("strstreambuf",1314,G__G__stream_17_1_0,105,G__get_linked_tagnum(&G__G__streamLN_strstreambuf),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("strstreambuf",1314,G__G__stream_17_2_0,105,G__get_linked_tagnum(&G__G__streamLN_strstreambuf),-1,0,1,1,1,0,"i - - 0 - -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("strstreambuf",1314,G__G__stream_17_3_0,105,G__get_linked_tagnum(&G__G__streamLN_strstreambuf),-1,0,3,1,1,0,
"C - - 0 - b i - - 0 - size "
"C - - 0 0 pstart",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("freeze",641,G__G__stream_17_4_0,121,-1,-1,0,1,1,1,0,"i - - 0 1 n",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("str",345,G__G__stream_17_5_0,67,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("setbuf",649,G__G__stream_17_9_0,85,G__get_linked_tagnum(&G__G__streamLN_streambuf),-1,0,2,1,1,0,
"C - - 0 - p i - - 0 - l",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("seekoff",739,G__G__stream_17_0_1,108,-1,G__defined_typename("streampos"),0,3,1,1,0,
"l - 'streamoff' 0 - - i 'ios::seek_dir' - 0 - - "
"i - - 0 - -",(char*)NULL,(void*)NULL,1);
   // automatic destructor
   G__memfunc_setup("~strstreambuf",1440,G__G__stream_17_1_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncistrstream(void) {
   /* istrstream */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__streamLN_istrstream));
   G__memfunc_setup("istrstream",1102,G__G__stream_18_1_0,105,G__get_linked_tagnum(&G__G__streamLN_istrstream),-1,0,1,1,1,0,"C - - 0 - str",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("istrstream",1102,G__G__stream_18_2_0,105,G__get_linked_tagnum(&G__G__streamLN_istrstream),-1,0,2,1,1,0,
"C - - 0 - str i - - 0 - size",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("istrstream",1102,G__G__stream_18_3_0,105,G__get_linked_tagnum(&G__G__streamLN_istrstream),-1,0,1,1,1,0,"C - - 10 - str",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("istrstream",1102,G__G__stream_18_4_0,105,G__get_linked_tagnum(&G__G__streamLN_istrstream),-1,0,2,1,1,0,
"C - - 10 - str i - - 0 - size",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("rdbuf",531,G__G__stream_18_5_0,85,G__get_linked_tagnum(&G__G__streamLN_strstreambuf),-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("str",345,G__G__stream_18_6_0,67,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~istrstream",1228,G__G__stream_18_7_0,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncostrstream(void) {
   /* ostrstream */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__streamLN_ostrstream));
   G__memfunc_setup("ostrstream",1108,G__G__stream_19_1_0,105,G__get_linked_tagnum(&G__G__streamLN_ostrstream),-1,0,3,1,1,0,
"C - - 0 - str i - - 0 - size "
"i - - 0 ios::out -",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ostrstream",1108,G__G__stream_19_2_0,105,G__get_linked_tagnum(&G__G__streamLN_ostrstream),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("pcount",665,G__G__stream_19_3_0,105,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("rdbuf",531,G__G__stream_19_4_0,85,G__get_linked_tagnum(&G__G__streamLN_strstreambuf),-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("str",345,G__G__stream_19_5_0,67,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~ostrstream",1234,G__G__stream_19_6_0,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncstrstream(void) {
   /* strstream */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__streamLN_strstream));
   G__memfunc_setup("strstream",997,G__G__stream_20_1_0,105,G__get_linked_tagnum(&G__G__streamLN_strstream),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("strstream",997,G__G__stream_20_2_0,105,G__get_linked_tagnum(&G__G__streamLN_strstream),-1,0,3,1,1,0,
"C - - 0 - str i - - 0 - size "
"i - - 0 - mode",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("pcount",665,G__G__stream_20_3_0,105,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("rdbuf",531,G__G__stream_20_4_0,85,G__get_linked_tagnum(&G__G__streamLN_strstreambuf),-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("str",345,G__G__stream_20_5_0,67,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~strstream",1123,G__G__stream_20_6_0,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}


/*********************************************************
* Member function information setup
*********************************************************/
extern "C" void G__cpp_setup_memfuncG__stream() {
}

/*********************************************************
* Global variable information setup for each class
*********************************************************/
static void G__cpp_setup_global0() {

   /* Setting up global variables */
   G__resetplocal();

   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__IOSTREAM_H=0",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__MANIP_SUPPORT=0",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__OLDIMPLEMENTATION762=0",1,(char*)NULL);
   G__memvar_setup((void*)(&cin),117,0,0,G__get_linked_tagnum(&G__G__streamLN_istream),-1,-1,1,"cin=",0,(char*)NULL);
   G__memvar_setup((void*)(&cout),117,0,0,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,-1,1,"cout=",0,(char*)NULL);
   G__memvar_setup((void*)(&cerr),117,0,0,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,-1,1,"cerr=",0,(char*)NULL);
   G__memvar_setup((void*)(&clog),117,0,0,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,-1,1,"clog=",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__FSTREAM_H=0",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__STDSTREAM_H=0",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__STRSTREAM_H=0",1,(char*)NULL);

   G__resetglobalenv();
}
extern "C" void G__cpp_setup_globalG__stream() {
  G__cpp_setup_global0();
}

/*********************************************************
* Global function information setup for each class
*********************************************************/
static void G__cpp_setup_func0() {
   G__lastifuncposition();

   G__memfunc_setup("dec",300,G__G__stream__0_0,117,G__get_linked_tagnum(&G__G__streamLN_ios),-1,1,1,1,1,0,"u 'ios' - 1 - -",(char*)NULL
,(void*)NULL,0);
   G__memfunc_setup("endl",419,G__G__stream__1_0,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"u 'ostream' - 1 - i",(char*)NULL
,(void*)NULL,0);
   G__memfunc_setup("ends",426,G__G__stream__2_0,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"u 'ostream' - 1 - i",(char*)NULL
,(void*)NULL,0);
   G__memfunc_setup("flush",546,G__G__stream__3_0,117,G__get_linked_tagnum(&G__G__streamLN_ostream),-1,1,1,1,1,0,"u 'ostream' - 1 - -",(char*)NULL
,(void*)NULL,0);
   G__memfunc_setup("hex",325,G__G__stream__4_0,117,G__get_linked_tagnum(&G__G__streamLN_ios),-1,1,1,1,1,0,"u 'ios' - 1 - -",(char*)NULL
,(void*)NULL,0);
   G__memfunc_setup("oct",326,G__G__stream__5_0,117,G__get_linked_tagnum(&G__G__streamLN_ios),-1,1,1,1,1,0,"u 'ios' - 1 - -",(char*)NULL
,(void*)NULL,0);
   G__memfunc_setup("ws",234,G__G__stream__6_0,117,G__get_linked_tagnum(&G__G__streamLN_istream),-1,1,1,1,1,0,"u 'istream' - 1 - -",(char*)NULL
,(void*)NULL,0);
}

static void G__cpp_setup_func1() {

   G__resetifuncposition();
}

extern "C" void G__cpp_setup_funcG__stream() {
  G__cpp_setup_func0();
  G__cpp_setup_func1();
}

/*********************************************************
* Class,struct,union,enum tag information setup
*********************************************************/
/* Setup class/struct taginfo */
G__linked_taginfo G__G__streamLN_streambuf = { "streambuf" , 99 , -1 };
G__linked_taginfo G__G__streamLN_ostream = { "ostream" , 99 , -1 };
G__linked_taginfo G__G__streamLN_ios = { "ios" , 99 , -1 };
G__linked_taginfo G__G__streamLN_ioscLcLio_state = { "ios::io_state" , 101 , -1 };
G__linked_taginfo G__G__streamLN_ioscLcLopen_mode = { "ios::open_mode" , 101 , -1 };
G__linked_taginfo G__G__streamLN_ioscLcLseek_dir = { "ios::seek_dir" , 101 , -1 };
G__linked_taginfo G__G__streamLN_ioscLcLdA = { "ios::$" , 101 , -1 };
G__linked_taginfo G__G__streamLN_istream = { "istream" , 99 , -1 };
G__linked_taginfo G__G__streamLN_iostream = { "iostream" , 99 , -1 };
G__linked_taginfo G__G__streamLN_filebuf = { "filebuf" , 99 , -1 };
G__linked_taginfo G__G__streamLN_ifstream = { "ifstream" , 99 , -1 };
G__linked_taginfo G__G__streamLN_ofstream = { "ofstream" , 99 , -1 };
G__linked_taginfo G__G__streamLN_fstream = { "fstream" , 99 , -1 };
G__linked_taginfo G__G__streamLN_strstreambuf = { "strstreambuf" , 99 , -1 };
G__linked_taginfo G__G__streamLN_istrstream = { "istrstream" , 99 , -1 };
G__linked_taginfo G__G__streamLN_ostrstream = { "ostrstream" , 99 , -1 };
G__linked_taginfo G__G__streamLN_strstream = { "strstream" , 99 , -1 };

/* Reset class/struct taginfo */
extern "C" void G__cpp_reset_tagtableG__stream() {
  G__G__streamLN_streambuf.tagnum = -1 ;
  G__G__streamLN_ostream.tagnum = -1 ;
  G__G__streamLN_ios.tagnum = -1 ;
  G__G__streamLN_ioscLcLio_state.tagnum = -1 ;
  G__G__streamLN_ioscLcLopen_mode.tagnum = -1 ;
  G__G__streamLN_ioscLcLseek_dir.tagnum = -1 ;
  G__G__streamLN_ioscLcLdA.tagnum = -1 ;
  G__G__streamLN_istream.tagnum = -1 ;
  G__G__streamLN_iostream.tagnum = -1 ;
  G__G__streamLN_filebuf.tagnum = -1 ;
  G__G__streamLN_ifstream.tagnum = -1 ;
  G__G__streamLN_ofstream.tagnum = -1 ;
  G__G__streamLN_fstream.tagnum = -1 ;
  G__G__streamLN_strstreambuf.tagnum = -1 ;
  G__G__streamLN_istrstream.tagnum = -1 ;
  G__G__streamLN_ostrstream.tagnum = -1 ;
  G__G__streamLN_strstream.tagnum = -1 ;
}


extern "C" void G__cpp_setup_tagtableG__stream() {

   /* Setting up class,struct,union tag entry */
   G__tagtable_setup(G__get_linked_tagnum(&G__G__streamLN_streambuf),sizeof(streambuf),-1,1792,(char*)NULL,G__setup_memvarstreambuf,G__setup_memfuncstreambuf);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__streamLN_ostream),sizeof(ostream),-1,33792,(char*)NULL,G__setup_memvarostream,G__setup_memfuncostream);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__streamLN_ios),sizeof(ios),-1,36608,(char*)NULL,G__setup_memvarios,G__setup_memfuncios);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__streamLN_ioscLcLio_state),0,-1,0,(char*)NULL,NULL,NULL);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__streamLN_ioscLcLopen_mode),0,-1,0,(char*)NULL,NULL,NULL);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__streamLN_ioscLcLseek_dir),0,-1,0,(char*)NULL,NULL,NULL);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__streamLN_ioscLcLdA),0,-1,0,(char*)NULL,NULL,NULL);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__streamLN_istream),sizeof(istream),-1,34048,(char*)NULL,G__setup_memvaristream,G__setup_memfuncistream);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__streamLN_iostream),sizeof(iostream),-1,34048,(char*)NULL,G__setup_memvariostream,G__setup_memfunciostream);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__streamLN_filebuf),sizeof(filebuf),-1,34048,(char*)NULL,G__setup_memvarfilebuf,G__setup_memfuncfilebuf);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__streamLN_ifstream),sizeof(ifstream),-1,34048,(char*)NULL,G__setup_memvarifstream,G__setup_memfuncifstream);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__streamLN_ofstream),sizeof(ofstream),-1,34048,(char*)NULL,G__setup_memvarofstream,G__setup_memfuncofstream);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__streamLN_fstream),sizeof(fstream),-1,34048,(char*)NULL,G__setup_memvarfstream,G__setup_memfuncfstream);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__streamLN_strstreambuf),sizeof(strstreambuf),-1,34048,(char*)NULL,G__setup_memvarstrstreambuf,G__setup_memfuncstrstreambuf);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__streamLN_istrstream),sizeof(istrstream),-1,33792,(char*)NULL,G__setup_memvaristrstream,G__setup_memfuncistrstream);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__streamLN_ostrstream),sizeof(ostrstream),-1,34048,(char*)NULL,G__setup_memvarostrstream,G__setup_memfuncostrstream);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__streamLN_strstream),sizeof(strstream),-1,34048,(char*)NULL,G__setup_memvarstrstream,G__setup_memfuncstrstream);
}
extern "C" void G__cpp_setupG__stream(void) {
  G__check_setup_version(G__CREATEDLLREV,"G__cpp_setupG__stream()");
  G__set_cpp_environmentG__stream();
  G__cpp_setup_tagtableG__stream();

  G__cpp_setup_inheritanceG__stream();

  G__cpp_setup_typetableG__stream();

  G__cpp_setup_memvarG__stream();

  G__cpp_setup_memfuncG__stream();
  G__cpp_setup_globalG__stream();
  G__cpp_setup_funcG__stream();

   if(0==G__getsizep2memfunc()) G__get_sizep2memfuncG__stream();
  return;
}
