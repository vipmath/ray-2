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

#include <BubbleTool.h>
#include <common_functions.h>
#include <map>
#include <set>
#include <iostream>
using namespace std;

//#define DEBUG_BUBBLES

void BubbleTool::printStuff(uint64_t root,vector<vector<uint64_t> >*trees,
map<uint64_t,int>*coverages){
	int m_wordSize=m_parameters->getWordSize();
	cout<<"Trees="<<trees->size()<<endl;
	cout<<"root="<<idToWord(root,m_wordSize)<<endl;
	if(trees->size()==2){
		cout<<"b1="<<idToWord(trees->at(0).at(0),m_wordSize)<<endl;
		cout<<"b2="<<idToWord(trees->at(1).at(0),m_wordSize)<<endl;
	}
	cout<<"digraph{"<<endl;
	map<uint64_t,set<uint64_t> > printedEdges;
	
	for(map<uint64_t,int>::iterator i=coverages->begin();i!=coverages->end();i++){
		cout<<idToWord(i->first,m_wordSize)<<" [label=\""<<idToWord(i->first,m_wordSize)<<" "<<i->second<<"\"]"<<endl;
	}
	for(int j=0;j<(int)trees->size();j++){
		cout<<idToWord(root,m_wordSize)<<" -> "<<idToWord(trees->at(j).at(0),m_wordSize)<<endl;
		for(int i=0;i<(int)trees->at(j).size();i+=2){
			uint64_t a=trees->at(j).at(i+0);
			uint64_t b=trees->at(j).at(i+1);
			if(printedEdges.count(a)>0 && printedEdges[a].count(b)>0){
				continue;
			}
			cout<<idToWord(a,m_wordSize)<<" -> "<<idToWord(b,m_wordSize)<<endl;
			printedEdges[a].insert(b);
		}
	}
	cout<<"}"<<endl;

}

/**
 *
 */
