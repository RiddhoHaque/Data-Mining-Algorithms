#include <bits/stdc++.h>
#define K 4
#define SIZE 10010
using namespace std;

struct BPlusTree{
	struct node{
		string str[K+4];
		int ara[K+4];
		int numberOfKeys;
		bool isLeaf;
		bool isRoot;
		int parent;
		int lsibling, rsibling;
		node(){
			for(int i=0; i<=K; i++){
				ara[i]=-1;
				str[i]="";
			}
			isLeaf=false;
			isRoot=false;
			parent=-1;
			lsibling=-1;
			rsibling=-1;
			numberOfKeys=0;
		}
		
		bool cmp(string a, string b){
			for(int i=0; i<(int)min(a.size(), b.size()); i++){
				if(a[i]!=b[i]){
					return (a[i]<b[i]);
				}
			}
			return (a.size()<b.size());
		}
		
		void addKey(string s, int leftChild, int rightChild){
			int pos=numberOfKeys;
			for(int i=0; i<numberOfKeys; i++){
				if(cmp(s, str[i])){
					pos=i;
					break;
				}
			}
			ara[numberOfKeys+1]=ara[numberOfKeys];
			for(int i=numberOfKeys; i>pos; i--){
				str[i]=str[i-1];
				ara[i]=ara[i-1];
			}
			ara[pos+1]=rightChild;
			ara[pos]=leftChild;
			str[pos]=s;
			numberOfKeys++;
		}
		
		void addKey(string s, int rightChild){
			int pos=numberOfKeys;
			for(int i=0; i<numberOfKeys; i++){
				if(cmp(s, str[i])){
					pos=i;
					break;
				}
			}
			ara[numberOfKeys+1]=ara[numberOfKeys];
			for(int i=numberOfKeys; i>pos; i--){
				str[i]=str[i-1];
				ara[i]=ara[i-1];
			}
			ara[pos+1]=rightChild;
			str[pos]=s;
			numberOfKeys++;
		}
		
		void print(){
			for(int i=0; i<numberOfKeys; i++){
				cout<<"|"<<str[i];
			}
			cout<<"|";
		}
		
		bool hasKey(string s){
			for(int i=0; i<numberOfKeys; i++){
				if(s==str[i]) return true;
			}
			return false;
		}
		
		int numberOfChildren(){
			int cnt=0;
			for(int i=0; i<=numberOfKeys; i++){
				if(ara[i]!=-1) cnt++;
			}
			return cnt;
		}
		
		int getChild(){
			int cnt=0;
			for(int i=0; i<=numberOfKeys; i++){
				if(ara[i]!=-1) return ara[i];
			}
			return cnt;
		}
		
		void removeKey(string s){
			int pos=-1;
			for(int i=0; i<numberOfKeys; i++){
				if(str[i]==s) pos=i;
			}
			assert(pos!=-1);
			for(int i=pos; i<(numberOfKeys-1); i++){
				ara[i]=ara[i+1];
				str[i]=str[i+1];
			}
			ara[numberOfKeys-1]=ara[numberOfKeys];
			ara[numberOfKeys]=-1;
			numberOfKeys--;
		}
		
		void removeLink(int nxt){
			int pos=-1;
			for(int i=0; i<=numberOfKeys; i++){
				cout<<"Link "<<i<<" : "<<nxt<<endl;
				if(ara[i]==nxt){
					pos=i;
					break;
				}
			}
			assert(pos!=-1);
			str[pos-1]=str[pos];
			for(int i=pos; i<(numberOfKeys-1); i++){
				str[i]=str[i+1];
				ara[i]=ara[i+1];
			}
			numberOfKeys--;
			ara[numberOfKeys]=ara[numberOfKeys+1];
		}
		
		int findLink(int nxt){
			int pos=-1;
			for(int i=0; i<=numberOfKeys; i++){
				if(ara[i]==nxt){
					pos=i;
					break;
				}
			}
			return pos;
		}
		
