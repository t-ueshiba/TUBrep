/*
 *  $Id: GraphCuts.h,v 1.2 2012-01-03 23:32:16 ueshiba Exp $
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
BOOST_INSTALL_PROPERTY(vertex, id);
BOOST_INSTALL_PROPERTY(vertex, label);
BOOST_INSTALL_PROPERTY(vertex, sedge);
BOOST_INSTALL_PROPERTY(vertex, tedge);
#ifdef WITH_DUMMY_EDGES
enum edge_dummy_t	{edge_dummy};		// �_�~�[�̕�
BOOST_INSTALL_PROPERTY(edge, dummy);
#endif
    
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
#ifdef WITH_DUMMY_EDGES
	,
	EdmondsKarp,			//!< Edmonds-Karp�A���S���Y��
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
	    property<edge_reverse_t,		edge_t
#ifdef WITH_DUMMY_EDGES
	  , property<edge_dummy_t,		bool>
#endif
		     > > > >				EdgeProps;
    typedef adjacency_list<EL, vecS, directedS,
			   VertexProps, EdgeProps>	graph_t;
    typedef typename graph_traits<graph_t>::vertex_iterator
							vertex_iterator;
    typedef typename graph_traits<graph_t>::out_edge_iterator
							out_edge_iterator;

  //! �ő�t���[�����߂���ɂ���ɑΉ�����ŏ��J�b�g��T������N���X
    class MinCutVisitor : public default_bfs_visitor
    {
      public:
	MinCutVisitor(vertex_t s)					;

	void		initialize_vertex(vertex_t v, const graph_t& g)	;
	void		examine_vertex(vertex_t v, const graph_t& g)	;
	void		examine_edge(edge_t e, const graph_t& g)	;
#ifdef _DEBUG
	void		discover_vertex(vertex_t v, const graph_t& g)	;
	void		tree_edge(edge_t e, const graph_t& g)		;
	void		non_tree_edge(edge_t e, const graph_t& g)	;
	void		gray_target(edge_t e, const graph_t& g)		;
	void		black_target(edge_t e, const graph_t& g)	;
	void		finish_vertex(vertex_t v, const graph_t& g)	;
#endif
      private:
	const vertex_t	_s;		//!< �J�n�_
	color_t		_color;		//!< ���ݒ��ׂĂ���ӂ̎n�_�̃}�[�N
	bool		_saturated;	//!< ���ݒ��ׂĂ���ӂ̖O�a�̗L��
    };

  public:
    typedef vertex_t			site_type;	//!< �T�C�g�̌^
    typedef std::pair<vertex_iterator,
		      vertex_iterator>	site_range;	//!< �T�C�g�͈̔�
    
  public:
  // �\���̐����Ɣj��
    GraphCuts()								;
    site_type		createDataTerm(const id_type id)		;
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
    value_type		maxFlow(label_type alpha, Algorithm alg)	;
    template <class F>
    value_type		alphaExpansion(label_type alpha, F energyTerm,
				       Algorithm alg)			;

  // ���o��
    std::istream&	getDimacsMaxFlow(std::istream& in)		;
    std::ostream&	putCapacities(std::ostream& out)	const	;
    std::ostream&	putMaxFlow(std::ostream& out)		const	;
    std::ostream&	putMinCut(std::ostream& out)		const	;
    
  private:
    edge_t		createEdgePair(vertex_t u, vertex_t v)		;
    
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
GraphCuts<T, ID, L, EL>::createDataTerm(const id_type id)
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
    createEdgePair(u, v);
#ifdef WITH_DUMMY_EDGES
    createEdgePair(v, u);
#endif
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
#ifdef WITH_DUMMY_EDGES
    BOOST_FOREACH (edge_t e, edge_range(u, v, _g))
	if (!get(edge_dummy, _g, e))
	    return true;
    return false;
#else
    return edge(u, v, _g).second;
#endif
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
    return get(edge_capacity, _g)[e];
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
    return get(edge_capacity, _g)[e];
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
#ifdef WITH_DUMMY_EDGES
    BOOST_FOREACH (edge_t e, out_edges(u, _g))
	if (target(e, _g) == v && !get(edge_dummy, _g, e))
	    return get(edge_capacity, _g)[e];
    return 0;
#else
    edge_t	e;
    bool	exists;
    tie(e, exists) = edge(u, v, _g);
    return (exists ? get(edge_capacity, _g)[e] : 0);
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
#ifdef WITH_DUMMY_EDGES
    BOOST_FOREACH (edge_t e, out_edges(u, _g))
	if (target(e, _g) == v && !get(edge_dummy, _g, e))
	    return get(edge_capacity, _g)[e];
    throw std::runtime_error("GraphCuts<T, ID, L, EL>::energy(): non-existing smoothing term!");
    return get(edge_capacity, _g)[get(vertex_sedge, _g, u)];  // �����ɂ͓��B����
#else
    edge_t	e;
    bool	exists;
    tie(e, exists) = edge(u, v, _g);
    if (!exists)
	throw std::runtime_error("GraphCuts<T, ID, L, EL>::energy(): non-existing smoothing term!");
    return get(edge_capacity, _g)[e];
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
    value_type	val2 = 0;				// �G�l���M�[�l��2�{
    BOOST_FOREACH (vertex_t u, sites())
    {
	const id_type		uid = get(vertex_id,	_g, u);
	const label_type	Xu  = get(vertex_label, _g, u);

	BOOST_FOREACH (edge_t e, out_edges(u, _g))
	{
	    const vertex_t	v = target(e, _g);

	    if (v == _s || v == _t)
		val2 += energyTerm(uid, Xu);	// �f�[�^����2��J�E���g�����D
	    else
#ifdef WITH_DUMMY_EDGES
	    if (!get(edge_dummy, _g, e))
#endif
	      // ��������: �S�̂ł� (u, v) �� (v, u) ��2��J�E���g�����D
		val2 += energyTerm(uid, get(vertex_id,    _g, v),
				   Xu,  get(vertex_label, _g, v));
	}
    }
    
    return val2 / 2;	// 1/2�{���Ė{���̃G�l���M�[�l�ɖ߂�
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
    value_type	flow;
    switch (alg)
    {
#ifdef WITH_DUMMY_EDGES
      case EdmondsKarp:
	flow = edmonds_karp_max_flow(_g, _s, _t);
	breadth_first_search(_g, _s, visitor(MinCutVisitor(_s)));
	break;
      case PushRelabel:
	flow = push_relabel_max_flow(_g, _s, _t);
	breadth_first_search(_g, _s, visitor(MinCutVisitor(_s)));
	break;
#endif
      default:
	flow = boykov_kolmogorov_max_flow(_g, _s, _t);
    }

  // �I�[�_���̒��_�Ƀ��x���lalpha��^����D
    BOOST_FOREACH (vertex_t v, sites())
    {
	if (get(vertex_color, _g, v) != color_traits_t::black())
	    put(vertex_label, _g, v, alpha);
    }
#ifdef _DEBUG
  // xxx_max_flow() ���Ԃ��ő�t���[�l�����������`�F�b�N����D
    value_type	computed_flow = 0;
    BOOST_FOREACH (edge_t e, edges(_g))
    {
	if (get(vertex_color, _g, source(e, _g)) == color_traits_t::black() &&
	    get(vertex_color, _g, target(e, _g)) != color_traits_t::black())
	    computed_flow += (get(edge_capacity, _g, e) -
			      get(edge_residual_capacity, _g, e));
    }

    std::cerr << "flow(diff) = " << flow
	      << '(' << computed_flow - flow << "), ";
#endif
    return flow;
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
    value_type	bias2 = 0;		// �o�C�A�X��2�{
    BOOST_FOREACH (vertex_t u, sites())
    {
	const id_type		uid = get(vertex_id,	_g, u);
	const label_type	Xu  = get(vertex_label, _g, u);

	if (Xu != alpha)		// u �̃��x����alpha�łȂ����...
	{
	    BOOST_FOREACH (edge_t e, out_edges(u, _g))
	    {
		const vertex_t	v = target(e, _g);
		
		if (v == _s)		// e = (u, _s), Xu != alpha
		{
		    const edge_t	er = get(edge_reverse, _g, e);
		    get(edge_capacity, _g)[er] += 2*energyTerm(uid, alpha);
		}
		else if (v == _t)	// e = (u, _t), Xu != alpha
		{
		    get(edge_capacity, _g)[e] += 2*energyTerm(uid, Xu);
		}
		else			// e = (u, v), Xu != alpha
#ifdef WITH_DUMMY_EDGES
		if (!get(edge_dummy, _g, e))
#endif
		{
		    const id_type	vid = get(vertex_id,	_g, v);
		    const label_type	Xv  = get(vertex_label, _g, v);
		    const edge_t	es  = get(vertex_sedge, _g, u);
		    const edge_t	et  = get(vertex_tedge, _g, u);
		    
		    if (Xv == alpha)	// e = (u, v), Xu != alpha, Xv = alpha
		    {
			get(edge_capacity, _g)[es] +=
			    2*energyTerm(uid, vid, alpha, alpha);
			get(edge_capacity, _g)[et] +=
			    2*energyTerm(uid, vid, Xu, alpha);
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

			get(edge_capacity, _g)[es] += (h10 - h00);
			get(edge_capacity, _g)[et] += (h01 - h11);
			get(edge_capacity, _g)[e ] += h;
		    
			bias2 -= (h - h00 - h11);
		    }
		}
	    }
	}
	else				// u �̃��x����alpha�Ȃ��...
	{
	    BOOST_FOREACH (edge_t e, out_edges(u, _g))
	    {
		const vertex_t	v = target(e, _g);

		if (v == _s)		// e = (u, _s), Xu = alpha
		{
		    bias2 += 4*energyTerm(uid, alpha);
		}
		else if (v != _t)	// e = (u, v),  Xu = alpha
#ifdef WITH_DUMMY_EDGES
		if (!get(edge_dummy, _g, e))
#endif
		{
		    const id_type	vid = get(vertex_id,	_g, v);
		    const label_type	Xv  = get(vertex_label, _g, v);

		    if (Xv == alpha)	// e = (u, v), Xu = Xv = alpha
			bias2 += 2*energyTerm(uid, vid, alpha, alpha);
		}
	    }
	}
    }

  // _s ���痬��o���ӂ� _t �ɗ��ꍞ�ޕӂ̗e�ʂ��񕉂ɂȂ�悤�ɒ�������D
    BOOST_FOREACH (edge_t es, out_edges(_s, _g))
    {
	const edge_t	et = get(vertex_tedge, _g, target(es, _g));
	value_type&	cs = get(edge_capacity, _g)[es];
	value_type&	ct = get(edge_capacity, _g)[et];

	if (cs < ct)
	{
	    bias2 += 2*cs;
	    ct -= cs;
	    cs  = 0;
	}
	else
	{
	    bias2 += 2*ct;
	    cs -= ct;
	    ct  = 0;
	}
    }

  // �ő�t���[�ƍŏ��J�b�g�����߂�D
    value_type	flow = maxFlow(alpha, alg);
    
    return (flow + bias2/2) / 2;
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
	    << (get(vertex_color, _g, v) == color_traits_t::black() ? 0 : 1)
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
    put(edge_capacity, _g, e0, 0);
    edge_t	e1 = add_edge(v, u, _g).first;
    put(edge_capacity, _g, e1, 0);
    put(edge_reverse, _g, e0, e1);
    put(edge_reverse, _g, e1, e0);
#ifdef WITH_DUMMY_EDGES
    put(edge_dummy, _g, e0, false);
    put(edge_dummy, _g, e1, true);
#endif
    return e0;
}
    
/************************************************************************
*  class GraphCuts<T, ID, L, EL>::MinCutVisitor				*
************************************************************************/
template <class T, class ID, class L, class EL> inline
GraphCuts<T, ID, L, EL>::MinCutVisitor::MinCutVisitor(vertex_t s)
    :_s(s), _color(color_traits_t::black()), _saturated(false)
{
}

