/*
 *  $BJ?@.(B9$BG/(B $BEE;R5;=QAm9g8&5f=j(B $B?"<G=SIW(B $BCx:n8"=jM-(B
 *
 *  $BCx:n<T$K$h$k5v2D$J$7$K$3$N%W%m%0%i%`$NBh;0<T$X$N3+<(!"J#@=!"2~JQ!"(B
 *  $B;HMQEy$=$NB>$NCx:n?M3J8"$r?/32$9$k9T0Y$r6X;_$7$^$9!#(B
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!"Cx:n<T$O@UG$(B
 *  $B$rIi$$$^$;$s!#(B 
 *
 *
 *  Copyright 1996
 *  Toshio UESHIBA, Electrotechnical Laboratory
 *
 *  All rights reserved.
 *  Any changing, copying or giving information about source programs of
 *  any part of this software and/or documentation without permission of the
 *  authors are prohibited.
 *
 *  No Warranty.
 *  Authors are not responsible for any damage in use of this program.
 */

/*
 *  $Id$
 */
#include "TU/Calib++.h"
#include <fstream>
#include <string>

namespace TU
{
Matrix<double>
get_Cdata(const char* filename, const char* suffix, u_int ncol, u_int nrow_min)
{
    using namespace	std;
	
    if (filename == 0)
    {
	cerr << "get_Cdata: filename must be specified!!" << endl;
	exit(1);
    }
    string	s(filename);
    if (suffix != 0)
	s += suffix;
    
    ifstream	in(s.c_str());
    if (in.fail())
    {
	cerr << "get_Cdata: <" << s << "> cannot be opened!!"
	     << endl;
	exit(1);
    }
    Matrix<double>	tmp;
    in >> tmp;

    if (tmp.nrow() < nrow_min || tmp.ncol() < ncol)
    {
	cerr << '<' << s << ">: illegal size of input data ("
	     << tmp.nrow() << 'x' << tmp.ncol() << ")!!"
	     << endl;
	exit(1);
    }
    
    Matrix<double>	Cdata(tmp.nrow(), ncol+1);
    const int		j = tmp.ncol() - ncol;
    for (int i = 0; i < Cdata.nrow(); ++i)
    {
	Cdata[i](0, ncol) = tmp[i](j, ncol);
	Cdata[i][ncol] = 1.0;
    }

    return Cdata;
}

Matrix<double>
get_full_Cdata(const char* filename, const char* suffix, u_int nrow_min)
{
    using namespace	std;
    Matrix<double>	tmp;

    if (filename == 0)
	cin >> tmp;
    else
    {
	string	s(filename);
	if (suffix != 0)
	    s += suffix;
    
	ifstream	in(s.c_str());
	if (in.fail())
	{
	    cerr << "get_full_Cdata: <" << s << "> cannot be opened!!"
		 << endl;
	    exit(1);
	}
	in >> tmp;
    }
    
    if (tmp.nrow() < nrow_min || tmp.ncol() != 5)
    {
	cerr << "get_full_Cdata: illegal size of input data ("
	     << tmp.nrow() << 'x' << tmp.ncol() << ")!!"
	     << endl;
	exit(1);
    }
    
    Matrix<double>	Cdata(tmp.nrow(), 7);
    for (int i = 0; i < Cdata.nrow(); ++i)
    {
	Cdata[i](0, 3) = tmp[i](0, 3);
	Cdata[i][3] = 1.0;
	Cdata[i](4, 2) = tmp[i](3, 2);
	Cdata[i][6] = 1.0;
    }

    return Cdata;
}

Matrix<double>
get_matrix(const char* filename, const char* suffix)
{
    using namespace	std;	
    Matrix<double>	m;
    
    if (filename == 0)
	cin >> m;
    else
    {
	string	s(filename);
	if (suffix != 0)
	    s += suffix;
	ifstream	in(s.c_str());
	if (in.fail())
	{
	    cerr << "get_matrix: <" << s << "> cannot be opened!!" << endl;
	    exit(1);
	}
	in >> m;
    }

    return m;
}

void
put_matrix(const char* filename, const char* suffix, const Matrix<double>& m)
{
    using namespace	std;
	
    if (filename == 0)
	cout << m;
    else
    {
	string	s(filename);
	if (suffix != 0)
	    s += suffix;
	ofstream	out(s.c_str());
	if (out.fail())
	{
	    cerr << "put_matrix: <" << s << "> cannot be opened!!" << endl;
	    exit(1);
	}
	out << m;
    }
}

Matrix<double>
get_HomogeneousMatrix(const char* filename, const char* suffix)
{
    Matrix<double>	tmp = get_matrix(filename, suffix);
    u_int		npoints = tmp.nrow(), nviews = tmp.ncol() / 2;
    Matrix<double>	m(npoints, 3 * nviews);

    for (u_int i = 0; i < npoints; ++i)
	for (u_int j = 0; j < nviews; ++j)
	{
	    m[i][3*j]	  = tmp[i][2*j];
	    m[i][3*j + 1] = tmp[i][2*j + 1];
	    m[i][3*j + 2] = 1.0;
	}

    return m;
}

void
put_InHomogeneousMatrix(const char* filename, const char* suffix,
			const Matrix<double>& m)
{
    u_int		npoints = m.nrow(), nviews = m.ncol() / 3;
    Matrix<double>	tmp(npoints, 2 * nviews);

    for (u_int i = 0; i < npoints; ++i)
	for (u_int j = 0; j < nviews; ++j)
	{
	    tmp[i][2*j]	    = m[i][3*j]	    / m[i][3*j + 2];
	    tmp[i][2*j + 1] = m[i][3*j + 1] / m[i][3*j + 2];
	}

    put_matrix(filename, suffix, tmp);
}
 
}
