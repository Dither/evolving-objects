// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// BitOp.h
// (c) GeNeura Team, 2000 - EEAAX 2000 - Maarten Keijzer 2000
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
             Marc.Schoenauer@polytechnique.fr
             mak@dhi.dk
 CVS Info: $Date: 2007-08-21 14:52:50 $ $Header: /home/nojhan/dev/eodev/eodev_cvs/eo/src/ga/BitOp.h,v 1.18 2007-08-21 14:52:50 kuepper Exp $ $Author: kuepper $
 */
//-----------------------------------------------------------------------------

#ifndef BitOp_h
#define BitOp_h

//-----------------------------------------------------------------------------

#include <algorithm>    // swap_ranges
#include <utils/RNG.h>
#include <Init.h>       // MonOp
#include <ga/Bit.h>

namespace eo
{

    /** BitFlip --> changes 1 bit
	\class BitBitFlip BitOp.h ga/BitOp.h
	\ingroup bitstring

	@ingroup Variators
    */

    template<class Chrom> class OneBitFlip: public MonOp<Chrom>
    {
    public:
	/// The class name.
	virtual std::string className() const { return "OneBitFlip"; }

	/**
	 * Change one bit.
	 * @param chrom The cromosome which one bit is going to be changed.
	 */
	bool operator()(Chrom& chrom)
	{
	    unsigned i = rng.random(chrom.size());
	    chrom[i] = (chrom[i]) ? false : true;
	    return true;
	}
    };

    /** DetBitFlip --> changes exactly k bits
	\class DetBitFlip BitOp.h ga/BitOp.h
	\ingroup bitstring
    */

    template<class Chrom> class DetBitFlip: public MonOp<Chrom>
    {
    public:
	/**
	 * (Default) Constructor.
	 * @param _num_bit The number of bits to change
	 * default is one - equivalent to OneBitFlip then
	 */
	DetBitFlip(const unsigned& _num_bit = 1): num_bit(_num_bit) {}

	/// The class name.
	virtual std::string className() const { return "DetBitFlip"; }

	/**
	 * Change num_bit bits.
	 * @param chrom The cromosome which one bit is going to be changed.
	 */
	bool operator()(Chrom& chrom)
	{
	    // does not check for duplicate: if someone volunteers ....
	    for (unsigned k=0; k<num_bit; k++)
		{
		    unsigned i = rng.random(chrom.size());
		    chrom[i] = (chrom[i]) ? false : true;
		}
	    return true;
	}
    private:
	unsigned num_bit;
    };


    /** BitMutation --> classical mutation
	\class BitMutation BitOp.h ga/BitOp.h
	\ingroup bitstring
    */

    template<class Chrom> class BitMutation: public MonOp<Chrom>
    {
    public:
	/**
	 * (Default) Constructor.
	 * @param _rate Rate of mutation.
	 * @param _normalize use rate/chrom.size if true
	 */
	BitMutation(const double& _rate = 0.01, bool _normalize=false):
	    rate(_rate), normalize(_normalize) {}

	/// The class name.
	virtual std::string className() const { return "BitMutation"; }

	/**
	 * Mutate a chromosome.
	 * @param chrom The chromosome to be mutated.
	 */
	bool operator()(Chrom& chrom)
	{
	    double actualRate = (normalize ? rate/chrom.size() : rate);
	    bool changed_something = false;
	    for (unsigned i = 0; i < chrom.size(); i++)
		if (rng.flip(actualRate))
		    {
			chrom[i] = !chrom[i];
			changed_something = true;
		    }

	    return changed_something;
	}

    private:
	double rate;
	bool normalize;		   // divide rate by chromSize
    };


    /** BitInversion: inverts the bits of the chromosome between an interval
	\class BitInversion BitOp.h ga/BitOp.h
	\ingroup bitstring
    */