		void remover(string k, int p){
			if(isLeaf){
				int pos=-1;
				for(int i=0; i<numberOfKeys; i++){
					if(str[i]==k && p==ara[i]){
						pos=i;
						break;
					}
				}
				assert(pos!=-1);
				for(int i=pos; i<(numberOfKeys-1); i++){
					str[i]=str[i+1];
				}
				for(int i=pos; i<numberOfKeys; i++){
					ara[i]=ara[i+1];
				}
				ara[numberOfKeys]=-1;
				numberOfKeys--;
			}
			else{
				int pos=-1;
				for(int i=0; i<numberOfKeys; i++){
					if(str[i]==k && p==ara[i+1]){
						pos=i;
						break;
					}
				}
				assert(pos!=-1);
				for(int i=pos; i<(numberOfKeys-1); i++){
					str[i]=str[i+1];
				}
				for(int i=pos+1; i<numberOfKeys; i++){
					ara[i]=ara[i+1];
					printf("Pointer %d becomes %d\n", i, ara[i]);
				}
				ara[numberOfKeys]=-1;
				numberOfKeys--;
			}
		}
		
		bool tooFew(){
			int n=K+1;
			if(isLeaf){
				return numberOfKeys<(n/2);
			}
			else return numberOfChildren()<(n+1)/2;
		}
		
		void appendKey(string s){
			str[numberOfKeys]=s;
			numberOfKeys++;
		}
		void addKeyAndLink(string s, int p){
			ara[numberOfKeys]=p;
			str[numberOfKeys]=s;
			numberOfKeys++;
		}
		void addLink(int p){
			ara[numberOfKeys]=p;
		}
		
	}tree[SIZE];
	int treeSize;
	int rootNode;
	BPlusTree(){
		rootNode=1;
		tree[1]=node();
		tree[1].isRoot=1;
		tree[1].isLeaf=1;
		treeSize=1;
	}
	
	
	void splitter(int nd, int &parnode, int &leftchild, int &rightchild, string &parVal){
		assert(tree[nd].numberOfKeys==(K+1));
		treeSize++;
		tree[treeSize]=node();
		rightchild=treeSize;
		tree[rightchild].isLeaf=tree[nd].isLeaf;
		leftchild=nd;
		if(tree[leftchild].isRoot){
			tree[leftchild].isRoot=false;
			treeSize++;
			rootNode=treeSize;
			tree[rootNode]=node();
			tree[rootNode].isRoot=true;
			tree[leftchild].parent=rootNode;
		}
		tree[rightchild].parent=tree[leftchild].parent;
		tree[rightchild].rsibling=tree[leftchild].rsibling;
		tree[tree[leftchild].rsibling].lsibling=rightchild;
		tree[leftchild].rsibling=rightchild;
		tree[rightchild].lsibling=leftchild;
		int pos=(tree[leftchild].numberOfKeys/2);
		parVal=tree[leftchild].str[pos];
		if(!tree[nd].isLeaf) pos++;
		for(int i=pos; i<tree[leftchild].numberOfKeys; i++){
			tree[rightchild].addKey(tree[leftchild].str[i], tree[leftchild].ara[i], tree[leftchild].ara[i+1]);
			if(tree[leftchild].ara[i]!=-1){
				tree[tree[leftchild].ara[i]].parent=rightchild;
			}
		}
		int i=tree[leftchild].numberOfKeys;
		if(tree[leftchild].ara[i]!=-1){
			tree[tree[leftchild].ara[i]].parent=rightchild;
		}
		tree[leftchild].numberOfKeys=((K+1)/2);
		parnode=tree[rightchild].parent;
	}
	
	void propagate(int nd){
			int nxt;
			int leftchild, rightchild;
			string val;
			splitter(nd, nxt, leftchild, rightchild, val);
			tree[nxt].addKey(val, leftchild, rightchild);
			if(tree[nxt].numberOfKeys>K){
				propagate(nxt);
			}
	}
	
