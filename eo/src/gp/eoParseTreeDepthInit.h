// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-
 
//-----------------------------------------------------------------------------
// eoParseTreeDepthInit.h : initializor for eoParseTree class
// (c) Maarten Keijzer 2000  
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
            
 */
//-----------------------------------------------------------------------------

#ifndef eoParseTreeDepthInit_h
#define eoParseTreeDepthInit_h

#include <EO.h>
#include <gp/eoParseTree.h>
#include <eoInit.h>
#include <eoOp.h>
#include <eoPop.h>

using namespace gp_parse_tree;
using namespace std;

// we need this for sorting the initializor vector
template <class Node>
bool lt_arity(const Node &node1, const Node &node2) 
{
     	return (node1.arity() < node2.arity());
}
 
/** eoGpDepthInitializer : the initializer class for eoParseTree
\class eoGpDepthInitializer eoParseTreeDepthInit.h gp/eoParseTreeDepthInit.h
\ingroup ParseTree
*/

template <class FType, class Node>
class eoGpDepthInitializer : public eoInit< eoParseTree<FType, Node> >
{
    public :

    typedef eoParseTree<FType, Node> EoType;
    
    /**
     * Constructor
     * @parm _max_depth The maximum depth of a tree
     * @param _initializor A vector containing the possible nodes
     * @param _grow False results in a full tree, True result is a randomly grown tree
     */
	eoGpDepthInitializer(
        unsigned _max_depth,
		const vector<Node>& _initializor,
        bool _grow = true)
            :
            eoInit<EoType>(),
			max_depth(_max_depth),
			initializor(_initializor),
			grow(_grow)
    {
      if(initializor.empty())
      {
        throw logic_error("eoGpDepthInitializer: uhm, wouldn't you rather give a non-empty set of Nodes?");
      }
      // lets sort the initializor vector according to  arity (so we can be sure the terminals are in front)
      // we use stable_sort so that if element i was in front of element j and they have the same arity i remains in front of j
      stable_sort(initializor.begin(), initializor.end(), lt_arity<Node>);
    }
        /// My class name
	virtual string className() const { return "eoDepthInitializer"; };

    /**initialize a tree
     * @param _tree : the tree to be initialized
     */
    void operator()(EoType& _tree)
	{
        list<Node> sequence;

        generate(sequence, max_depth);

        parse_tree<Node> tmp(sequence.begin(), sequence.end());
        _tree.swap(tmp);
	}
   private :
    void generate(list<Node>& sequence, int the_max, int last_terminal = -1)
    {
	    if (last_terminal == -1)
	    { // check where the last terminal in the sequence resides
            vector<Node>::iterator it;
		    for (it = initializor.begin(); it != initializor.end(); ++it)
		    {
			    if (it->arity() > 0)
				    break;
		    }
		
		    last_terminal = it - initializor.begin();
	    }

	    if (the_max == 1)
	    { // generate terminals only
		    vector<Node>::iterator it = initializor.begin() + rng.random(last_terminal);
		    sequence.push_front(*it);
		    return;
	    }
	
	    vector<Node>::iterator what_it;

	    if (grow)
	    {
		    what_it = initializor.begin() + rng.random(initializor.size());
	    }
	    else // full
	    {
		    what_it = initializor.begin() + last_terminal + rng.random(initializor.size() - last_terminal);
	    }

        what_it->randomize();

	    sequence.push_front(*what_it);

	    for (int i = 0; i < what_it->arity(); ++i)
		    generate(sequence, the_max - 1, last_terminal);
    }



     
	unsigned max_depth; 
    std::vector<Node> initializor;
	bool grow;
};

/**
     * A template function for ramped half and half initialization of an eoParseTree population
     * @param pop the population to be created
     * @param population_size the size of the population to be created
     * @param init_max_depth the initial maximum tree depth
     * @param initializor A vector containing the possible nodes
     
     \ingroup ParseTree
     */
template <class FType, class Node>
void  eoInitRampedHalfAndHalf(eoPop< eoParseTree<FType,Node> > &pop, unsigned int population_size, unsigned int init_max_depth, vector<Node> &initializor)
{
	typedef eoParseTree<FType,Node> EoType;
	typedef eoPop< EoType > Pop;
	
	unsigned int M = init_max_depth - 1;
	unsigned int part_pop_size = population_size / (2*M);
	unsigned int m=0;
	
	
	pop.clear();
	
	// initialize with Depth's (D) -> 2
	for(m=init_max_depth; m >= 2; m--)
	{
		eoGpDepthInitializer<FType, Node> grow_initializer(m, initializor, true);
		Pop grow(part_pop_size, grow_initializer);
		pop.insert(pop.begin(), grow.begin(), grow.end());
		
		eoGpDepthInitializer<FType, Node> full_initializer(m, initializor, false);
		Pop full(part_pop_size, full_initializer);
		pop.insert(pop.begin(), full.begin(), full.end());
	}	
	
	bool g = true;
	while (pop.size() < population_size)
	{
		eoGpDepthInitializer<FType, Node> initializer(init_max_depth, initializor, g);
		Pop p(1, initializer);
		pop.insert(pop.begin(), p.begin(), p.end());
		g= !g;
	}
}


#endif