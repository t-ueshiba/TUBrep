/*
 *  ����14-19�N�i�Ɓj�Y�ƋZ�p���������� ���쌠���L
 *  
 *  �n��ҁF�A�ŏr�v
 *
 *  �{�v���O�����́i�Ɓj�Y�ƋZ�p�����������̐E���ł���A�ŏr�v���n�삵�C
 *  �i�Ɓj�Y�ƋZ�p���������������쌠�����L����閧���ł��D���쌠���L
 *  �҂ɂ�鋖�Ȃ��ɖ{�v���O�������g�p�C�����C���ρC��O�҂֊J������
 *  ���̍s�ׂ��֎~���܂��D
 *  
 *  ���̃v���O�����ɂ���Đ����邢���Ȃ鑹�Q�ɑ΂��Ă��C���쌠���L�҂�
 *  ��ёn��҂͐ӔC�𕉂��܂���B
 *
 *  Copyright 2002-2007.
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Creator: Toshio UESHIBA
 *
 *  [AIST Confidential and all rights reserved.]
 *  This program is confidential. Any using, copying, changing or
 *  giving any information concerning with this program to others
 *  without permission by the copyright holder are strictly prohibited.
 *
 *  [No Warranty.]
 *  The copyright holder or the creator are not responsible for any
 *  damages caused by using this program.
 *  
 *  $Id: Mesh++.h,v 1.22 2011-08-22 00:06:25 ueshiba Exp $
 */
/*!
  \file		Mesh++.h
  \brief	�N���X TU::Mesh �̒�`�Ǝ���
*/
#ifndef __TUMeshPP_h
#define __TUMeshPP_h

#include <list>
#include <vector>
#include <map>
#include "TU/Geometry++.h"

namespace TU
{
/************************************************************************
*  class Mesh<V, F, M>							*
************************************************************************/
//! ���p�`���b�V����\���N���X
/*!
  \param V	���_�̌^
  \param F	�ʂ̌^�DMesh<V, F, M>::Face�̔h���N���X�łȂ���΂Ȃ�Ȃ��D
  \param M	1�̖ʂ����ӂ̐�
*/
template <class V, class F, u_int M=3u>
class Mesh
{
  public:
    typedef V						vertex_type;
    typedef F						face_type;
    typedef typename std::list<V>::iterator		viterator;
    typedef typename std::list<V>::const_iterator	const_viterator;
    typedef typename std::list<F>::iterator		fiterator;
    typedef typename std::list<F>::const_iterator	const_fiterator;

    enum
    {
	NSides = M			//!< 1�̖ʂ����ӂ̐�
    };
    
  //! ���p�`���b�V���̖ʂ̊��ƂȂ�N���X
    class Face
    {
      public:
	V&		v(u_int e)				const	;
	F&		f(u_int e)				const	;
	
	friend class	Edge;

      protected:
#ifndef TUMeshPP_DEBUG
	Face(viterator v[])						;
#else
	Face(viterator v[], u_int fn)					;
#endif

      private:
	fiterator	self()					const	;

      private:
	viterator	_v[NSides];	//!< ���̖ʂ̒��_���w�������q
	fiterator	_f[NSides];	//!< ���̖ʂɗאڂ���ʂ��w�������q
#ifdef TUMeshPP_DEBUG
      public:
	const u_int	fnum;
#endif
    };

  //! ���p�`���b�V���̕ӂ�\���N���X
  /*!
    �ʂ����Ɍ���悤�Ɍ����t������Ă���D
  */
    class Edge
    {
      public:
	Edge(const Face& f)						;
	
	V&		v()					const	;
	F&		f()					const	;
	u_int		e()					const	;
	bool		operator ==(const Edge& edge)		const	;
	bool		operator !=(const Edge& edge)		const	;
	bool		commonVertex(const Edge& edge)		const	;
	u_int		valence()				const	;
	Edge&		operator ++()					;
	Edge&		operator --()					;
	Edge&		operator ~()					;
	Edge		next()					const	;
	Edge		prev()					const	;
	Edge		conj()					const	;

	friend class	Mesh;

      private:
	Edge(fiterator f)						;

	viterator	viter()					const	;
	fiterator	fiter()					const	;
	void		pair(const Edge& edge)			const	;
	void		replaceVertex(viterator v,
				      const Edge& edgeE)	const	;
	void		replaceVertex(viterator v)		const	;