    template<class Chrom> class BitInversion: public MonOp<Chrom>
    {
    public:
	/// The class name.
	virtual std::string className() const { return "BitInversion"; }

	/**
	 * Inverts a range of bits in a binary chromosome.
	 * @param chrom The chromosome whos bits are going to be inverted (a range).
	 */
	bool operator()(Chrom& chrom)
	{

	    unsigned u1 = rng.random(chrom.size() + 1) , u2;
	    do u2 = rng.random(chrom.size() + 1); while (u1 == u2);
	    unsigned r1 = std::min(u1, u2), r2 = std::max(u1, u2);

	    std::reverse(chrom.begin() + r1, chrom.begin() + r2);
	    return true;
	}
    };


    /** BitNext --> next value when bitstring considered as binary value
	\class BitNext BitOp.h ga/BitOp.h
	\ingroup bitstring
    */

    template<class Chrom> class BitNext: public MonOp<Chrom>
    {
    public:
	/// The class name.
	virtual std::string className() const { return "BitNext"; }

	/**
	 * Change the bit std::string x to be x+1.
	 * @param chrom The chromosome to be added one.
	 */
	bool operator()(Chrom& chrom)
	{
	    for (int i = chrom.size() - 1; i >= 0; i--)
		if (chrom[i])
		    {
			chrom[i] = 0;
			continue;
		    }
		else
		    {
			chrom[i] = 1;
			break;
		    }

	    return true;
	}
    };


    /** BitPrev --> previous value when bitstring treated as binary value
	\class BitPrev BitOp.h ga/BitOp.h
	\ingroup bitstring
    */

    template<class Chrom> class BitPrev: public MonOp<Chrom>
    {
    public:
	/// The class name.
	virtual std::string className() const { return "BitPrev"; }

	/**
	 * Change the bit std::string x to be x-1.
	 * @param chrom The chromosome to be substracted one.
	 */
	bool operator()(Chrom& chrom)
	{
	    for (int i = chrom.size() - 1; i >= 0; i--)
		if (chrom[i])
		    {
			chrom[i] = 0;
			break;
		    }
		else
		    {
			chrom[i] = 1;
			continue;
		    }

	    return true;
	}
    };


    /** 1PtBitXover --> classic 1-point crossover
	\class 1PtBitCrossover BitOp.h ga/BitOp.h
	\ingroup bitstring
    */

    template<class Chrom> class OnePtBitXover: public QuadOp<Chrom>
    {
    public:
	/// The class name.
	virtual std::string className() const { return "1PtBitXover"; }

	/**
	 * 1-point crossover for binary chromosomes.
	 * @param chrom1 The first chromosome.
	 * @param chrom2 The first chromosome.
	 */
	bool operator()(Chrom& chrom1, Chrom& chrom2)
	{
	    unsigned site = rng.random(std::min(chrom1.size(), chrom2.size()));

	    if (!std::equal(chrom1.begin(), chrom1.begin()+site, chrom2.begin()))
		{

		    std::swap_ranges(chrom1.begin(), chrom1.begin() + site, chrom2.begin());

		    return true;
		}
	    return false;
	}
    };


    /** UBitXover --> classic Uniform crossover
	\class UBitXover BitOp.h ga/BitOp.h
	\ingroup bitstring
    */

    template<class Chrom> class UBitXover: public QuadOp<Chrom>
    {
    public:
	/// (Default) Constructor.
	UBitXover(const float& _preference = 0.5): preference(_preference)
	{
	    if ( (_preference <= 0.0) || (_preference >= 1.0) )
		std::runtime_error("UxOver --> invalid preference");
	}
	/// The class name.
	virtual std::string className() const { return "UBitXover"; }

	/**
	 * Uniform crossover for binary chromosomes.
	 * @param chrom1 The first chromosome.
	 * @param chrom2 The first chromosome.
	 *    std::runtime_error if sizes don't match
	 */
	bool operator()(Chrom& chrom1, Chrom& chrom2)
	{
	    if ( chrom1.size() != chrom2.size())
		std::runtime_error("UxOver --> chromosomes sizes don't match" );
	    bool changed = false;
	    for (unsigned int i=0; i<chrom1.size(); i++)
		{
		    if (chrom1[i] != chrom2[i] && rng.flip(preference))
			{
			    bool tmp = chrom1[i];
			    chrom1[i]=chrom2[i];
			    chrom2[i] = tmp;
			    changed = true;
			}
		}
	    return changed;
	}
    private:
	float preference;
    };


