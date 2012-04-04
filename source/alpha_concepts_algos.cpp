#include <string>
#include <vector>

#include "../headers/alpha_concepts_algos.h"

void AddClusterMembership( IOSet *objs, int clusterNum, NCluster *memberships){
    for(int i=0; i < objs->Size(); i++)
        memberships->GetSetById(objs->At(i))->Add(clusterNum);
}
void AlphaConceptsAlgos::Make_Jarbs_Pairs(){
    IOSet *r1 = NETWORK->GetRContext(1)->GetLabels(2);
    vector<NameMap*> *nmps = NETWORK->GetNameMaps();
    for(int i=0; i < r1->Size(); i++){
             IOSet *clusI = clusterMembership->GetSetById(i);
             if(clusI->Size() > 0){
                for(int j = i+1; j < r1->Size(); j++){
                    IOSet *clusJ = clusterMembership->GetSetById(j);
                    if(clusJ->Size() > 0){
                        IOSet *inter = Intersect(clusI,clusJ);
                        IOSet *unin = Union(clusI,clusJ);
                        if(inter->Size() > 0){
                            double jarbs1 = abs(corrMap[ CantorPairing(i,j)])/(double) unin->Size();
                            if(jarbs1 > 0)
                                cout<<"\n"<<nmps->at(1)->GetName(i)<<"\t"<<nmps->at(1)->GetName(j)<<"\t"<<jarbs1<<"\t"<<inter->Size()<<"\t"<<unin->Size()<<"\t"<<corrMap[ CantorPairing(i,j)];
                        }
                        delete inter;
                        delete unin;
                    }
                }
             }
    }
}