bool BubbleTool::isGenuineBubble(uint64_t root,vector<vector<uint64_t> >*trees,
map<uint64_t,int>*coverages){
	if((*coverages)[root]==m_parameters->getMaxCoverage()){
		return false;
	}

	#ifdef ASSERT
	int m_wordSize=m_parameters->getWordSize();
	for(int i=0;i<(int)trees->size();i++){
		for(int j=0;j<(int)trees->at(i).size();j+=2){
			uint64_t a=trees->at(i).at(j+0);
			uint64_t b=trees->at(i).at(j+1);
			string as=idToWord(a,m_wordSize);
			string bs=idToWord(b,m_wordSize);
			assert(as.substr(1,m_wordSize-1)==bs.substr(0,m_wordSize-1));
		}
	}
	#endif

	if(trees->size()<2){
		#ifdef DEBUG_BUBBLES
		if(idToWord(root,m_wordSize)=="CCTATTATTGAAAAAACGGGA"){
			cout<<"<2"<<endl;
		}
		#endif
		return false;
	}

	if(trees->size()!=2){
		#ifdef DEBUG_BUBBLES
		if(idToWord(root,m_wordSize)=="CCTATTATTGAAAAAACGGGA"){
			cout<<"!=2"<<endl;
		}
		#endif
		return false;// we don'T support that right now ! triploid stuff are awesome.
	}


	// given the word size
	// check that they join.
	//
	// substitution SNP is d=0
	// del is 1, 2, or 3


	map<uint64_t,int> coveringNumber;

	uint64_t target=0;
	bool foundTarget=false;
	for(int j=0;j<(int)trees->size();j++){
		for(int i=0;i<(int)trees->at(j).size();i+=2){
			uint64_t a=trees->at(j).at(i+1);
			//cout<<"Tree="<<j<<" Visiting "<<idToWord(a,m_wordSize)<<endl;
			coveringNumber[a]++;
			if(!foundTarget && coveringNumber[a]==2){
				foundTarget=true;
				target=a;
				break;
			}
		}
	}

	if(!foundTarget){

		#ifdef DEBUG_BUBBLES
		if(idToWord(root,m_wordSize)=="CCTATTATTGAAAAAACGGGA"){
			cout<<"No target found for CCTATTATTGAAAAAACGGGA"<<endl;
		}
		#endif
		return false;
	}

	if((*coverages)[target]==m_parameters->getMaxCoverage()){
		return false;
	}

	double multiplicator=1.5;
	int peak=m_parameters->getPeakCoverage();
	int multiplicatorThreshold=(int)multiplicator*peak;

	// the two alternative paths must have less redundancy.
	if((*coverages)[target]>=multiplicatorThreshold
	&&(*coverages)[root]>=multiplicatorThreshold){
		return false;
	}

	vector<map<uint64_t,uint64_t> > parents;

	for(int j=0;j<(int)trees->size();j++){
		map<uint64_t,uint64_t> aVector;
		parents.push_back(aVector);
		for(int i=0;i<(int)trees->at(j).size();i+=2){
			uint64_t a=trees->at(j).at(i+0);
			uint64_t b=trees->at(j).at(i+1);
			parents[j][b]=a;
		}
	}

	vector<vector<int> > observedValues;
	/*
 *
 *  BUBBLE is below
 *
 *    *  ----  * -------*  --------*
 *      \                          /
 *        ---- * --------* ------ *
 *
 */
	// accumulate observed values
	// and stop when encountering
	for(int j=0;j<(int)trees->size();j++){
		vector<int> aVector;
		observedValues.push_back(aVector);
		set<uint64_t> visited;
		
		uint64_t startingPoint=trees->at(j).at(0);
		uint64_t current=target;

		while(current!=startingPoint){
			if(visited.count(current)>0){
				return false;
			}
			visited.insert(current);
			uint64_t theParent=parents[j][current];
			int coverageValue=(*coverages)[theParent];

			if(coverageValue>m_parameters->getPeakCoverage()){
				return false;
			}

			observedValues[j].push_back(coverageValue);
			current=theParent;
		}
	}
	#ifdef DEBUG_BUBBLES
	cout<<"root="<<idToWord(root,m_wordSize)<<" target="<<idToWord(target,m_wordSize)<<endl;
	#endif

	#ifdef DEBUG_BUBBLES
	cout<<"O1="<<observedValues[0].size()<<" O2="<<observedValues[1].size()<<endl;
	#endif

	int sum1=0;
	for(int i=0;i<(int)observedValues[0].size();i++){
		sum1+=observedValues[0][i];
	}

	#ifdef DEBUG_BUBBLES
	cout<<"O1Values= ";
	for(int i=0;i<(int)observedValues[0].size();i++){
		cout<<observedValues[0][i]<<" ";
	}
	cout<<endl;
	#endif

	int sum2=0;
	for(int i=0;i<(int)observedValues[1].size();i++){
		sum2+=observedValues[1][i];
	}
	
	#ifdef DEBUG_BUBBLES
	cout<<"O2Values= ";
	for(int i=0;i<(int)observedValues[1].size();i++){
		cout<<observedValues[1][i]<<" ";
	}
	cout<<endl;

	#endif

	if((int)observedValues[0].size()<2*m_parameters->getWordSize()
	&& (int)observedValues[1].size()<2*m_parameters->getWordSize()){
		if(sum1>sum2){
			m_choice=trees->at(0).at(0);
		}else if(sum2>sum1){
			m_choice=trees->at(1).at(0);

		// this will not happen often
		}else if(sum1==sum2){
			// take the shortest, if any
			if(observedValues[0].size()<observedValues[1].size()){
				m_choice=trees->at(0).at(0);
			}else if(observedValues[1].size()<observedValues[0].size()){
				m_choice=trees->at(1).at(0);
			// same length and same sum, won't happen very often anyway
			}else{
				m_choice=trees->at(0).at(0);
			}
		}
		#ifdef DEBUG_BUBBLES
		cout<<"This is a genuine bubble"<<endl;
		cout<<"root="<<idToWord(root,m_wordSize)<<" target="<<idToWord(target,m_wordSize)<<endl;
		printStuff(root,trees,coverages);
		#endif
		return true;
	}

	#ifdef DEBUG_BUBBLES
	cout<<"False at last"<<endl;
	printStuff(root,trees,coverages);
	#endif

	return false;
}

uint64_t BubbleTool::getTraversalStartingPoint(){
	return m_choice;
}

void BubbleTool::constructor(Parameters*parameters){
	m_parameters=parameters;
}
