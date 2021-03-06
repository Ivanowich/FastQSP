/* Copyright (C) 2005-2010 Valeriy Argunov (nporep AT mail DOT ru) */
/*
* This library is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "declarations.h"
#include <vector>

#ifndef QSP_TEXTDEFINES
#define QSP_TEXTDEFINES

#define QSP_STRSDELIM QSP_FMT("\r\n")
#define QSP_LSUBEX QSP_FMT("<<")
#define QSP_RSUBEX QSP_FMT(">>")

/* Helpers */
#define QSP_LEN(x) (sizeof(x) / sizeof(QSP_CHAR) - 1)

/* External functions */
int qspAddText(QSP_CHAR **, const QSP_CHAR*, int, int, QSP_BOOL);
QSP_CHAR *qspGetNewText(const QSP_CHAR*, int);
QSP_CHAR *qspGetAddText(QSP_CHAR *, QSP_CHAR *, int, int);
QSP_BOOL qspClearText(QSP_CHAR **, int *);
QSP_BOOL qspIsInList(const QSP_CHAR*, QSP_CHAR);
QSP_BOOL qspIsInListEOL(const QSP_CHAR*, QSP_CHAR);
QSP_BOOL qspIsDigit(QSP_CHAR);
const QSP_CHAR* qspSkipSpaces(const QSP_CHAR*);
QSP_CHAR *qspSkipSpaces(QSP_CHAR *s);
const QSP_CHAR* qspStrEnd(const QSP_CHAR*);
QSP_CHAR *qspDelSpc(QSP_CHAR *);
QSP_CHAR *qspDelSpcCanRetSelf(QSP_CHAR *);
QSP_BOOL qspIsAnyString(QSP_CHAR *);
void qspLowerStr(QSP_CHAR *);
void qspUpperStr(QSP_CHAR *);
int qspStrsNComp(const QSP_CHAR*, const QSP_CHAR*, int);
int qspStrsComp(const QSP_CHAR*, const QSP_CHAR*);
QSP_CHAR *qspStrCopy(QSP_CHAR *, const QSP_CHAR*);
QSP_CHAR *qspStrChar(QSP_CHAR *, QSP_CHAR);
QSP_CHAR *qspStrNCopy(QSP_CHAR*, const QSP_CHAR*, int);
int qspStrLen(const QSP_CHAR*);
const QSP_CHAR* qspStrStr(const QSP_CHAR*, const QSP_CHAR*);
QSP_CHAR *qspStrStr(QSP_CHAR *str, const QSP_CHAR *strSearch);
const QSP_CHAR* qspStrPBrk(const QSP_CHAR*, const QSP_CHAR*);
QSP_CHAR *qspInStrRChars(QSP_CHAR *, const QSP_CHAR*, QSP_CHAR *);
QSP_CHAR *qspJoinStrs(QSP_CHAR **, int, QSP_CHAR *);
int qspSplitStr(QSP_CHAR *, const QSP_CHAR*, QSP_CHAR ***, int);
void qspCopyStrs(QSP_CHAR ***, QSP_CHAR **, int, int);
QSP_BOOL qspIsNumber(QSP_CHAR *);
int qspStrToNum(QSP_CHAR *, QSP_BOOL *);
QSP_CHAR *qspNumToStr(QSP_CHAR *, int);
const QSP_CHAR* qspStrPos(const QSP_CHAR*, const QSP_CHAR*, QSP_BOOL);
QSP_CHAR *qspStrPos(QSP_CHAR *txt, const QSP_CHAR *str, QSP_BOOL isIsolated);
QSP_CHAR *qspStrPosPartial(QSP_CHAR *, QSP_CHAR *, const QSP_CHAR*, QSP_BOOL);
QSP_CHAR *qspReplaceText(QSP_CHAR *, const QSP_CHAR*, const QSP_CHAR*);
QSP_CHAR *qspFormatText(QSP_CHAR *, QSP_BOOL);

int qspAddText2(std::vector<QSP_CHAR> *dest, const QSP_CHAR *val, int valLen, QSP_BOOL isCreate);

template <typename CharType> void qspFreeStrs(CharType **strs, int count) {
  if (strs) {
    while (--count >= 0)
      free(strs[count]);
    free(strs);
  }
}

#ifdef _UNICODE
int qspToWLower(int);
int qspToWUpper(int);
#endif

#endif
