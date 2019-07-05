#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include <mutex>
#define NumHilos 4

std::mutex mtx;

template<typename Class>
void ImprimirVector(std::vector<Class> v) {
    for (auto item : v) {
        std::cout << item << ' ';
    }
    std::cout << std::endl;
}

template<typename Class>
void Comparar(std::vector<Class>& v, int infLimit, int supLimit, int length) {
    mtx.lock();
    for (int i = infLimit; i < supLimit; i++)
        for (int j = 0; j < length-i-1; j++)
            if (v[j] < v[j+1]) {
                Class temp = v[j+1];
                v[j+1] = v[j];
                v[j] = temp;
            }
    mtx.unlock();
}

template<>
void Comparar(std::vector<std::string>& v, int infLimit, int supLimit, int length) {
    mtx.lock();
    for (int i = infLimit; i < supLimit; i++)
        for (int j = 0; j < length-i-1; j++)
            if (v[j].size() < v[j+1].size()) {
                std::string temp = v[j+1];
                v[j+1] = v[j];
                v[j] = temp;
            }
    mtx.unlock();
}

template <typename Class>
Class buscarMayorValor(std::vector<Class> v) {

    std::vector <Class> vAComparar = v;
    std::vector <std::thread> threads;

    int param = 0;
    int paso = vAComparar.size()/NumHilos;
    for (int i = 0; i < NumHilos; i++) {
        int temp = param;
        param += paso;
        threads.emplace_back(Comparar<Class>, ref(vAComparar), temp, param, vAComparar.size());
    }

    for (int i = 0; i < NumHilos; i++) {
        threads[i].join();
    }

    return vAComparar[0];
}

int main() {

    std::cout << std::endl;
    std::cout << "El vector de enteros es: " << std::endl;
    std::vector <int> v1 = {1,2,3,4,5,6,7,8,9,10,2,11,12,13,14,15,16,2,18,19,20,21,2,23,24,25,26,27,28,2,29,30};
    ImprimirVector(v1);
    int HighestValue = buscarMayorValor(v1);
    std::cout << "El valor maximo es: " << HighestValue << std::endl;
    std::cout << std::endl;

    std::cout << "El vector de double es: " << std::endl;
    std::vector <double> v2 = {50.298,203.2,15.12,1.1,1000.5};
    ImprimirVector(v2);
    double HighestValue_2 = buscarMayorValor(v2);
    std::cout << "El valor maximo es: " << HighestValue_2 << std::endl;
    std::cout << std::endl;

    std::cout << "El vector de strings es: " << std::endl;
    std::vector <std::string> v3 = {"car", "test", "planet", "teacher", "solar system"};
    ImprimirVector(v3);
    std::string HighestValue_3 = buscarMayorValor(v3);
    std::cout << "El valor maximo es: " << HighestValue_3 << std::endl;
    std::cout << std::endl;

    return 0;
}