	void insert(int nd, string s){
		if(tree[nd].isLeaf){
			tree[nd].addKey(s, -1, -1);
			if(tree[nd].numberOfKeys>K){
				propagate(nd);
			}
			return;
		}
		int pos=tree[nd].numberOfKeys;
		for(int i=0; i<tree[nd].numberOfKeys; i++){
			if(tree[nd].cmp(s, tree[nd].str[i])){
				pos=i;
				break;
			}
		}
		cout<<"Going to "<<tree[nd].ara[pos]<<endl;
		insert(tree[nd].ara[pos], s);
	}
	
	void print(int nd){
		if(nd<1) return;
		int cur=nd;
		while(cur>=1){
			cout<<"("<<cur<<")";
			tree[cur].print();
			cur=tree[cur].rsibling;
			cout<<" ";
		}
		cout<<endl;
		print(tree[nd].ara[0]);
	}
	
	bool search(int nd, string s){
		if(tree[nd].isLeaf){
			cout<<"Have com to "<<nd<<endl;
			for(int i=0; i<tree[nd].numberOfKeys; i++){
				if(tree[nd].str[i]==s) return true;
			}
			return false;
		}
		int pos=tree[nd].numberOfKeys;
		for(int i=0; i<tree[nd].numberOfKeys; i++){
			if(tree[nd].cmp(s, tree[nd].str[i])){
				pos=i;
				break;
			}
		}
		return search(tree[nd].ara[pos], s);
	}
	
	int findLeaf(int nd, string s){
		if(tree[nd].isLeaf) return nd;
		int pos=tree[nd].numberOfKeys;
		for(int i=0; i<tree[nd].numberOfKeys; i++){
			if(tree[nd].cmp(s, tree[nd].str[i])){
				pos=i;
				break;
			}
		}
		return findLeaf(tree[nd].ara[pos], s);
	}
	
	void refreshChildren(int nd){
		for(int i=0; i<=tree[nd].numberOfKeys; i++){
			if(tree[nd].ara[i]!=-1){
				tree[tree[nd].ara[i]].parent=nd;
			}
		}
	}
	
