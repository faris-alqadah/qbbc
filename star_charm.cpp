/*!
 Driver program for qbbc clustering (query based bi-clustering)


 */

#include <stdlib.h>
#include <fstream>
#include <map>

#include "./headers/basic_prefix_alpha.h"
#include "./headers/core.h"
#include "./headers/helpers.h"

using namespace std;

string inputFile="~";
string outFile="~";
int numArgs=2;
int numContexts=0;
BasicPrefix la;
IOSet *query;
int queryDomain=-1;
int otherDomain=-1;
double beta=0.5;
void(*paramFunction)(RContext *,IOSet *, int, int,int, vector<double> &);


void DisplayUsage(){
    cout<<"\nUSAGE: test -i input_file "
        <<"\nREQUIRED: "
        <<"\n-i <inputFile>"
        <<"\n-m n (number of domains) min1 min2 ... min_n the minimum cardinalites of each domain for n-cluster enumeration"
        <<"\nOPTIONAL (use in this order): "
        <<"\n-alpha <num_contexts> <alpha value_1>...<alpha_value_num> (default is 1.0 for all)"
        <<"\n-c <consistency function> 1- alpha_sigma 2- max_space_uniform (default is 1)"
        <<"\n-o <output file path>"
        <<"\n-opairs <output file path>"
        <<"\n\t these will be output to filename.concepts and filenames.concepts.names"
        <<"\n-k <num>(>0) only enumerate the top k clusters (default is to use area to rank) "
        <<"\n-beta <num>(0-1) specify beta value to use beta-ranking of clusters "
        <<"\n-ovlp <num>(0-1) specify percent overlap to use when ranking top k (default is 0.25)"
        <<"\n-jards <use this flag to compute jards scores>"
        <<"\n-membership <use this flag to keep track of cluster membership and output the matrix as a fimi file (only on single domain)>"
        <<"\n-prog <display progress>"
        <<"\n\n";
    exit(1);
}

void CheckArguments(){
    if (inputFile == "~"){
        cerr<<"\nINPUT FILE NOT ENTERED!";
        DisplayUsage();
    }
    if(la.consistencyMode < 1 || la.consistencyMode > 2){
        cerr<<"\nINVALID CONSISTENCY MODE ENTERED";
        DisplayUsage();
    }else{
        if (la.consistencyMode == la.ALPHA_SIGMA){
            la.consistencyFunction=&AlphaSigma;
            la.paramFunction =&Construct_AlphaSigma_Params;
        }
        else if(la.consistencyMode == la.MAX_SPACE_UNIFORM){
            la.consistencyFunction=&AlphaSigma;
            la.paramFunction = &Construct_MaxSpaceUniform_Params;
        }
    }
    if(la.enumerationMode == la.ENUM_FILE && la.OUTFILE != "~")
        cout<<"\nOutput clusters option enabled: "<<la.OUTFILE;
    else if( la.enumerationMode == la.ENUM_FILE && la.OUTFILE == "~"){
        cout<<"\nOutput option enabled but file not specified!";
        DisplayUsage();
    }
    else if(la.enumerationMode == la.ENUM_TOPK_FILE && la.OUTFILE == "~"){
         cout<<"\nOutput option enabled but file not specified!";
        DisplayUsage();
    }
    else if(la.enumerationMode == la.ENUM_TOPK_FILE && la.OUTFILE != "~"){
          cout<<"\nOutput clusters option enabled: "<<la.OUTFILE;
    }
    if(la.outputEdges){
        cout<<"\nOutput edges option enabled: "<<la.EDGES_FILE;
    }

   if(la.enumerationMode == la.ENUM_TOPK_FILE || la.enumerationMode == la.ENUM_TOPK_MEM){
        if(la.ovlpThresh < 0 || la.ovlpThresh > 1){
            cout<<"\nOverlap threshold not in correct range!";
             DisplayUsage();
        }
        if(beta < 0 || beta > 1){
            cout<<"\nBeta value not in correct range!";
            DisplayUsage();
        }
        if(la.topKK < 0){
            cout<<"\nk not in correct range!";
            DisplayUsage();
        }
        //if reached here then all parameters are correct
        cout<<"\nTOP K mode enabled with k= "<<la.topKK<<" and max overlap= "<<la.ovlpThresh<<" and clusters ranked by ";
        if(la.qualityMode == la.BETA){
            cout<<"BETA-balance function";
            //set the correct parameters for params
            la.topKparams.resize(2);
            la.topKparams[0] = 0;
            la.topKparams[1] = beta;
        }
        else if(la.qualityMode == la.AREA){
            cout<<"Area function";
            la.topKparams.resize(1);
            la.topKparams[0] = 1;
        }
    }
    if(la.pruneMode == la.PRUNE_SIZE){
        cout<<"\nPrune size enabled...limits are: ";
        if (la.PRUNE_SIZE_VECTOR.size() < 2){
            cout<<"\nMin cardinalities not defined!";
            DisplayUsage();
        }
        for(int i=0; i < la.PRUNE_SIZE_VECTOR.size(); i++)
            cout<<"\nDOMAIN "<<i+1<<" min: "<<la.PRUNE_SIZE_VECTOR[i];
    }
    if(la.trackClusterMembership){
        cout<<"\nTrack cluster membership enabled";
    }
    if(la.computeJards){
        cout<<"\nCompute JARDS scores enabled";
    }
    la.dispersionFunction=&Range;
    if(numContexts == 0){
        cout<<"\nALPHA values not defined for contexts! Please specify!\n";
        DisplayUsage();
    }
    cout<<"\ninput file: "<<inputFile<<"\nalpha: ";
    for(int i=0; i < numContexts; i++) cout<<la.alpha[i+1]<<" ";
    cout<<"\nconsistency mode: "<<la.consistencyMode;
    if(la.dispProgress) cout<<"\nDisplay progress option enabled";

    cout<<"\n"<<endl;
}


