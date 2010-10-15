#############################################################################
# Makefile for building: libIscore.a
# Generated by qmake (2.01a) (Qt 4.7.0) on: ven. oct. 15 11:55:27 2010
# Project:  mac_iscore.pro
# Template: lib
# Command: /usr/bin/qmake -spec /usr/local/Qt4.7/mkspecs/macx-g++ -o Makefile mac_iscore.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = 
CFLAGS        = -pipe -O2 -fPIC $(DEFINES)
CXXFLAGS      = -pipe -O2 -fPIC $(DEFINES)
INCPATH       = -I/usr/local/Qt4.7/mkspecs/macx-g++ -I. -I. -IIncludes -I/Library/Frameworks -I/usr/local/include/IScore -I/usr/local/include -I/usr/local/include/libxml2 -I. -IIncludes -IEnginesPrivate -IECOMachine -IECOMachine/PetriNet -IECOMachine/Process -IEditor/CSP -IStoryLine -IEditor/gecode-solver -IEditor
AR            = ar cq
RANLIB        = ranlib -s
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
STRIP         = 
INSTALL_FILE  = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = $(COPY_FILE)
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
export MACOSX_DEPLOYMENT_TARGET = 10.4

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = Engines.cpp \
		ECOMachine/ChangeTempo.cpp \
		ECOMachine/ControlPointInformations.cpp \
		ECOMachine/ECOMachine.cpp \
		ECOMachine/TriggerPointInformations.cpp \
		Editor/Editor.cpp \
		EnginesPrivate/EnginesPrivate.cpp \
		StoryLine/StoryLine.cpp \
		ECOMachine/PetriNet/Arc.cpp \
		ECOMachine/PetriNet/ExtendedInt.cpp \
		ECOMachine/PetriNet/PetriNet.cpp \
		ECOMachine/PetriNet/PetriNetItem.cpp \
		ECOMachine/PetriNet/PetriNetNode.cpp \
		ECOMachine/PetriNet/Place.cpp \
		ECOMachine/PetriNet/PriorityTransitionAction.cpp \
		ECOMachine/PetriNet/ThreadSafeList.cpp \
		ECOMachine/PetriNet/Token.cpp \
		ECOMachine/PetriNet/Transition.cpp \
		ECOMachine/PetriNet/TransitionAction.cpp \
		ECOMachine/PetriNet/TransitionBitArray.cpp \
		ECOMachine/Process/MultiTypeCurve.cpp \
		ECOMachine/Process/NetworkMessageCurves.cpp \
		ECOMachine/Process/Process.cpp \
		ECOMachine/Process/SendNetworkMessageProcess.cpp \
		ECOMachine/Process/StringParser.cpp \
		ECOMachine/Process/StringTypeFunctions.cpp \
		Editor/CSP/AllenRelation.cpp \
		Editor/CSP/AntPostRelation.cpp \
		Editor/CSP/BinaryRelation.cpp \
		Editor/CSP/BinaryTemporalRelation.cpp \
		Editor/CSP/ConstrainedBox.cpp \
		Editor/CSP/ConstrainedTemporalEntity.cpp \
		Editor/CSP/ControlPoint.cpp \
		Editor/CSP/CSP.cpp \
		Editor/CSP/CSPConstrainedVariable.cpp \
		Editor/CSP/CSPLinearConstraint.cpp \
		Editor/CSP/IntervalRelation.cpp \
		Editor/CSP/TriggerPoint.cpp \
		Editor/CSP/XMLConversion.cpp \
		Editor/gecode-solver/customSpace.cpp \
		Editor/gecode-solver/integerVariable.cpp \
		Editor/gecode-solver/linearConstraint.cpp \
		Editor/gecode-solver/searchEngine.cpp \
		Editor/gecode-solver/solver.cpp \
		Editor/gecode-solver/solver_wrap.cpp 
OBJECTS       = Engines.o \
		ChangeTempo.o \
		ControlPointInformations.o \
		ECOMachine.o \
		TriggerPointInformations.o \
		Editor.o \
		EnginesPrivate.o \
		StoryLine.o \
		Arc.o \
		ExtendedInt.o \
		PetriNet.o \
		PetriNetItem.o \
		PetriNetNode.o \
		Place.o \
		PriorityTransitionAction.o \
		ThreadSafeList.o \
		Token.o \
		Transition.o \
		TransitionAction.o \
		TransitionBitArray.o \
		MultiTypeCurve.o \
		NetworkMessageCurves.o \
		Process.o \
		SendNetworkMessageProcess.o \
		StringParser.o \
		StringTypeFunctions.o \
		AllenRelation.o \
		AntPostRelation.o \
		BinaryRelation.o \
		BinaryTemporalRelation.o \
		ConstrainedBox.o \
		ConstrainedTemporalEntity.o \
		ControlPoint.o \
		CSP.o \
		CSPConstrainedVariable.o \
		CSPLinearConstraint.o \
		IntervalRelation.o \
		TriggerPoint.o \
		XMLConversion.o \
		customSpace.o \
		integerVariable.o \
		linearConstraint.o \
		searchEngine.o \
		solver.o \
		solver_wrap.o
