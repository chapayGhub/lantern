//
//  EffectRegistry.hpp
//  lantern
//
//  Created by Eric Miller on 2/9/17.
//  Copyright © 2017 patternleaf LLC. All rights reserved.
//

#ifndef EffectRegistry_hpp
#define EffectRegistry_hpp

#include <stdio.h>
#include <map>

class LanternEffect;
class EffectParameter;

class EffectRegistry {

private:
	EffectRegistry();
	~EffectRegistry();
	static EffectRegistry* sSingleton;
	
public:

	static EffectRegistry* shared();
	
	void registerEffect(LanternEffect* effect);
	void unregisterEffect(LanternEffect* effect);
	
	void registerParameter(EffectParameter* param);
	void unregisterParameter(EffectParameter* param);
	
	static std::string makeUuid();
	
	EffectParameter* findParameter(const std::string& id);
	LanternEffect* findEffect(const std::string& id);
	
private:

	std::map<std::string, LanternEffect*> mEffects;
	std::map<std::string, EffectParameter*> mParameters;

};

#endif /* EffectRegistry_hpp */
