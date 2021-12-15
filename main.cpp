#include <iostream>
#include <vector>
#include <math.h>
#define clg(N) \
cout << N << endl;

using namespace std;

class Fraction {
    private:
        // Calculates the greates common divisor with
        // Euclid's algorithm
        // both arguments have to be positive
        long long gcd(long long a, long long b) {
            while (a != b) {
                if (a > b) {
                    a -= b;
                } else {
                    b -= a;
                }
            }
            return a;
        }

    public:
        long long numerator, denominator;

        Fraction() {
            numerator = 0;
            denominator = 1;
        }

        Fraction(long long n, long long d) {
            if (d==0) {
                cerr << "Denominator may not be 0." << endl;
                exit(0);
            } else if (n == 0) {
                numerator = 0;
                denominator = 1;
            } else {
                int sign = 1;
                if (n < 0) {
                    sign *= -1;
                    n *= -1;
                }
                if (d < 0) {
                    sign *= -1;
                    d *= -1;
                }

                long long tmp = gcd(n, d);
                numerator = n/tmp*sign;
                denominator = d/tmp;
            }
        }

        operator int() {return (numerator)/denominator;}
        operator float() {return ((float)numerator)/denominator;}
        operator double() {return ((double)numerator)/denominator;}
};

Fraction operator+(const Fraction& lhs, const Fraction& rhs) {
    Fraction tmp(lhs.numerator*rhs.denominator
                +rhs.numerator*lhs.denominator,
                lhs.denominator*rhs.denominator);
    return tmp;
}

Fraction operator+=(Fraction& lhs, const Fraction& rhs) {
    Fraction tmp(lhs.numerator*rhs.denominator
                +rhs.numerator*lhs.denominator,
                lhs.denominator*rhs.denominator);
    lhs = tmp;
    return lhs;
}

Fraction operator-(const Fraction& lhs, const Fraction& rhs) {
    Fraction tmp(lhs.numerator*rhs.denominator
                -rhs.numerator*lhs.denominator,
                lhs.denominator*rhs.denominator);
    return tmp;
}

Fraction operator-=(Fraction& lhs, const Fraction& rhs) {
    Fraction tmp(lhs.numerator*rhs.denominator
                -rhs.numerator*lhs.denominator,
                lhs.denominator*rhs.denominator);
    lhs = tmp;
    return lhs;
}

Fraction operator*(const Fraction& lhs, const Fraction& rhs) {
    Fraction tmp(lhs.numerator*rhs.numerator,
               lhs.denominator*rhs.denominator);
    return tmp;
}

Fraction operator*=(Fraction& lhs, const Fraction& rhs) {
    Fraction tmp(lhs.numerator*rhs.numerator,
               lhs.denominator*rhs.denominator);
    lhs = tmp;
    return lhs;
}

Fraction operator*(int lhs, const Fraction& rhs) {
    Fraction tmp(lhs*rhs.numerator,rhs.denominator);
    return tmp;
}

Fraction operator*(const Fraction& rhs, int lhs) {
    Fraction tmp(lhs*rhs.numerator,rhs.denominator);
    return tmp;
}

Fraction operator/(const Fraction& lhs, const Fraction& rhs) {
    Fraction tmp(lhs.numerator*rhs.denominator,
                 lhs.denominator*rhs.numerator);
    return tmp;
}

std::ostream& operator<<(std::ostream &strm, const Fraction &a) {
    if (a.denominator == 1) {
        strm << a.numerator;
    } else {
        strm << a.numerator << "/" << a.denominator;
    }
    return strm;
}


template <typename T>
class segmentTree
{
public:
    unsigned long length;
    
    segmentTree(vector<T> vec)
    {
        length = vec.size();
        originalTree = vec;
        buildedTree = vec;
        convertToEmptyVector();
        convertToEmptyVectorByIndex();
    }
    