	void delete_entry(int N, string k, int P){
		cout<<"Node "<<N<<" String "<<k<<endl;
		tree[N].remover(k, P);
		tree[N].print();
		if(tree[N].isRoot && tree[N].numberOfChildren()==1){
			int nxt=tree[N].getChild();
			tree[nxt].isRoot=true;
			rootNode=nxt;
			tree[nxt].parent=-1;
			puts("Changing root");
		}
		if(tree[N].isRoot) return;
		else if(tree[N].tooFew()){
			puts("Doing something");
			int nd=N;
			for(int loopCnt=0; loopCnt<4; loopCnt++){
				int np;
				bool done=false;
				if(!(loopCnt&1)){
					np=tree[nd].lsibling;
				}
				else np=tree[nd].rsibling;
				if(np==-1) continue;
				cout<<"Other node="<<np<<" loopcnt="<<loopCnt<<endl;
				if(tree[nd].parent!=tree[np].parent) continue;
				int par=tree[nd].parent;
				printf("Parent=%d\n", par);
				int ind=min(tree[par].findLink(nd), tree[par].findLink(np));
				printf("Ind=%d\n", ind);
				string kp=tree[par].str[ind];
				if(loopCnt<2 && (tree[nd].numberOfKeys+tree[np].numberOfKeys)<=K){
					cout<<"Here"<<endl;
					if(np==tree[nd].rsibling){
						swap(np, nd);
					}
					if(!tree[nd].isLeaf){
						tree[np].appendKey(kp);
						for(int i=0; i<tree[nd].numberOfKeys; i++) tree[np].addKeyAndLink(tree[nd].str[i], tree[nd].ara[i]);
						tree[np].addLink(tree[nd].ara[tree[nd].numberOfKeys]);
					}
					else{
						cout<<"Here"<<endl;
						for(int i=0; i<tree[nd].numberOfKeys; i++) tree[np].addKeyAndLink(tree[nd].str[i], tree[nd].ara[i]);
						tree[np].addLink(tree[nd].ara[tree[nd].numberOfKeys]);
					}
					done=true;
					tree[np].rsibling=tree[nd].rsibling;
					if(tree[np].rsibling!=-1) tree[tree[np].rsibling].lsibling=np;
					refreshChildren(np);
					refreshChildren(par);
					delete_entry(par, kp, nd);
				}
				else if(loopCnt>=2 && tree[np].numberOfKeys<K){
					if(np==tree[nd].rsibling){
						swap(np, nd);
					}
					cout<<"Redistributing"<<endl;
					if(!tree[nd].isLeaf){
						string km_1=tree[np].str[tree[np].numberOfKeys-1];
						int pm=tree[np].ara[tree[np].numberOfKeys];
						tree[np].ara[tree[np].numberOfKeys]=-1;
						tree[np].numberOfKeys--;
						tree[nd].ara[tree[nd].numberOfKeys+1]=tree[nd].ara[tree[nd].numberOfKeys];
						for(int i=tree[nd].numberOfKeys; i>0; i--){
							tree[nd].str[i]=tree[nd].str[i-1];
							tree[nd].ara[i]=tree[nd].ara[i-1];
						}
						tree[nd].numberOfKeys++;
						tree[nd].ara[0]=pm;
						tree[nd].str[0]=kp;
						tree[par].str[ind]=km_1;
						refreshChildren(nd);
						refreshChildren(par);
					}
					else{
						string km_1=tree[np].str[tree[np].numberOfKeys-1];
						int pm=tree[np].ara[tree[np].numberOfKeys-1];
						tree[np].ara[tree[np].numberOfKeys]=-1;
						tree[np].numberOfKeys--;
						tree[nd].ara[tree[nd].numberOfKeys+1]=tree[nd].ara[tree[nd].numberOfKeys];
						for(int i=tree[nd].numberOfKeys; i>0; i--){
							tree[nd].str[i]=tree[nd].str[i-1];
							tree[nd].ara[i]=tree[nd].ara[i-1];
						}
						tree[nd].numberOfKeys++;
						tree[nd].ara[0]=pm;
						tree[nd].str[0]=km_1;
						tree[par].str[ind]=km_1;
						refreshChildren(nd);
						refreshChildren(par);
					}
					done=true;
				}
				if(done) break;
			}
		}
		else{
			int par=tree[N].parent;
			printf("Updating parent %d\n", par);
			if(par!=-1){
				int ind=tree[par].findLink(N);
				ind--;
				printf("Index is %d\n", ind);
				if(ind>=0){
					tree[par].str[ind]=tree[N].str[0];
					printf("Parent is now ");
					tree[par].print();
					puts("");
				}
			}
		}
	}
	
	void deleter(string s){
		int L=findLeaf(rootNode, s);
		delete_entry(L, s, -1);
	}
	
}btree;

void menu(){
	puts("Press 1 to insert");
	puts("Press 2 to display");
	puts("Press 3 to search");
	puts("Press 4 to delete");
	puts("Press 5 to exit");
}

int main(){
	//freopen("in.txt", "r", stdin);
	//freopen("out.txt", "w", stdout);
	int command;
	btree=BPlusTree();
	while(true){
		menu();
		scanf("%d", &command);
		if(command==1){
			printf("Enter string to insert: ");
			string str;
			cin>>str;
			btree.insert(btree.rootNode, str);
		}
		if(command==2){
			btree.print(btree.rootNode);
			//printf("\n\n");
		}
		if(command==3){
			string query;
			printf("Enter string to search: ");
			cin>>query;
			if(btree.search(btree.rootNode, query)) puts("Key exists");
			else puts("Key not found");
		}
		if(command==4){
			string query;
			printf("Enter string to delete: ");
			cin>>query;
			if(!btree.search(btree.rootNode, query)) puts("String doesn't exist");
			else{
				btree.deleter(query);
			}
		}
		if(command==5){
			break;
		}
	}
	return 0;
}
