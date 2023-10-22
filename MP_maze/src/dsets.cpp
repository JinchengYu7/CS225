
#include "dsets.h"

void DisjointSets::addelements(int num){
    for(int i = 0; i < num; i ++){
		v.push_back(-1);
	}
}
int DisjointSets::find(int elem){
	if(v[elem] < 0){
		return elem;
	}else{
		v[elem] = find(v[elem]);
		return v[elem];
	}

}

void DisjointSets::setunion(int a, int b){
    int roota=find(a);
    int rootb=find(b);
    int newsize=v[roota]+v[rootb];
    if(v[roota]<v[rootb]){
        v[rootb]=roota;
        v[roota]=newsize;
    }
    else{
        v[roota]=rootb;
        v[rootb]=newsize;
    }
}

int DisjointSets::size(int elem) {
	return (v[find(elem)]*-1);
}