    void buildTree(T currentIndex, T leftQueryBorder, T rightQueryBorder)
    {
        if(leftQueryBorder == rightQueryBorder - 1)
        {
            if(leftQueryBorder < rightQueryBorder)
                buildedTree[currentIndex] = originalTree[leftQueryBorder];
            return;
        }
        else
        {
            long long half = floor((rightQueryBorder + leftQueryBorder) / 2);
            buildTree(2 * currentIndex + 1, leftQueryBorder, half);
            buildTree(2 * currentIndex + 2, half, rightQueryBorder);
//            cout << "Value = ";
            buildedTree[currentIndex] = buildedTree[2 * currentIndex + 1] + buildedTree[2 * currentIndex + 2];
//            cout << buildedTree[currentIndex] << endl;
        }
    }
    
    void push(T currentIndex, T leftQuery, T rightQuery)
    {
        if(2 * currentIndex + 1 < changes.size() && changes[currentIndex] != 0)
        {
            changes[currentIndex * 2] = changes[currentIndex * 2 + 1] = changes[currentIndex];
            changes[currentIndex] = 0;
            T tm = (leftQuery + rightQuery) / 2;
            buildedTree[currentIndex * 2] = (tm - leftQuery + 1) * changes[currentIndex * 2];
            buildedTree[currentIndex * 2 + 1] = (rightQuery - tm) * changes[currentIndex * 2 + 1];
        }
    }
    
    T getSum(T currentIndex, T leftQueryBorder, T rightQueryBorder)
    {
        push(currentIndex, leftQueryBorder, rightQueryBorder);
        if((buildedTree.size()-1) <= leftQueryBorder || rightQueryBorder <= 0)
            return 0;
        if(0 <= leftQueryBorder && rightQueryBorder <= buildedTree.size()-1)
            return buildedTree[currentIndex];
        T half = floor((leftQueryBorder + rightQueryBorder) / 2);
        
        firstChild = getSum(2*currentIndex+1, leftQueryBorder, half);
        secondChild = getSum(2*currentIndex+2, half, rightQueryBorder);
        
        return firstChild + secondChild;
    }
    
    T findValue(T currentIndex, T leftQueryBorder, T rightQueryBorder)
    {
        if(leftQueryBorder < 0 || rightQueryBorder > buildedTree.size()-1)
        {
            cout << "Ended with code: 0";
            return 0;
        }
        push(currentIndex, leftQueryBorder, rightQueryBorder);
        if(leftQueryBorder == rightQueryBorder - 1)
        {
            return buildedTree[currentIndex];
        }
        T half = floor((leftQueryBorder + rightQueryBorder) / 2);
        firstChild += findValue(2*currentIndex+1, leftQueryBorder, half);
        secondChild += findValue(2*currentIndex+2, half, rightQueryBorder);
        return sum += firstChild + secondChild;
    }
    
    void setWithRecount(T currentIndex, T leftQueryBorder, T rightQueryBorder, T indexOfElementToChange, T value)
    {
        if(leftQueryBorder == rightQueryBorder - 1)
        {
            buildedTree[currentIndex] = value;
            originalTree[currentIndex] = value;
            return;
        }
        T half = floor((leftQueryBorder + rightQueryBorder) / 2);
        if(indexOfElementToChange < half) setWithRecount(2*currentIndex+1, leftQueryBorder, half, indexOfElementToChange, value);
        else setWithRecount(2*currentIndex+2, half, rightQueryBorder, indexOfElementToChange, value);
        buildedTree[currentIndex] = buildedTree[2*currentIndex+2] + buildedTree[2*currentIndex+1];
    }
    
    void changeRange(T currentIndex,  T leftQueryBorder, T rightQueryBorder, T value)
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
         T tm = floor((leftQueryBorder + rightQueryBorder) / 2);
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
    T firstChild = 0;
    T secondChild = 0;
    T sum = 0;
protected:
    
    T convertToEmptyVector()
    {
        for (typename vector<T>::iterator it = buildedTree.begin(); it != buildedTree.end(); it++)
            *it = 0;
        return 0;
    }
    
