/*----------------------------------------------------------------------------------------------
*
* Generate Samples
* 
*----------------------------------------------------------------------------------------------*/
RT_Point2f GenerateRegularSample(int numSamples)
{ 
	int n = (int)sqrt((float)numSamples);

	int y = get_local_id(0) / n;
	int x = get_local_id(0) % n;
	RT_Point2f p = {(x + 0.5) / n, (y + 0.5) / n};
	return p;
}

inline RT_Point2f SampleOneSet(__constant RT_Point2f *samples,
							   __global RT_DataScene *world)
{ 
	world->count=+1;
	return samples[world->count % world->numSamples];
}

inline RT_Point2f SampleUnitSquare(__constant RT_Point2f *samples, 
								   __constant ulong *shuffledIndices,
								   __global RT_DataScene *world)
{ 
	if(world->count % world->numSamples == 0)
	{    
		ulong seed = world->seed;
		world->jump = (randInt(&seed)%world->numSets) * world->numSamples;
		world->seed = seed;
	}
	world->count=+1;
	ulong index = world->jump + shuffledIndices[world->jump + (world->count % world->numSamples)];
	index = (index > world->numShuffledIndices - 1)? world->numShuffledIndices - 1 : index;
	return samples[index];
}

/*inline RT_Point2f SampleUnitDisk(__constant RT_Point2f *diskSamples, 
								 __constant int *shuffledIndices,
								 __constant RT_DataScene *world)
{ 
	if(world->count % world->numSamples == 0)
		world->jump = randInt(&world->seed, world->numSets) * world->numSamples;

	return diskSamples[world->jump + shuffledIndices[world->jump + world->count++ % world->numSamples]];
}

inline RT_Point3f SampleHemisphere(__constant RT_Point3f *hemisphereSamples, 
								   __constant int *shuffledIndices,
								   __constant RT_DataScene *world)
{ 
	if(world->count % world->numSamples == 0)
		world->jump = randInt(&world->seed, world->numSets) * world->numSamples;

	return hemisphereSamples[world->jump + shuffledIndices[world->jump + world->count++ % world->numSamples]];
}

inline RT_Point3f SampleHemisphere(__constant RT_Point3f *sphereSamples, 
								   __constant int *shuffledIndices,
								   __constant RT_DataScene *world)
{ 
	if(world->count % world->numSamples == 0)
		world->jump = randInt(&world->seed, world->numSets) * world->numSamples;

	return sphereSamples[world->jump + shuffledIndices[world->jump + world->count++ % world->numSamples]];
}*/
