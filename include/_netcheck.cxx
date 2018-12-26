/*****************************************************************************
 * _netcheck.cxx
 *  Description: 
 *    Netlist checker
 *  Usage:
 *    cint _netcheck.cxx <option> [netList] [materialList]
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

#include <netcheck.dll>
#include <string>
//#include <CSV.h>

///////////////////////////////////////////////////////////////////////////////
GlobalMaterialList ml;
GlobalNetlist netlist;

ALLMaterialListReader mlreader;
ALLNetlistReader netlistreader;

MLfileformat mlfmt = MLSPICE;
NLfileformat nlfmt = NLSPICE;
extern FILE* output;

int main(int argc,char** argv) {
  
  for(int i=1;i<argc;i++) {
    string netfile,mlfile;
    // directory scan
    if(strcmp(argv[i],"-spice")==0) {mlfmt=MLSPICE;nlfmt=NLSPICE; }
    else if(strcmp(argv[i],"-mentor")==0) {mlfmt=MentorML;nlfmt=MentorNL; }
    else if(strcmp(argv[i],"-bom2")==0) {mlfmt=BOM2;nlfmt=MentorNL; }
    else if(strcmp(argv[i],"-bomcsv")==0) {mlfmt=BOMCSV;nlfmt=MentorNL; }
    else if(strcmp(argv[i],"-0912")==0) {mlfmt=ML0912;nlfmt=NL0912; }
    else if(strcmp(argv[i],"-auto")==0) {mlfmt=AutoML;nlfmt=AutoNL; }
    else if(strcmp(argv[i],"-net")==0) netfile = argv[++i];
    else if(strcmp(argv[i],"-ml")==0) mlfile = argv[++i];
    else if(strcmp(argv[i],"-v")==0) {vervose++;}

    // help
    else if(strcmp(argv[i],"-?")==0) {
      printf("Usage: cint _netcheck.cxx <option> [netList] [materialList]\n");
      printf("  -spice   : SPICE circuit\n");
      printf("  -mentor  : Mentor Graphics format\n");
      printf("  -bom2    : Mentor Graphics format 2\n");
      printf("  -bomcsv  : Mentor Graphics format 3\n");
      printf("  -auto    : auto judge\n");
      printf("  -v       : vervose mode\n");

      exit(0);
    }
    else {
      if(netfile=="") netfile = argv[i];
      else if(mlfile=="") mlfile = argv[i];
    }
  }

  if(mlfile!="") {
    mlreader.Read(mlfile,ml,BOM2);
    printf("mk.size=%d\n",ml.size(),mlfmt);
  }
  
  if(netfile!="") {
    netlistreader.Read(netfile,netlist,ml,nlfmt);
  }

  while(!G__pause());

  return(0);
}