      private:
	fiterator	_f;		//!< �e�̖ʂ��w�������q
	u_int		_e;		//!< �ӂ̔ԍ�
    };

  public:
    Edge		initialize(const V vertex[])			;
    void		clear()						;
    
    Edge		kill(Edge& edge)				;
    Edge		make(const Edge& edge0,
			     const Edge& edge1, const V& v)		;
    Edge		swap(const Edge& edge)				;
    BoundingBox<V>	boundingBox()				const	;

    viterator		vbegin()					;
    const_viterator	vbegin()				const	;
    viterator		vend()						;
    const_viterator	vend()					const	;
    fiterator		fbegin()					;
    const_fiterator	fbegin()				const	;
    fiterator		fend()						;
    const_fiterator	fend()					const	;
#ifdef TUMeshPP_DEBUG
    std::ostream&	showTopology(std::ostream& out)		const	;
#endif
    
  private:
    std::istream&	get(std::istream& in)				;
    std::ostream&	put(std::ostream& out)			const	;

    viterator		newVertex(const V& v)				;
    fiterator		newFace(const F& f)				;
    void		deleteVertex(viterator v)			;
    void		deleteFace(fiterator f)				;
    
  //! ���̓X�g���[�����烁�b�V����ǂݍ��ށD
  /*!
    \param in	���̓X�g���[��
    \param mesh	�ǂݍ��ݐ�̃��b�V��
    \return	in�Ŏw�肵�����̓X�g���[��
  */
    friend std::istream&
	operator >>(std::istream& in, Mesh& mesh)	{return mesh.get(in);}

  //! �o�̓X�g���[���Ƀ��b�V���������o���D
  /*!
    \param out	�o�̓X�g���[��
    \param mesh	�����o�����̃��b�V��
    \return	out�Ŏw�肵���o�̓X�g���[��
  */
    friend std::ostream&
	operator <<(std::ostream& out, const Mesh& mesh){return mesh.put(out);}

