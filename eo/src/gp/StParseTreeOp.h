// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-
 
//-----------------------------------------------------------------------------
// StParseTreeOp.h : crossover and mutation operators for  the strongly typed GP
// (c) Jeroen Eggermont 2001 for other mutation operators

/*
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 
    Contact: todos@geneura.ugr.es, http://geneura.ugr.es
    	     mak@dhi.dk 
             jeggermo@liacs.nl
 */
//-----------------------------------------------------------------------------

#ifndef StParseTreeOp_h
#define StParseTreeOp_h

#include <EO.h>
#include <Op.h>
#include <map.h>
#include <iostream>
#include <set>

#include <gp/ParseTree.h>

namespace eo
{

    // a help function
    template <class EOT>
    void get_possible_nodes(const EOT &_eo, std::vector<int> &possible_nodes, const int type)
    {
	int n=0;
	possible_nodes.clear();
	// collect the possible crossover points in _eo (nodes with the same type)
	for(n=0; n < _eo.size(); n++)
	    if (type == _eo[n]->type())
		possible_nodes.push_back(n);
    }	


    /** StSubtreeXOver --> subtree xover for strongly typed tree-based genetic programming
	\class StSubtreeXOver StParseTreeOp.h gp/StParseTreeOp.h 
	\ingroup StParseTree
    */
    template<class FType, class Node>
    class StSubtreeXOver: public QuadOp< ParseTree<FType, Node> > {
    public:

	typedef ParseTree<FType,Node> EoType;
	/**
	 * Constructor
	 * @param _max_length the maximum size of an individual
	 */
	StSubtreeXOver( unsigned _max_length)
	    : QuadOp<EoType>(), max_length(_max_length) {};

	/// the ckassname
	virtual std::string className() const { return "StSubtreeXOver"; };

	/// Dtor
	virtual ~StSubtreeXOver () {};

	/**
	 * Perform crossover on two individuals
	 * param _eo1 The first parent individual
	 * param _eo2 The second parent individual
	 */
	bool operator()(EoType & _eo1, EoType & _eo2 )
	{
	    int i = 0;
	    std::vector<int> nodes;
	    int n = 0;
	    int type = 0;
	    int j = 0;
	    std::set<int> test;
	    do
		{
		    do // select a random node in _eo1 as crossover point, and check if we didn't try it already
			{
			    i = rng.random(_eo1.size());
			}while(test.count(i) > 0);
		
		    test.insert(i);
		
		    type = _eo1[i]->type();
		
		    get_possible_nodes<EoType>(_eo2, nodes, type);
		
		}while(nodes.empty() && (test.size() < _eo1.size()));	
	 
	    if (nodes.empty()) // we failed to select a crossover point but tried all points (test.size() == _eo1.size()).
	 	return true;  // should this be false ??

	    // we did find at least one possible crossover point in _eo2
	 	
	    n = rng.random(nodes.size());
	    j = nodes[n];
	   

	
	    typename ParseTree<FType, Node>::subtree tmp = _eo1[i];
	    _eo1[i] = _eo2[j]; // insert subtree
	    _eo2[j] = tmp;

	    // we can't prune anymore
	    /*	
		_eo1.pruneTree(max_length);
		_eo2.pruneTree(max_length);
	    */
	 
	    return true;
	}
    private:
	unsigned max_length;
    };

    /** StBranchMutation --> replace a strongly typed subtree with a randomly created strongly typed subtree 
	\class StBranchMutation StParseTreeOp.h gp/StParseTreeOp.h
	\ingroup StParseTree
    */
    template<class FType, class Node>
    class StBranchMutation: public MonOp< ParseTree<FType, Node> >
    {
    public:

	typedef ParseTree<FType,Node> EoType;
	/**
	 * Constructor
	 * @param _init An instantiation of GpDepthInitializer
	 * @param _max_length the maximum size of an individual
	 */
	StBranchMutation(Init<EoType>& _init, unsigned _max_length)
	    : MonOp<EoType>(), max_length(_max_length), initializer(_init)
	{};
  
	/// the class name
	virtual std::string className() const { return "StBranchMutation"; };

