TEMPLATE = lib
TARGET = Iscore
#CONFIG = staticlib

linux-g++ {
INCLUDEPATH += . ./Includes /usr/local/include/IScore /usr/local/include /usr/local/include/libxml2 /usr/include/libxml2 /usr/local/include/Iscore
QMAKE_LFLAGS += -L/usr/local/lib/ -L/usr/lib/ -L/usr/lib/libxml2 -L/usr/local/lib/IScore
LIBS += -lDeviceManager -lxml2 -lgecodeint -lgecodesearch -lgecodedriver -lgecodeflatzinc -lgecodekernel -lgecodeminimodel -lgecodescheduling -lgecodeset -lgecodesupport -lgecodegraph
}

linux-g++-64 {
INCLUDEPATH += . ./Includes /usr/local/include/IScore /usr/local/include /usr/local/include/libxml2 /usr/include/libxml2 /usr/local/include/Iscore
QMAKE_LFLAGS += -L/usr/local/lib/ -L/usr/lib/ -L/usr/lib/libxml2 -L/usr/local/lib/IScore
LIBS += -lDeviceManager -lxml2 -lgecodeint -lgecodesearch -lgecodedriver -lgecodeflatzinc -lgecodekernel -lgecodeminimodel -lgecodescheduling -lgecodeset -lgecodesupport -lgecodegraph
}

macx-g++ {
INCLUDEPATH += . ./Includes /Library/Frameworks/ /usr/local/include/IScore /usr/local/include /usr/local/include/libxml2
QMAKE_LFLAGS += -L/usr/local/lib/ -L/usr/lib/ -L/System/Library/Frameworks/ -L/Library/Frameworks/
QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
#LIBS += -liscore -lDeviceManager -lxml2 -framework gecode
LIBS += -lDeviceManager -lxml2 -framework gecode
}

DEPENDPATH += . \
              ECOMachine \
              Editor \
              EnginesPrivate \
              Includes \
              libController \
              StoryLine \
              ECOMachine/PetriNet \
              ECOMachine/Process \
              Editor/CSP \
              Editor/gecode-solver

INCLUDEPATH += . \
               Includes \
               EnginesPrivate \
               ECOMachine \
               ECOMachine/PetriNet \
               ECOMachine/Process \
               Editor/CSP \
               StoryLine \
               Editor/gecode-solver \
               Editor

# Input
HEADERS += ECOMachine/ChangeTempo.hpp \
           ECOMachine/ControlPointInformations.hpp \
           ECOMachine/ECOMachine.hpp \
           ECOMachine/TriggerPointInformations.hpp \
           Editor/Editor.hpp \
           EnginesPrivate/EnginesPrivate.hpp \
           Includes/CSPTypes.hpp \
           Includes/Engines.hpp \
           libController/Controller.h \
           libController/Device.h \
           libController/Plugin.h \
           libController/PluginFactories.h \
           StoryLine/StoryLine.hpp \
           ECOMachine/PetriNet/Arc.hpp \
           ECOMachine/PetriNet/ArithmeticException.hpp \
           ECOMachine/PetriNet/ExtendedInt.hpp \
           ECOMachine/PetriNet/IllegalArgumentException.hpp \
           ECOMachine/PetriNet/IncoherentStateException.hpp \
           ECOMachine/PetriNet/OutOfBoundException.hpp \
           ECOMachine/PetriNet/PetriNet.hpp \
           ECOMachine/PetriNet/PetriNetItem.hpp \
           ECOMachine/PetriNet/PetriNetNode.hpp \
           ECOMachine/PetriNet/Place.hpp \
           ECOMachine/PetriNet/PriorityTransitionAction.hpp \
           ECOMachine/PetriNet/ThreadSafeList.hpp \
           ECOMachine/PetriNet/Token.hpp \
           ECOMachine/PetriNet/Transition.hpp \
           ECOMachine/PetriNet/TransitionAction.hpp \
           ECOMachine/PetriNet/TransitionBitArray.hpp \
           ECOMachine/Process/MultiTypeCurve.hpp \
           ECOMachine/Process/NetworkMessageCurves.hpp \
           ECOMachine/Process/Process.hpp \
           ECOMachine/Process/ProcessInclude.hpp \
           ECOMachine/Process/ProcessStep.hpp \
           ECOMachine/Process/SendNetworkMessageProcess.hpp \
           ECOMachine/Process/StringParser.hpp \
           ECOMachine/Process/StringTypeFunctions.hpp \
           Editor/CSP/AllenRelation.hpp \
           Editor/CSP/AntPostRelation.hpp \
           Editor/CSP/BinaryRelation.hpp \
           Editor/CSP/BinaryTemporalRelation.hpp \
           Editor/CSP/ConstrainedBox.hpp \
           Editor/CSP/ConstrainedTemporalEntity.hpp \
           Editor/CSP/ControlPoint.hpp \
           Editor/CSP/CSP.hpp \
           Editor/CSP/CSPConstrainedVariable.hpp \
           Editor/CSP/CSPLinearConstraint.hpp \
           Editor/CSP/IllegalArgumentException.hpp \
           Editor/CSP/IntervalRelation.hpp \
           Editor/CSP/InvalidFileFormatException.hpp \
           Editor/CSP/OutOfBoundException.hpp \
           Editor/CSP/TriggerPoint.hpp \
           Editor/CSP/XMLConversion.hpp \
           Editor/CSP/XMLInclude.hpp \
           Editor/gecode-solver/customSpace.hpp \
           Editor/gecode-solver/gecode_headers.hpp \
           Editor/gecode-solver/integerVariable.hpp \
           Editor/gecode-solver/linearConstraint.hpp \
           Editor/gecode-solver/relations_type.hpp \
           Editor/gecode-solver/searchEngine.hpp \
           Editor/gecode-solver/solver.hpp \
           Editor/gecode-solver/solver_wrap.hpp

SOURCES += Engines.cpp \
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

