//! Author: Faris Alqadah
/*!
 Dispersion functions to be used with alpha-concepts, to quantify the
 level of dispersion in a subspace

 \sa LatticeAlgos
 */

#ifndef DISPERSION_H
#define	DISPERSION_H

#include "core.h"
#include <cmath>

//! Returns the range of the NRCluster
/*!
 Range is computed as the maximum value - minimum value. The values
 are all assumed to be stored in the rset at a[0]. This is the naive computation.
 Runs in O(n) time.
    \params a the NRcluster to compute range for
    \params params no parameters needed so pass empty vector
 */
double Range(RSet *a, vector<double> &params);


//! Returns the "imperfect query quality" of a bi-cluster, this is essentially fraction of query objects * average range
/*!
 The imperfect query quality is computed simply as fraction of query in the bi-cluster* average range of each row/column of the bi-cluster
 This function assumes the average range is stored in the quality variable of the ncluster.
 Also assumes query objects are a[0] and supporting objects a[1].
 Also assumes the size of the original query is stored in params[0].
    \params a the bi-cluster to imperfect query quality (just indices)
    \params params params[0] holds the original query size
 */
double Imperfect_Query_Quality(NCluster *a, vector<double> &params);


double Distortion(IOSet *s1, IOSet *s2, IOSet *t1, IOSet *t2, RContext *k, int s, int t);

double Std_Within(NCluster *a, RContext *k, int s, int t);

double Std_Across(NCluster *a, RContext *k, int s, int t);

double Mean_Square_Error(NCluster *a, RContext *k, int s, int t);



/*!
 *
 * Computer the spearman rank correlation between all pairs of objects in the specified domain over specified indices. The function writes the values
 * to the correlation context
 * @params objs ioset containing the list of ids
 * @param subspace ioset containing the subset of objects in other domain over which spearman should be computed
 * @params domainId the domain id from which the objects are derived
 * @params k the real valued context in which the objects live'
 * @params corrContext the real valued context to which the spearman rank correlation will be written to
 */
void Spearman_Rank_Correlation_AllPairs(IOSet *objs, IOSet *subspace, int domainId, RContext *k,map<int,double> &corrMap);


#endif