    /** NPtsBitXover --> n-point crossover
	\class NPtsBitXover BitOp.h ga/BitOp.h
	\ingroup bitstring
    */
    template<class Chrom> class NPtsBitXover : public QuadOp<Chrom>
    {
    public:

	/** (Default) Constructor. */
	NPtsBitXover(const unsigned& _num_points = 2) : num_points(_num_points)
	{
	    if (num_points < 1)
		std::runtime_error("NxOver --> invalid number of points");
	}

	/** The class name */
	virtual std::string className() const { return "NPtsBitXover"; }

	/** n-point crossover for binary chromosomes.

	    @param chrom1 The first chromosome.
	    @param chrom2 The first chromosome.
	*/
	bool operator()(Chrom& chrom1, Chrom& chrom2) {
	    unsigned max_size(std::min(chrom1.size(), chrom2.size()));
	    unsigned max_points(std::min(max_size - 1, num_points));
	    std::vector<bool> points(max_size, false);

	    // select ranges of bits to swap
	    do {
		unsigned bit(rng.random(max_size));
		if(points[bit])
		    continue;
		else {
		    points[bit] = true;
		    --max_points;
		}
	    } while(max_points);

	    // swap bits between chromosomes
	    bool change(false);
	    for (unsigned bit = 1; bit < points.size(); bit++) {
		if (points[bit])
		    change = !change;
		if (change) {
		    typename Chrom::AtomType tmp = chrom1[bit];
		    chrom1[bit] = chrom2[bit];
		    chrom2[bit] = tmp;
		}
	    }
	    return true;
	}

    private:

	/** @todo Document this data member */
	unsigned num_points;
    };



    /** BitGxOver --> Npts crossover when bistd::string considered
	as a std::string of binary-encoded genes (exchanges genes)
	Is anybody still using it apart from historians ??? :-)
	\class BitGxOver BitOp.h ga/BitOp.h
	\ingroup bitstring
    */

    template<class Chrom> class BitGxOver: public QuadOp<Chrom>
    {
    public:
	/// Constructor.
	BitGxOver(const unsigned _gene_size, const unsigned _num_points = 2):
	    gene_size(_gene_size), num_points(_num_points)
	{
	    if (gene_size < 1)
		std::runtime_error("GxOver --> invalid gene size");
	    if (num_points < 1)
		std::runtime_error("GxOver --> invalid number of points");
	}

	/// The class name
	virtual std::string className() const { return "BitGxOver"; }

	/**
	 * Gene crossover for binary chromosomes.
	 * @param chrom1 The first chromosome.
	 * @param chrom2 The first chromosome.
	 */
	bool operator()(Chrom& chrom1, Chrom& chrom2)
	{
	    unsigned max_genes = std::min(chrom1.size(), chrom2.size()) / gene_size;
	    unsigned cut_genes = std::min(max_genes, num_points);

	    std::vector<bool> points(max_genes, false);

	    // selects genes to swap
	    do {
		unsigned bit = rng.random(max_genes);
		if (points[bit])
		    continue;
		else
		    {
			points[bit] = true;
			cut_genes--;
		    }
	    } while (cut_genes);

	    // swaps genes
	    for (unsigned i = 0; i < points.size(); i++)
		if (points[i])
		    std::swap_ranges(chrom1.begin() + i * gene_size,
				     chrom1.begin() + i * gene_size + gene_size,
				     chrom2.begin() + i * gene_size);

	    return true;
	}

    private:
	unsigned gene_size;
	unsigned num_points;
    };

}

//-----------------------------------------------------------------------------
//@}
#endif