template <class T, class ID, class L, class EL> inline void
GraphCuts<T, ID, L, EL>::MinCutVisitor::initialize_vertex(vertex_t v,
							  const graph_t& g)
{
    put(vertex_color, const_cast<graph_t&>(g),
	v, (v != _s ? color_traits_t::white() : color_traits_t::black()));
}
    
template <class T, class ID, class L, class EL> inline void
GraphCuts<T, ID, L, EL>::MinCutVisitor::examine_vertex(vertex_t v,
						       const graph_t& g)
{
#ifdef _DEBUG
    using namespace	std;
    cerr << "examine vertex " << v << endl;
#endif
    _color = get(vertex_color, g, v);
}

template <class T, class ID, class L, class EL> inline void
GraphCuts<T, ID, L, EL>::MinCutVisitor::examine_edge(edge_t e,
						     const graph_t& g)
{
#ifdef _DEBUG
    using namespace	std;
    cerr << " examine edge (" << source(e, g) << ' ' << target(e, g) << ')';
#endif
  // �n�_�̃��x����0�ł��̕ӂ��O�a���Ă��Ȃ���ΏI�_�̃��x����0�ɂ���D
    if (_color == color_traits_t::black() &&
	get(edge_residual_capacity, g, e))
#ifdef _DEBUG
    {
	put(vertex_color, const_cast<graph_t&>(g),
	    target(e, g), color_traits_t::black());
	cerr << " *";
    }
    cerr << endl;
#else
	put(vertex_color, const_cast<graph_t&>(g),
	    target(e, g), color_traits_t::black());
#endif
}

