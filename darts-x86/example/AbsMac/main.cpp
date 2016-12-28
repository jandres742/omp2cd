/* 
 * File:   main.cpp
 * Author: koliai
 *
 * Created on June 15, 2012, 2:51 PM
 */

#include <iostream>
#include "AbstractMachine.h"

using namespace darts;

int main(void) 
{
	
	hwloc::AbstractMachine AbsMac;  /* Define the abstract machine */
	
	unsigned nbClusters;
	unsigned nbUnits;
	unsigned ClusterID;
	nbClusters = AbsMac.getNbClusters();
	std::cout << "Number of CLusters: " << nbClusters << std::endl;
  
	for (unsigned i=0 ; i<nbClusters ; i++) {
		ClusterID = AbsMac.getClusterMap()[i].getId();
		std::cout << "Cluster ID: " << ClusterID << std::endl;
  
		nbUnits = AbsMac.getClusterMap()[i].getNbUnits();
		std::cout << "Number of Units: " << nbUnits << std::endl;
	}
 
	return 0;
}
