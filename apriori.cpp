#include <bits/stdc++.h>
#define INPUTFILE "in.txt"
#define ITEMS 105
#define MOD1 1000000007
#define MOD2 500000003
#define BASE1 ITEMS+3
#define BASE2 ITEMS+5
using namespace std;
typedef long long ll;
typedef pair <ll, ll> pll;
int fre[ITEMS];
bool in[ITEMS];
int threshold;
set < pll > prevFreqItemSets;
vector < vector <int> > freqItemSets;
vector < vector <int> > prevItemSetList;
vector < vector <int> > candidateList;
vector < vector <int> > prunedList;

struct trieNode{
	int nxt[ITEMS];
	int cnt;
	bool isLeaf;
	trieNode(){
		memset(nxt, -1, sizeof(nxt));
		cnt=0;
		isLeaf=false;
	}
	
};

struct trie{
	vector < trieNode > nodes;
	vector < int> transaction;
	vector <int> can;
	int rootNode;
	void init(){
		rootNode=0;
		nodes.clear();
		nodes.push_back(trieNode());
	}
	
	void insert(){
		int curr=rootNode;
		for(auto c: can){
			if(nodes[curr].nxt[c]==-1){
				nodes.push_back(trieNode());
				nodes[curr].nxt[c]=(int)nodes.size()-1;
			}
			curr=nodes[curr].nxt[c];
		}
		nodes[curr].isLeaf=true;
	}
	
	void CountSub(int pos, int curr){
		if(nodes[curr].isLeaf){
			nodes[curr].cnt++;
			return;
		}
		if(pos==(int)transaction.size()){
			nodes[curr].cnt++;
			return;
		}
		CountSub(pos+1, curr);
		if(nodes[curr].nxt[transaction[pos]]!=-1)CountSub(pos+1, nodes[curr].nxt[transaction[pos]]);
	}
	
	bool isFrequent(int threshold){
		int curr=rootNode;
		for(auto c: can){
			curr=nodes[curr].nxt[c];
		}
		return (nodes[curr].cnt>=threshold);
	}
	void closeTrie(){
		nodes.clear();
	}
}tr;



pll hashItemset(vector <int> itemset){
	ll res1=0;
	ll res2=0;
	for(auto pr: itemset){
		res1*=BASE1;
		res1+=pr;
		res1%=MOD1;
		
		res2*=BASE2;
		res2+=pr;
		res2%=MOD2;
		
	}
	return pll(res1, res2);
}

void findFrequentItems(){
	FILE* fp=fopen(INPUTFILE, "r");
	int item;
	int mxItem=0, mnItem=ITEMS;
	char ch;
	memset(in, false, sizeof(in));
	memset(fre, 0, sizeof(fre));
	while(fscanf(fp, "%d%c", &item, &ch)!=EOF){
		if(!in[item]) fre[item]++;
		in[item]=true;
		if(ch=='\n'){
			memset(in, false, sizeof(in));
		}
		mxItem=max(mxItem, item);
		mnItem=min(mnItem, item);
	}
	assert(mnItem>0);
	vector <int> temp;
	for(int i=mnItem; i<=mxItem; i++){
		if(fre[i]>=threshold){
			temp.clear();
			temp.push_back(i);
			freqItemSets.push_back(temp);
			prevItemSetList.push_back(temp);
			prevFreqItemSets.insert(hashItemset(temp));
		}
	}
}


void printOutput(){
	puts("The frequent itemsets are");
	for(auto f : freqItemSets){
		for(auto v : f){
			printf("%d ", v);
		}
		puts("");
	}
}

inline bool prefixesMatch(int k, vector <int> itemset1, vector <int> itemset2){
	for(int i=0; i<(k-2); i++){
		if(itemset1[i]!=itemset2[i]){
			return false;
		}
	}
	return true;
}

vector <int> makeList(vector <int> itemset1, vector <int> itemset2){
	vector <int> newItemset;
	for(int i=0; i<((int)itemset1.size()); i++){
		newItemset.push_back(itemset1[i]);
	}
	newItemset.push_back(itemset2[itemset2.size()-1]);
	return newItemset;
}

