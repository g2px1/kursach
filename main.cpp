#include <iostream>
#include <vector>
#define clg(N) \
cout << N << endl;


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
        buildSumTree(0, 0, length, sumed);
        sumed.pop_back();
        for(int i = 0; i < arr.size()*3; i++)
        {
            modifications.push_back(0);
        }
    }
    ~segmentTree(){}
    void print();
    void printSum();
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
        cout << "value = ";
        clg(sumed[2 * v + 1] + sumed[2 * v + 2]);
        cout << "Get by: " << sumed[2 * v + 1] << ", " << sumed[2 * v + 2] << endl << endl;
        sumed[v] = sumed[2 * v + 1] + sumed[2 * v + 2]; // Обновляем текущую вершину
    }
//    void change(int left, int right, vector<int> ch)
//    {
//        if((left > array.size()-1 || right > array.size()-1) && left > 0 && right > 0 && ch.size() > 0)
//        {
//            int count = 0;
//            for(vector<int>::iterator it = array.begin(); *it != array[right+1]; it++)
//            {
//                *it = ch[count];
//                count++;
//            }
//        }
//        else
//        {
//            return;
//        }
//    }

//    int getSum(int l, int r, int v, int left, int right) {
    int getSum( int v, int left, int right) {
        //вариант 1
        if (0 <= left && right <= sumed.size()) {
            return sumed[v];
        }

        //вариант 2
        if (right < 0 || sumed.size() < left) {
            return 0;
        }

        //вариант 3
        push(v, left, right);    //Проталкиваем модификации перед каждым разделением запроса!
        int tm = (left + right) / 2;
//        return getSum(l, r, v * 2,     left,     tm)
//             + getSum(l, r, v * 2 + 1, tm + 1, right);
        return getSum(v * 2,     left,     tm)
             + getSum(v * 2 + 1, tm + 1, right);
    }
//    void assign(int l, int r, int val, int v, int left, int right) {
    void assign(int val, int v, int left, int right) {
        //вариант 1
        if (0 <= left && right <= sumed.size()) {
            sumed[v] = val * (right - left + 1);
            modifications[v] = val;   //сохраняем несогласованность
            return;
        }

        //вариант 2
        if (right < 0 || sumed.size() < left)
        {
            return;
        }

        //вариант 3
        push(v, left, right);    //Проталкиваем модификации перед каждым разделением запроса!
        int tm = (left + right) / 2;
//        assign(0, sumed.size(), val, v * 2,     left,     tm);
//        assign(0, sumed.size(), val, v * 2 + 1, tm + 1, right);
        assign(val, v * 2,     left,     tm);
        assign(val, v * 2 + 1, tm + 1, right);
        sumed[v] = sumed[v * 2] + sumed[v * 2 + 1];
    }

private:
    vector<int> array;
    vector<int> sumed;
    vector<int> modifications;
//    void push(int v, int tl, int tr) {
//        if (modifications[v] != 0 && v * 2 + 1 < 400004) {    //если есть что и куда проталкивать
//
//            //проталкиваем несогласованность
//            modifications[v * 2] = modifications[v * 2 + 1] = modifications[v];
//            modifications[v] = 0;
//
//            //и пересчитываем значения
//            int tm = (tl + tr) / 2;
//            sumed[v * 2] = (tm - tl + 1) * modifications[v * 2];
//            sumed[v * 2 + 1] = (tr - tm) * modifications[v * 2 + 1];
//        }
//    }
    void push(int v, int left, int right) {
        if((left > array.size()-1 || right > array.size()-1) && left > 0 && right > 0)
        {
            if (modifications[v] != 0 && v * 2 + 1 < sumed.size())
            {    //если есть что и куда проталкивать

                //проталкиваем несогласованность
                modifications[v * 2] = modifications[v * 2 + 1] = modifications[v];
                modifications[v] = 0;

                //и пересчитываем значения
                int tm = (left + right) / 2;
                sumed[v * 2] = (tm - left + 1) * modifications[v * 2];
                sumed[v * 2 + 1] = (right - tm) * modifications[v * 2 + 1];
            }
        }
    }
protected:
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
    vector<int> arr = {1,2,3,4,5,6};
    segmentTree a(arr);
//    a.print();
    cout << endl;
    a.printSum();
    cout << endl;
    vector<int> b = {4,3,29,54};
//    a.change(0, 21, b);
    cout << "Before change: " << endl;
    a.printSum();
    a.assign(254, 0, 0, 0);
    cout << "After change: " << endl;
    a.printSum();
}

