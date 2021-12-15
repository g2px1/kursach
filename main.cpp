#include <iostream>
#include <vector>
#include <math.h>
#define clg(N) \
cout << N << endl;

using namespace std;

class Fraction {
    private:
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
        double convertToDouble()
        {
            return (double)numerator/denominator;
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
        if((is_same<T, Fraction>::value))
        {
            cout << "You should use segmentFractionTree instead of segmentTree";
            return;
        }
        length = vec.size();
        originalTree = vec;
        buildedTree = vec;
        convertToEmptyVector();
        convertToEmptyVectorByIndex();
    }
    
    virtual void buildTree(T currentIndex, T leftQueryBorder, T rightQueryBorder)
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


    virtual void push(T currentIndex, T leftQuery, T rightQuery)
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

    virtual T getSum(T currentIndex, T leftQueryBorder, T rightQueryBorder)
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

    virtual T findValue(T currentIndex, T leftQueryBorder, T rightQueryBorder)
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

    virtual void setWithRecount(T currentIndex, T leftQueryBorder, T rightQueryBorder, T indexOfElementToChange, T value)
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

    virtual void changeRange(T currentIndex,  T leftQueryBorder, T rightQueryBorder, T value)
    {
         if (0 <= leftQueryBorder && rightQueryBorder <= buildedTree.size()-1) {
//             buildedTree[currentIndex] = value * (rightQueryBorder - leftQueryBorder + 1);
             buildedTree[currentIndex] = value;
             changes[currentIndex] = value;   //сохраняем несогласованность
             return;
         }

         if (rightQueryBorder < 0 || buildedTree.size()-1 < leftQueryBorder) {
             return;
         }
        
         push(currentIndex, leftQueryBorder, rightQueryBorder);
         T tm = floor((leftQueryBorder + rightQueryBorder) / 2);
         changeRange(currentIndex * 2,     leftQueryBorder, tm, value);
         changeRange(currentIndex * 2 + 1, tm + 1, rightQueryBorder, value);
         buildedTree[currentIndex] = buildedTree[currentIndex * 2] + buildedTree[currentIndex * 2 + 1];
    }

