/*==================================================================*/
/*macro per la compilazione one call di tutte le classi usate       */
#include <TSystem.h>

/*==================================================================*/

void CompileAllClasses(TString myopt="fast"){
  char * opt;
  if(myopt.Contains("force")){
    opt = "kfg";
  }
  else {
    opt = "kg";
  }
  gSystem->CompileMacro("AliOCDBRun.h",opt);
  gSystem->CompileMacro("TSmartTree.h",opt);
  gSystem->CompileMacro("AliRPCValueDCS.cpp",opt);
  gSystem->CompileMacro("AliRPCValueCurrent.cpp",opt);
  gSystem->CompileMacro("AliRPCValueVoltage.cpp",opt);
  gSystem->CompileMacro("AliRPCValueScaler.cpp",opt);
  gSystem->CompileMacro("AliRPCOverflowStatistics.cpp",opt);
  gSystem->CompileMacro("AliRPCRunStatistics.cpp",opt);
  gSystem->CompileMacro("AliRPCData.cpp",opt);
  gSystem->CompileMacro("AliRPCAutoIntegrator.cpp",opt);
}
