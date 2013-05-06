//
// Copyright (c) 2013 MASA Group recastdetour@masagroup.net
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#include "DetourAlignmentBehavior.h"

#include "DetourCrowd.h"
#include "DetourCommon.h"


static const float EPSILON = 0.0001f;

dtAlignmentBehavior::dtAlignmentBehavior()
	: dtSteeringBehavior()
{
}

void dtAlignmentBehavior::update(dtCrowdAgent* oldAgent, dtCrowdAgent* newAgent, float dt)
{
	if (!oldAgent || !newAgent)
		return;

	float force[] = {0, 0, 0};

	computeForce(oldAgent, force);
	applyForce(oldAgent, newAgent, force, dt);
}

void dtAlignmentBehavior::computeForce(const dtCrowdAgent* ag, float* force)
{
	if (!ag)
		return;

	const dtCrowdAgent* agents = ag->params.alignmentAgents;
	const int* targets = ag->params.alignmentTargets;
	const int nbTargets = ag->params.alignmentNbTargets;

	if (!agents || !targets || !nbTargets)
		return;

	int count = 0;

	for (int i = 0; i < nbTargets; ++i)
		if (agents[targets[i]].active)
		{
			++ count;
			dtVadd(force, force, agents[targets[i]].vel);
		}

	dtVscale(force, force, 1.f / (float) count);
	dtVsub(force, force, ag->vel);
}