    void print()
    {
        for(typename vector<T>::iterator it = buildedTree.begin(); it != buildedTree.end(); it++)
            cout << *it << endl;
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

template<typename T>
class segmentFractionTree : public Fraction
{
public:
    int length;
    segmentFractionTree(vector<Fraction> a)
    {
        if(!(is_same<T, Fraction>::value))
        {
            cout << endl << "You should use segmentTree instead of segmentFractionTree" << endl;
            return;
        }
        length = a.size();
        originalTree = a;
        convertIntoDouble(a, buildedTree);
        convertToEmptyVector();
        convertToEmptyVectorByIndex();
    }
    virtual void buildTree(int currentIndex, int leftQueryBorder, int rightQueryBorder)
    {
        if(leftQueryBorder == rightQueryBorder - 1)
        {
            if(leftQueryBorder < rightQueryBorder)
                buildedTree[currentIndex] = originalTree[leftQueryBorder].convertToDouble();
            return;
        }
        else
        {
            int half = floor((rightQueryBorder + leftQueryBorder) / 2);
            buildTree(2 * currentIndex + 1, leftQueryBorder, half);
            buildTree(2 * currentIndex + 2, half, rightQueryBorder);
            buildedTree[currentIndex] = buildedTree[2 * currentIndex + 1] + buildedTree[2 * currentIndex + 2];
        }
    }
    
    virtual void push(int currentIndex, int leftQuery, int rightQuery)
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

    virtual double getSum(double currentIndex, double leftQueryBorder, double rightQueryBorder)
    {
        push(currentIndex, leftQueryBorder, rightQueryBorder);
        if((buildedTree.size()-1) <= leftQueryBorder || rightQueryBorder <= 0)
            return 0;
        if(0 <= leftQueryBorder && rightQueryBorder <= buildedTree.size()-1)
            return buildedTree[currentIndex];
        double half = floor((leftQueryBorder + rightQueryBorder) / 2);

        firstChild = getSum(2*currentIndex+1, leftQueryBorder, half);
        secondChild = getSum(2*currentIndex+2, half, rightQueryBorder);

        return firstChild + secondChild;
    }

    virtual double findValue(double currentIndex, double leftQueryBorder, double rightQueryBorder)
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
        double half = floor((leftQueryBorder + rightQueryBorder) / 2);
        firstChild += findValue(2*currentIndex+1, leftQueryBorder, half);
        secondChild += findValue(2*currentIndex+2, half, rightQueryBorder);
        return sum += firstChild + secondChild;
    }
    
    virtual void setWithRecount(int currentIndex, int leftQueryBorder, int rightQueryBorder, int indexOfElementToChange, Fraction value)
    {
        if(leftQueryBorder == rightQueryBorder - 1)
        {
            buildedTree[currentIndex] = value.convertToDouble();
            originalTree[currentIndex] = value;
            return;
        }
        int half = floor((leftQueryBorder + rightQueryBorder) / 2);
        if(indexOfElementToChange < half) setWithRecount(2*currentIndex+1, leftQueryBorder, half, indexOfElementToChange, value);
        else setWithRecount(2*currentIndex+2, half, rightQueryBorder, indexOfElementToChange, value);
        buildedTree[currentIndex] = buildedTree[2*currentIndex+2] + buildedTree[2*currentIndex+1];
    }

    virtual void changeRange(int currentIndex,  int leftQueryBorder, int rightQueryBorder, int value)
    {
         if (0 <= leftQueryBorder && rightQueryBorder <= buildedTree.size()-1) {
//             buildedTree[currentIndex] = value * (rightQueryBorder - leftQueryBorder + 1);
             buildedTree[currentIndex] = value;
             changes[currentIndex] = value;   //сохраняем несогласованность
             return;
         }

         if (rightQueryBorder < 0 || buildedTree.size()-1 < leftQueryBorder) {
             return;
         }
        
         push(currentIndex, leftQueryBorder, rightQueryBorder);
         int tm = floor((leftQueryBorder + rightQueryBorder) / 2);
         changeRange(currentIndex * 2,     leftQueryBorder, tm, value);
         changeRange(currentIndex * 2 + 1, tm + 1, rightQueryBorder, value);
         buildedTree[currentIndex] = buildedTree[currentIndex * 2] + buildedTree[currentIndex * 2 + 1];
    }
    
    void printInDouble()
    {
        for(typename vector<double>::iterator it = buildedTree.begin(); it != buildedTree.end(); it++)
            cout << *it << endl;
    }
    
    void printInFractions()
    {
        for(typename vector<double>::iterator it = buildedTree.begin(); it != buildedTree.end(); it++)
            decimalToFraction(*it);
    }
    
private:
    vector<Fraction> originalTree;
    vector<double> buildedTree;
    vector<double> changes;
    double firstChild = 0;
    double secondChild = 0;
    double sum = 0;
protected:
    void convertToEmptyVector()
    {
        for (typename vector<double>::iterator it = buildedTree.begin(); it != buildedTree.end(); it++)
            *it = Fraction(0,1);
    }

    void convertToEmptyVectorByIndex()
    {
        for (long i = 0; i < originalTree.size()*20; ++i)
            changes.push_back(0);
    }
    void convertIntoDouble(vector<Fraction>& a, vector<double>& b)
    {
        for (int i = 0; i < a.size(); i++)
        {
            b.push_back(a[i]);
        }
    }
    long long gcd(long long a, long long b)
    {
        if (a == 0)
            return b;
        else if (b == 0)
            return a;
        if (a < b)
            return gcd(a, b % a);
        else
            return gcd(b, a % b);
    }
     
    // Function to convert decimal to fraction
    void decimalToFraction(double number)
    {
        // Fetch integral value of the decimal
        double intVal = floor(number);
     
        // Fetch fractional part of the decimal
        double fVal = number - intVal;
     
        // Consider precision value to
        // convert fractional part to
        // integral equivalent
        const long pVal = 1000000000;
     
        // Calculate GCD of integral
        // equivalent of fractional
        // part and precision value
        long long gcdVal = gcd(round(fVal * pVal), pVal);
     
        // Calculate num and deno
        long long num
            = round(fVal * pVal) / gcdVal;
        long long deno = pVal / gcdVal;
     
        // Print the fraction
        cout << (intVal * deno) + num
             << "/" << deno << endl;
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
    
    cout << endl << "------------ Fractions(double) ------------" << endl << endl;
    vector<double> vec4 = {};
    for(int i = 0; i < 5; i++)
    {
        vec4.push_back(Fraction(i+1, i+3).convertToDouble());
    }
    segmentTree<double> d(vec4);
    d.buildTree(0, 0, vec4.size());
    d.print();
    cout << "Sum double = " << d.getSum(0, 3, 4) << endl;
    cout << "Recount double: " << endl;
    d.setWithRecount(1, 3, 5, 3, 87613213);
    d.print();
    cout << "changeRange double: " << endl;
    d.changeRange(1, 3, 3, 68);
    d.print();
    cout << endl << "Recount double 3: " << endl;
    d.setWithRecount(1, 3, 5, 3, 243.66);
    d.print();

    cout << endl << "------------ Fractions ------------" << endl << endl;
    vector<Fraction> vec5;
    for(int i = 0; i < 5; i++)
    {
        vec5.push_back(Fraction(i+5, i+1));
    }

    segmentFractionTree<Fraction> e(vec5);
    e.buildTree(0, 0, vec5.size());
    e.printInDouble();
    cout << endl << "Print in fractions: " << endl;
    e.printInFractions();
    cout << "Sum double = " << e.getSum(0, 3, 4) << endl;
    cout << "Recount double: " << endl;
    e.setWithRecount(1, 3, 5, 3, Fraction(1321,24));
    e.printInFractions();
    cout << "changeRange double: " << endl;
    e.changeRange(1, 3, 3, 68);
    e.printInFractions();
    cout << endl << "Recount double 3: " << endl;
    e.setWithRecount(1, 3, 5, 3, Fraction(243.66, 1321));
    e.printInFractions();
}
