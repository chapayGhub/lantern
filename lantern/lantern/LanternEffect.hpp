//
//  LanternEffect.hpp
//  lantern
//
//  Created by Eric Miller on 1/24/17.
//  Copyright © 2017 patternleaf LLC. All rights reserved.
//

#ifndef LanternEffect_hpp
#define LanternEffect_hpp

#include <stdio.h>
#include "lib/effect.h"
#include "JsonConversions.hpp"

class LanternEffect : public Effect {
public:
	LanternEffect();

	virtual nlohmann::json getState() = 0;
	virtual void setState(nlohmann::json state) = 0;
	virtual nlohmann::json getParameterDescription() = 0;
	
	std::string getId();
protected:
	std::string mId;
};

#endif /* LanternEffect_hpp */