DIST          = /usr/local/Qt4.7/mkspecs/common/unix.conf \
		/usr/local/Qt4.7/mkspecs/common/mac.conf \
		/usr/local/Qt4.7/mkspecs/common/mac-g++.conf \
		/usr/local/Qt4.7/mkspecs/qconfig.pri \
		/usr/local/Qt4.7/mkspecs/modules/qt_webkit_version.pri \
		/usr/local/Qt4.7/mkspecs/features/qt_functions.prf \
		/usr/local/Qt4.7/mkspecs/features/qt_config.prf \
		/usr/local/Qt4.7/mkspecs/features/exclusive_builds.prf \
		/usr/local/Qt4.7/mkspecs/features/default_pre.prf \
		/usr/local/Qt4.7/mkspecs/features/mac/default_pre.prf \
		/usr/local/Qt4.7/mkspecs/features/release.prf \
		/usr/local/Qt4.7/mkspecs/features/default_post.prf \
		/usr/local/Qt4.7/mkspecs/features/mac/default_post.prf \
		/usr/local/Qt4.7/mkspecs/features/mac/objective_c.prf \
		/usr/local/Qt4.7/mkspecs/features/staticlib.prf \
		/usr/local/Qt4.7/mkspecs/features/static.prf \
		mac_iscore.pro
QMAKE_TARGET  = Iscore
DESTDIR       = 
TARGET        = libIscore.a

####### Custom Compiler Variables
QMAKE_COMP_QMAKE_OBJECTIVE_CFLAGS = -pipe \
		-O2


first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET) 

staticlib: $(TARGET)

$(TARGET):  $(OBJECTS) $(OBJCOMP) 
	-$(DEL_FILE) $(TARGET)
	$(AR) $(TARGET) $(OBJECTS)
	$(RANLIB) $(TARGET)


Makefile: mac_iscore.pro  /usr/local/Qt4.7/mkspecs/macx-g++/qmake.conf /usr/local/Qt4.7/mkspecs/common/unix.conf \
		/usr/local/Qt4.7/mkspecs/common/mac.conf \
		/usr/local/Qt4.7/mkspecs/common/mac-g++.conf \
		/usr/local/Qt4.7/mkspecs/qconfig.pri \
		/usr/local/Qt4.7/mkspecs/modules/qt_webkit_version.pri \
		/usr/local/Qt4.7/mkspecs/features/qt_functions.prf \
		/usr/local/Qt4.7/mkspecs/features/qt_config.prf \
		/usr/local/Qt4.7/mkspecs/features/exclusive_builds.prf \
		/usr/local/Qt4.7/mkspecs/features/default_pre.prf \
		/usr/local/Qt4.7/mkspecs/features/mac/default_pre.prf \
		/usr/local/Qt4.7/mkspecs/features/release.prf \
		/usr/local/Qt4.7/mkspecs/features/default_post.prf \
		/usr/local/Qt4.7/mkspecs/features/mac/default_post.prf \
		/usr/local/Qt4.7/mkspecs/features/mac/objective_c.prf \
		/usr/local/Qt4.7/mkspecs/features/staticlib.prf \
		/usr/local/Qt4.7/mkspecs/features/static.prf
	$(QMAKE) -spec /usr/local/Qt4.7/mkspecs/macx-g++ -o Makefile mac_iscore.pro
/usr/local/Qt4.7/mkspecs/common/unix.conf:
/usr/local/Qt4.7/mkspecs/common/mac.conf:
/usr/local/Qt4.7/mkspecs/common/mac-g++.conf:
/usr/local/Qt4.7/mkspecs/qconfig.pri:
/usr/local/Qt4.7/mkspecs/modules/qt_webkit_version.pri:
/usr/local/Qt4.7/mkspecs/features/qt_functions.prf:
/usr/local/Qt4.7/mkspecs/features/qt_config.prf:
/usr/local/Qt4.7/mkspecs/features/exclusive_builds.prf:
/usr/local/Qt4.7/mkspecs/features/default_pre.prf:
/usr/local/Qt4.7/mkspecs/features/mac/default_pre.prf:
/usr/local/Qt4.7/mkspecs/features/release.prf:
/usr/local/Qt4.7/mkspecs/features/default_post.prf:
/usr/local/Qt4.7/mkspecs/features/mac/default_post.prf:
/usr/local/Qt4.7/mkspecs/features/mac/objective_c.prf:
/usr/local/Qt4.7/mkspecs/features/staticlib.prf:
/usr/local/Qt4.7/mkspecs/features/static.prf:
qmake:  FORCE
	@$(QMAKE) -spec /usr/local/Qt4.7/mkspecs/macx-g++ -o Makefile mac_iscore.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/Iscore1.0.0 || $(MKDIR) .tmp/Iscore1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/Iscore1.0.0/ && (cd `dirname .tmp/Iscore1.0.0` && $(TAR) Iscore1.0.0.tar Iscore1.0.0 && $(COMPRESS) Iscore1.0.0.tar) && $(MOVE) `dirname .tmp/Iscore1.0.0`/Iscore1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/Iscore1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

compiler_objective_c_make_all:
compiler_objective_c_clean:
compiler_clean: 

####### Compile

