#include <iostream>
#include <array>

using namespace std;

class segmentTree
{
public:
    int length;
    segmentTree(){}
    segmentTree(int *arr, int leng)
    {
        length = leng;
        array = new int[length];
        for(int i = 0; i < length; ++i)
            array[i] = arr[i];
        buildSumTree(array[0], 0, length, arr);
    }
    ~segmentTree(){ for(int i = 0; i < length; ++i) { array[i] = NULL; } delete [] array; }
    void print();
    int64_t getSummary(int v, int L, int R, int l, int r) // L, R - обрабатываемые функцией границы, l, r - границы запроса
    {
        if (r <= L || R <= l) return 0; // Первое условие
        if (l <= L && R <= r) return array[v]; // Второе условие
        int M = (L + R) / 2;
        int64_t first_child = getSummary(2 * v + 1, L, M, l, r);
        int64_t second_child = getSummary(2 * v + 2, M, R, l, r);
        return first_child + second_child;
    }
private:
    int *array;
protected:
    void buildSumTree(int v, int L, int R, int *a)
    {
        if (L == R - 1) // Условие выхода
        {
            if (L < length) // Поскольку мы объявляем большую размерность, необходимо следить за границей
            {
                array[v] = a[L];
            }
            return; // Присвоили, возвращаемся
        }
        int M = (L + R) / 2; // Выбираем середину отрезка [L..R]
        buildSumTree(2 * v + 1, L, M, a); // Запускаем сумму для левого потомка
        buildSumTree(2 * v + 2, M, R, a); // И для правого
        array[v] = array[2 * v + 1] + array[2 * v + 2]; // Обновляем текущую вершину
    }
    
    int getHead(int *arr)
    {
        if(length != 0)
        {
            return arr[0];
        }
        return NULL;
    }
    
    int getTail(int *arr)
    {
        if(length != 0)
        {
            return arr[length-1];
        }
        return NULL;
    }
};

void segmentTree::print()
{
    for(int i = 0; i < length; i++)
    {
        cout << segmentTree::array[i] << endl;
    }
}

int main()
{
    int arr[8] = {2,3,7,6,5,4,2,8};
    segmentTree a(arr, 8);
//    a.print();
}
