#include "StoryLine.hpp"

using namespace std;

StoryLine::StoryLine(CSP* cspToCompile)
{
	vector<unsigned int>* boxesId = new vector<unsigned int>;
	vector<AntPostRelation*>* antPostRelations = new vector<AntPostRelation*>;
	map<unsigned int, TriggerPoint *>* triggerPoints = new map<unsigned int, TriggerPoint *>;

	cspToCompile->getAllBoxesId(boxesId);
	cspToCompile->getAllAntPostRelations(antPostRelations);
	
	for (unsigned int i = 0 ; i < boxesId->size() ; ++i) {
		m_constrainedBoxes.push_back(cspToCompile->getBoxById(boxesId->at(i)));
	}
	
	for (unsigned int i = 0 ; i < antPostRelations->size() ; ++i) {
		m_antPostRelations.push_back(antPostRelations->at(i));
	}
	
	cspToCompile->getTriggerPointMap(triggerPoints);
	
	m_triggerPoints = *triggerPoints; 
	
	delete antPostRelations;
	delete boxesId;
}

StoryLine::~StoryLine()
{
}
