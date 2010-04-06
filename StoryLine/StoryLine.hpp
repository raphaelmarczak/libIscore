#ifndef STORYLINE_HPP_
#define STORYLINE_HPP_

#include <vector>
#include <map>
#include "../Editor/CSP/CSP.hpp"

class StoryLine
{
public:
	StoryLine(CSP* cspToCompile);
	virtual ~StoryLine();

	std::vector<ConstrainedBox*> m_constrainedBoxes;
	std::vector<AntPostRelation*> m_antPostRelations; 
	
	std::map<unsigned int, TriggerPoint *>  m_triggerPoints;
	
};

#endif /*STORYLINE_HPP_*/