Engines.o: Engines.cpp Includes/Engines.hpp \
		Includes/CSPTypes.hpp \
		EnginesPrivate/EnginesPrivate.hpp \
		ECOMachine/ECOMachine.hpp \
		ECOMachine/PetriNet/PetriNet.hpp \
		ECOMachine/PetriNet/Place.hpp \
		ECOMachine/PetriNet/Token.hpp \
		ECOMachine/PetriNet/PetriNetNode.hpp \
		ECOMachine/PetriNet/PetriNetItem.hpp \
		ECOMachine/PetriNet/Arc.hpp \
		ECOMachine/PetriNet/ExtendedInt.hpp \
		ECOMachine/PetriNet/ArithmeticException.hpp \
		ECOMachine/PetriNet/IllegalArgumentException.hpp \
		ECOMachine/PetriNet/OutOfBoundException.hpp \
		ECOMachine/PetriNet/Transition.hpp \
		ECOMachine/PetriNet/TransitionBitArray.hpp \
		ECOMachine/PetriNet/PriorityTransitionAction.hpp \
		ECOMachine/PetriNet/TransitionAction.hpp \
		ECOMachine/PetriNet/IncoherentStateException.hpp \
		ECOMachine/PetriNet/ThreadSafeList.hpp \
		ECOMachine/Process/ProcessInclude.hpp \
		ECOMachine/Process/Process.hpp \
		Editor/CSP/XMLInclude.hpp \
		Editor/CSP/XMLConversion.hpp \
		Editor/CSP/InvalidFileFormatException.hpp \
		ECOMachine/Process/ProcessStep.hpp \
		ECOMachine/Process/SendNetworkMessageProcess.hpp \
		ECOMachine/Process/NetworkMessageCurves.hpp \
		ECOMachine/Process/MultiTypeCurve.hpp \
		ECOMachine/Process/StringParser.hpp \
		ECOMachine/Process/StringTypeFunctions.hpp \
		ECOMachine/ControlPointInformations.hpp \
		ECOMachine/TriggerPointInformations.hpp \
		Editor/Editor.hpp \
		Editor/CSP/CSP.hpp \
		Editor/CSP/ConstrainedBox.hpp \
		Editor/CSP/ConstrainedTemporalEntity.hpp \
		Editor/gecode-solver/solver.hpp \
		Editor/gecode-solver/gecode_headers.hpp \
		Editor/gecode-solver/relations_type.hpp \
		Editor/CSP/ControlPoint.hpp \
		Editor/CSP/AntPostRelation.hpp \
		Editor/CSP/BinaryTemporalRelation.hpp \
		Editor/CSP/BinaryRelation.hpp \
		Editor/CSP/TriggerPoint.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Engines.o Engines.cpp

ChangeTempo.o: ECOMachine/ChangeTempo.cpp ECOMachine/ChangeTempo.hpp \
		ECOMachine/ECOMachine.hpp \
		ECOMachine/PetriNet/PetriNet.hpp \
		ECOMachine/PetriNet/Place.hpp \
		ECOMachine/PetriNet/Token.hpp \
		ECOMachine/PetriNet/PetriNetNode.hpp \
		ECOMachine/PetriNet/PetriNetItem.hpp \
		ECOMachine/PetriNet/Arc.hpp \
		ECOMachine/PetriNet/ExtendedInt.hpp \
		ECOMachine/PetriNet/ArithmeticException.hpp \
		ECOMachine/PetriNet/IllegalArgumentException.hpp \
		ECOMachine/PetriNet/OutOfBoundException.hpp \
		ECOMachine/PetriNet/Transition.hpp \
		ECOMachine/PetriNet/TransitionBitArray.hpp \
		ECOMachine/PetriNet/PriorityTransitionAction.hpp \
		ECOMachine/PetriNet/TransitionAction.hpp \
		ECOMachine/PetriNet/IncoherentStateException.hpp \
		ECOMachine/PetriNet/ThreadSafeList.hpp \
		ECOMachine/Process/ProcessInclude.hpp \
		ECOMachine/Process/Process.hpp \
		Editor/CSP/XMLInclude.hpp \
		Editor/CSP/XMLConversion.hpp \
		Editor/CSP/InvalidFileFormatException.hpp \
		ECOMachine/Process/ProcessStep.hpp \
		ECOMachine/Process/SendNetworkMessageProcess.hpp \
		ECOMachine/Process/NetworkMessageCurves.hpp \
		ECOMachine/Process/MultiTypeCurve.hpp \
		ECOMachine/Process/StringParser.hpp \
		ECOMachine/Process/StringTypeFunctions.hpp \
		ECOMachine/ControlPointInformations.hpp \
		ECOMachine/TriggerPointInformations.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ChangeTempo.o ECOMachine/ChangeTempo.cpp

ControlPointInformations.o: ECOMachine/ControlPointInformations.cpp ECOMachine/ControlPointInformations.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ControlPointInformations.o ECOMachine/ControlPointInformations.cpp

ECOMachine.o: ECOMachine/ECOMachine.cpp ECOMachine/ECOMachine.hpp \
		ECOMachine/PetriNet/PetriNet.hpp \
		ECOMachine/PetriNet/Place.hpp \
		ECOMachine/PetriNet/Token.hpp \
		ECOMachine/PetriNet/PetriNetNode.hpp \
		ECOMachine/PetriNet/PetriNetItem.hpp \
		ECOMachine/PetriNet/Arc.hpp \
		ECOMachine/PetriNet/ExtendedInt.hpp \
		ECOMachine/PetriNet/ArithmeticException.hpp \
		ECOMachine/PetriNet/IllegalArgumentException.hpp \
		ECOMachine/PetriNet/OutOfBoundException.hpp \
		ECOMachine/PetriNet/Transition.hpp \
		ECOMachine/PetriNet/TransitionBitArray.hpp \
		ECOMachine/PetriNet/PriorityTransitionAction.hpp \
		ECOMachine/PetriNet/TransitionAction.hpp \
		ECOMachine/PetriNet/IncoherentStateException.hpp \
		ECOMachine/PetriNet/ThreadSafeList.hpp \
		ECOMachine/Process/ProcessInclude.hpp \
		ECOMachine/Process/Process.hpp \
		Editor/CSP/XMLInclude.hpp \
		Editor/CSP/XMLConversion.hpp \
		Editor/CSP/InvalidFileFormatException.hpp \
		ECOMachine/Process/ProcessStep.hpp \
		ECOMachine/Process/SendNetworkMessageProcess.hpp \
		ECOMachine/Process/NetworkMessageCurves.hpp \
		ECOMachine/Process/MultiTypeCurve.hpp \
		ECOMachine/Process/StringParser.hpp \
		ECOMachine/Process/StringTypeFunctions.hpp \
		ECOMachine/ControlPointInformations.hpp \
		ECOMachine/TriggerPointInformations.hpp \
		ECOMachine/ChangeTempo.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ECOMachine.o ECOMachine/ECOMachine.cpp

