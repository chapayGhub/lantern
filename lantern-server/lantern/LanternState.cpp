//
//  LanternState.cpp
//  lantern
//
//  Created by Eric Miller on 1/24/17.
//  Copyright © 2017 patternleaf LLC. All rights reserved.
//

#include <sstream>
#include "LanternState.hpp"
#include "LanternEffect.hpp"

using namespace nlohmann;
using namespace std;

LanternState::LanternState(EffectRunner* runner, LanternMixer* mixer)
: mRunner(runner), mMixer(mixer)
{
	
}

LanternState::~LanternState()
{
	
}
void LanternState::setFader(int channel, float value)
{
	mMixer->setFader(channel, value);
}

void LanternState::setWith(json json)
{
	lock_guard<mutex> lock(mStateMutex);
	
	auto mixer = json["mixer"];
	auto faders = mixer["faders"];
	assert(faders.is_array());
	
	int i = 0;
	//for (json::iterator it = faders.begin(); it != faders.end(); ++it) {
	for (auto level: faders) {
		if (i < mMixer->numChannels()) {
			mMixer->setFader(i, level);
		}
		else {
			cerr << "incoming state has more mixer channels than exist. channel " << i << "." << endl;
		}
		i++;
	}
	
	auto effects = json["effects"];
	auto states = effects["states"];
	
	i = 0;
	for (auto state: states) {
		if (i < mMixer->numChannels()) {
			LanternEffect* effect = dynamic_cast<LanternEffect*>(mMixer->getEffect(i));
			if (effect != nullptr && state["id"] == effect->getId()) {
				effect->setState(state);
			}
		}
		else {
			cerr << "incoming state has more effect states than exist. state " << i << "." << endl;
		}
		i++;
	}
}

json LanternState::toJson()
{
	lock_guard<mutex> lock(mStateMutex);
	
	json channels = json::array();
	
	LanternEffect* effect = nullptr;
	for (int i = 0; i < mMixer->numChannels(); i++) {
		json channel = json::object();
		
		channel["fader"] = mMixer->getFader(i);
		
		effect = dynamic_cast<LanternEffect*>(mMixer->getEffect(i));
		if (effect != nullptr) {	
			channel["effect"] = effect->getState();
		}
		else {
			channel["effect"] = {{ "id", "unknown-effect" }};
		}
		
		channels.push_back(channel);
	}
	
	return {{ "channels", channels }};
}
