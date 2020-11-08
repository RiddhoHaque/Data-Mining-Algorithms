#include <bits/stdc++.h>
#define NUMBEROFATTRIBUTES 100
#define DOMAINNAMESIZE 40
#define MAXDOMAINSIZE 10
#define SIZE 105
const bool PROMPT=false;
using namespace std;

int attributeCount;
int targetAttribute;

bool checkBit(int mask, int pos){
    return (((mask & (1<<pos)))!=0);
}

int setBit(int mask, int pos){
    return (mask | (1<<pos));
}

struct dataTuple{
    vector < int > data;
    void print(){
		printf("(");
		for(auto d: data){
			printf("%d,", d);
		}
		puts("\b)");
	}
};

struct attribute{
    int domainSize;
    string domain[MAXDOMAINSIZE];
    bool isTarget;
    map <string, int> mp;
}att[NUMBEROFATTRIBUTES];

struct node{
    bool isLeaf;
    int result;
    int classifyingAttribute;
    vector < node* > children;
    node(){
        isLeaf=false;
        children.clear();
    }
};

vector <dataTuple> dataSet, trainingSet;

/*
    Input Format:
    attributeCount
    targetAttribute (0-based)
    domainSize domain1 domain2 domain3 ...
    ....
    numberOfTuples
    attribute1 attribute2 ...
    ....
*/

void takeInput(){
    if(PROMPT) printf("Enter the number of attributes:\n");
    scanf("%d", &attributeCount);
    if(PROMPT) printf("Enter the index of the target attribute:[0-based]\n");
    scanf("%d", &targetAttribute);
    string str;
    if(PROMPT) printf("In each line, start with the domainsize, and then enter domainSize space separated words\n");
    for(int i=0; i<attributeCount; i++){
        scanf("%d", &att[i].domainSize);
        for(int j=0; j<att[i].domainSize; j++){
            cin>>str;
			att[i].domain[j]=str;
            att[i].mp[str]=j+1;
            cout<<i<<" "<<str<<" "<<att[i].mp[str]<<endl;
        }
        att[i].isTarget=false;
    }
    att[targetAttribute].isTarget=true;
    puts("Attributes listed");
    int tupleCount;
    scanf("%d", &tupleCount);
    for(int i=0; i<tupleCount; i++){
        dataTuple tmp;
        for(int j=0; j<attributeCount; j++){
            cin>>str;
            tmp.data.push_back(att[j].mp[str]-1);
            //cout<<i<<" "<<str<<" "<<att[j].mp[str]-1<<endl;
            printf("%d ", att[j].mp[str]-1);
        }
        puts("");
        trainingSet.push_back(tmp);
    }
}

node* root;

inline int isPure(vector <dataTuple> D){
    for(auto d: D){
        if(d.data[targetAttribute]!=D[0].data[targetAttribute]){
            return -1;
        }
    }
    return D[0].data[targetAttribute];
}

int getProjectionSize(vector <dataTuple> D, int column, int val){
    int res=0;
    for(auto d: D){
        if(d.data[column]==val) res++;
    }
    return res;
}

vector <dataTuple> projectDatabase(vector <dataTuple> D, int column, int val){
    vector <dataTuple> vec;
    for(auto d: D){
        if(d.data[column]==val){
            vec.push_back(d);
        }
    }
    return vec;
}

double info(vector <dataTuple> D){
    vector <int> cnt;
    int sz=D.size();
    cnt.resize(att[targetAttribute].domainSize);
    for(int i=0; i<att[targetAttribute].domainSize; i++) cnt[i]=0;
    for(auto d: D){
        cnt[d.data[targetAttribute]]++;
    }
    double p;
    double sum=0;
    for(auto c: cnt){
        if(c==0) continue;
        p=(c*1.0)/(sz*1.0);
        sum-=p*log2(p);
    }
    return sum;
}

int majorityPoll(vector <dataTuple> D){
    vector <int> cnt;
    cnt.resize(att[targetAttribute].domainSize);
    for(int i=0; i<att[targetAttribute].domainSize; i++) cnt[i]=0;
    for(auto d: D){
        cnt[d.data[targetAttribute]]++;
    }
    int mx=0;
    for(int i=1; i<att[targetAttribute].domainSize; i++){
        if(cnt[i]>cnt[mx]) mx=i;
    }
    return mx;
}

inline bool allAttributesFulfilled(int attributeMask){
    for(int i=0; i<attributeCount; i++){
        if(i!=targetAttribute && !checkBit(attributeMask, i)) return false;
    }
    return true;
}

void buildTree(node* nd, vector <dataTuple> D, int attributeMask, int level){
    printf("New node at level %d\n", level);
    nd->result=majorityPoll(D);
    printf("Majority poll yields result %d\n", nd->result);
    if(allAttributesFulfilled(attributeMask) || isPure(D)!=-1){
        nd->isLeaf=true;
        printf("Leaf reached\n");
        return;
    }
	int mx=-1;
    double mxGain=0.0;
    double sum=0.0;
    double gain;
    double currInfo=info(D);
    int sz=D.size();
    for(int i=0; i<attributeCount; i++){
        if(checkBit(attributeMask, i) || i==targetAttribute) continue;
        sum=0.0;
        //printf("For attribute %d\n", i);
        for(int j=0; j<att[i].domainSize; j++){
			//printf("Projection size of domain %d is %d, info=%f\n", j, getProjectionSize(D, i, j),  info(projectDatabase(D, i, j)));
            sum+=((getProjectionSize(D, i, j)*1.0)/((int)sz*1.0))*info(projectDatabase(D, i, j));
        }
        gain=currInfo-sum;
        //printf("Partition size=%d currInfo=%f, Gain=%f for splitting according to attribute %d\n", sz, currInfo, gain, i); 
        if(gain>mxGain){
            mx=i;
            mxGain=gain;
        }
    }
    nd->classifyingAttribute=mx;
    //cout<<"Splitting according to "<<mx<<endl;
    nd->children.resize(att[mx].domainSize);
    int ind=0;
    for(auto child: nd->children){
        if(getProjectionSize(D, mx, ind)!=0){
            child=new node();
            buildTree(child, projectDatabase(D, mx, ind), setBit(attributeMask, mx), level+1);
        }
        else child=NULL;
        ind++;
    }
}


int predict(dataTuple testData, node* nd){
    if(nd->isLeaf) return nd->result;
    if(nd->children[testData.data[nd->classifyingAttribute]]==NULL){
        return nd->result;
    }
    return predict(testData, nd->children[testData.data[nd->classifyingAttribute]]);
}

int main(){
    freopen("in.txt", "r", stdin);
    takeInput();
    root=new node();
    buildTree(root, trainingSet, 0, 1);
    return 0;
}
