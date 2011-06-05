/*
 	Ray
    Copyright (C) 2010, 2011  Sébastien Boisvert

	http://DeNovoAssembler.SourceForge.Net/

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You have received a copy of the GNU General Public License
    along with this program (COPYING).  
	see <http://www.gnu.org/licenses/>

*/

#ifndef _IndexerWorker
#define _IndexerWorker

#include <Parameters.h>
#include <RingAllocator.h>
#include <VirtualCommunicator.h>
#include <stdint.h>
#include <ArrayOfReads.h>

// this class is a worker for sequence indexing
class IndexerWorker{
	ArrayOfReads*m_reads;
	int m_sequenceId;
	bool m_done;
	int m_position;
	int m_theLength;
	VirtualCommunicator*m_virtualCommunicator;
	bool m_indexedTheVertex;
	Parameters*m_parameters;
	int m_workerId;
	bool m_checkedCoverage;
	string m_sequence;
	RingAllocator*m_outboxAllocator;
	bool m_forwardIndexed;
	bool m_reverseIndexed;
	bool m_vertexInitiated;
	bool m_vertexIsDone;
	bool m_coverageRequested;
	bool m_fetchedCoverageValues;

	vector<Kmer> m_vertices;
	vector<int> m_coverages;
public:
	void constructor(int sequenceId,char*sequence,Parameters*parameters,RingAllocator*outboxAllocator,
		VirtualCommunicator*vc,uint64_t workerId,ArrayOfReads*a);
	bool isDone();
	void work();
	uint64_t getWorkerId();
};

#endif
