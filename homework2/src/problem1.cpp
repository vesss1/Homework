#include <iostream>
#include <algorithm>
using namespace std;

class Polynomial;  // 前置宣告


class Term {
    friend class Polynomial;  
    friend ostream& operator<<(ostream&, const Polynomial&);
private:
    float coef; // 係數
    int exp;    // 次方
};


class Polynomial {
private:
    Term* termArray; 
    int capacity;    
    int terms;       
public:
    Polynomial();                      
    ~Polynomial();                     
    Polynomial(const Polynomial&);     // 拷貝建構子
    Polynomial& operator=(const Polynomial&);  // 指定運算子

    void newTerm(const float, const int);
    Polynomial Add(const Polynomial& b) const;
    friend istream& operator>>(istream&, Polynomial&);
    friend ostream& operator<<(ostream&, const Polynomial&);
};


Polynomial::Polynomial() {
    capacity = 2;
    terms = 0;
    termArray = new Term[capacity];
}


Polynomial::~Polynomial() {
    delete[] termArray;
}

// ===== 拷貝建構子 =====
Polynomial::Polynomial(const Polynomial& other) {
    capacity = other.capacity;
    terms = other.terms;
    termArray = new Term[capacity];
    copy(other.termArray, other.termArray + terms, termArray);
}


Polynomial& Polynomial::operator=(const Polynomial& other) {
    if (this == &other) return *this;
    delete[] termArray;
    capacity = other.capacity;
    terms = other.terms;
    termArray = new Term[capacity];
    copy(other.termArray, other.termArray + terms, termArray);
    return *this;
}


void Polynomial::newTerm(const float theCoef, const int theExp) {
    if (theCoef == 0) return;
    if (terms == capacity) {
        capacity *= 2;
        Term* temp = new Term[capacity];
        copy(termArray, termArray + terms, temp);
        delete[] termArray;
        termArray = temp;
    }
    termArray[terms].coef = theCoef;
    termArray[terms].exp = theExp;
    terms++;
}


istream& operator>>(istream& is, Polynomial& poly) {
    int n;
    is >> n;  // 輸入項數
    for (int i = 0; i < n; i++) {
        float c;
        int e;
        is >> c >> e;
        poly.newTerm(c, e);
    }
  
   
    return is;
}


ostream& operator<<(ostream& os, const Polynomial& poly) {
    for (int i = 0; i < poly.terms; i++) {
        if (i > 0 && poly.termArray[i].coef >= 0)
            os << "+";
        os << poly.termArray[i].coef << "X^" << poly.termArray[i].exp;
    }
    return os;
}


Polynomial Polynomial::Add(const Polynomial& b) const {
    Polynomial c;
    int aPos = 0, bPos = 0;
    while (aPos < terms && bPos < b.terms) {
        if (termArray[aPos].exp == b.termArray[bPos].exp) {
            float sum = termArray[aPos].coef + b.termArray[bPos].coef;
            if (sum != 0)
                c.newTerm(sum, termArray[aPos].exp);
            aPos++;
            bPos++;
        }
        else if (termArray[aPos].exp > b.termArray[bPos].exp) {
            c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }
        else {
            c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
            bPos++;
        }
    }
    while (aPos < terms)
        c.newTerm(termArray[aPos].coef, termArray[aPos++].exp);
    while (bPos < b.terms)
        c.newTerm(b.termArray[bPos].coef, b.termArray[bPos++].exp);

    return c;
}


int main() {
    Polynomial A, B, C;
    cin >> A;
    cin >> B;
    C = A.Add(B);
    cout << C << endl;
    return 0;
}