void joiningStep(int k){ //K represents the length of itemsets to be formed
	candidateList.clear();
	int last=0;
	for(int i=0; i<(int)prevItemSetList.size(); i++){
		if(i==(int)(prevItemSetList.size()-1) || !prefixesMatch(k, prevItemSetList[i+1], prevItemSetList[i])){
			for(int j=last; j<=(i-1); j++){
				for(int k=j+1; k<=i; k++){
					candidateList.push_back(makeList(prevItemSetList[j], prevItemSetList[k]));
				}
			} 
			last=i+1;
		}
	} 
}

void printCandidateList(){
	puts("The itemsets in the candidate list are");
	for(auto f : candidateList){
		for(auto v : f){
			printf("%d ", v);
		}
		puts("");
	}
}
void insertToTrie(){
	tr.init();
	for(auto pruned: prunedList){
		tr.can=pruned;
		tr.insert();
	}
	printf("Shesh\n");
}

void pruningStep(int k){
	prunedList.clear();
	vector <int> baseExp1, baseExp2;
	baseExp1.push_back(1);
	baseExp2.push_back(1);
	for(int i=1; i<(int)candidateList[0].size()+2; i++){
		baseExp1.push_back((baseExp1[i-1]*BASE1)%MOD1);
		baseExp2.push_back((baseExp2[i-1]*BASE2)%MOD2);
	}
	pll suffix;
	ll hash1, hash2, temp1, temp2;
	for(auto can: candidateList){
		/*printf("In candidate list:\n");
		for(auto c : can){
			printf("%d ", c);
		}
		puts("");*/
		ll prefix1=0, prefix2=0;
		suffix=hashItemset(can);
		ll suffix1=suffix.first;
		ll suffix2=suffix.second;
		int pw=(int)can.size()-1;
		bool keep=true;
		for(int i=0; i<((int)can.size()-2); i++){
			temp1=(can[i]*baseExp1[pw])%MOD1;
			temp2=(can[i]*baseExp2[pw])%MOD2;
			
			suffix1-=temp1;
			suffix2-=temp2;
			
			suffix1+=MOD1;
			suffix1%=MOD1;
			suffix2+=MOD2;
			suffix2%=MOD2;
			//printf("Deleting %d\n", can[i]);
			//printf("Prefix is %lld, %lld\n", prefix1, prefix2);
			//printf("Suffix is %lld, %lld\n", suffix1, suffix2);
			hash1=((prefix1*baseExp1[pw])%MOD1+suffix1)%MOD1;
			hash2=((prefix2*baseExp2[pw])%MOD2+suffix2)%MOD2;
			//printf("Hash is %lld and %lld\n", hash1, hash2);
			if(prevFreqItemSets.find(pll(hash1, hash2))==prevFreqItemSets.end()){
				keep=false;
				break;
			}
			
			prefix1*=BASE1;
			prefix1+=can[i];
			prefix1%=MOD1;
			
			prefix2*=BASE2;
			prefix2+=can[i];
			prefix2%=MOD2;
			
			
			pw--;
		}
		
		if(keep) prunedList.push_back(can);
	}
	insertToTrie();
}

void printPrunedList(){
	puts("The itemsets in the pruned list are");
	for(auto f : prunedList){
		for(auto v : f){
			printf("%d ", v);
		}
		puts("");
	}
}

void scanDatabase(){
	FILE* fp=fopen(INPUTFILE, "r");
	vector <int> transaction;
	int item;
	char ch;
	while(fscanf(fp, "%d%c", &item, &ch)!=EOF){
		transaction.push_back(item);
		if(ch=='\n'){
			tr.transaction=transaction;
			tr.CountSub(0, tr.rootNode);
			transaction.clear();
		}
	}
}




int main(){
	printf("Enter your threshold\n");
	scanf("%d", &threshold);
	findFrequentItems();
	printOutput();
	joiningStep(2);
	printCandidateList();
	pruningStep(2);
	printPrunedList();
	tr.closeTrie();
	return 0;
}