void ProcessCmndLine(int argc, char ** argv){

    if (argc < numArgs+1) DisplayUsage();
     else{
        for(int i=1; i < argc; i++){
           string temp = argv[i];
           if(temp == "-i")//input file
                inputFile = argv[++i];
           else if(temp == "-m"){
              int nn = atoi(argv[++i]);
              la.PRUNE_SIZE_VECTOR.resize(nn);
              for(int j=0; j < nn; j++)
                  la.PRUNE_SIZE_VECTOR[j] = atoi(argv[++i]);

           }
           else if(temp == "-alpha"){
                numContexts=atoi(argv[++i]);
               for(int j=0; j < numContexts; j++)
                    la.alpha[j+1] = atof(argv[++i]);
           }
           else if(temp == "-c"){
               la.consistencyMode = atoi(argv[++i]);
           }
           else if(temp == "-o"){
               la.OUTFILE= argv[++i];
               if (la.enumerationMode == la.ENUM_TOPK_MEM)
                   la.enumerationMode = la.ENUM_TOPK_FILE;
               else
                la.enumerationMode = la.ENUM_FILE;
           }else if(temp == "-opairs"){
               la.outputEdges=true;
               la.EDGES_FILE=argv[++i];
           }
           else if(temp == "-prog"){
               la.dispProgress=true;
           }
           else if(temp == "-k"){
               if(la.enumerationMode == la.ENUM_FILE)
                    la.enumerationMode = la.ENUM_TOPK_FILE;
               else
                   la.enumerationMode = la.ENUM_TOPK_MEM;
               la.topKK = atoi(argv[++i]);
           }
           else if(temp == "-ovlp"){
               la.ovlpThresh = atof(argv[++i]);
           }
           else if(temp == "-beta"){
               la.qualityMode= la.BETA;
               beta = atof(argv[++i]);
           }
           else if(temp == "-jards"){
               la.computeJards=true;
               la.trackClusterMembership=true;
           }
           else if(temp == "-membership"){
               la.trackClusterMembership=true;
           }
        }
    }
    CheckArguments();
}


void OutputStats(){
    ofstream outStat("stats",ios_base::app);
    outStat<<ComputeTime()<<"\n";
    outStat.close();

}
void OutputClustersFile(){
    int lim = la.CONCEPTS.size() > la.topKK ? la.topKK:la.CONCEPTS.size();
    for(int i=0; i < lim; i++){
        OutputCluster(la.CONCEPTS[i], la.OUT1);
        OutputCluster(la.CONCEPTS[i],la.OUT2,la.NAME_MAPS);
    }
    la.OUT1.close();
    la.OUT2.close();
}


int main(int argc, char** argv) {
    ProcessCmndLine(argc,argv);
    
    la.NETWORK = MakeRelationGraphR(inputFile);
    //compute standard deviations of each context
    la.NETWORK->ComputeStdDevs();
    //compute ranges of each context
    la.NETWORK->ComputeRange();
    la.NETWORK->Print();
    la.s = 1; //central domain assumed to be first domain listed
    StartTiming();
    la.StarCharm();
    EndTiming();
    OutputStats();
    if(la.enumerationMode == la.ENUM_TOPK_FILE){
        cout<<"\nOutputing clusters to file....\n";
        sort(la.CONCEPTS.begin(),la.CONCEPTS.end(),Compare_Quality_NCluster);
        OutputClustersFile();
    }
    if(la.outputEdges){
        cout<<"\nOutput edges to edge file...\n";
        //assuming bi-clusters are domains 1 and 2
        OutputEdges( la.EDGES, la.EDGES_OUT,1,2,la.NAME_MAPS);
        ofstream matOut((la.EDGES_FILE+".matrix").c_str());
        OutputEdgesBinaryMatrix(la.EDGES, matOut, la.NETWORK->NumObjsInDomain(1), la.NETWORK->NumObjsInDomain(2));
        la.EDGES_OUT.close();
        matOut.close();
    }
    if(la.computeJards){
        cout<<"\n-----------Computing Jards----------\n";
        la.Make_Jarbs_Pairs();
    }
    if(la.trackClusterMembership){
        cout<<"\n-----------Outputting cluster membership----------\n";
        la.OutputClusterMembership();
    }
    cout<<"\n";
    return (EXIT_SUCCESS);
}
