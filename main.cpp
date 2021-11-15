#include <iostream>
#include <vector>

using namespace std;

class segmentTree
{
public:
    unsigned long length;
    segmentTree(){}
    segmentTree(vector<int> arr)
    {
        array = arr;
        sumed = array;
        length = arr.size();
        buildSumTree(sumed[0], 0, length, sumed);
    }
    ~segmentTree(){}
    void print();
    void printSum();
private:
    vector<int> array;
    vector<int> sumed;
protected:
    void buildSumTree(long int v, unsigned long int L, unsigned long R, vector<int> a)
    {
        if (L == R - 1) // Условие выхода
        {
            if (L < R) // Поскольку мы объявляем большую размерность, необходимо следить за границей
            {
                sumed[v] = a[L];
            }
            return; // Присвоили, возвращаемся
        }
        unsigned long int M = (L + R) / 2; // Выбираем середину отрезка [L..R]
        buildSumTree(2 * v + 1, L, M, a); // Запускаем сумму для левого потомка
        buildSumTree(2 * v + 2, M, R, a); // И для правого
        sumed[v] = sumed[2 * v + 1] + sumed[2 * v + 2]; // Обновляем текущую вершину
    }
    

};

void segmentTree::print()
{
    
   for(vector<int>::iterator iter = segmentTree::array.begin(); iter != segmentTree::array.end(); ++iter)
   {
       std::cout << *iter << std::endl;
   }
}

void segmentTree::printSum()
{
    for(vector<int>::iterator iter = sumed.begin(); iter != sumed.end(); ++iter)
   {
       std::cout << *iter << std::endl;
   }
}

int main()
{
    vector<int> arr = {1,2,3,4};
    segmentTree a(arr);
//    a.print();
    cout << endl;
    a.printSum();
}
