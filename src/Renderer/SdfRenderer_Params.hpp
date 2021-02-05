#pragma once

struct SdfRenderer_Params {
	float absorptionCoefficient = 0.5f;
	float absorptionCutoff = 0.25f;
	float marchMultiplier = 1.8f;
	int   maxVolumeMarchSteps = 20;
	int   maxVolumeLightMarchSteps = 4;
	int   maxSdfSphereSteps = 15;
	int   maxOpaqueShadowMarchSteps = 10;
};