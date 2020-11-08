#include <bits/stdc++.h>
#define SIZE 100105
#define ITEMS 120
#define INPUTFILE "chess.in"
using namespace std;
typedef long long ll;
typedef pair <ll, ll> pll;

struct node{
	int cnt;
	int lastItem;
	node* parent;
	node* next;
	vector < node* > adj;
	bool isRoot;
	node(){
		cnt=0;
		isRoot=false;
		next=NULL;
		adj.clear();
		adj.resize(ITEMS);
		for(int i=0; i<ITEMS; i++) adj[i]=NULL;
	}
	node(bool root){
		cnt=0;
		isRoot=root;
		next=NULL;
		adj.clear();
		adj.resize(ITEMS);
		for(int i=0; i<ITEMS; i++) adj[i]=NULL;
	}
};

node* root=new node(true);

node* header[ITEMS];
node* last[ITEMS];

int threshold;

vector <int> frequentItems;
vector < vector <int> > frequentItemSets;
int freqSubCount;
int fre[ITEMS];
bool in[ITEMS];
int pos[ITEMS];
void insert(vector <int > itemset){
	node* curr=root;
	for(auto item: itemset){
		curr->cnt++;
		if(curr->adj[item]==NULL){
			curr->adj[item]=new node();
			curr->adj[item]->parent=curr;
			curr->adj[item]->lastItem=item;
		}
		curr=curr->adj[item];
	}
	curr->cnt++;
}

void dfs(node* nd, int item){
	if(item!=-1){
		if(header[item]==NULL){
			header[item]=nd;
		}
		else{
			last[item]->next=nd;
		}
		last[item]=nd;
	}
	int i=0;
	for(auto v: nd->adj){
		if(v!=NULL){
			dfs(v, i);
		}
		i++;
	}
}


bool cmp(int a, int b){
	return fre[a]>fre[b];
}


int mxItem;
int mnItem;
void findFrequentItems(int percentage){
	FILE* fp=fopen(INPUTFILE, "r");
	int item, cnt=0;
	mxItem=0;
	mnItem=ITEMS;
	char ch;
	memset(in, false, sizeof(in));
	memset(fre, 0, sizeof(fre));
	while(fscanf(fp, "%d%c", &item, &ch)!=EOF){
		if(!in[item]) fre[item]++;
		in[item]=true;
		if(ch=='\n'){
			memset(in, false, sizeof(in));
			cnt++;
		}
		mxItem=max(mxItem, item);
		mnItem=min(mnItem, item);
	}
	threshold=(percentage*cnt+99)/100;
	cout<<"Threshold="<<threshold<<endl;
	assert(mnItem>0);
	assert(mxItem<ITEMS);
	for(int i=mnItem; i<=mxItem;i++){
		if(fre[i]>=threshold){
			frequentItems.push_back(i);
		}
	}
	sort(frequentItems.begin(), frequentItems.end(), cmp);
}

void insertIntoFPTree(){
	FILE* fp=fopen(INPUTFILE, "r");
	memset(in, false, sizeof(in));
	vector <int> temp;
	int item;
	char ch;
	while(fscanf(fp, "%d%c", &item, &ch)!=EOF){
		if(!in[item]){
			if(fre[item]>=threshold) temp.push_back(item);
			in[item]=true;
		}
		if(ch=='\n'){
			sort(temp.begin(), temp.end(), cmp);
			insert(temp);
			temp.clear();
			memset(in,false, sizeof(in));
		}
	}
}

vector <int> getPath(node* init){
	vector <int> temp;
	init=init->parent;
	while(!init->isRoot){
		temp.push_back(init->lastItem);
		init=init->parent;
	}
	reverse(temp.begin(), temp.end());
	return temp;
}

struct conditionalFPTree{
	node* root;
	node* header[ITEMS];
	node* last[ITEMS];
	bool singlePath;
	void insert(vector <int > itemset, int weight){
		node* curr=root;
		//printf("Counts being updated:\n");
		for(auto item: itemset){
			curr->cnt+=weight;
			//printf("%d\n", curr->cnt);
			if(curr->adj[item]==NULL){
				curr->adj[item]=new node();
				curr->adj[item]->parent=curr;
				curr->adj[item]->lastItem=item;
			}
			curr=curr->adj[item];
		}
		curr->cnt+=weight;
		//printf("%d\n", curr->cnt);
	}
	void dfs(node* nd, int item){
		if(item!=-1){
			if(header[item]==NULL){
				header[item]=nd;
				//printf("Header[%d] is updated\n", item);
			}
			else{
				last[item]->next=nd;
			}
			last[item]=nd;
		}
		int i=0;
		for(auto v: nd->adj){
			if(v!=NULL){
				dfs(v, i);
			}
			i++;
		}
	}
	
	bool isSinglePath(node *curr){
		int cnt=0;
		for(auto v: curr->adj){
			if(v!=NULL){
				cnt++;
				if(cnt>1){
					return false;
				}
				else{
					if(!isSinglePath(v)) return false;
				}
			}
		}
		return true;
	}
	