TriggerPointInformations.o: ECOMachine/TriggerPointInformations.cpp ECOMachine/TriggerPointInformations.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o TriggerPointInformations.o ECOMachine/TriggerPointInformations.cpp

Editor.o: Editor/Editor.cpp Editor/Editor.hpp \
		Editor/CSP/CSP.hpp \
		Editor/CSP/XMLInclude.hpp \
		Editor/CSP/XMLConversion.hpp \
		Editor/CSP/InvalidFileFormatException.hpp \
		Includes/CSPTypes.hpp \
		Editor/CSP/ConstrainedBox.hpp \
		Editor/CSP/ConstrainedTemporalEntity.hpp \
		Editor/gecode-solver/solver.hpp \
		Editor/gecode-solver/gecode_headers.hpp \
		Editor/gecode-solver/relations_type.hpp \
		Editor/CSP/ControlPoint.hpp \
		Editor/CSP/AntPostRelation.hpp \
		Editor/CSP/BinaryTemporalRelation.hpp \
		Editor/CSP/BinaryRelation.hpp \
		Editor/CSP/TriggerPoint.hpp \
		ECOMachine/PetriNet/OutOfBoundException.hpp \
		ECOMachine/PetriNet/IllegalArgumentException.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Editor.o Editor/Editor.cpp

EnginesPrivate.o: EnginesPrivate/EnginesPrivate.cpp EnginesPrivate/EnginesPrivate.hpp \
		ECOMachine/ECOMachine.hpp \
		ECOMachine/PetriNet/PetriNet.hpp \
		ECOMachine/PetriNet/Place.hpp \
		ECOMachine/PetriNet/Token.hpp \
		ECOMachine/PetriNet/PetriNetNode.hpp \
		ECOMachine/PetriNet/PetriNetItem.hpp \
		ECOMachine/PetriNet/Arc.hpp \
		ECOMachine/PetriNet/ExtendedInt.hpp \
		ECOMachine/PetriNet/ArithmeticException.hpp \
		ECOMachine/PetriNet/IllegalArgumentException.hpp \
		ECOMachine/PetriNet/OutOfBoundException.hpp \
		ECOMachine/PetriNet/Transition.hpp \
		ECOMachine/PetriNet/TransitionBitArray.hpp \
		ECOMachine/PetriNet/PriorityTransitionAction.hpp \
		ECOMachine/PetriNet/TransitionAction.hpp \
		ECOMachine/PetriNet/IncoherentStateException.hpp \
		ECOMachine/PetriNet/ThreadSafeList.hpp \
		ECOMachine/Process/ProcessInclude.hpp \
		ECOMachine/Process/Process.hpp \
		Editor/CSP/XMLInclude.hpp \
		Editor/CSP/XMLConversion.hpp \
		Editor/CSP/InvalidFileFormatException.hpp \
		ECOMachine/Process/ProcessStep.hpp \
		ECOMachine/Process/SendNetworkMessageProcess.hpp \
		ECOMachine/Process/NetworkMessageCurves.hpp \
		ECOMachine/Process/MultiTypeCurve.hpp \
		ECOMachine/Process/StringParser.hpp \
		ECOMachine/Process/StringTypeFunctions.hpp \
		ECOMachine/ControlPointInformations.hpp \
		ECOMachine/TriggerPointInformations.hpp \
		Editor/Editor.hpp \
		Editor/CSP/CSP.hpp \
		Includes/CSPTypes.hpp \
		Editor/CSP/ConstrainedBox.hpp \
		Editor/CSP/ConstrainedTemporalEntity.hpp \
		Editor/gecode-solver/solver.hpp \
		Editor/gecode-solver/gecode_headers.hpp \
		Editor/gecode-solver/relations_type.hpp \
		Editor/CSP/ControlPoint.hpp \
		Editor/CSP/AntPostRelation.hpp \
		Editor/CSP/BinaryTemporalRelation.hpp \
		Editor/CSP/BinaryRelation.hpp \
		Editor/CSP/TriggerPoint.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o EnginesPrivate.o EnginesPrivate/EnginesPrivate.cpp

StoryLine.o: StoryLine/StoryLine.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o StoryLine.o StoryLine/StoryLine.cpp

Arc.o: ECOMachine/PetriNet/Arc.cpp ECOMachine/PetriNet/Arc.hpp \
		ECOMachine/PetriNet/PetriNetItem.hpp \
		ECOMachine/PetriNet/ExtendedInt.hpp \
		ECOMachine/PetriNet/ArithmeticException.hpp \
		ECOMachine/PetriNet/IllegalArgumentException.hpp \
		ECOMachine/PetriNet/Transition.hpp \
		ECOMachine/PetriNet/PetriNetNode.hpp \
		ECOMachine/PetriNet/OutOfBoundException.hpp \
		ECOMachine/PetriNet/TransitionBitArray.hpp \
		ECOMachine/PetriNet/PriorityTransitionAction.hpp \
		ECOMachine/PetriNet/TransitionAction.hpp \
		ECOMachine/PetriNet/IncoherentStateException.hpp \
		ECOMachine/PetriNet/Place.hpp \
		ECOMachine/PetriNet/Token.hpp \
		ECOMachine/PetriNet/PetriNet.hpp \
		ECOMachine/PetriNet/ThreadSafeList.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Arc.o ECOMachine/PetriNet/Arc.cpp

