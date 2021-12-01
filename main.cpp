#include <iostream>
#include <vector>
#define clg(N) \
cout << N << endl;

using namespace std;

template <typename T>
class segmentTree
{
public:
    int length;
    segmentTree(vector<T> vec)
    {
        length = vec.size();
        originalTree = vec;
        buildedTree = vec;
        convertToEmptyVector();
        convertToEmptyVectorByIndex();
    }
    void buildTree(int currentIndex, int leftQueryBorder, int rightQueryBorder)
    {
        if(leftQueryBorder == rightQueryBorder - 1)
        {
            if(leftQueryBorder < rightQueryBorder)
                buildedTree[currentIndex] = originalTree[leftQueryBorder];
            return;
        }
        else
        {
            buildTree(2 * currentIndex + 1, leftQueryBorder, (rightQueryBorder + leftQueryBorder) / 2);
            buildTree(2 * currentIndex + 2, (rightQueryBorder + leftQueryBorder) / 2, rightQueryBorder);
            cout << "Value = ";
            clg(buildedTree[2 * currentIndex + 1] + buildedTree[2 * currentIndex + 2]);
            buildedTree[currentIndex] = buildedTree[2 * currentIndex + 1] + buildedTree[2 * currentIndex + 2];
        }
    }
    T getSum(int currentIndex, int leftQueryBorder, int rightQueryBorder)
    {
        push(currentIndex, leftQueryBorder, rightQueryBorder);
        if((buildedTree.size()-1) <= leftQueryBorder || rightQueryBorder <= 0) return 0;
        if(0 <= leftQueryBorder && rightQueryBorder <= buildedTree.size()-1) return buildedTree[currentIndex];
        int half = (leftQueryBorder + rightQueryBorder) / 2;
        T firstChild = getSum(2*currentIndex+1, leftQueryBorder, half);
        T secondChild = getSum(2*currentIndex+2, half, rightQueryBorder);
        return firstChild + secondChild;
    }
    T setWithRecount(int currentIndex, int leftQueryBorder, int rightQueryBorder, int indexOfElementToChange, T value)
    {
        if(leftQueryBorder == rightQueryBorder - 1)
        {
            buildedTree[currentIndex] = value;
            originalTree[currentIndex] = value;
            return 0;
        }
        int half = (leftQueryBorder + rightQueryBorder) / 2;
        if(indexOfElementToChange < half) setWithRecount(2*currentIndex+1, leftQueryBorder, half, indexOfElementToChange, value);
        else setWithRecount(2*currentIndex+2, half, rightQueryBorder, indexOfElementToChange, value);
        buildedTree[currentIndex] = buildedTree[2*currentIndex+1] + buildedTree[2*currentIndex+2];
        return 0;
    }
//    T push(int currentIndex,  int leftQueryBorder, int rightQueryBorder)
//    {
//        if(leftQueryBorder != rightQueryBorder - 1)
//        {
//            changes[2*currentIndex+1] = changes[currentIndex];
//            changes[2*currentIndex+2] = changes[currentIndex];
//        }
//        buildedTree[currentIndex] = changes[currentIndex] * (rightQueryBorder-leftQueryBorder);
//        changes[currentIndex] = 0;
//        return 0;
//    }
    void push(int currentIndex, int leftQuery, int rightQuery)
    {
        if(2 * currentIndex + 1 < changes.size() && changes[currentIndex] != 0)
        {
            changes[currentIndex * 2] = changes[currentIndex * 2 + 1] = changes[currentIndex];
            changes[currentIndex] = 0;
            int tm = (leftQuery + rightQuery) / 2;
            buildedTree[currentIndex * 2] = (tm - leftQuery + 1) * changes[currentIndex * 2];
            buildedTree[currentIndex * 2 + 1] = (rightQuery - tm) * changes[currentIndex * 2 + 1];
        }
    }
    void changeRange(int currentIndex,  int leftQueryBorder, int rightQueryBorder, T value)
    {
        //вариант 1
         if (0 <= leftQueryBorder && rightQueryBorder <= buildedTree.size()-1) {
//             buildedTree[currentIndex] = value * (rightQueryBorder - leftQueryBorder + 1);
             buildedTree[currentIndex] = value;
             changes[currentIndex] = value;   //сохраняем несогласованность
             return;
         }

         //вариант 2
         if (rightQueryBorder < 0 || buildedTree.size()-1 < leftQueryBorder) {
             return;
         }

         //вариант 3
         push(currentIndex, leftQueryBorder, rightQueryBorder);    //Проталкиваем модификации перед каждым разделением запроса!
         int tm = (leftQueryBorder + rightQueryBorder) / 2;
         changeRange(currentIndex * 2,     leftQueryBorder, tm, value);
         changeRange(currentIndex * 2 + 1, tm + 1, rightQueryBorder, value);
         buildedTree[currentIndex] = buildedTree[currentIndex * 2] + buildedTree[currentIndex * 2 + 1];
    }
    T print()
    {
        for(typename vector<T>::iterator it = buildedTree.begin(); it != buildedTree.end(); it++)
            cout << *it << endl;
        return 0;
    }
private:
    vector<T> originalTree;
    vector<T> buildedTree;
    vector<T> changes;
protected:
    T convertToEmptyVector()
    {
        for (typename vector<T>::iterator it = buildedTree.begin(); it != buildedTree.end(); it++)
            *it = 0;
        return 0;
    }
    T convertToEmptyVectorByIndex()
    {
        for (int i = 0; i < originalTree.size()*3; ++i)
            changes.push_back(0);
        return 0;
    }
};

int main()
{
    vector<int> vec1 = {1,2,3,4,5};
    segmentTree<int> a(vec1);
    a.buildTree(0, 0, vec1.size());
    a.print();
    cout << "Sum = " << a.getSum(0, 1, 1) << endl;
    cout << "Recount: " << endl;
    a.setWithRecount(1, 3, 5, 4, 243);
    a.print();
    cout << "changeRange: " << endl;
    a.changeRange(1, 3, 5, 68);
    a.print();
}
