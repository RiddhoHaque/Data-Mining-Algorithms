#include <bits/stdc++.h>
using namespace std;
int MOD;
int BUCKETSIZE;
struct bucket{
	vector <int> keys;
	int nxt;
	int prefixSize;
	
	bucket(int pSize){
		keys.clear();
		nxt=-1;
		prefixSize=pSize;
	}
};


struct globalPointer{
	int prefixSize;
	vector <int> indexes;
	globalPointer(){
		prefixSize=0;
		indexes.clear();
	}
};


string toStr(int bits, int n){
	string str="";  
	for(int i=bits-1; i>=0; i--){
		if((n&(1<<i))!=0) str+='1';
		else str+='0';
	} 
	return str;
}

struct DynamicHash{
	int MOD;
	int bits;
	int bucketSize;
	vector < bucket > bucketList;
	int sz;
	globalPointer gp;
	
	DynamicHash(int mod, int BUCKETSIZE){
		MOD=mod;
		for(int i=0; i<32; i++){
			if((1<<i)==mod){
				bits=i;
				break;
			}
		}
		bucketSize=BUCKETSIZE;
		gp=globalPointer();
		sz=0;
		bucket firstBucket=bucket(0);
		bucketList.push_back(firstBucket);
		gp.indexes.push_back(0);
		sz++;
	}
	//void insert(int num);
	void addToBucket(int ind, int num, int prefix){
		prefix=prefix>>(gp.prefixSize-bucketList[ind].prefixSize);
		bucketList[ind].keys.push_back(num);
		if((int)bucketList[ind].keys.size()>bucketSize){
			if(bucketList[ind].prefixSize==gp.prefixSize){
				if(gp.prefixSize==bits){
					while((int)bucketList[ind].keys.size()>bucketSize){
						bucketList[ind].keys.pop_back();
						if(bucketList[ind].nxt==-1){
							bucket newBucket=bucket(bucketList[ind].prefixSize);
							newBucket.keys.push_back(num);
							bucketList.push_back(newBucket);
							sz++;
							bucketList[ind].nxt=sz-1;
							ind=bucketList[ind].nxt;
						}
						else{
							ind=bucketList[ind].nxt;
							bucketList[ind].keys.push_back(num);
						}
					}
					return;
				}
				else{
					vector <int> vec;
					for(auto v: gp.indexes){
						vec.push_back(v);
						vec.push_back(v);
					}
					gp.indexes=vec;
					gp.prefixSize++;
				}
			}
			
			int reqSize=bucketList[ind].prefixSize+1;
			bucketList.push_back(bucket(reqSize));
			int ind1=sz;
			sz++;
			int nprefix=prefix+prefix;
			for(int i=0; i<(1<<(gp.prefixSize-reqSize)); i++){
				int par=(nprefix<<(gp.prefixSize-reqSize))+i;
				gp.indexes[par]=ind1;
			}
			bucketList.push_back(bucket(reqSize));
			int ind2=sz;
			sz++;
			nprefix++;
			for(int i=0; i<(1<<(gp.prefixSize-reqSize)); i++){
				int par=(nprefix<<(gp.prefixSize-reqSize))+i;
				gp.indexes[par]=ind2;
			}
			for(auto num: bucketList[ind].keys){
				insert(num);
			}
		}
	}
	
	void insert(int num){
		int ind=(num%MOD)>>(bits-gp.prefixSize);
		addToBucket(gp.indexes[ind], num, ind);
	}
	
	void print(){
		for(int i=0; i<(1<<gp.prefixSize); i++){
			//printf("%d: ", i);
			cout<<toStr(gp.prefixSize, i)<<": ";
			if(i==0 || gp.indexes[i]!=gp.indexes[i-1]){
				cout<<"("<<gp.prefixSize<<")";
				//printf("(%d)", bucketList[gp.indexes[i]].prefixSize);
				for(int cur=gp.indexes[i]; cur!=-1; cur=bucketList[cur].nxt){
					for(auto key : bucketList[cur].keys){
						printf("%d, ", key);
					}
					puts("");
				}
			}
			else puts(".....");
		}
	}
	
};


void menu(){
	puts("Enter 1 to insert");
	puts("Enter 2 to show");
	puts("Enter 3 to exit");
}

int main(){
	printf("Enter the hash value for the mod: (Must be a power of 2)\n");
	scanf("%d", &MOD);
	bool powerOf2=false;
	for(int i=0; i<32; i++){
		if((1<<i)==MOD){
			powerOf2=true;
			break;
		}
	}
	assert(powerOf2);
	printf("Enter the bucket size\n");
	scanf("%d", &BUCKETSIZE);
	DynamicHash dh=DynamicHash(MOD, BUCKETSIZE);
	while(true){
		menu();
		int choice, num;
		scanf("%d", &choice);
		if(choice==1){
			printf("Enter your number:");
			scanf("%d", &num);
			dh.insert(num);
		}
		else if(choice==2){
			dh.print();
		}
		else{
			break;
		}
	}
	return 0;
}
