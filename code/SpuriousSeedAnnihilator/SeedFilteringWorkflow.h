/*
 *  Ray -- Parallel genome assemblies for parallel DNA sequencing
 *  Copyright (C) 2013 Sébastien Boisvert
 *
 *  http://DeNovoAssembler.SourceForge.Net/
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You have received a copy of the GNU General Public License
 *  along with this program (gpl-3.0.txt).
 *  see <http://www.gnu.org/licenses/>
 */

#ifndef _SeedFilteringWorkflow_h
#define _SeedFilteringWorkflow_h

#include <code/plugin_SeedingData/GraphPath.h>

#include <RayPlatform/core/ComputeCore.h>
#include <RayPlatform/scheduling/TaskCreator.h>
#include <RayPlatform/communication/VirtualCommunicator.h>

/**
 * This is the workflow that will be deposited on the
 * virtual processor.
 *
 * \author Sébastien Boisvert
 */
class SeedFilteringWorkflow: public TaskCreator {

	ComputeCore*m_core;
	int m_seedIndex;

	vector<GraphPath>*m_seeds;

/* TODO: maybe this should be in the TaskCreator */
	VirtualCommunicator * m_virtualCommunicator;
public:

/*
 * Methods to implement for the TaskCreator interface.
 *
 * The TaskCreator stack is used in the handler
 * RAY_SLAVE_MODE_FILTER_SEEDS.
 */

	/** initialize the whole thing */
	void initializeMethod();

	/** finalize the whole thing */
	void finalizeMethod();

	/** has an unassigned task left to compute */
	bool hasUnassignedTask();

	/** assign the next task to a worker and return this worker */
	Worker*assignNextTask();

	/** get the result of a worker */
	void processWorkerResult(Worker*worker);

	/** destroy a worker */
	void destroyWorker(Worker*worker);

	void initialize(vector<GraphPath>*seeds, VirtualCommunicator*virtualCommunicator,
		VirtualProcessor * virtualProcessor, ComputeCore * core);
};

#endif
