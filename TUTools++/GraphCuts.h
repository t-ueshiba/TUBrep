/*
 *  $Id: GraphCuts.h,v 1.3 2012-04-20 00:47:07 ueshiba Exp $
 */
#ifndef __GRAPHCUTS_H
#define __GRAPHCUTS_H

#include <boost/version.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/read_dimacs.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#if BOOST_VERSION<104601
#  include <boost/graph/kolmogorov_max_flow.hpp>
#  define boykov_kolmogorov_max_flow	kolmogorov_max_flow
#else
#  include <boost/graph/boykov_kolmogorov_max_flow.hpp>
#endif
#include <boost/foreach.hpp>
#include <iostream>
#include <algorithm>
#include <stack>
#include <cassert>

/*!
  \namespace	boost
  \brief	boost���C�u�����𗘗p�����N���X��֐��𖼑O���boost�ɒǉ�
*/
namespace boost
{
enum vertex_id_t	{vertex_id};		// �T�C�g��ID
enum vertex_label_t	{vertex_label};		// �T�C�g�̃��x��
enum vertex_sedge_t	{vertex_sedge};		// �J�n�_����̕�
enum vertex_tedge_t	{vertex_tedge};		// �I�[�_�ւ̕�
enum edge_smooth_t	{edge_smooth};		// ����������\����
BOOST_INSTALL_PROPERTY(vertex, id);
BOOST_INSTALL_PROPERTY(vertex, label);
BOOST_INSTALL_PROPERTY(vertex, sedge);
BOOST_INSTALL_PROPERTY(vertex, tedge);
BOOST_INSTALL_PROPERTY(edge, smooth);
    
/************************************************************************
*  class GraphCuts<T, ID, L, EL>					*
************************************************************************/
//! �O���t�J�b�g�ɂ���ăG�l���M�[�ŏ������s���N���X
/*!
  \param T	�G�l���M�[�l�̌^(������\���ł��Ȃ���΂Ȃ�Ȃ�)
  \param ID	�T�C�g����肷��ID�̌^
  \param L	�T�C�g�̃��x���̌^
  \param EL	�O���t�̕ӂ̎���(boost::vecS, boost::listS �̂����ꂩ)
*/
template <class T, class ID=int, class L=bool, class EL=vecS>
class GraphCuts
{
  public:
    typedef T	value_type;		//!< �G�l���M�[�l�̌^
    typedef ID	id_type;		//!< �T�C�g����肷��ID�̌^
    typedef L	label_type;		//!< �T�C�g�̃��x���̌^
    enum Algorithm			//<! �ő�t���[�A���S���Y��
    {
	BoykovKolmogorov		//!< Boykov-Kolmogorov�A���S���Y��
#ifdef WITH_PARALLEL_EDGES
	, EdmondsKarp,			//!< Edmonds-Karp�A���S���Y��
	PushRelabel			//!< Push-Relabel�A���S���Y��
#endif
    };
    
  private:
    typedef adjacency_list_traits<EL, vecS, directedS>	traits_t;
    typedef typename traits_t::vertex_descriptor	vertex_t;
    typedef typename traits_t::edge_descriptor		edge_t;
    typedef default_color_type			color_t;
    typedef color_traits<color_t>		color_traits_t;
    typedef property<vertex_id_t,		id_type,
	    property<vertex_label_t,		label_type,
	    property<vertex_color_t,		color_t,
	    property<vertex_sedge_t,		edge_t,
	    property<vertex_tedge_t,		edge_t,
	    property<vertex_distance_t,		long,
	    property<vertex_predecessor_t,	edge_t
		     > > > > > > >			VertexProps;
    typedef property<edge_index_t,		int,
	    property<edge_capacity_t,		value_type,
	    property<edge_residual_capacity_t,	value_type,
	    property<edge_reverse_t,		edge_t,
	    property<edge_smooth_t,		bool
		     > > > > >				EdgeProps;
    typedef adjacency_list<EL, vecS, directedS,
			   VertexProps, EdgeProps>	graph_t;
    typedef typename graph_traits<graph_t>::vertex_iterator
							vertex_iterator;
    typedef typename graph_traits<graph_t>::out_edge_iterator
							out_edge_iterator;

