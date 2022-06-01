#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <ctime>
using namespace std;
void triple_traingle(vector<int> v,int arr[4]){
    sort(v.begin(), v.end());
    int ans = INT_MAX;
    for (unsigned int i=0; i<v.size();i++){
        int j = i+1;
        int k = i+2;
        while (j<v.size()&&k<v.size()){
            if(v.at(i)+v.at(j)>v.at(k)&&v.at(i)+v.at(k)>v.at(j)&&v.at(k)+v.at(j)>v.at(i)){
                if(v.at(i)+v.at(j)+v.at(k)<ans){
                    ans = v.at(i)+v.at(j)+v.at(k);
                    arr[0]=i;
                    arr[2]=k;
                    arr[1]=j;
                    arr[3]=ans;
                }
            }
            k++;
            j++;
        }
    }
}
void naive_triple(vector<int> v,int arr[4]) {
    int ans = INT_MAX;
    for (unsigned int i = 0; i < v.size(); ++i) {
        for (unsigned int j = 0; j < v.size(); j++) {
            for (unsigned int k = 0; k < v.size(); k++) {
                if (i != k && i != j && j != k && v.at(i) + v.at(j) > v.at(k) && v.at(i) + v.at(k) > v.at(j) &&
                    v.at(k) + v.at(j) > v.at(i)) {
                    if (v.at(i) + v.at(j) + v.at(k) < ans) {
                        ans = v.at(i) + v.at(j) + v.at(k);
                        arr[0] = i;
                        arr[2] = k;
                        arr[1] = j;
                        arr[3] = ans;
                    }
                }
            }
        }

    }
}

int main(){
    srand(time(0));
    int arr[4];
    int arr2[4];
    vector<int> v;
    for (int i = 0; i < 100; ++i) {
        for (int i = 0; i < 50; ++i) {
            v.push_back(rand() % 100 );
        }
        triple_traingle(v,arr);
        naive_triple(v,arr2);
        v.clear();
        cout<<arr[3]<<" triangle val mySul ---- "<<arr2[3]<< "  triangle val right solution"<<endl<<endl;
    }
//    triple_traingle(v,arr);
//    naive_triple(v,arr2);
//    cout<<arr[0]<<" i val mySul ---- "<<arr2[0]<< " i val right solution"<<endl;
//    cout<<arr[1]<<" j val mySul ----"<<arr2[1]<< " j val right solution"<<endl;
//    cout<<arr[2]<<" k val mySul ----"<<arr2[2]<< " k valright solution"<<endl<<endl;
//    cout<<arr[3]<<" triangle val mySul ----"<<arr2[3]<< "  triangle val right solution"<<endl;


}
