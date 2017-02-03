//
//  LanternEffect.cpp
//  lantern
//
//  Created by Eric Miller on 1/24/17.
//  Copyright © 2017 patternleaf LLC. All rights reserved.
//

#include "LanternEffect.hpp"
#include <uuid/uuid.h>


LanternEffect::LanternEffect()
{
	uuid_t uuid;
	uuid_string_t s;
	uuid_generate_random(uuid);
	uuid_unparse(uuid, s);
	mId = std::string(s);
}

std::string LanternEffect::getId()
{
	return mId;
}