  private:
    std::list<V>	_vertices;			//!< ���_�̃��X�g
    std::list<F>	_faces;				//!< �ʂ̃��X�g
};

//! �w�肳�ꂽ���_����w�����킹��2�̖ʂ𐶐����ă��b�V��������������D
/*!
  \param vertex	M�̒��_
  \return	v[0] ���n�_�Ƃ����
*/
template <class V, class F, u_int M> typename Mesh<V, F, M>::Edge
Mesh<V, F, M>::initialize(const V vertex[])
{
  // �\�̖ʂ𐶐�����D
    viterator	v[NSides];
    for (u_int e = 0; e < NSides; ++e)
	v[e] = newVertex(vertex[e]);
    fiterator	f = newFace(F(v));

  // ���̖ʂ𐶐�����D
    viterator	vC[NSides];
    for (u_int e = 0; e < NSides; ++e)
	vC[e] = v[NSides-1-e];
    fiterator	fC = newFace(F(vC));

  // �\�Ɨ���\�荇�킹��D
    Edge	edge0(f), edge(edge0), edgeC(fC);
    --edgeC;
    do
    {
	edge.pair(--edgeC);
    } while (++edge != edge0);

    return edge0;
}

//! ���b�V���̑S�Ă̒��_�Ɩʂ��������ċ�ɂ���D
template <class V, class F, u_int M> inline void
Mesh<V, F, M>::clear()
{
    _vertices.clear();
    _faces.clear();
}
    
//! 3�p�`���b�V���ɂ��āC�w�肳�ꂽ�ӂ���������D
/*!
  �w�肳�ꂽ�ӂ̗����̖ʂ���ѕӂ̎n�_�����������D
  \param edge	��������ӁD
		���^�[�����edge�̎�O�̗��̕ӂ��w���悤�ɍX�V�����D
  \return	edge�̗��̎�O�̗��̕ӂ��w�������q
*/
template <class V, class F, u_int M> typename Mesh<V, F, M>::Edge
Mesh<V, F, M>::kill(Edge& edge)
{
    using namespace	std;
    
  // edge���܂ޖʂƂ��̗��̕ӂ��܂ޖʂ̒��_(�v4��)�ɂ��āC
  // ���̉������[�������ׂ�D
    Edge	edgeNC(edge.next().conj()),
		edgeCPC(edge.conj().prev().conj()),
		edgeCNC(edge.conj().next().conj());
    if (edge.valence() + edgeCPC.valence() <= 6 ||
	edgeNC.valence() <= 3 || edgeCNC.valence() <= 3)
    {
	cerr << "TU::Mesh<V, F, 3u>::kill(): Too small valence!" << endl;
	return edge;
    }

    Edge	edgeCN(edge.conj().next());
    viterator	vn = edge.next().viter();		// edge�̏I�_
    for (Edge tmp(edge.prev().conj()); ~(--tmp) != edgeCN; )
	for (Edge tmp1(tmp); --(~tmp1) != tmp; )
	    if (tmp1.viter() == vn)
	    {
		cerr << "TU::Mesh<V, F, 3u>::kill(): "
		     << "Pre-requisits for topology are not met!"
		     << endl;
		return edge;
	    }
    
  // edge�̎n�_�𒸓_�Ɏ��S�Ă̖ʂɂ��āC���̒��_��edge�̏I�_�ɒu��������D
    viterator	v = edge.viter();
    edge.replaceVertex(vn, edge);
    deleteVertex(v);					// ���_v������

  // edgePC, edgeCPC�����ꂼ��edgeNC, edgeCNC�Ɣw�����킹�ɂ���D
    fiterator	f = edge.fiter(), fC = edge.conj().fiter();
    ~(--edge);						// edgePC
    edge   .pair(edgeNC);
    edgeCPC.pair(edgeCNC);
    deleteFace(f);					// ��f������
    deleteFace(fC);					// ��fC������

    return edgeCPC;
}

//! 3�p�`���b�V���ɂ��āC�w�肳�ꂽ2�̕ӂ̎n�_�ԂɐV���ɕӂ����D
/*!
  1�̒��_��2�̖ʂ����������D
  \param edge0	�V���ȕӂƎn�_�����L�����
  \param edge1	�V���ȕӂƏI�_�����L����ӁD
		edge0�Ǝn�_�����L���Ă��Ȃ���΂Ȃ�Ȃ��D
  \param v	�V���ȕӂ����edge0�̎n�_�ƂȂ钸�_
  \return	�V���ȕ�
*/
template <class V, class F, u_int M> typename Mesh<V, F, M>::Edge
Mesh<V, F, M>::make(const Edge& edge0, const Edge& edge1, const V& v)
{
    using namespace	std;

  // edge0��edge1���n�_�����L���Ă��邩�`�F�b�N�D
    if (!edge0.commonVertex(edge1))
	throw domain_error("TU::Mesh<V, F, 3u>::make(): Given two edges have no common vertex!");

  // edge0��edge1������łȂ����`�F�b�N�D
    if (edge0 == edge1)
	throw domain_error("TU::Mesh<V, F, 3u>::make(): Given two edges are identical!");

  // �V�������_�����D
    viterator	vnew = newVertex(v);

  // �V�����ʂ�2���D
    viterator	vp[3];
    vp[0] = vnew;
    vp[1] = edge0.viter();
    vp[2] = edge0.next().viter();
    fiterator	f = newFace(F(vp));
    vp[0] = edge1.viter();
    vp[1] = vnew;
    vp[2] = edge1.next().viter();
    fiterator	fC = newFace(F(vp));

  // edge0�̎n�_��u��������O��edge0��edge1�̗���ێ����Ă����D
    Edge	edge0C(edge0.conj()), edge1C(edge1.conj());

  // [edge0, edge1)�͈̔͂̕ӂ̎n�_��V�������_�ɒu��������.
    edge0.replaceVertex(vnew, edge1);

  // winged-edge�\�������D
    Edge	edge(f), edgeC(fC);
    edge.pair(edgeC);
    (--edge ).pair(edge0);
    (--edge ).pair(edge0C);
    (--edgeC).pair(edge1);
    (--edgeC).pair(edge1C);
    
    return --edge;
}

//! 3�p�`���b�V���̎w�肳�ꂽ�ӂ��������C����ɂ���Ăł���l�p�`�̂�������̑Ίp����V���ȕӂƂ��Đ�������D
/*!
  \param edge	���������
  \return	�������ꂽ��
*/
template <class V, class F, u_int M> typename Mesh<V, F, M>::Edge
Mesh<V, F, M>::swap(const Edge& edge)
{
    using namespace	std;
    
  // edge�̎n�_�ƏI�_�̉�����1�����Ă�3�ȏ�ɂȂ邩���ׂ�D
    Edge	edgePC(edge.prev().conj()),
		edgeCPC(edge.conj().prev().conj());
    if (edgePC.valence() <= 3 || edgeCPC.valence() <= 3)
    {
	cerr << "TU::Mesh<V, F, 3u>::swap(): Too small valence!" << endl;
	return edge;
    }
    
  // edge�̎n�_�ƏI�_��u��������D
    Edge	edgeC(edge.conj()),
		edgeNC(edge.next().conj()),
		edgeCNC(edge.conj().next().conj());
    edge.replaceVertex(edgeCNC.viter());
    edge.next().replaceVertex(edgeNC.viter());
    edge.prev().replaceVertex(edgePC.viter());
    edgeC.replaceVertex(edgeNC.viter());
    edgeC.next().replaceVertex(edgeCNC.viter());
    edgeC.prev().replaceVertex(edgeCPC.viter());

  // �ӂ����ւ���winged-edge�\�������D
    edge.next().pair(edgePC);
    edge.prev().pair(edgeCNC);
    edgeC.next().pair(edgeCPC);
    edgeC.prev().pair(edgeNC);

    return edge;
}

//! ���b�V����bounding box���v�Z����D
/*!
  \return	bounding box
*/
template <class V, class F, u_int M> BoundingBox<V>
Mesh<V, F, M>::boundingBox() const
{
    BoundingBox<V>	bbox;
    
    for (const_fiterator f = _faces.begin(); f != _faces.end(); ++f)
	for (u_int e = 0; e < NSides; ++e)
	    bbox.expand(f->v(e));
    
    return bbox;
}

//! ���̃��b�V���̍ŏ��̒��_���w�������q��Ԃ��D
/*!
  \return	�ŏ��̒��_���w�������q
*/
template <class V, class F, u_int M> inline typename Mesh<V, F, M>::viterator
Mesh<V, F, M>::vbegin()
{
    return _vertices.begin();
}
    
//! ���̃��b�V���̍ŏ��̒��_���w���萔�����q��Ԃ��D
/*!
  \return	�ŏ��̒��_���w���萔�����q
*/
template <class V, class F, u_int M>
inline typename Mesh<V, F, M>::const_viterator
Mesh<V, F, M>::vbegin() const
{
    return _vertices.begin();
}
    
//! ���̃��b�V���Ō�̒��_�̎����w�������q��Ԃ��D
/*!
  \return	�Ō�̒��_�̎����w�������q
*/
template <class V, class F, u_int M> inline typename Mesh<V, F, M>::viterator
Mesh<V, F, M>::vend()
{
    return _vertices.end();
}
    
//! ���̃��b�V���Ō�̒��_�̎����w���萔�����q��Ԃ��D
/*!
  \return	�Ō�̒��_�̎����w���萔�����q
*/
template <class V, class F, u_int M>
inline typename Mesh<V, F, M>::const_viterator
Mesh<V, F, M>::vend() const
{
    return _vertices.end();
}

//! ���̃��b�V���̍ŏ��̖ʂ��w�������q��Ԃ��D
/*!
  \return	�ŏ��̖ʂ��w�������q
*/
template <class V, class F, u_int M> inline typename Mesh<V, F, M>::fiterator
Mesh<V, F, M>::fbegin()
{
    return _faces.begin();
}
    
//! ���̃��b�V���̍ŏ��̖ʂ��w���萔�����q��Ԃ��D
/*!
  \return	�ŏ��̖ʂ��w���萔�����q
*/
template <class V, class F, u_int M>
inline typename Mesh<V, F, M>::const_fiterator
Mesh<V, F, M>::fbegin() const
{
    return _faces.begin();
}
    
//! ���̃��b�V���Ō�̖ʂ̎����w�������q��Ԃ��D
/*!
  \return	�Ō�̖ʂ̎����w�������q
*/
template <class V, class F, u_int M> inline typename Mesh<V, F, M>::fiterator
Mesh<V, F, M>::fend()
{
    return _faces.end();
}
    
//! ���̃��b�V���Ō�̖ʂ̎����w���萔�����q��Ԃ��D
/*!
  \return	�Ō�̖ʂ̎����w���萔�����q
*/
template <class V, class F, u_int M>
inline typename Mesh<V, F, M>::const_fiterator
Mesh<V, F, M>::fend() const
{
    return _faces.end();
}

#ifdef TUMeshPP_DEBUG
//! �o�̓X�g���[���ɂ��̃��b�V�����\������ʂ̐ڑ��֌W�������o���D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class V, class F, u_int M> std::ostream&
Mesh<V, F, M>::showTopology(std::ostream& out) const
{
    for (const_fiterator f = _faces.begin(); f != _faces.end(); ++f)
    {
	out << "Face[" << f->fnum << "]:";
	for (u_int e = 0; e < NSides; ++e)
	    out << ' ' << f->f(e).fnum;
	out << std::endl;
    }

    return out;
}
#endif
    
//! ���̓X�g���[�����烁�b�V����ǂݍ��ށD
/*!
  \param in	���̓X�g���[��
  \return	in�Ŏw�肵�����̓X�g���[��
*/
template <class V, class F, u_int M> std::istream&
Mesh<V, F, M>::get(std::istream& in)
{
    using namespace	std;
    
    typedef vector<fiterator>			Faces;
    typedef typename Faces::const_iterator	FacesIterator;
    typedef vector<pair<viterator, Faces> >	VerticesWithFaces;
    typedef typename VerticesWithFaces::const_iterator
						VerticesWithFacesIterator;

  // ���_�Ɩʂ̃��X�g����ɂ���D
    clear();
    
  // �S�Ă̒��_��ǂݍ��ށD
    VerticesWithFaces	verticesWithFaces;
    char		c;
    while (in >> c && c == 'V')
    {
	char		dummy[64];
	u_int		vnum;
	V		vertex;
	in >> dummy >> vnum >> vertex;		// ���_��ǂݍ��ށD
	viterator	v = newVertex(vertex);	// �V�������_�𐶐�

      // �ǉ��������_�ւ̔����q���C���̒��_�����L����ʂ̋󃊃X�g�ƈꏏ�ɓo�^�D
	verticesWithFaces.push_back(make_pair(v, Faces()));
    }
    in.putback(c);

  // �S�Ă̖ʂ�ǂݍ��ށD
    while (in >> c && c == 'F')
    {
	char	dummy[64];
	u_int	fnum;
	in >> dummy >> fnum;		// �ʔԍ����X�L�b�v�D

	viterator	v[NSides];	// ���̖ʂ̒��_���w���S�����q
	u_int		vnum[NSides];	// ���̖ʂ̒��_�̑S�ԍ�
	for (u_int e = 0; e < NSides; ++e)
	{
	    in >> vnum[e];		// ���̖ʂ̒��_�̔ԍ���ǂݍ��ށD
	    --vnum[e];			// ���_�ԍ���1����n�܂�̂Ńf�N�������g
	    v[e] = verticesWithFaces[vnum[e]].first;	// �����q�����o��
	}
#ifndef TUMeshPP_DEBUG
	fiterator	f = newFace(F(v));		// �V�����ʂ𐶐�
#else
	fiterator	f = newFace(F(v, fnum));	// �V�����ʂ𐶐�
#endif
      // �X�̒��_�Ɏ��g�̐e�Ƃ��Ă��̖ʂ�o�^����D
	for (u_int e = 0; e < NSides; ++e)
	    verticesWithFaces[vnum[e]].second.push_back(f);
    }
    if (in)
	in.putback(c);

  // �X�̒��_�ɂ��āC������͂�2�̗אږʊԂ�winged-edge�\�������D
    for (VerticesWithFacesIterator vertex  = verticesWithFaces.begin();
	 vertex != verticesWithFaces.end(); ++vertex)
    {
	viterator	v     = vertex->first;
	const Faces&	faces = vertex->second;		// v���͂ޖʂ̃��X�g

      // v���͂ތX�̖�f�ɂ��āD�D�D
	for (FacesIterator f = faces.begin(); f != faces.end(); ++f)
	{
	  // f�ɂ��āCv���n�_�Ƃ����edge��T���D
	    Edge	edge(*f);
	    while (edge.viter() != v)
		++edge;

	    viterator	vn = edge.next().viter();	// edge�̏I�_
	    
	  // vertex���͂ޕʂ̖�f1�ɂ��āD�D�D
	    for (FacesIterator f1 = faces.begin(); f1 != f; ++f1)
	    {
	      // f1�̂܂����������edge�̏I�_vn���n�_�Ƃ���ӂ�T���D
		Edge	edgeF0(*f1), edgeF(edgeF0);
		do
		{
		    if (edgeF.viter() == vn)		// �n�_��vn�ł����
		    {
			edge.pair(edgeF);		// ���̕ӂ�edge�̗���
			goto done;
		    }
		} while (++edgeF != edgeF0);
	    }
	  done:
	    continue;
	}
    }
#ifdef TUMeshPP_DEBUG
    showTopology(cerr);
#endif
    
    return in;
}

//! �o�̓X�g���[���Ƀ��b�V���������o���D
/*!
  \param out	�o�̓X�g���[��
  \return	out�Ŏw�肵���o�̓X�g���[��
*/
template <class V, class F, u_int M> std::ostream&
Mesh<V, F, M>::put(std::ostream& out) const
{
    using namespace	std;
    
    map<const V*, u_int>	dict;
    u_int			vnum = 1;
    for (const_viterator v = vbegin(); v != vend(); ++v)
    {
	dict[&(*v)] = vnum;
	out << "Vertex " << vnum++ << ' ' << *v;
    }
    u_int	fnum = 1;
    for (const_fiterator f = fbegin(); f != fend(); ++f)
    {
	out << "Face " << fnum++;
	for (u_int e = 0; e < NSides; ++e)
	    out << ' ' << dict[&(f->v(e))];
	out << std::endl;
    }
    
    return out;
}

//! �V�������_�𐶐����Ē��_���X�g�ɓo�^����D
/*!
 \param f	�������钸�_�̃v���g�^�C�v
 \return	�������ꂽ���_���w�������q
*/
template <class V, class F, u_int M> inline typename Mesh<V, F, M>::viterator
Mesh<V, F, M>::newVertex(const V& v)
{
    _vertices.push_front(v);
    return _vertices.begin();
}
    
//! �V�����ʂ𐶐����Ėʃ��X�g�ɓo�^����D
/*!
 \param f	��������ʂ̃v���g�^�C�v
 \return	�������ꂽ�ʂ��w�������q
*/
template <class V, class F, u_int M> inline typename Mesh<V, F, M>::fiterator
Mesh<V, F, M>::newFace(const F& f)
{
    _faces.push_front(f);
    return _faces.begin();
}
    
//! �w�肳�ꂽ���_��j�󂵂Ē��_���X�g�����菜���D
/*!
 \param f	�j�󂷂钸�_���w�������q
*/
template <class V, class F, u_int M> inline void
Mesh<V, F, M>::deleteVertex(viterator v)
{
    _vertices.erase(v);
}
    
//! �w�肳�ꂽ�ʂ�j�󂵂Ėʃ��X�g�����菜���D
/*!
 \param f	�j�󂷂�ʂ��w�������q
*/
template <class V, class F, u_int M> inline void
Mesh<V, F, M>::deleteFace(fiterator f)
{
    _faces.erase(f);
}
    
/************************************************************************
*  class Mesh<V, F, M>::Face						*
************************************************************************/
//! ���_���w�肵�Ėʂ𐶐�����D
/*!
  \param v	M�̒��_�ւ̔����q
*/
template <class V, class F, u_int M>
#ifndef TUMeshPP_DEBUG
Mesh<V, F, M>::Face::Face(viterator v[])
#else
Mesh<V, F, M>::Face::Face(viterator v[], u_int fn)
    :fnum(fn)
#endif
{
    for (u_int e = 0; e < NSides; ++e)
    {
      //_f[e] = 0;
	_v[e] = v[e];
    }
}

//! �w�肳�ꂽ�ӂ̎n�_��Ԃ��D
/*!
    \param e	�ӂ�index, 0 <= e < M
    \return	e�Ԗڂ̕ӂ̎n�_���Ȃ킿���̖ʂ�e�Ԗڂ̒��_
*/
template <class V, class F, u_int M> inline V&
Mesh<V, F, M>::Face::v(u_int e) const
{
    return *_v[e];
}

//! �w�肳�ꂽ�ӂ���Ă��̖ʂɗאڂ���ʂ�Ԃ��D
/*!
    \param e	�ӂ�index, 0 <= e < M
    \return	e�Ԗڂ̕ӂ���ėאڂ����
*/
template <class V, class F, u_int M> inline F&
Mesh<V, F, M>::Face::f(u_int e) const
{
    return *_f[e];
}

//! ���̖ʂ��w�������q��Ԃ��D
/*!
    \return	���̖ʂ��w�������q
*/
template <class V, class F, u_int M> typename Mesh<V, F, M>::fiterator
Mesh<V, F, M>::Face::self() const
{
    using namespace	std;
    
    fiterator	fC = _f[0];		// 0�Ԗڂ̕ӂ���ėאڂ����
    for (u_int e = 0; e < NSides; ++e)
    {					// fC��e�Ԗڂ̕ӂ�
	fiterator	f = fC->_f[e];	// ��ėאڂ���ʂւ̔����qf��
	if (&(*f) == this)		// ���̖ʂ��w���Ă�����
	    return f;			// f�����̖ʂւ̔����q�ł���D
    }

    throw runtime_error("TU::Mesh<V, F, M>::Face::self(): Internal error!");

    return fC;
}

/************************************************************************
*  class Mesh<V, F, M>::Edge						*
************************************************************************/
//! �w�肳�ꂽ�ʂ̍ŏ��̕ӂ��w���悤�ɏ���������D
/*!
  \param face	��
*/
template <class V, class F, u_int M> inline
Mesh<V, F, M>::Edge::Edge(const Face& face)
    :_f(face.self()), _e(0)
{
}

//! ���̕ӂ̎n�_��Ԃ��D
/*!
  \return	���̕ӂ̎n�_
*/
template <class V, class F, u_int M> inline V&
Mesh<V, F, M>::Edge::v() const
{
    return *viter();
}
    
//! ���̕ӂ����L����ʂ�Ԃ��D
/*!
  \return	���̕ӂ����L�����
*/
template <class V, class F, u_int M> inline F&
Mesh<V, F, M>::Edge::f() const
{
    return *fiter();
}
    
//! ���̕ӂ̔ԍ���Ԃ��D
/*!
  \return	�ӂ̔ԍ�
*/
template <class V, class F, u_int M> inline u_int
Mesh<V, F, M>::Edge::e() const
{
    return _e;
}

//! 2�̕ӂ�����ł��邩���ׂ�D
/*!
  \param edge	��r�Ώۂ̕�
  \return	����Ȃ�true, �����łȂ����false
*/
template <class V, class F, u_int M> inline bool
Mesh<V, F, M>::Edge::operator ==(const Edge& edge) const
{
    return (_e == edge._e) && (_f == edge._f);
}

//! 2�̕ӂ��قȂ邩���ׂ�D
/*!
  \param edge	��r�Ώۂ̕�
  \return	�قȂ��true, �����łȂ����false
*/
template <class V, class F, u_int M> inline bool
Mesh<V, F, M>::Edge::operator !=(const Edge& edge) const
{
    return !(*this == edge);
}

//! 2�̕ӂ��n�_�����L���Ă��邩���ׂ�D
/*!
  \param edge	��r�Ώۂ̕�
  \return	���L���Ă����true, �����łȂ����false
*/
template <class V, class F, u_int M> bool
Mesh<V, F, M>::Edge::commonVertex(const Edge& edge) const
{
    Edge	tmp(*this);
    do
    {
	if (tmp == edge)
	    return true;
    } while (~(--tmp) != *this);
    
    return false;
}

//! �ӂ̎n�_�̉����C���Ȃ킿���̓_�����L�����(��)�̐���Ԃ��D
/*!
  \return	�ӂ̎n�_�̉���
*/
template <class V, class F, u_int M> u_int
Mesh<V, F, M>::Edge::valence() const
{
    u_int	n = 0;
    Edge	edge(*this);
    do
    {
	++n;
    } while (~(--edge) != *this);

    return n;
}

//! ���̕ӂɑO�i����D
/*!
  \return	�O�i��̕�
*/
template <class V, class F, u_int M> inline typename Mesh<V, F, M>::Edge&
Mesh<V, F, M>::Edge::operator ++()
{
    if (_e == NSides - 1)
	_e = 0;
    else
	++_e;
    return *this;
}

//! ��O�̕ӂɌ�ނ���D
/*!
  \return	��ތ�̕�
*/
template <class V, class F, u_int M> inline typename Mesh<V, F, M>::Edge&
Mesh<V, F, M>::Edge::operator --()
{
    if (_e == 0)
	_e = NSides - 1;
    else
	--_e;
    return *this;
}

//! �����̕ӂɈړ�����D
/*!
  \return	�ړ���̕�
*/
template <class V, class F, u_int M> inline typename Mesh<V, F, M>::Edge&
Mesh<V, F, M>::Edge::operator ~()
{
    return *this = conj();
}

//! ���̕ӂ̎��̕ӂ�Ԃ��D
/*!
  \return	���̕�
*/
    template <class V, class F, u_int M> inline typename Mesh<V, F, M>::Edge
Mesh<V, F, M>::Edge::next() const
{
    Edge	edge(*this);
    return ++edge;
}

//! ���̕ӂ̎�O�̕ӂ�Ԃ��D
/*!
  \return	��O�̕�
*/
template <class V, class F, u_int M> inline typename Mesh<V, F, M>::Edge
Mesh<V, F, M>::Edge::prev() const
{
    Edge	edge(*this);
    return --edge;
}

//! ���̕ӂ̗����̕ӂ�Ԃ��D
/*!
  \return	�����̕�
*/
template <class V, class F, u_int M> typename Mesh<V, F, M>::Edge
Mesh<V, F, M>::Edge::conj() const
{
    viterator	vn = next().viter();	// ���̕ӂ̏I�_
    Edge	edge(_f->_f[_e]);	// ���̕ӂ���ėאڂ���ʂ̍ŏ��̕�
    while (edge.viter() != vn)		// ���̕ӂ̏I�_���n�_�Ƃ���ӂ�T��
	++edge;
    return edge;
}

//! ���̕ӂ����L����ʂ��w�������q��Ԃ��D
/*!
  \return	���̕ӂ����L����ʂ��w�������q
*/
template <class V, class F, u_int M> inline typename Mesh<V, F, M>::fiterator
Mesh<V, F, M>::Edge::fiter() const
{
    return _f;
}
    
//! ���̕ӂ̎n�_���w�������q��Ԃ��D
/*!
  \return	���̕ӂ̎n�_���w�������q
*/
template <class V, class F, u_int M> inline typename Mesh<V, F, M>::viterator
Mesh<V, F, M>::Edge::viter() const
{
    return _f->_v[_e];
}
    
//! �w�肳�ꂽ�ʂ̍ŏ��̕ӂ��w���悤�ɏ���������D
/*!
  \param f	�ʂ��w�������q
*/
template <class V, class F, u_int M> inline
Mesh<V, F, M>::Edge::Edge(fiterator f)
    :_f(f), _e(0)
{
}

//! ���̕ӂƎw�肳�ꂽ�ӂ�w�����킹�ɂ���D
/*!
  \param edge	�w�����킹�̑ΏۂƂȂ��
*/
template <class V, class F, u_int M> inline void
Mesh<V, F, M>::Edge::pair(const Edge& edge) const
{
    _f->_f[_e] = edge._f;	// ���̕ӂ̗��ʂ�edge�̐e�ʂ�
    edge._f->_f[edge._e] = _f;	// edge�̗��ʂ����̕ӂ̐e�ʂ�
}

//! ���̕ӂ���w�肳�ꂽ�ӂ̎�O�܂ł̕ӂ̎n�_���w�肳�ꂽ���_�ɒu��������D
/*!
  ���̕ӂ̎n�_�����L����ӂ𔽎��v���ɑ������Ȃ���n�_��u�������Ă䂭�D
  \param v	���_���w�������q
  \param edgeE	�����̏I�_�ƂȂ�� (���̕ӂ̎n�_�͒u���������Ȃ�)
*/
template <class V, class F, u_int M> void
Mesh<V, F, M>::Edge::replaceVertex(viterator v, const Edge& edgeE) const
{
  // ��Ɏn�_�����������Ă��܂��Ɨ��Ɉڂ�Ȃ��Ȃ��Ă��܂��̂ŁC
  // �ċA�I�ɏ������邱�Ƃɂ���Ă܂�edgeE��1��O�̕ӂ܂ňړ����C
  // �߂�Ȃ��珇������������D
    Edge	edgePC(prev().conj());
    if (edgePC != edgeE)
	edgePC.replaceVertex(v, edgeE);		// �ċA����D
    _f->_v[_e] = v;
}

//! ���̕ӂ̎n�_���w�肳�ꂽ���_�ɒu��������D
/*!
  \param v	���_���w�������q
*/
template <class V, class F, u_int M> inline void
Mesh<V, F, M>::Edge::replaceVertex(viterator v) const
{
    _f->_v[_e] = v;
}

}
#endif	// !__TUMeshPP_h