	/// Dtor
	virtual ~StBranchMutation() {};
  
	/**
	 * Mutate an individual
	 * @param _eo1 The individual that is to be changed
	 */
	bool operator()(EoType& _eo1 )
	{
	    int i = rng.random(_eo1.size());
	    std::vector<int> nodes;
	    int type = _eo1[i]->type();
	    int j=0;
	    int n=0;

	    EoType eo2;
	  
	    do
		{
		    initializer(eo2);
		    get_possible_nodes(eo2, nodes, type);
		}while (nodes.empty());	

	    n = rng.random(nodes.size());
	    j = nodes[n];
	  
	    _eo1[i] = eo2[j]; // insert subtree
	  
	    // no more pruning
	    /*
	      _eo1.pruneTree(max_length);
	    */

	    return true;
	}

    private :

	unsigned max_length;
	Init<EoType>& initializer;
    };


    /** StPointMutation --> replace a Node with a Node of the same arity and type
	\class StPointMutation StParseTreeOp.h gp/StParseTreeOp.h
	\ingroup StParseTree
    */
    template<class FType, class Node>
    class StPointMutation: public MonOp< ParseTree<FType, Node> >
    {
    public:

	typedef ParseTree<FType,Node> EoType;

	/**
	 * Constructor
	 * @param _node The std::vector of Nodes given to the GpDepthInitializer
	 */
	StPointMutation( std::vector<Node>& _node)
	    : MonOp<EoType>()
	{
	    unsigned int i=0;
	    int arity=0;
	    int type=0;
	    std::vector<Node> node_vector;
	    for(i=0; i < _node.size(); i++)
		{
		    arity = _node[i].arity();
		    type = _node[i].type();
		
		    node_vector = node[type][arity];
		    node_vector.push_back(_node[i]);
		    node[type][arity]= node_vector;
		
		};
	};
  
	/// the class name
	virtual std::string className() const { return "StPointMutation"; };

	/// Dtor
	virtual ~StPointMutation() {};

	/**
	 * Mutate an individual
	 * @param _eo1 The individual that is to be changed
	 */
	bool operator()(EoType& _eo1 )
	{
	    // select a random node i that is to be mutated
	    int i = rng.random(_eo1.size());
	    int arity = _eo1[i].arity();
	    int type = _eo1[i]->type();
	    int j = rng.random(node[type][arity].size());

	
	    _eo1[i] = node[type][arity][j];
	    return true;
	}

    private :
	
	std::map < int, std::map < int, std::vector<Node> > > node;
    };

 
    /** StHoistMutation -->  replace the individual with one of its strongly typed subtree's 
	\class StHoistMutation StParseTreeOp.h gp/StParseTreeOp.h
	\ingroup StParseTree
    */
    template<class FType, class Node>
    class StHoistMutation: public MonOp< ParseTree<FType, Node> >
    {
    public:

	typedef ParseTree<FType,Node> EoType;
	/**
	 * Constructor
	 * @param _init An instantiation of StDepthInit
	 * @param _max_length the maximum size of an individual
	 */
	StHoistMutation(Init<EoType>& _init, unsigned _max_length)
	    : MonOp<EoType>(), max_length(_max_length), initializer(_init)
	{};
  
	/// the class name
	virtual std::string className() const { return "StHoistMutation"; };

	/// Dtor
	virtual ~StHoistMutation() {};
  
	/**
	 * Mutate an individual
	 * @param _eo1 The individual that is to be changed
	 */
	bool operator()(EoType& _eo1 )
	{

	    std::vector<int> nodes;
	    // get the type of the current tree
	    int type = _eo1[ _eo1.size() - 1 ]->type();

	    get_possible_nodes(_eo1, nodes, type);	
	  
	    // select a subtree-node to replace the current tree
	    int n = rng.random(nodes.size());
	    int i = nodes[n];
	  
	    EoType eo2(_eo1[i]);
	  
	    _eo1 = eo2;

	    return true;
	}

    private :

	unsigned max_length;
	Init<EoType>& initializer;
    };

}

#endif