ExtendedInt.o: ECOMachine/PetriNet/ExtendedInt.cpp ECOMachine/PetriNet/ExtendedInt.hpp \
		ECOMachine/PetriNet/ArithmeticException.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ExtendedInt.o ECOMachine/PetriNet/ExtendedInt.cpp

PetriNet.o: ECOMachine/PetriNet/PetriNet.cpp ECOMachine/PetriNet/PetriNet.hpp \
		ECOMachine/PetriNet/Place.hpp \
		ECOMachine/PetriNet/Token.hpp \
		ECOMachine/PetriNet/PetriNetNode.hpp \
		ECOMachine/PetriNet/PetriNetItem.hpp \
		ECOMachine/PetriNet/Arc.hpp \
		ECOMachine/PetriNet/ExtendedInt.hpp \
		ECOMachine/PetriNet/ArithmeticException.hpp \
		ECOMachine/PetriNet/IllegalArgumentException.hpp \
		ECOMachine/PetriNet/OutOfBoundException.hpp \
		ECOMachine/PetriNet/Transition.hpp \
		ECOMachine/PetriNet/TransitionBitArray.hpp \
		ECOMachine/PetriNet/PriorityTransitionAction.hpp \
		ECOMachine/PetriNet/TransitionAction.hpp \
		ECOMachine/PetriNet/IncoherentStateException.hpp \
		ECOMachine/PetriNet/ThreadSafeList.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o PetriNet.o ECOMachine/PetriNet/PetriNet.cpp

PetriNetItem.o: ECOMachine/PetriNet/PetriNetItem.cpp ECOMachine/PetriNet/PetriNetItem.hpp \
		ECOMachine/PetriNet/PetriNet.hpp \
		ECOMachine/PetriNet/Place.hpp \
		ECOMachine/PetriNet/Token.hpp \
		ECOMachine/PetriNet/PetriNetNode.hpp \
		ECOMachine/PetriNet/Arc.hpp \
		ECOMachine/PetriNet/ExtendedInt.hpp \
		ECOMachine/PetriNet/ArithmeticException.hpp \
		ECOMachine/PetriNet/IllegalArgumentException.hpp \
		ECOMachine/PetriNet/OutOfBoundException.hpp \
		ECOMachine/PetriNet/Transition.hpp \
		ECOMachine/PetriNet/TransitionBitArray.hpp \
		ECOMachine/PetriNet/PriorityTransitionAction.hpp \
		ECOMachine/PetriNet/TransitionAction.hpp \
		ECOMachine/PetriNet/IncoherentStateException.hpp \
		ECOMachine/PetriNet/ThreadSafeList.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o PetriNetItem.o ECOMachine/PetriNet/PetriNetItem.cpp

PetriNetNode.o: ECOMachine/PetriNet/PetriNetNode.cpp ECOMachine/PetriNet/PetriNetNode.hpp \
		ECOMachine/PetriNet/PetriNetItem.hpp \
		ECOMachine/PetriNet/Arc.hpp \
		ECOMachine/PetriNet/ExtendedInt.hpp \
		ECOMachine/PetriNet/ArithmeticException.hpp \
		ECOMachine/PetriNet/IllegalArgumentException.hpp \
		ECOMachine/PetriNet/OutOfBoundException.hpp \
		ECOMachine/PetriNet/PetriNet.hpp \
		ECOMachine/PetriNet/Place.hpp \
		ECOMachine/PetriNet/Token.hpp \
		ECOMachine/PetriNet/Transition.hpp \
		ECOMachine/PetriNet/TransitionBitArray.hpp \
		ECOMachine/PetriNet/PriorityTransitionAction.hpp \
		ECOMachine/PetriNet/TransitionAction.hpp \
		ECOMachine/PetriNet/IncoherentStateException.hpp \
		ECOMachine/PetriNet/ThreadSafeList.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o PetriNetNode.o ECOMachine/PetriNet/PetriNetNode.cpp

Place.o: ECOMachine/PetriNet/Place.cpp ECOMachine/PetriNet/Place.hpp \
		ECOMachine/PetriNet/Token.hpp \
		ECOMachine/PetriNet/PetriNetNode.hpp \
		ECOMachine/PetriNet/PetriNetItem.hpp \
		ECOMachine/PetriNet/Arc.hpp \
		ECOMachine/PetriNet/ExtendedInt.hpp \
		ECOMachine/PetriNet/ArithmeticException.hpp \
		ECOMachine/PetriNet/IllegalArgumentException.hpp \
		ECOMachine/PetriNet/OutOfBoundException.hpp \
		ECOMachine/PetriNet/PetriNet.hpp \
		ECOMachine/PetriNet/Transition.hpp \
		ECOMachine/PetriNet/TransitionBitArray.hpp \
		ECOMachine/PetriNet/PriorityTransitionAction.hpp \
		ECOMachine/PetriNet/TransitionAction.hpp \
		ECOMachine/PetriNet/IncoherentStateException.hpp \
		ECOMachine/PetriNet/ThreadSafeList.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Place.o ECOMachine/PetriNet/Place.cpp

PriorityTransitionAction.o: ECOMachine/PetriNet/PriorityTransitionAction.cpp ECOMachine/PetriNet/PriorityTransitionAction.hpp \
		ECOMachine/PetriNet/IllegalArgumentException.hpp \
		ECOMachine/PetriNet/ExtendedInt.hpp \
		ECOMachine/PetriNet/ArithmeticException.hpp \
		ECOMachine/PetriNet/Transition.hpp \
		ECOMachine/PetriNet/Arc.hpp \
		ECOMachine/PetriNet/PetriNetItem.hpp \
		ECOMachine/PetriNet/PetriNetNode.hpp \
		ECOMachine/PetriNet/OutOfBoundException.hpp \
		ECOMachine/PetriNet/TransitionBitArray.hpp \
		ECOMachine/PetriNet/TransitionAction.hpp \
		ECOMachine/PetriNet/IncoherentStateException.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o PriorityTransitionAction.o ECOMachine/PetriNet/PriorityTransitionAction.cpp

