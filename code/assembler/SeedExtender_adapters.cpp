/*
 	Ray
    Copyright (C) 2012 Sébastien Boisvert

	http://DeNovoAssembler.SourceForge.Net/

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You have received a copy of the GNU General Public License
    along with this program (gpl-3.0.txt).  
	see <http://www.gnu.org/licenses/>
*/


#include <assembler/SeedExtender_adapters.h>
#include <assembler/SeedExtender.h>

void Adapter_RAY_SLAVE_MODE_EXTENSION::setObject(SeedExtender*object){
	m_object=object;
}

void Adapter_RAY_SLAVE_MODE_EXTENSION::call(){
	m_object->call_RAY_SLAVE_MODE_EXTENSION();
}