  public:
    typedef vertex_t			site_type;	//!< �T�C�g�̌^
    typedef std::pair<vertex_iterator,
		      vertex_iterator>	site_range;	//!< �T�C�g�͈̔�
    
  public:
  // �\���̐����Ɣj��
    GraphCuts()								;
    site_type		createDataTerm(const id_type& id)		;
    void		createSmoothingTerm(site_type u, site_type v)	;
    void		clear()						;

  // �T�C�g���̎擾�Ɛݒ�
    static site_type	nullSite()					;
    site_range		sites()					const	;
    size_t		nsites()				const	;
    id_type		id(site_type v)				const	;
    label_type		operator ()(site_type v)		const	;
    label_type&		operator ()(site_type v)			;
    bool		haveSmoothingTerm(site_type u,
					  site_type v)		const	;
    value_type		dataEnergy(site_type v, bool source)	const	;
    value_type&		dataEnergy(site_type v, bool source)		;
    value_type		smoothingEnergy(site_type u, site_type v) const	;
    value_type&		smoothingEnergy(site_type u, site_type v)	;
    
  // �œK���v�Z
    template <class F>
    value_type		value(F energyTerm)			const	;
    template <class F>
    value_type		value(label_type alpha, F energyTerm)	const	;
    value_type		maxFlow(label_type alpha, Algorithm alg)	;
    template <class F>
    value_type		alphaExpansion(label_type alpha, F energyTerm,
				       Algorithm alg)			;
    void		check()					const	;
    
  // ���o��
    std::istream&	getDimacsMaxFlow(std::istream& in)		;
    std::ostream&	putDimacsMaxFlow(std::ostream& out)	const	;
    std::ostream&	putCapacities(std::ostream& out)	const	;
    std::ostream&	putMaxFlow(std::ostream& out)		const	;
    std::ostream&	putMinCut(std::ostream& out)		const	;
    
  private:
    edge_t		createEdgePair(vertex_t u, vertex_t v)		;
    void		computeMinCut()					;
    value_type		flow(edge_t e)				const	;
    
