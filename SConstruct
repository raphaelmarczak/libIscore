# ENVIRONMENT
env = Environment(CPPFLAGS=' -I/usr/include/libxml2 -IlibController',LIBS=['xml2','pthread','gecodekernel','gecodeint','gecodesearch','gecodeset','gecodesupport','gecodeminimodel','gecodegraph','gecodescheduling','gecodedriver'])

# SOURCE FILES
base = Glob('*.cpp')
editor = Glob('Editor/*.cpp')
csp = Glob('Editor/CSP/*.cpp')
gecodesolver = Glob('Editor/gecode-solver/*.cpp')
eco = Glob('ECOMachine/*.cpp')
petrinet = Glob('ECOMachine/PetriNet/*.cpp')
process = Glob('ECOMachine/Process/*.cpp')
storyline = Glob('StoryLine/*.cpp')
controller = Split('''libController/Controller.cpp libController/Device.cpp libController/PluginFactories.cpp''')

sources = base+editor+csp+gecodesolver+eco+petrinet+process+storyline+controller
	
# BUILD
if not 'install' in COMMAND_LINE_TARGETS:
	libIscore = env.Library('libIscore',sources)
	Clean(libIscore,'.sconsign.dblite')
