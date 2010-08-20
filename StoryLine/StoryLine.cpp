#include "StoryLine.hpp"

using namespace std;

StoryLine::StoryLine()
{

}

StoryLine::StoryLine(CSP* cspToCompile)
{
	vector<unsigned int> boxesId;
	vector<AntPostRelation*> antPostRelations;

	cspToCompile->getAllBoxesId(boxesId);
	cspToCompile->getAllAntPostRelations(antPostRelations);

	for (unsigned int i = 0 ; i < boxesId.size() ; ++i) {
		m_constrainedBoxes.push_back(cspToCompile->getBoxById(boxesId[i]));
	}

	for (unsigned int i = 0 ; i < antPostRelations.size() ; ++i) {
		m_antPostRelations.push_back(antPostRelations[i]);
	}

	m_triggerPoints.clear();
	cspToCompile->getTriggerPointMap(m_triggerPoints);
}

void computeHierarchyStoryLine(unsigned int rootBoxId, CSP* cspToCompile, std::map<unsigned int, StoryLine> & hierarchy)
{
	vector<unsigned int> boxesId;

	cspToCompile->getAllBoxesId(boxesId);

	if (boxesId.size() == 0) {
		return;
	}

	hierarchy[rootBoxId] = StoryLine(cspToCompile);

	StoryLine currentStoryline = hierarchy[rootBoxId];

	for (unsigned int i = 0; i < currentStoryline.m_constrainedBoxes.size(); ++i) {
		ConstrainedBox* currentBox = currentStoryline.m_constrainedBoxes[i];
		computeHierarchyStoryLine(currentBox->getId(), currentBox->getCSP(), hierarchy);
	}
}

StoryLine::~StoryLine()
{
}