  private:
    graph_t		_g;	//!< �O���t
    vertex_t		_s;	//!< �J�n�_
    vertex_t		_t;	//!< �I�[�_
};

/*
 * ----------------------- �\���̐����Ɣj�� ----------------------------
 */
//! �O���t�J�b�g���s��𐶐�����D
template <class T, class ID, class L, class EL> inline
GraphCuts<T, ID, L, EL>::GraphCuts()
    :_g(),
     _s(add_vertex(_g)),
     _t(add_vertex(_g))
{
}

//! �f�[�^���𐶐�����D
/*!
  \param id	�T�C�g��ID
  \return	�T�C�g��\�����_
*/ 
template <class T, class ID, class L, class EL>
inline typename GraphCuts<T, ID, L, EL>::site_type
GraphCuts<T, ID, L, EL>::createDataTerm(const id_type& id)
{
    const vertex_t	v = add_vertex(_g);	      // ���_�𐶐�
    put(vertex_id,    _g, v, id);		      // ����ID��o�^
    put(vertex_sedge, _g, v, createEdgePair(_s, v));  // �J�n�_�ւ̕ӑ΂𐶐�
    put(vertex_tedge, _g, v, createEdgePair(v, _t));  // �I�[�_�ւ̕ӑ΂𐶐�

    return v;
}

//! �����������Z�b�g����D
/*!
  \param u	�����������ΏۂƂ������̃T�C�g
  \param v	�����������ΏۂƂ����������̃T�C�g
*/ 
template <class T, class ID, class L, class EL> inline void
GraphCuts<T, ID, L, EL>::createSmoothingTerm(site_type u, site_type v)
{
#ifdef WITH_PARALLEL_EDGES
    edge_t	e = createEdgePair(u, v);
#else
    edge_t	e;
    bool	exists;
    tie(e, exists) = edge(u, v, _g);	// ����(u, v)������΁C�����e�ɃZ�b�g���C
    if (!exists)			// �Ȃ����...
	e = createEdgePair(u, v);	// �V���ɍ��D
#endif
    put(edge_smooth, _g, e, true);	// ���������t���O���Z�b�g
}
    
//! �O���t�̑S�Ă̕ӂ���ъJ�n�_�ƏI�[�_�������S�Ă̒��_����������D
template <class T, class ID, class L, class EL> inline void
GraphCuts<T, ID, L, EL>::clear()
{
    _g.clear();
    _s = add_vertex(_g);
    _t = add_vertex(_g);
}

/*
 * ----------------------- �T�C�g���̎擾�Ɛݒ� ----------------------
 */
//! ���ۂɂ͑��݂��Ă��Ȃ��_�~�[�T�C�g��Ԃ��D
/*!
  \return	�_�~�[�T�C�g
*/
template <class T, class ID, class L, class EL>
inline typename GraphCuts<T, ID, L, EL>::site_type
GraphCuts<T, ID, L, EL>::nullSite()
{
    return graph_traits<graph_t>::null_vertex();
}
    
//! �S�T�C�g(�J�n�_�ƏI�[�_�������S�Ă̒��_)�͈̔͂����������q�y�A��Ԃ��D
/*!
  \return	�S�T�C�g�͈̔�
*/
template <class T, class ID, class L, class EL>
inline typename GraphCuts<T, ID, L, EL>::site_range
GraphCuts<T, ID, L, EL>::sites() const
{
    site_range	range = vertices(_g);
    assert(*range.first++ == _s);
    assert(*range.first++ == _t);

    return range;
}
    
//! �T�C�g����Ԃ��D
/*!
  \return	�T�C�g��
*/
template <class T, class ID, class L, class EL> inline size_t
GraphCuts<T, ID, L, EL>::nsites() const
{
    return num_vertices(_g) - 2;
}
    
//! �w�肳�ꂽ�T�C�g��ID��Ԃ��D
/*!
  \param v	�T�C�g��\�����_
  \return	�T�C�g��ID
*/
template <class T, class ID, class L, class EL>
inline typename GraphCuts<T, ID, L, EL>::id_type
GraphCuts<T, ID, L, EL>::id(site_type v) const
{
    return get(vertex_id, _g, v);
}
    
//! �w�肳�ꂽ�T�C�g�̃��x����Ԃ��D
/*!
  \param v	�T�C�g��\�����_
  \return	�T�C�g�̃��x��
*/
template <class T, class ID, class L, class EL>
inline typename GraphCuts<T, ID, L, EL>::label_type
GraphCuts<T, ID, L, EL>::operator ()(site_type v) const
{
    return get(vertex_label, _g, v);
}

//! �w�肳�ꂽ�T�C�g�̃��x����Ԃ��D
/*!
  \param v	�T�C�g��\�����_
  \return	�T�C�g�̃��x��
*/
template <class T, class ID, class L, class EL>
inline typename GraphCuts<T, ID, L, EL>::label_type&
GraphCuts<T, ID, L, EL>::operator ()(site_type v)
{
    return get(vertex_label, _g)[v];
}

//! �w�肳�ꂽ2�̃T�C�g�Ԃɕ������G�l���M�[�������邩���ׂ�D
/*!
  \param u	�T�C�g��\�����_
  \param v	������̃T�C�g��\�����_
  \return	�������G�l���M�[���������true, �Ȃ����false
*/
template <class T, class ID, class L, class EL> inline bool
GraphCuts<T, ID, L, EL>::haveSmoothingTerm(site_type u, site_type v) const
{
    BOOST_FOREACH (edge_t e, out_edges(u, _g))
	if (target(e, _g) == v && get(edge_smooth, _g, e))
	    return true;
    return false;
}
    
//! �w�肳�ꂽ�T�C�g�̃f�[�^�G�l���M�[�l��Ԃ��D
/*!
  \param v	�T�C�g��\�����_
  \param source	true�Ȃ�ΊJ�n�_�����Cfalse�Ȃ�ΏI�[�_�������ꂼ��ؒf
  \return	�f�[�^�G�l���M�[�l
*/
template <class T, class ID, class L, class EL>
inline typename GraphCuts<T, ID, L, EL>::value_type
GraphCuts<T, ID, L, EL>::dataEnergy(site_type v, bool source) const
{
    edge_t	e = (source ? get(vertex_sedge, _g, v)
			    : get(vertex_tedge, _g, v));
    return get(edge_capacity, _g, e);
}

//! �w�肳�ꂽ�T�C�g�̃f�[�^�G�l���M�[�l�ւ̎Q�Ƃ�Ԃ��D
/*!
  \param v	�T�C�g��\�����_
  \param source	true�Ȃ�ΊJ�n�_�����Cfalse�Ȃ�ΏI�[�_�������ꂼ��ؒf
  \return	�f�[�^�G�l���M�[�l�ւ̎Q��
*/
template <class T, class ID, class L, class EL>
inline typename GraphCuts<T, ID, L, EL>::value_type&
GraphCuts<T, ID, L, EL>::dataEnergy(site_type v, bool source)
{
    edge_t	e = (source ? get(vertex_sedge, _g, v)
			    : get(vertex_tedge, _g, v));
    return get(edge_capacity, _g, e);
}

//! �w�肳�ꂽ2�̃T�C�g�Ԃ̕������G�l���M�[�l��Ԃ��D
/*!
  �����̃T�C�g�Ԃɕ������G�l���M�[�����Ȃ����0���Ԃ����D
  \param u	�T�C�g��\�����_
  \param v	������̃T�C�g��\�����_
  \return	�������G�l���M�[��������΂��̃G�l���M�[�l�C�Ȃ����0
*/
template <class T, class ID, class L, class EL>
typename GraphCuts<T, ID, L, EL>::value_type
GraphCuts<T, ID, L, EL>::smoothingEnergy(site_type u, site_type v) const
{
#ifdef WITH_PARALLEL_EDGES
    BOOST_FOREACH (edge_t e, out_edges(u, _g))
	if (target(e, _g) == v && get(edge_smooth, _g, e))
	    return get(edge_capacity, _g, e);
    return 0;
#else
    edge_t	e;
    bool	exists;
    tie(e, exists) = edge(u, v, _g);
    return (exists && get(edge_smooth, _g, e) ? get(edge_capacity, _g, e) : 0);
#endif
}

//! �w�肳�ꂽ2�̃T�C�g�Ԃ̕������G�l���M�[�l�ւ̎Q�Ƃ�Ԃ��D
/*!
  �����̃T�C�g�Ԃɕ������G�l���M�[�����Ȃ���΁C��O�����o�����D
  \param u	�T�C�g��\�����_
  \param v	������̃T�C�g��\�����_
  \return	�������G�l���M�[�l�ւ̎Q��
*/
template <class T, class ID, class L, class EL>
typename GraphCuts<T, ID, L, EL>::value_type&
GraphCuts<T, ID, L, EL>::smoothingEnergy(site_type u, site_type v)
{
#ifdef WITH_PARALLEL_EDGES
    BOOST_FOREACH (edge_t e, out_edges(u, _g))
	if (target(e, _g) == v && !get(edge_smooth, _g, e))
	    return get(edge_capacity, _g, e);
    throw std::runtime_error("GraphCuts<T, ID, L, EL>::energy(): non-existing smoothing term!");
    return get(edge_capacity, _g, get(vertex_sedge, _g, u));  // �����ɂ͓��B����
#else
    edge_t	e;
    bool	exists;
    tie(e, exists) = edge(u, v, _g);
    if (!exists || !get(edge_smooth, _g, e))
	throw std::runtime_error("GraphCuts<T, ID, L, EL>::energy(): non-existing smoothing term!");
    return get(edge_capacity, _g, e);
#endif
}

/*
 * ----------------------------- �œK���v�Z -----------------------------
 */
//! ���݂̃��x���z�u�̂��Ƃł̃G�l���M�[�l�����߂�D
/*!
  \param energyTerm	�T�C�g��ID�Ƃ��̃��x����^����ƃf�[�^�G�l���M�[�l��
			�Ԃ������o����їאڂ���2�̃T�C�g��ID�Ƃ�����
			���x����^����ƕ������G�l���M�[�l��Ԃ������o��2��
			�����֐��I�u�W�F�N�g
  \return		�G�l���M�[�l
*/
template <class T, class ID, class L, class EL> template <class F>
typename GraphCuts<T, ID, L, EL>::value_type
GraphCuts<T, ID, L, EL>::value(F energyTerm) const
{
    value_type	val = 0;				// �G�l���M�[�l
    BOOST_FOREACH (vertex_t u, sites())
    {
	const id_type		uid = id(u);
	const label_type	Xu  = (*this)(u);

	val += energyTerm(uid, Xu);				// �f�[�^��
	
	BOOST_FOREACH (edge_t e, out_edges(u, _g))
	{
	    if (get(edge_smooth, _g, e))
	    {
		const vertex_t	v = target(e, _g);

		val += energyTerm(uid, id(v), Xu, (*this)(v));	// ��������
	    }
	}
    }
    
    return val;
}
    
//! �w�肳�ꂽ�ő�t���[�A���S���Y���ɂ���ăt���[�̍ő�l�����߂�D
/*!
  \param alpha	�ŏ��J�b�g�ɂ��I�[�_���ɕ��ނ��ꂽ�T�C�g�ɗ^���郉�x��
  \param alg	�g�p����ő�t���[�A���S���Y��
  \return	�t���[�̍ő�l
*/
template <class T, class ID, class L, class EL>
typename GraphCuts<T, ID, L, EL>::value_type
GraphCuts<T, ID, L, EL>::maxFlow(label_type alpha, Algorithm alg)
{
    value_type	f;
    switch (alg)
    {
#ifdef WITH_PARALLEL_EDGES
      case EdmondsKarp:
	f = edmonds_karp_max_flow(_g, _s, _t,
				  color_map(get(vertex_color, _g)).
				  predecessor_map(get(vertex_predecessor, _g)));
	break;
      case PushRelabel:
	f = push_relabel_max_flow(_g, _s, _t);
	computeMinCut();
	break;
#endif
      default:
	f = boykov_kolmogorov_max_flow(_g, _s, _t);
	computeMinCut();
	break;
    }

  // �I�[�_���̒��_�Ƀ��x���lalpha��^����D
    BOOST_FOREACH (vertex_t v, sites())
	if (get(vertex_color, _g, v) == color_traits_t::white())
	    (*this)(v) = alpha;

    return f;
}

//! �A���t�@�g����1��s���D
/*!
  alpha�łȂ����x�������T�C�g�ɂ��āC"���x����ς��Ȃ�"/"alpha�ɕς���"��
  2�l�ōŏ��J�b�g�����߁C���̌��ʂɉ����ă��x����t���ւ���D
  \param alpha		�g����̃��x��
  \param energyTerm	�T�C�g��ID�Ƃ��̃��x����^����ƃf�[�^�G�l���M�[�l��
			�Ԃ������o����їאڂ���2�̃T�C�g��ID�Ƃ�����
			���x����^����ƕ������G�l���M�[�l��Ԃ������o��2��
			�����֐��I�u�W�F�N�g
  \param alg		�ő�t���[�A���S���Y��
  \return		�A���t�@�g���ŒB�����ꂽ�ŏ��G�l���M�[�l
*/
template <class T, class ID, class L, class EL> template <class F>
typename GraphCuts<T, ID, L, EL>::value_type
GraphCuts<T, ID, L, EL>::alphaExpansion(label_type alpha, F energyTerm,
					Algorithm alg)
{
    using namespace	std;
    
  // ���ׂĂ̕ӂ̗e�ʂ�0�ɂ���D
    BOOST_FOREACH (edge_t e, edges(_g))
	put(edge_capacity, _g, e, 0);

  // alpha�łȂ����x���������_�ɐڑ�����ӂɃG�l���M�[�l��t�^����D
    value_type	bias = 0;		// �o�C�A�X
    BOOST_FOREACH (vertex_t u, sites())
    {
	const id_type		uid = id(u);
	const label_type	Xu  = (*this)(u);

	if (Xu != alpha)		// u �̃��x����alpha�łȂ����...
	{
	    const edge_t	es = get(vertex_sedge, _g, u),
				et = get(vertex_tedge, _g, u);

	    get(edge_capacity, _g)[es] += energyTerm(uid, alpha);
	    get(edge_capacity, _g)[et] += energyTerm(uid, Xu);
	    
	    BOOST_FOREACH (edge_t e, out_edges(u, _g))
	    {
		if (get(edge_smooth, _g, e))	// e ������������\���Ȃ�...
		{
		    const vertex_t	v   = target(e, _g);
		    const id_type	vid = id(v);
		    const label_type	Xv  = (*this)(v);
		    
		    if (Xv == alpha)	// e = (u, v), Xu != alpha, Xv = alpha
		    {
			get(edge_capacity, _g)[es] +=
			    energyTerm(uid, vid, alpha, alpha);
			get(edge_capacity, _g)[et] +=
			    energyTerm(uid, vid, Xu, alpha);
		    }
		    else		// e = (u, v), Xu != alpha, Xv != alpha
		    {
			const value_type
			    h00 = energyTerm(uid, vid,    Xu,    Xv),
			    h01 = energyTerm(uid, vid,    Xu, alpha),
			    h10 = energyTerm(uid, vid, alpha,    Xv),
			    h11 = energyTerm(uid, vid, alpha, alpha),
			    h   = h01 + h10 - h00 - h11;

			if (h < 0)
			    throw std::runtime_error("boost::GraphCuts<T, ID, L, EL>::alphaExpansion(): submodularity constraint is violated!");

			const edge_t	ev = get(vertex_tedge, _g, v);

			get(edge_capacity, _g)[es] += (h10 - h00);
			get(edge_capacity, _g)[ev] += (h10 - h11);
			get(edge_capacity, _g)[e ] += h;
		    
			bias -= (h - h01);
		    }
		}
	    }
	}
	else				// u �̃��x����alpha�Ȃ��...
	{
	    bias += energyTerm(uid, alpha);
	    
	    BOOST_FOREACH (edge_t e, out_edges(u, _g))
	    {
		if (get(edge_smooth, _g, e))
		{
		    const vertex_t	v   = target(e, _g);
		    const id_type	vid = id(v);
		    const label_type	Xv  = (*this)(v);

		    if (Xv == alpha)	// e = (u, v), Xu = Xv = alpha
			bias += energyTerm(uid, vid, alpha, alpha);
		    else		// e = (u, v), Xu = alpha, Xv != alpha
		    {
			const edge_t	es = get(vertex_sedge, _g, v),
					et = get(vertex_tedge, _g, v);
			
			get(edge_capacity, _g)[es] +=
			    energyTerm(uid, vid, alpha, alpha);
			get(edge_capacity, _g)[et] +=
			    energyTerm(uid, vid, alpha, Xv);
		    }
		}
	    }
	}
    }

  // _s ���痬��o���ӂ� _t �ɗ��ꍞ�ޕӂ̗e�ʂ��񕉂ɂȂ�悤�ɒ�������D
    BOOST_FOREACH (vertex_t v, sites())
    {
	const edge_t	es = get(vertex_sedge, _g, v),
			et = get(vertex_tedge, _g, v);
	value_type&	cs = get(edge_capacity, _g)[es];
	value_type&	ct = get(edge_capacity, _g)[et];

	if (cs < ct)
	{
	    bias += cs;
	    ct -= cs;
	    cs  = 0;
	}
	else
	{
	    bias += ct;
	    cs -= ct;
	    ct  = 0;
	}
    }
    
  // �ő�t���[�ƍŏ��J�b�g�����߂�D
    return bias + maxFlow(alpha, alg);
}

template <class T, class ID, class L, class EL> void
GraphCuts<T, ID, L, EL>::check() const
{
    using namespace	std;
    
    BOOST_FOREACH (edge_t e, edges(_g))
	if (get(edge_smooth, _g, e))
	    if (flow(e) + flow(get(edge_reverse, _g, e)) != 0)
		cerr << "Inconsistent flow!" << endl;
    
    value_type	f = 0;
    BOOST_FOREACH (edge_t e, out_edges(_s, _g))
	f += flow(e);
    cerr << " s => " << f << ", ";
    
  // xxx_max_flow() ���Ԃ��ő�t���[�l�����������`�F�b�N����D
    f = 0;
    BOOST_FOREACH (edge_t e, edges(_g))
    {
	vertex_t	u = source(e, _g), v = target(e, _g);

#if 1
	if (get(vertex_color, _g, u) == color_traits_t::black() &&
	    get(vertex_color, _g, v) != color_traits_t::black())
#else	
	if (get(vertex_color, _g, u) != color_traits_t::white() &&
	    get(vertex_color, _g, v) == color_traits_t::white())
#endif
	{
	    f += get(edge_capacity, _g, e);

	    if (get(edge_residual_capacity, _g, e) != 0)
	    {
		edge_t	er = get(edge_reverse, _g, e);
		cerr << "\tc(" << u << ',' << v << ") = "
		     << get(edge_capacity, _g, e)
		     << ", r(" << u << ',' << v << ") = "
		     << get(edge_residual_capacity, _g, e)
		     << ", c(" << v << ',' << u << ") = "
		     << get(edge_capacity, _g, er)
		     << ", r(" << v << ',' << u << ") = "
		     << get(edge_residual_capacity, _g, er)
		     << endl;
	    }
	}
    }
    cerr << "cut = " << f << ", ";

    f = 0;
    BOOST_FOREACH (edge_t e, out_edges(_t, _g))
	f += flow(get(edge_reverse, _g, e));
    cerr << f << " => t" << endl;
}
    
/*
 * ------------------------------- ���o�� -------------------------------
 */
//! ���̓X�g���[������ő�t���[����ǂݍ��ށD
template <class T, class ID, class L, class EL> std::istream&
GraphCuts<T, ID, L, EL>::getDimacsMaxFlow(std::istream& in)
{
  // �S�Ă̕ӂƊJ�n�_�ƏI�[�_���܂ޑS�Ă̒��_����������D
    _g.clear();

    read_dimacs_max_flow(_g, get(edge_capacity, _g), get(edge_reverse, _g),
			 _s, _t, in);
    return in;
}

//! �o�̓X�g���[���ɍő�t���[�����������ށD
template <class T, class ID, class L, class EL> std::ostream&
GraphCuts<T, ID, L, EL>::putDimacsMaxFlow(std::ostream& out) const
{
    using namespace	std;

#if 1
    out << "p max\t" << num_vertices(_g) << '\t' << num_edges(_g)/2 << endl;

    out << "n\t" << _s + 1 << " s" << endl;	// �J�n�_
    out << "n\t" << _t + 1 << " t" << endl;	// �I�[�_

    int	nedges = 0;
    BOOST_FOREACH (vertex_t v, sites())
    {
	edge_t	es = get(vertex_sedge, _g, v),
		et = get(vertex_tedge, _g, v);
	
	out << "a\t" << _s + 1 << '\t' << v + 1
	    << '\t' << get(edge_capacity, _g, es) << endl;
	out << "a\t" << v + 1 << '\t' << _t + 1
	    << '\t' << get(edge_capacity, _g, et) << endl;
	nedges += 2;
	
	BOOST_FOREACH (edge_t e, out_edges(v, _g))
	{
	    if (get(edge_smooth, _g, e))
	    {
		out << "a\t" << v + 1 << '\t' << target(e, _g) + 1
		    << '\t' << get(edge_capacity, _g, e) << endl;
		++nedges;
	    }
	}
    }
    cerr << "#v = " << num_vertices(_g)
	 << ", #e(!zero)/#e = " << nedges << '/' << num_edges(_g)/2
	 << endl;
#else
    int	nNonzeroEdges = 0;
    BOOST_FOREACH (edge_t e, edges(_g))
    {
	if (get(edge_capacity, _g, e) > 0)
	    ++nNonzeroEdges;
    }

    out << "p max\t" << num_vertices(_g) << '\t' << nNonzeroEdges << endl;

    out << "n\t" << _s + 1 << " s" << endl;	// �J�n�_
    out << "n\t" << _t + 1 << " t" << endl;	// �I�[�_

    BOOST_FOREACH (edge_t e, edges(_g))
    {
	const value_type	c = get(edge_capacity, _g, e);
	if (c > 0)
	    out << "a\t" << source(e, _g) + 1 << '\t' << target(e, _g) + 1
		<< '\t' << c << endl;
    }
    cerr << "#v = " << num_vertices(_g)
	 << ", #e(!zero)/#e = " << nNonzeroEdges << '/' << num_edges(_g)
	 << endl;
#endif    
    return out;
}

template <class T, class ID, class L, class EL> std::ostream&
GraphCuts<T, ID, L, EL>::putCapacities(std::ostream& out) const
{
    using namespace	std;

    BOOST_FOREACH (vertex_t v, vertices(_g))
    {
	BOOST_FOREACH (edge_t e, out_edges(v, _g))
	{
	    out << "c(" << v << ", " << target(e, _g) << ") = "
		<< get(edge_capacity, _g, e)
		<< endl;
	}
    }

    return out;
}
    
template <class T, class ID, class L, class EL> std::ostream&
GraphCuts<T, ID, L, EL>::putMaxFlow(std::ostream& out) const
{
    using namespace	std;

    BOOST_FOREACH (vertex_t v, vertices(_g))
    {
	BOOST_FOREACH (edge_t e, out_edges(v, _g))
	{
	    value_type	c = get(edge_capacity, _g, e);
	    
	    if (c > 0)
	    {
		value_type	r = get(edge_residual_capacity, _g, e);

		out << "f(" << v << ", " << target(e, _g) << ") = "
		    << c - r;
		if (r == 0)
		    out << " *";
		out << endl;
	    }
	}
    }

    return out;
}
    
template <class T, class ID, class L, class EL> std::ostream&
GraphCuts<T, ID, L, EL>::putMinCut(std::ostream& out) const
{
    using namespace	std;

    BOOST_FOREACH (vertex_t v, vertices(_g))
    {
	out << "p[" << v << "] = "
	    << (get(vertex_color, _g, v) == color_traits_t::white() ? 1 : 0)
	    << endl;
    }

    return out;
}

/*
 * ----------------------- private members -----------------------------
 */
template <class T, class ID, class L, class EL>
inline typename GraphCuts<T, ID, L, EL>::edge_t
GraphCuts<T, ID, L, EL>::createEdgePair(vertex_t u, vertex_t v)
{
    edge_t	e0 = add_edge(u, v, _g).first;
    edge_t	e1 = add_edge(v, u, _g).first;
    put(edge_capacity, _g, e0, 0);
    put(edge_capacity, _g, e1, 0);
    put(edge_residual_capacity, _g, e0, 0);
    put(edge_residual_capacity, _g, e1, 0);
    put(edge_reverse,  _g, e0, e1);
    put(edge_reverse,  _g, e1, e0);
    put(edge_smooth,   _g, e0, false);
    put(edge_smooth,   _g, e1, false);

    return e0;
}
    
template <class T, class ID, class L, class EL> inline void
GraphCuts<T, ID, L, EL>::computeMinCut()
{
  // �S���_�𔒂ɓh��D
    BOOST_FOREACH (vertex_t v, vertices(_g))
	put(vertex_color, _g, v, color_traits_t::white());

  // �J�n�_����O�a���Ă��Ȃ��ӂ�ʂ��ē��B�ł��钸�_�����ɓh��D
    std::stack<vertex_t>	stack;	// �[���D��T���̂��߂̃X�^�b�N
    stack.push(_s);
    
    while (!stack.empty())		// �[���D��T��
    {
	vertex_t	u = stack.top();
	stack.pop();

	if (get(vertex_color, _g, u) != color_traits_t::black())
	{
	    put(vertex_color, _g, u, color_traits_t::black());

	    BOOST_FOREACH (edge_t e, out_edges(u, _g))
	    {
		vertex_t	v = target(e, _g);
		
		if ((get(vertex_color, _g, v) != color_traits_t::black()) &&
		    (get(edge_residual_capacity, _g, e) > 0))
		    stack.push(v);
	    }
	}
    }
}

template <class T, class ID, class L, class EL>
inline typename GraphCuts<T, ID, L, EL>::value_type
GraphCuts<T, ID, L, EL>::flow(edge_t e) const
{
    return get(edge_capacity, _g, e) - get(edge_residual_capacity, _g, e);
}

}
#endif	// !__GRAPHCUTS_H
