##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Physics
ConfigurationName      :=Debug
WorkspacePath          := "/Users/Taris/Documents/workshop/Monte_Carlo/BrownianWX"
ProjectPath            := "/Users/Taris/Documents/workshop/Monte_Carlo/BrownianWX/Physics"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Taris
Date                   :=2014-08-29
CodeLitePath           :="/Users/Taris/Library/Application Support/codelite"
LinkerName             :=/usr/bin/g++ 
SharedObjectLinkerName :=/usr/bin/g++ -dynamiclib -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).so
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="Physics.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++ 
CC       := /usr/bin/gcc 
CXXFLAGS :=  -g -O3 $(Preprocessors)
CFLAGS   :=  -g -O3 $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as 


##
## User defined environment variables
##
CodeLiteDir:=/Users/Taris/Downloads/codelite.app/Contents/SharedSupport/
Objects0=$(IntermediateDirectory)/Monte_Carlo_Gnuplot_Gen.cpp$(ObjectSuffix) $(IntermediateDirectory)/Monte_Carlo_Molecule.cpp$(ObjectSuffix) $(IntermediateDirectory)/Monte_Carlo_Photon.cpp$(ObjectSuffix) $(IntermediateDirectory)/Monte_Carlo_Physics.cpp$(ObjectSuffix) $(IntermediateDirectory)/Monte_Carlo_Random.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)
	@$(MakeDirCommand) "/Users/Taris/Documents/workshop/Monte_Carlo/BrownianWX/.build-debug"
	@echo rebuilt > "/Users/Taris/Documents/workshop/Monte_Carlo/BrownianWX/.build-debug/Physics"

$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/Monte_Carlo_Gnuplot_Gen.cpp$(ObjectSuffix): ../../Gnuplot_Gen.cpp $(IntermediateDirectory)/Monte_Carlo_Gnuplot_Gen.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/Taris/Documents/workshop/Monte_Carlo/Gnuplot_Gen.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Monte_Carlo_Gnuplot_Gen.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Monte_Carlo_Gnuplot_Gen.cpp$(DependSuffix): ../../Gnuplot_Gen.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Monte_Carlo_Gnuplot_Gen.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Monte_Carlo_Gnuplot_Gen.cpp$(DependSuffix) -MM "../../Gnuplot_Gen.cpp"

$(IntermediateDirectory)/Monte_Carlo_Gnuplot_Gen.cpp$(PreprocessSuffix): ../../Gnuplot_Gen.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Monte_Carlo_Gnuplot_Gen.cpp$(PreprocessSuffix) "../../Gnuplot_Gen.cpp"

$(IntermediateDirectory)/Monte_Carlo_Molecule.cpp$(ObjectSuffix): ../../Molecule.cpp $(IntermediateDirectory)/Monte_Carlo_Molecule.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/Taris/Documents/workshop/Monte_Carlo/Molecule.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Monte_Carlo_Molecule.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Monte_Carlo_Molecule.cpp$(DependSuffix): ../../Molecule.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Monte_Carlo_Molecule.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Monte_Carlo_Molecule.cpp$(DependSuffix) -MM "../../Molecule.cpp"

$(IntermediateDirectory)/Monte_Carlo_Molecule.cpp$(PreprocessSuffix): ../../Molecule.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Monte_Carlo_Molecule.cpp$(PreprocessSuffix) "../../Molecule.cpp"

$(IntermediateDirectory)/Monte_Carlo_Photon.cpp$(ObjectSuffix): ../../Photon.cpp $(IntermediateDirectory)/Monte_Carlo_Photon.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/Taris/Documents/workshop/Monte_Carlo/Photon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Monte_Carlo_Photon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Monte_Carlo_Photon.cpp$(DependSuffix): ../../Photon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Monte_Carlo_Photon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Monte_Carlo_Photon.cpp$(DependSuffix) -MM "../../Photon.cpp"

$(IntermediateDirectory)/Monte_Carlo_Photon.cpp$(PreprocessSuffix): ../../Photon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Monte_Carlo_Photon.cpp$(PreprocessSuffix) "../../Photon.cpp"

$(IntermediateDirectory)/Monte_Carlo_Physics.cpp$(ObjectSuffix): ../../Physics.cpp $(IntermediateDirectory)/Monte_Carlo_Physics.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/Taris/Documents/workshop/Monte_Carlo/Physics.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Monte_Carlo_Physics.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Monte_Carlo_Physics.cpp$(DependSuffix): ../../Physics.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Monte_Carlo_Physics.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Monte_Carlo_Physics.cpp$(DependSuffix) -MM "../../Physics.cpp"

$(IntermediateDirectory)/Monte_Carlo_Physics.cpp$(PreprocessSuffix): ../../Physics.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Monte_Carlo_Physics.cpp$(PreprocessSuffix) "../../Physics.cpp"

$(IntermediateDirectory)/Monte_Carlo_Random.cpp$(ObjectSuffix): ../../Random.cpp $(IntermediateDirectory)/Monte_Carlo_Random.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/Users/Taris/Documents/workshop/Monte_Carlo/Random.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Monte_Carlo_Random.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Monte_Carlo_Random.cpp$(DependSuffix): ../../Random.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Monte_Carlo_Random.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Monte_Carlo_Random.cpp$(DependSuffix) -MM "../../Random.cpp"

$(IntermediateDirectory)/Monte_Carlo_Random.cpp$(PreprocessSuffix): ../../Random.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Monte_Carlo_Random.cpp$(PreprocessSuffix) "../../Random.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) ./Debug/*$(ObjectSuffix)
	$(RM) ./Debug/*$(DependSuffix)
	$(RM) $(OutputFile)
	$(RM) "../.build-debug/Physics"