ThreadSafeList.o: ECOMachine/PetriNet/ThreadSafeList.cpp ECOMachine/PetriNet/ThreadSafeList.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ThreadSafeList.o ECOMachine/PetriNet/ThreadSafeList.cpp

Token.o: ECOMachine/PetriNet/Token.cpp ECOMachine/PetriNet/Token.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Token.o ECOMachine/PetriNet/Token.cpp

Transition.o: ECOMachine/PetriNet/Transition.cpp ECOMachine/PetriNet/Transition.hpp \
		ECOMachine/PetriNet/Arc.hpp \
		ECOMachine/PetriNet/PetriNetItem.hpp \
		ECOMachine/PetriNet/ExtendedInt.hpp \
		ECOMachine/PetriNet/ArithmeticException.hpp \
		ECOMachine/PetriNet/IllegalArgumentException.hpp \
		ECOMachine/PetriNet/PetriNetNode.hpp \
		ECOMachine/PetriNet/OutOfBoundException.hpp \
		ECOMachine/PetriNet/TransitionBitArray.hpp \
		ECOMachine/PetriNet/PriorityTransitionAction.hpp \
		ECOMachine/PetriNet/TransitionAction.hpp \
		ECOMachine/PetriNet/IncoherentStateException.hpp \
		ECOMachine/PetriNet/PetriNet.hpp \
		ECOMachine/PetriNet/Place.hpp \
		ECOMachine/PetriNet/Token.hpp \
		ECOMachine/PetriNet/ThreadSafeList.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Transition.o ECOMachine/PetriNet/Transition.cpp

TransitionAction.o: ECOMachine/PetriNet/TransitionAction.cpp ECOMachine/PetriNet/TransitionAction.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o TransitionAction.o ECOMachine/PetriNet/TransitionAction.cpp

TransitionBitArray.o: ECOMachine/PetriNet/TransitionBitArray.cpp ECOMachine/PetriNet/TransitionBitArray.hpp \
		ECOMachine/PetriNet/OutOfBoundException.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o TransitionBitArray.o ECOMachine/PetriNet/TransitionBitArray.cpp

MultiTypeCurve.o: ECOMachine/Process/MultiTypeCurve.cpp ECOMachine/Process/MultiTypeCurve.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MultiTypeCurve.o ECOMachine/Process/MultiTypeCurve.cpp

NetworkMessageCurves.o: ECOMachine/Process/NetworkMessageCurves.cpp ECOMachine/Process/NetworkMessageCurves.hpp \
		ECOMachine/Process/MultiTypeCurve.hpp \
		ECOMachine/Process/StringParser.hpp \
		Editor/CSP/XMLInclude.hpp \
		Editor/CSP/XMLConversion.hpp \
		Editor/CSP/InvalidFileFormatException.hpp \
		ECOMachine/Process/StringTypeFunctions.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o NetworkMessageCurves.o ECOMachine/Process/NetworkMessageCurves.cpp

Process.o: ECOMachine/Process/Process.cpp ECOMachine/Process/Process.hpp \
		Editor/CSP/XMLInclude.hpp \
		Editor/CSP/XMLConversion.hpp \
		Editor/CSP/InvalidFileFormatException.hpp \
		ECOMachine/Process/ProcessStep.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Process.o ECOMachine/Process/Process.cpp

SendNetworkMessageProcess.o: ECOMachine/Process/SendNetworkMessageProcess.cpp ECOMachine/Process/SendNetworkMessageProcess.hpp \
		ECOMachine/Process/Process.hpp \
		Editor/CSP/XMLInclude.hpp \
		Editor/CSP/XMLConversion.hpp \
		Editor/CSP/InvalidFileFormatException.hpp \
		ECOMachine/Process/NetworkMessageCurves.hpp \
		ECOMachine/Process/MultiTypeCurve.hpp \
		ECOMachine/Process/StringParser.hpp \
		ECOMachine/Process/StringTypeFunctions.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o SendNetworkMessageProcess.o ECOMachine/Process/SendNetworkMessageProcess.cpp

StringParser.o: ECOMachine/Process/StringParser.cpp ECOMachine/Process/StringParser.hpp \
		ECOMachine/Process/StringTypeFunctions.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o StringParser.o ECOMachine/Process/StringParser.cpp

StringTypeFunctions.o: ECOMachine/Process/StringTypeFunctions.cpp ECOMachine/Process/StringTypeFunctions.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o StringTypeFunctions.o ECOMachine/Process/StringTypeFunctions.cpp

AllenRelation.o: Editor/CSP/AllenRelation.cpp Editor/CSP/AllenRelation.hpp \
		Includes/CSPTypes.hpp \
		Editor/CSP/BinaryTemporalRelation.hpp \
		Editor/CSP/BinaryRelation.hpp \
		Editor/CSP/ConstrainedTemporalEntity.hpp \
		Editor/gecode-solver/solver.hpp \
		Editor/gecode-solver/gecode_headers.hpp \
		Editor/gecode-solver/relations_type.hpp \
		Editor/CSP/CSP.hpp \
		Editor/CSP/XMLInclude.hpp \
		Editor/CSP/XMLConversion.hpp \
		Editor/CSP/InvalidFileFormatException.hpp \
		Editor/CSP/ConstrainedBox.hpp \
		Editor/CSP/ControlPoint.hpp \
		Editor/CSP/AntPostRelation.hpp \
		Editor/CSP/TriggerPoint.hpp \
		ECOMachine/PetriNet/OutOfBoundException.hpp \
		ECOMachine/PetriNet/IllegalArgumentException.hpp \
		Editor/CSP/CSPLinearConstraint.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o AllenRelation.o Editor/CSP/AllenRelation.cpp