    T convertToEmptyVectorByIndex()
    { //((std::is_same<T, int>::value) ? 11 : 10)
        for (long i = 0; i < originalTree.size()*8; ++i)
            changes.push_back(0);
        return 0;
    }
};

int main()
{
    cout << endl << "------------ INTEGER ------------" << endl << endl;
    vector<int> vec1 = {1,2,3,4,5};
    segmentTree<int> a(vec1);
    a.buildTree(0, 0, vec1.size());
    a.print();
    cout << "Sum = " << a.getSum(0, 1, 1) << endl;
    cout << "Recount: " << endl;
    a.setWithRecount(1, 3, 5, 4, 243);
    a.print();
    cout << endl << "changeRange: " << endl;
    a.changeRange(1, 3, 3, 68);
    a.print();
    cout << endl << "SumFindValue = " << a.findValue(0, 2, 4) << endl << endl;
    cout << endl << "Recount integer 2: " << endl;
    a.setWithRecount(1, 3, 5, 3, 243.66);
    a.print();
    
    // double
    cout << endl << "------------ DOUBLE ------------" << endl << endl;
    
    vector<double> vec2 = {1.3, 2.4, 3.99, 42.1, 5.8};
    segmentTree<double> b(vec2);
    b.buildTree(0, 0, vec2.size());
    b.print();
    cout << "Sum double = " << b.getSum(0, 3, 4) << endl;
    cout << "Recount double: " << endl;
    b.setWithRecount(1, 3, 5, 3, 243.66);
    b.print();
    cout << "changeRange double: " << endl;
    b.changeRange(1, 3, 3, 68);
    b.print();
    cout << endl << "Recount double 2: " << endl;
    b.setWithRecount(1, 3, 5, 3, 243.66);
    b.print();
    
    cout << endl << "------------ float ------------" << endl << endl;
    
    vector<float> vec3 = {1.331, 2.45, 3.991312, 42.1312, 5.8313};
    segmentTree<float> c(vec3);
    c.buildTree(0, 0, vec3.size());
    c.print();
    cout << "Sum double = " << c.getSum(0, 3, 4) << endl;
    cout << "Recount double: " << endl;
    c.setWithRecount(1, 3, 5, 3, 87613213);
    c.print();
    cout << "changeRange double: " << endl;
    c.changeRange(1, 3, 3, 68);
    c.print();
    cout << endl << "Recount double 2: " << endl;
    c.setWithRecount(1, 3, 5, 3, 243.66);
    c.print();
    
    cout << endl << "------------ Fractions ------------" << endl << endl;
    
//    vector<Fraction> vec4 = {};
//    for(int i = 0; i < 5; i++)
//    {
//        vec4.push_back(Fraction (i+1, i+3));
//    }
//    segmentTree<Fraction> d(vec4);
//    d.buildTree(0, 0, vec3.size());
//    d.print();
//    cout << "Sum double = " << d.getSum(0, 3, 4) << endl;
//    cout << "Recount double: " << endl;
//    d.setWithRecount(1, 3, 5, 3, 87613213);
//    d.print();
//    cout << "changeRange double: " << endl;
//    d.changeRange(1, 3, 3, 68);
//    d.print();
//    cout << endl << "Recount double 2: " << endl;
//    d.setWithRecount(1, 3, 5, 3, 243.66);
//    d.print();
    
//    cout << endl << "------------ __int64 ------------" << endl << endl;
//
//    vector<__int64_t> vec4 = {1, 2, 4};
//    segmentTree<__int64_t> d(vec4);
//    d.buildTree(0, 0, vec4.size());
//    d.print();
//    cout << "Sum double = " << d.getSum(0, 3, 4) << endl;
//    cout << "Recount double: " << endl;
//    d.setWithRecount(1, 3, 5, 3, 87613213);
//    d.print();
//    cout << "changeRange double: " << endl;
//    d.changeRange(1, 3, 3, 68);
//    d.print();
//    cout << endl << "Recount double 2: " << endl;
//    d.setWithRecount(1, 3, 5, 3, 243.66);
//    d.print();
    return 0;
}
