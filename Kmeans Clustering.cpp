#include <bits/stdc++.h>
#define MAXOBJECTS
using namespace std;
int n, k, dim;
int previ[105];
vector < double > p[105], centroid[105];
vector < int > cluster[105];

double euclideanDis(vector <double> a, vector <double> b){
    double res=0.0;
    for(int i=0; i<(int)a.size(); i++){
        res+=(a[i]-b[i])*(a[i]-b[i]);
    }
    return sqrt(res);
}

int findNearest(vector <double> p){
    double minDis=euclideanDis(p, centroid[0]);
    int ind=0;
    for(int i=1; i<k; i++){
        if(euclideanDis(p, centroid[i])<minDis){
            minDis=euclideanDis(p, centroid[i]);
            ind=i;
        }
    }
    return ind;
}


void setMedian(int cno){
    for(int i=0; i<dim; i++){
        centroid[cno][i]=0;
    }
    int cnt=0;
    for(auto ind: cluster[cno]){
        cnt=0;
        for(auto x: p[ind]){
            centroid[cno][cnt]+=x;
            cnt++;
        }
    }
    for(int i=0; i<dim; i++){
        centroid[cno][i]/=(((int)cluster[cno].size())*1.0);
    }
    cluster[cno].clear();
}

void output(){
    for(int i=0; i<n; i++){
        printf("Point %d is in cluster %d\n", i, previ[i]);
    }
}

void kMeans(){
    bool stop=false;
    int tmp;
    memset(previ, -1, sizeof(previ));
    while(!stop){
        stop=true;
        for(int i=0; i<n; i++){
            tmp=findNearest(p[i]);
            cluster[tmp].push_back(i);
            if(tmp!=previ[i]){
                printf("Nearest for point %d changes to %d\n", i, tmp);
                previ[i]=tmp;
                stop=false;
            }
        }
        if(!stop){
            for(int i=0; i<k; i++){
                setMedian(i);
            }
        }
    }
    output();
}

void input(){
    printf("Enter number of points: ");
    scanf("%d", &n);
    printf("Enter number of dimensions: ");
    scanf("%d", &dim);
    printf("Enter number of clusters: ");
    scanf("%d", &k);
    double x;
    for(int i=0; i<n; i++){
        for(int j=0; j<dim; j++){
            scanf("%lf", &x);
            p[i].push_back(x);
        }
    }
    for(int i=0; i<k; i++){
        centroid[i]=p[i];
    }
}

int main(){
    freopen("in.txt", "r", stdin);
    input();
    kMeans();
    return 0;
}