AntPostRelation.o: Editor/CSP/AntPostRelation.cpp Editor/CSP/AntPostRelation.hpp \
		Includes/CSPTypes.hpp \
		Editor/CSP/BinaryTemporalRelation.hpp \
		Editor/CSP/BinaryRelation.hpp \
		Editor/CSP/ConstrainedTemporalEntity.hpp \
		Editor/gecode-solver/solver.hpp \
		Editor/gecode-solver/gecode_headers.hpp \
		Editor/gecode-solver/relations_type.hpp \
		Editor/CSP/CSP.hpp \
		Editor/CSP/XMLInclude.hpp \
		Editor/CSP/XMLConversion.hpp \
		Editor/CSP/InvalidFileFormatException.hpp \
		Editor/CSP/ConstrainedBox.hpp \
		Editor/CSP/ControlPoint.hpp \
		Editor/CSP/TriggerPoint.hpp \
		ECOMachine/PetriNet/OutOfBoundException.hpp \
		ECOMachine/PetriNet/IllegalArgumentException.hpp \
		Editor/CSP/CSPLinearConstraint.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o AntPostRelation.o Editor/CSP/AntPostRelation.cpp

BinaryRelation.o: Editor/CSP/BinaryRelation.cpp Editor/CSP/BinaryRelation.hpp \
		Includes/CSPTypes.hpp \
		Editor/CSP/ConstrainedTemporalEntity.hpp \
		Editor/gecode-solver/solver.hpp \
		Editor/gecode-solver/gecode_headers.hpp \
		Editor/gecode-solver/relations_type.hpp \
		Editor/CSP/CSPLinearConstraint.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o BinaryRelation.o Editor/CSP/BinaryRelation.cpp

BinaryTemporalRelation.o: Editor/CSP/BinaryTemporalRelation.cpp Editor/CSP/BinaryTemporalRelation.hpp \
		Includes/CSPTypes.hpp \
		Editor/CSP/BinaryRelation.hpp \
		Editor/CSP/ConstrainedTemporalEntity.hpp \
		Editor/gecode-solver/solver.hpp \
		Editor/gecode-solver/gecode_headers.hpp \
		Editor/gecode-solver/relations_type.hpp \
		Editor/CSP/CSPLinearConstraint.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o BinaryTemporalRelation.o Editor/CSP/BinaryTemporalRelation.cpp

ConstrainedBox.o: Editor/CSP/ConstrainedBox.cpp Editor/CSP/ConstrainedBox.hpp \
		Editor/CSP/ConstrainedTemporalEntity.hpp \
		Editor/gecode-solver/solver.hpp \
		Editor/gecode-solver/gecode_headers.hpp \
		Editor/gecode-solver/relations_type.hpp \
		Editor/CSP/ControlPoint.hpp \
		Editor/CSP/CSP.hpp \
		Editor/CSP/XMLInclude.hpp \
		Editor/CSP/XMLConversion.hpp \
		Editor/CSP/InvalidFileFormatException.hpp \
		Includes/CSPTypes.hpp \
		Editor/CSP/AntPostRelation.hpp \
		Editor/CSP/BinaryTemporalRelation.hpp \
		Editor/CSP/BinaryRelation.hpp \
		Editor/CSP/TriggerPoint.hpp \
		ECOMachine/PetriNet/OutOfBoundException.hpp \
		ECOMachine/PetriNet/IllegalArgumentException.hpp \
		Editor/CSP/CSPConstrainedVariable.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ConstrainedBox.o Editor/CSP/ConstrainedBox.cpp

ConstrainedTemporalEntity.o: Editor/CSP/ConstrainedTemporalEntity.cpp Editor/CSP/ConstrainedTemporalEntity.hpp \
		Editor/gecode-solver/solver.hpp \
		Editor/gecode-solver/gecode_headers.hpp \
		Editor/gecode-solver/relations_type.hpp \
		Editor/CSP/CSP.hpp \
		Editor/CSP/XMLInclude.hpp \
		Editor/CSP/XMLConversion.hpp \
		Editor/CSP/InvalidFileFormatException.hpp \
		Includes/CSPTypes.hpp \
		Editor/CSP/ConstrainedBox.hpp \
		Editor/CSP/ControlPoint.hpp \
		Editor/CSP/AntPostRelation.hpp \
		Editor/CSP/BinaryTemporalRelation.hpp \
		Editor/CSP/BinaryRelation.hpp \
		Editor/CSP/TriggerPoint.hpp \
		ECOMachine/PetriNet/OutOfBoundException.hpp \
		ECOMachine/PetriNet/IllegalArgumentException.hpp \
		Editor/CSP/CSPConstrainedVariable.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ConstrainedTemporalEntity.o Editor/CSP/ConstrainedTemporalEntity.cpp

ControlPoint.o: Editor/CSP/ControlPoint.cpp Editor/CSP/ControlPoint.hpp \
		Editor/CSP/ConstrainedTemporalEntity.hpp \
		Editor/gecode-solver/solver.hpp \
		Editor/gecode-solver/gecode_headers.hpp \
		Editor/gecode-solver/relations_type.hpp \
		Editor/CSP/TriggerPoint.hpp \
		Includes/CSPTypes.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ControlPoint.o Editor/CSP/ControlPoint.cpp

