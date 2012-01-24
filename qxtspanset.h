/******************************************************************************
 * Copyright (c) 2011, Matthew Schnee <HowAboutTau@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the <organization> nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

/******************************************************************************
 *  QxtSpan/QxtSpanSet written to conform to, and use, C++11
 *  If your compiler does not support C++11, it will not compile.
 *
 *  C++11 is the official name for what was called C++0x, presumably because
 *  the standard wasn't finalized until 2011.  C++0x and C++11 are used
 *  interchangibly in most documents referencing the standard.
 *
 *  QxtSpan/QxtSpanSet uses lambda and initializer lists.
 *
 *  GCC:
 *  GCC >= 4.5 (lambdas not available in GCC 4.4)
 *  CXXFLAGS += -std=c++0x
 *
 *  Microsoft Visual Studio / cl / nmake :
 *  Automatically supported in Visual C++ 2010
 ******************************************************************************/
#ifndef QXTSPANSET_H
#define QXTSPANSET_H
#include <algorithm>
#include <vector>

/** forward declarations **/



/**
 *  Just a basic pair structure.  Nothing fancy.
 */
template<class T> struct QxtSpan {
    QxtSpan() {
        A = B = T();
    }

    /**
     *  Constructor.  All spans are (A,B)[A<=B]
     */
    QxtSpan(const T &a, const T &b) {
        if(a>b) {
            A = b;B = a;
        } else {
            A = a;B = b;
        }
    }

    // C++11/C++0x {T,T} inititlizer
    QxtSpan(std::initializer_list<T> l){
        if(l.size()>=2) {
            const T *iter = l.begin();
            A = *(iter);
            iter++;
            B= *(iter);
        } else {
            A = B = T();
        }
    }

    inline bool operator<(const QxtSpan &other) const {
        return (A==other.A) ?  (B<other.B) :( A<other.A);
    }

    /**
     *  If we intersect, merge us
     *
     *  Me:        [-----]
     *  Span:         [----]
     *  Result:    [-------]
     */
    void merge(const QxtSpan &other) {
        if (!intersects(other)) {
            return;
        } else {
          A=min(A,other.A);
          B=max(B,other.B);
        }

    }


    /**
     *  Do I overlap this other span?
     *
     *  Me:        [-----]
     *  Span:         [----]
     *          or
     *  Me:           [-----]
     *  Span:      [----]
     */
    bool intersects(const QxtSpan &other) {
        return (B>=other.A && A<=other.B);
    }

    /**
     *  Do I completely contain this other span?
     *
     *  Me:        [-------------------]
     *  Span:         [----]
     */
    bool contains(const QxtSpan &other) {
        return (A<=other.A && B >= other.B);
    }

    /**
     * Am I identical to this other span?
     */
    bool operator==(const QxtSpan &other) const {
        return (A==other.A && B==other.B);
    }


    T A;
    T B;
};


/**
 When dealing with Spans, and Sets of spans, you will be faced with three kinds of comparisons:
    Span-to-span operations
    Set-to-span operations
    Set-to-set operations

    In all of these functions, the order of operations matters.  The first paramater, usually denoted by "A",
    is the predicate to test against.
    Set-to-set operations:

    does span B overlap span A?
    bool spansOverlap(A,B);
    A:    [----------]
    B:           [--------]

 */

/**
 *  _vector
 *  A wrapper class around std::vector to avoid problems with the Allocator template requirement.
 */
template<class T>
class _vector : public std::vector <T,std::allocator<T> > {};

/**
 *  A set of generic spans, using the storage mechanism of your choice.
 *  QVector has thus fare proven faster and more efficient than std::vector, or any
 *  Queue I've managed to come up with.  So, to be STL-alike, you can supply your own.
 *
 *  A QxtSpanSet contains a list of QxtSpans of your given primitive data type, and can
 *  perform comparison, check, merge, reduce, and other rationally boolean operations on them.
 *
 *  A QxtSpanSet can be initialized using C++2011/C++0x initializer lists, in the form:
 *  QxtSpanSet set = { {aStart,aEnd}, {bStart,bEnd}, {cStart,cEnd}... };
 *
 *  This is because \sa=QxtSpans can be initialized with initializer lists as well:
 *  QxtSpan span = {startPoint,endPoint}
 */
template<class T,
         template<class R> class Storage = _vector,
         void (SortFunction)(typename Storage<QxtSpan<T> >::iterator begin,typename Storage<QxtSpan<T> >::iterator end)=std::sort
         >
class QxtSpanSet
{

public:
    typedef QxtSpan<T> Span;
    typedef Storage<Span> Container;
    typedef QxtSpanSet<T,Storage> Set;

    inline void sort() {
        SortFunction(d.begin(),d.end());
    }

    QxtSpanSet();
    QxtSpanSet(std::initializer_list<Span> list) {
        for( const QxtSpan<T>* iter=list.begin(); iter != list.end(); iter++) {
            d.push_back(*(iter));
        }
    }

    /**
     *  Add a span to this span set.
     */
    void append(const T &item) { d.append(item); }

