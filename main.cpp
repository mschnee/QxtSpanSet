/******************************************************************************
 * Copyright (c) 2011, Matthew Schnee <matthew.schnee@gmail.com>
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
#include <QCoreApplication>
#include <QtCore>
#include <QWidget>
#include <QString>
#include <QTextStream>
#include "qxtspanset.h"

QString toString(const QUInt32Span &s) {
    return QString("("+QString::number(s.A)+","+QString::number(s.B)+")");
}

QString toString(const QUInt32SpanSet &set) {
    QString result;

    foreach (QUInt32Span s,set.constData()) {
        result.append(toString(s)).append(" ");
    }
    return result;
}

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    QTextStream out(stdout);
#ifdef QT_4
    QUInt32Span spanA = {100,200};
    QUInt32Span spanB = {1000,2000};
    QUInt32SpanSet set = { spanA,spanB };
    QUInt32SpanSet set2 = { {10,20},{100,200}, {1000,3000} };
#else
    UIntSpan spanA = {100,200};
    UIntSpan spanB = {1000,2000};
    UIntSpanSet set = { spanA,spanB };
    UIntSpanSet set2 = { {10,20},{100,200}, {1000,3000} };
#endif

    out << toString(set) << endl << toString(set2) << endl;
    //return a.exec();
}
