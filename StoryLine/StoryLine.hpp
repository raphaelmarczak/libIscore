#ifndef STORYLINE_HPP_
#define STORYLINE_HPP_

#include <vector>
#include <map>
#include "../Editor/CSP/CSP.hpp"

class StoryLine
{
public:
	StoryLine();
	StoryLine(CSP* cspToCompile);

	virtual ~StoryLine();

	std::vector<ConstrainedBox*> m_constrainedBoxes;
	std::vector<AntPostRelation*> m_antPostRelations;

	std::map<unsigned int, TriggerPoint *>  m_triggerPoints;

};

void computeHierarchyStoryLine(unsigned int rootBoxId, CSP* cspToCompile, std::map<unsigned int, StoryLine> & hierarchy);

#endif /*STORYLINE_HPP_*/
