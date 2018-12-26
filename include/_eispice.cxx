/*****************************************************************************
 * _eispice.cxx
 *  Description: 
 *    Open source eispice embedded into CINT
 *  Usage:
 *    cint _eispice.cxx <option> [spice].cki <[spice].txt>
 *  Status: Experimental
 *  Date: 2015/Aug/3
 *  Author: Masaharu Goto
 *****************************************************************************/
/************************************************************************
 * Copyright(c) 1995~2015  Masaharu Goto (cint@pcroot.cern.ch)
 *
 * Permission to use, copy, modify and distribute this software and its 
 * documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation. The author makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 ************************************************************************/

//#include <NVector.h>
//#include <CSV.h>
#include <eispice.dll>
#pragma include_noerr <netcheck.dll> // optional component

DVector ix;
DVector v0,v1,v2,v3,v4,v5,v6,v7,v8,v9;

////////////////////////////////////////////////////////////////////////
int main(int argc,char** argv) {
  eispice spice;
  string cki,ckf,txt,raw;
  for(int i=1;i<argc;i++) {
    if(argv[i][0]=='-') {
      if(strcmp(argv[i],"-p")==0) prompt=1;
      else if(strcmp(argv[i],"-v")==0) vervose++;
      else if(strcmp(argv[i],"-gnu")==0) gnuplot_mode();
      else if(strcmp(argv[i],"-root")==0) cintgraph_mode();
      else {
	printf("Usage: cint _eispice.cxx <option> [spice].cki <[spice].txt>\n");
	printf("      -p     : interactive prompt\n");
	printf("      -v     : vervose mode\n");
	printf("      -gnu   : gnuplot graphics\n");
	printf("      -root  : ROOT graphics\n");
	exit(255);
      }
    }
    else {
      cki=argv[i];
      if(++i<argc) txt=argv[i];
      if(++i<argc) raw=argv[i];
      break;
    }
  }
  EasyExp r("*.cki","$1=.ckf");
  EasyExp rt("*.ck?","$1=.txt,$2=");
  rt.replace(cki,txt);
  if(r.replace(cki,ckf)) {
    printf("flattening spice deck %s to %s\n",cki.c_str(),ckf.c_str());
    GlobalMaterialList ml;
    GlobalNetlist netlist;
    ALLNetlistReader netlistreader;
    NLfileformat nlfmt = NLSPICE;
    netlistreader.Read(cki,netlist,ml,nlfmt);
    ml.spicenet(ckf);
  }
  printf("reading flatened spice deck %s\n",ckf.c_str());
  spice.readCkt(ckf);
  spice.deployCkt();
  printf("simulating %s\n",ckf.c_str());
  spice.simulate();
  printf("plot graph\n");
  spice.plotGraph();
  printf("output %s\n",txt.c_str());
  spice.outputTxt(txt);

  if(prompt) {
    printf("You have access to following data variables\n");
    printf("  vector<string>  signame;\n");
    printf("  vector<DVextor> datavec;\n");
    printf("  DMatrix         datamap;\n");
    while(!G__pause());
  }

  cintgraph_stop();
  return(0);
}


int G__ateval(const vector<string>& s) {
  for(int i=0;i<s.size();i++) printf("%d:%s ",i,signame[i].c_str());
  printf("\n");
}

int G__ateval(const DVector& v) {
  for(int i=0;i<v.size();i++) printf("%d:%g ",i,v[i]);
  printf("\n");
}

////////////////////////////////////////////////////////////////////////