	conditionalFPTree(){}
	conditionalFPTree(vector <vector <int> > buffer, vector <int> weight){
		root=new node(true);
		int ind=0;
		for(auto itemset: buffer){
			//printf("Inserting list: ");
			/*for(auto item: itemset){
				cout<<item<<" ";
			}
			cout<<endl;*/
			insert(itemset, weight[ind]);
			ind++;
		}
		for(int i=0; i<ITEMS; i++){
			header[i]=NULL;
			last[i]=NULL;
		}
		dfs(root, -1);
		singlePath=(isSinglePath(root));
		//printf("Conditional FP-tree built with %d entries\n", root->cnt);
	}
	
	vector <int> getPath(node* init){
		vector <int> temp;
		init=init->parent;
		while(!init->isRoot){
			temp.push_back(init->lastItem);
			init=init->parent;
		}
		reverse(temp.begin(), temp.end());
		return temp;
	}
	
	int getFreq(int item){
		int sum=0;
		//printf("Getting frequency for %d\n", item);
		node* curr=header[item];
		//if(curr==NULL) printf("Header is null\n");
		while(curr!=NULL){
			sum+=curr->cnt;
			//printf("Adding %d\n", curr->cnt);
			curr=curr->next;
		}
		return sum;
	}
	
	int getDepth(node* curr, int threshold){
		if(curr->cnt<threshold) return -1;
		for(auto v: curr->adj){
			if(v!=NULL) return 1+getDepth(v, threshold);
		}
		return 0;
	}
	
	void freeTree(node* curr){
		for(auto v: curr->adj){
			if(v!=NULL) freeTree(v);
		}
		free(curr);
	}	
};


conditionalFPTree buildConditionalTree(conditionalFPTree tree, int item){
	node* curr=tree.header[item];
	node* temp;
	vector < vector <int> > buffer;
	vector <int> weight;
	while(curr!=NULL){
		weight.push_back(curr->cnt);
		//printf("Adding weight %d\n", curr->cnt);
		temp=curr;
		vector <int> ret=tree.getPath(curr);
		/*printf("Got list\n");
		for(auto v: ret){
			printf(" %d", v);
		}
		puts("");*/
		buffer.push_back(tree.getPath(curr));
		curr=temp;
		curr=curr->next;
	}
	return conditionalFPTree(buffer, weight);
}

conditionalFPTree buildConditionalTree(int item){
	node* curr=header[item];
	node* temp;
	vector < vector <int> > buffer;
	vector <int> weight;
	while(curr!=NULL){
		weight.push_back(curr->cnt);
		temp=curr;
		buffer.push_back(getPath(curr));
		curr=temp;
		curr=curr->next;
	}
	return conditionalFPTree(buffer, weight);
}

vector <int> conditionalPatternBase;
int cfpcount;
void frequentItemsetMiner(conditionalFPTree tree){
	/*printf("Conditional Pattern base: \n");
	for(auto item: conditionalPatternBase){
		cout<<item<<" ";
	}
	cout<<endl<<endl;*/
	cfpcount++;
	if(tree.singlePath){
		//cout<<"singlePathTree\n";
		//printf("Depth is %d\n", tree.getDepth(tree.root, threshold));
		freqSubCount+=(1<<tree.getDepth(tree.root, threshold));
		return;
	}
	frequentItemSets.push_back(conditionalPatternBase);
	for(auto i: frequentItems){
		//printf("Considering adding item %d\n", i);
		//printf("Frequency is %d\n", tree.getFreq(i));
		if(tree.getFreq(i)>=threshold){
			//printf("Adding item %d\n", i);
			conditionalPatternBase.push_back(i);
			//printf("Recursive call starts\n");
			conditionalFPTree cfp=buildConditionalTree(tree, i);
			frequentItemsetMiner(cfp);
			//printf("Recursive call returns\n");
			cfp.freeTree(cfp.root);
			conditionalPatternBase.pop_back();
		}
	}
}


int main(){
	int percentage;
	printf("Enter the percentage\n");
	scanf("%d", &percentage);
	time_t st_time=clock();
	findFrequentItems(percentage);
	insertIntoFPTree();
	dfs(root, -1);
	reverse(frequentItems.begin(), frequentItems.end());
	for(auto item: frequentItems){
		conditionalPatternBase.push_back(item);
		conditionalFPTree cfp=buildConditionalTree(item);
		frequentItemsetMiner(cfp);
		cfp.freeTree(cfp.root);
		conditionalPatternBase.pop_back();
	}
	time_t ed_time=clock();
	printf("Time taken=%f\n", (double)((ed_time-st_time)*1.0)/CLOCKS_PER_SEC);
	printf("Frequent Patterns: %d\n", freqSubCount+(int)frequentItemSets.size());
	printf("Conditional FP trees: %d\n", cfpcount);
	/*for(auto itemsets: frequentItemSets){
		for(auto item: itemsets){
			cout<<item<<" ";
		}
		cout<<endl;
	}*/
	return 0;
}