#ifdef _DEBUG
template <class T, class ID, class L, class EL> inline void
GraphCuts<T, ID, L, EL>::MinCutVisitor::discover_vertex(vertex_t v,
							const graph_t& g)
{
    using namespace	std;
    
    cerr << "  discover vertex " << v << endl;
}

template <class T, class ID, class L, class EL> inline void
GraphCuts<T, ID, L, EL>::MinCutVisitor::tree_edge(edge_t e,
						  const graph_t& g)
{
    using namespace	std;

    cerr << "  tree edge (" << source(e, g) << ' ' << target(e, g) << ')'
	 << endl;
}

template <class T, class ID, class L, class EL> inline void
GraphCuts<T, ID, L, EL>::MinCutVisitor::non_tree_edge(edge_t e,
						      const graph_t& g)
{
    using namespace	std;

    cerr << "  non-tree edge (" << source(e, g) << ' ' << target(e, g) << ')'
	 << endl;
}

template <class T, class ID, class L, class EL> inline void
GraphCuts<T, ID, L, EL>::MinCutVisitor::gray_target(edge_t e,
						    const graph_t& g)
{
    using namespace	std;
    
    cerr << "   gray target (" << source(e, g) << ' ' << target(e, g) << ')'
	 << endl;
}

template <class T, class ID, class L, class EL> inline void
GraphCuts<T, ID, L, EL>::MinCutVisitor::black_target(edge_t e,
						     const graph_t& g)
{
    using namespace	std;
    
    cerr << "   black target (" << source(e, g) << ' ' << target(e, g) << ')'
	 << endl;
}

template <class T, class ID, class L, class EL> inline void
GraphCuts<T, ID, L, EL>::MinCutVisitor::finish_vertex(vertex_t v,
							const graph_t& g)
{
    using namespace	std;
    
    cerr << "finish vertex " << v << endl;
}
#endif

}
#endif	// !__GRAPHCUTS_H