CSP.o: Editor/CSP/CSP.cpp Editor/gecode-solver/solver.hpp \
		Editor/gecode-solver/gecode_headers.hpp \
		Editor/gecode-solver/relations_type.hpp \
		Editor/CSP/CSP.hpp \
		Editor/CSP/XMLInclude.hpp \
		Editor/CSP/XMLConversion.hpp \
		Editor/CSP/InvalidFileFormatException.hpp \
		Includes/CSPTypes.hpp \
		Editor/CSP/ConstrainedBox.hpp \
		Editor/CSP/ConstrainedTemporalEntity.hpp \
		Editor/CSP/ControlPoint.hpp \
		Editor/CSP/AntPostRelation.hpp \
		Editor/CSP/BinaryTemporalRelation.hpp \
		Editor/CSP/BinaryRelation.hpp \
		Editor/CSP/TriggerPoint.hpp \
		ECOMachine/PetriNet/OutOfBoundException.hpp \
		ECOMachine/PetriNet/IllegalArgumentException.hpp \
		Editor/CSP/AllenRelation.hpp \
		Editor/CSP/CSPConstrainedVariable.hpp \
		Editor/CSP/CSPLinearConstraint.hpp \
		Editor/CSP/IntervalRelation.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o CSP.o Editor/CSP/CSP.cpp

CSPConstrainedVariable.o: Editor/CSP/CSPConstrainedVariable.cpp Editor/CSP/CSPConstrainedVariable.hpp \
		Includes/CSPTypes.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o CSPConstrainedVariable.o Editor/CSP/CSPConstrainedVariable.cpp

CSPLinearConstraint.o: Editor/CSP/CSPLinearConstraint.cpp Editor/CSP/CSPLinearConstraint.hpp \
		Includes/CSPTypes.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o CSPLinearConstraint.o Editor/CSP/CSPLinearConstraint.cpp

IntervalRelation.o: Editor/CSP/IntervalRelation.cpp Editor/CSP/IntervalRelation.hpp \
		Includes/CSPTypes.hpp \
		Editor/CSP/BinaryTemporalRelation.hpp \
		Editor/CSP/BinaryRelation.hpp \
		Editor/CSP/ConstrainedTemporalEntity.hpp \
		Editor/gecode-solver/solver.hpp \
		Editor/gecode-solver/gecode_headers.hpp \
		Editor/gecode-solver/relations_type.hpp \
		Editor/CSP/CSPConstrainedVariable.hpp \
		Editor/CSP/CSPLinearConstraint.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o IntervalRelation.o Editor/CSP/IntervalRelation.cpp

TriggerPoint.o: Editor/CSP/TriggerPoint.cpp Editor/CSP/TriggerPoint.hpp \
		Includes/CSPTypes.hpp \
		Editor/CSP/ControlPoint.hpp \
		Editor/CSP/ConstrainedTemporalEntity.hpp \
		Editor/gecode-solver/solver.hpp \
		Editor/gecode-solver/gecode_headers.hpp \
		Editor/gecode-solver/relations_type.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o TriggerPoint.o Editor/CSP/TriggerPoint.cpp

XMLConversion.o: Editor/CSP/XMLConversion.cpp Editor/CSP/XMLConversion.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o XMLConversion.o Editor/CSP/XMLConversion.cpp

customSpace.o: Editor/gecode-solver/customSpace.cpp Editor/gecode-solver/customSpace.hpp \
		Editor/gecode-solver/gecode_headers.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o customSpace.o Editor/gecode-solver/customSpace.cpp

integerVariable.o: Editor/gecode-solver/integerVariable.cpp Editor/gecode-solver/integerVariable.hpp \
		Editor/gecode-solver/gecode_headers.hpp \
		Editor/gecode-solver/relations_type.hpp \
		Editor/gecode-solver/customSpace.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o integerVariable.o Editor/gecode-solver/integerVariable.cpp

linearConstraint.o: Editor/gecode-solver/linearConstraint.cpp Editor/gecode-solver/linearConstraint.hpp \
		Editor/gecode-solver/gecode_headers.hpp \
		Editor/gecode-solver/customSpace.hpp \
		Editor/gecode-solver/integerVariable.hpp \
		Editor/gecode-solver/relations_type.hpp \
		Editor/gecode-solver/solver.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o linearConstraint.o Editor/gecode-solver/linearConstraint.cpp

searchEngine.o: Editor/gecode-solver/searchEngine.cpp Editor/gecode-solver/searchEngine.hpp \
		Editor/gecode-solver/gecode_headers.hpp \
		Editor/gecode-solver/customSpace.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o searchEngine.o Editor/gecode-solver/searchEngine.cpp

solver.o: Editor/gecode-solver/solver.cpp Editor/gecode-solver/solver.hpp \
		Editor/gecode-solver/gecode_headers.hpp \
		Editor/gecode-solver/relations_type.hpp \
		Editor/gecode-solver/customSpace.hpp \
		Editor/gecode-solver/integerVariable.hpp \
		Editor/gecode-solver/linearConstraint.hpp \
		Editor/gecode-solver/searchEngine.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o solver.o Editor/gecode-solver/solver.cpp

solver_wrap.o: Editor/gecode-solver/solver_wrap.cpp Editor/gecode-solver/solver_wrap.hpp \
		Editor/gecode-solver/relations_type.hpp \
		Editor/gecode-solver/integerVariable.hpp \
		Editor/gecode-solver/gecode_headers.hpp \
		Editor/gecode-solver/solver.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o solver_wrap.o Editor/gecode-solver/solver_wrap.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