    /**
     *  Return all spans in this set that are contained in the given span.
     */
    QxtSpanSet containedIn(const QxtSpan<T> &other ) const {
        Set c,r;
        c.d = d;
        c.sort();

        bool flag = false;
        QxtSpan<T> current = c.d.pop_front();

        // iterate until we get to a span that contains.
        // append spans until the next span isn't contained.
        // break, because no more spans will be contained
        while(QxtSpan<T> span = c.d.first()) {
            if(other.contains(span)) {
                if(!flag) {
                    flag=true;
                }
                r.append(span);
            } else {
                if(flag) {
                    break;
                }
            }
        }
        return r;
    }

    /**
     *  Return all spans in this set that intersect the given span.
     */
    QxtSpanSet intersectedIn(const QxtSpan<T> &other ) const {
        Set c,r;
        c.d = d;
        c.sort();

        bool flag = false;
        Span current = c.d.pop_front();

        // iterate until we get to a span that intersects.
        // append spans until the next span isn't intersectes.
        // break, because no more spans will intersect
        while(Span span = c.d.first()) {
            if(other.intersects(span)) {
                if(!flag) {
                    flag=true;
                }
                r.append(span);
            } else {
                if(flag) {
                    break;
                }
            }
        }
        return r;
    }

    /**
     *  Creates one span set that merges any spans that intersect.
     * O(N log N)
     */
    QxtSpanSet mergeSpans(const Set &other) const {
       Set c,r;
        c.d = d + other.d;
        c.sort();

        // recursive version
        // return QxtSpanSet::rMergeSpans(d.pop_front(),d);
        Span current = c.d.pop_front();
        while(Span span = c.d.first()) {
            if(current.intersects(span)) {
                current = current.merge(span);
            } else {
                r.append(current);
                current = span;
            }
            d.pop_front();
        }
        r.append(current);
        return r;
    }

    /**
     *  Performes a recursive merge of a span with a set.
     *  Included as an example, as it's practically not as efficient
     *  as the sort/iterate version above.
     */
    static QxtSpanSet rMergeSpans(const Span& span,Set &other = Set()) {
        if(other.d.empty()) {
            other.append(span);
            return other;
        }

        if(span.intersects(other.d.first()))
            return rMergeSpans(span.merge(other.d.pop_front()),other);
        else {
            return rMergeSpans(other.d.pop_front(),other)+span;
        }
    }


    bool empty() const {
        return d.empty();
    }
    size_t length() const {
        return d.length();
    }

    /**
     * Identity operator
     *  Checks that this and the comparison have the same Sets, in order.
     */
    bool operator==(const Set &other) const {
        if(other.length()!=length())
            return false;
        Set* m = d.begin();
        Set* o = other.d.begin();
        for(int i = 0; i<length();i++){
            if(*(m)!=*(o))
                return false;
            m++; o++;
        }
        return true;
    }

    bool isIdenticalTo(const Set &other) const {
        return operator==(other);
    }

    /**
     *  Equality operator.
     *  Tests to see that every element in `other` exists in `this`, order irrelevant.
     */
    bool isEqualTo(const Set &other) {
        Set a(*(this));
        Set b(other);

        a.sort();
        b.sort();
        return a.isIdenticalTo(b);
    }

    QxtSpanSet &operator+(const Span &other) {
        d.append(other);
        return this;
    }

    const Container &constData() const { return d; }
    Container data() { return d; }
private:
    Container d;

};



#ifdef QT_4
#include <QtCore>

typedef QxtSpan<qint32> QInt32Span;
typedef QxtSpan<qint64> QInt64Span;
typedef QxtSpan<quint32> QUInt32Span;
typedef QxtSpan<quint64> QUInt64Span;

Q_DECLARE_TYPEINFO(QInt32Span,Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QInt64Span,Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QUInt32Span,Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QUInt64Span,Q_PRIMITIVE_TYPE);

typedef QxtSpanSet<qint32,QVector> QInt32SpanSet;
typedef QxtSpanSet<qint64,QVector> QInt64SpanSet;
typedef QxtSpanSet<quint32,QVector> QUInt32SpanSet;
typedef QxtSpanSet<quint64,QVector> QUInt64SpanSet;


/* and some functions for testing */
QString toString(const QUInt32Span &);
QString toString(const QUInt32SpanSet &);

#else
typedef QxtSpan<int> IntSpan;
typedef QxtSpan<uint> UIntSpan;
typedef QxtSpan<char> CharSpan;
//typedef QxtSpan<uchar> UCharSpan;
typedef QxtSpan<float> FloatSpan;
typedef QxtSpan<double> DoubleSpan;

typedef QxtSpanSet<int> IntSpanSet;
typedef QxtSpanSet<uint> UIntSpanSet;
typedef QxtSpanSet<char> CharSpanSet;
//typedef QxtSpanSet<uchar> IntSpanSet;
typedef QxtSpanSet<float> FloatSpanSet;
typedef QxtSpanSet<double> DoubleSpanSet;

 QString toString(const UIntSpan &);
 QString toString(const UIntSpanSet &);
#endif



#endif // QXTSPANSET_H
