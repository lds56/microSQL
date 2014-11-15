/*
 * storage.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: Administrator
 */

#include "models/storage.h"

IndexBranch::IndexBranch(){
	 key="0";
	 child=0;
}

block::block(){
	isLeaf=true;
	isRoot=true;
	father=0;
	length=0;
}




