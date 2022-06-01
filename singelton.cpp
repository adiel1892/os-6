#include <iostream>
#include <pthread.h>
#include <assert.h>
using namespace std;
pthread_mutex_t lock;
pthread_mutex_t lock2;
template<typename T>
class Singleton
{
private:
    /* Here will be the instance stored. */
    static Singleton* instance;
    T val;

    /* Private constructor to prevent instancing. */
    Singleton(T val);


public:
    /* Static access method. */
    static Singleton* getInstance(T val);
    void destroy();
};

template<typename T>
Singleton<T> *Singleton<T>::instance = 0;
template<typename T>
Singleton<T> *Singleton<T>::getInstance(T val) {

    if (instance == 0)
    {
        pthread_mutex_lock(&lock);
        instance = new Singleton<T>(val);
        pthread_mutex_unlock(&lock);
    }

    return instance;
}
template<typename T>
Singleton<T>::Singleton(T val){
    lock= PTHREAD_MUTEX_INITIALIZER;
    lock2 = PTHREAD_MUTEX_INITIALIZER;
    this->val = val;
    this->instance=0;
    pthread_mutex_lock(&lock2);
}
template<typename T>
void Singleton<T>::destroy() {
    instance =0;
    pthread_mutex_unlock(&lock2);
}

int main()
{
    //new Singleton(); // Won't work
    FILE* f;
    Singleton<FILE *>* s1 = Singleton<FILE *>::getInstance(f); // Ok
    Singleton<FILE *>* s2 = Singleton<FILE *>::getInstance(f);
//    Singleton* r = Singleton::getInstance();
//
//    /* The addresses will be the same. */
    assert(s1==s2);
    cout<<"we passed the assert which means that the to singelton point to the same address as we wish"<<endl;
}