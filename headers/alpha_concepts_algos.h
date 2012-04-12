//! Author: Faris Alqadah
/*!

 This class serves as the base class for Information Network Clustering algorithms involve
 real-valued contexts.
 All algorithms for real-valued contexts should be formed as children classes of this class, as this
 class provides several generic class members that are utilized to indicate different modes
 of operation for each algorithm. For example, different modes of operation could be mining the clusters
 and outputing to a file, or maintaing in memory etc. Additionaly the class member variables indicate what
 quality functions should be used etc. Each derived class' documentation speficies what variables must be set
 in order to execute the underlying algorihtm correclty.

 */


#ifndef _ALPHA_CONCEPTS_ALGOS_H
#define	_ALPHA_CONCEPTS_ALGOS_H

#include "core.h"
#include "consistency.h"
#include "dispersion.h"
#include "helpers.h"

class AlphaConceptsAlgos : public LatticeAlgos{
public:
AlphaConceptsAlgos():LatticeAlgos() {
    consistencyMode=1; //default
    dispersionMode=1; //default
    trackClusterMembership=false;
    computeJards=false;
};

//! Paramater vector for top K cluster enumeration
vector<double> topKparams;


/*! Pointer to a function  that creates the parameters for a
 consistency function
 */
void(*paramFunction)(RContext *,IOSet *, int, int,int, vector<double> &);

//! map, mapping a context id to a value of alpha. alpha is the alpha value for low variance bi-clusteirng
map<int,double> alpha;
/////////////////////////////////Dispersion Modes///////////////////////////////
 //! dispersion function mode that indicate to use range as the dispersion function
 static const int RANGE=1;
 //! user will set this variable to indicate the desired disperion function
 int dispersionMode;
 //! function pointer to a dispersion fuinction, interface functions will set this according to dispersionMode
 double(*dispersionFunction)(RSet*,vector<double> &);

/////////////////////////////////Consistency Modes///////////////////////////////
//! consistency function mode that uses the alpha-sigma rule, with assumed normal distributions
 static const int ALPHA_SIGMA=1;
//! consistency function mode that uses the maximum spacing estimator with assumed uniform distributions
 static const int MAX_SPACE_UNIFORM=2;
//! user will set this variable to indicate the desired consistency function
 int consistencyMode;
 //! function pointer to a consistency function, interface functions will set this according to consistencyMode
 double(*consistencyFunction)(RSet*,vector<double>&);


/////////////////////////////////Additional options to track while cluster enumeration//////////////////////////////
 //! set to true to track cluster membership
 bool trackClusterMembership;
//! set to true to compute Jards, in this case must keep track of 1 level of clusters and correlation
 bool computeJards;


 /////////////////////////////////Data structures for easy reference///////////////////////////////
 //! The real valued context for which the algorithm is being applied
 RContext *K;
 //! Real-valued HIN
 RelationGraphR *NETWORK;
 //! Id of the source or query domain
 int s;
 //!Id of the target or non-query domain
 int t;
 ///////////////////////////////////////////Data structures for keeping track of objects in clusters////////////////////
 //! keep track of correlations between indiviual objects
 map< int, double> corrMap;
 //! keep track of which clusters objects occur in
 NCluster * clusterMembership;
 


 /**!
 * Computer Jarbs coefficients between all pairs of objects and output them
 *
 */
void Make_Jarbs_Pairs();


/**
 *
 * Outputs the cluster membership matrix to file cluster.membership
 *
 */
void OutputClusterMembership();
};

/*!
 * Add bi-cluster membership information when a bicluster is discovered
 *
 *
 *  @param objs the obect ids of objects in a single cluster
 *  @param clusterNum cluster id to which all the objects belong
 *  @param membership an ncluster representing a matrix of cluster membership
 */
void AddClusterMembership( IOSet *objs, int clusterNum, NCluster *memberships);




#endif	/* _ALPHA_CONCEPTS_ALGOS_H */


