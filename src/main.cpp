//======================================================================================================================
//  Project: ShortestEngineerUnlocking
//  Author:  Youda008
//======================================================================================================================

#include "modules.hpp"
#include "engineers.hpp"

#include <cstdint>
#include <type_traits>
#include <utility>
	using std::move;
	using std::make_pair;
#include <algorithm>
#include <iostream>
	using std::istream;
	using std::ostream;
	using std::cin;
	using std::cout;
	using std::cerr;
	using std::endl;
#include <sstream>
	using std::istringstream;
#include <fstream>
	using std::ifstream;
#include <iomanip>
#include <cctype>  // isdigit, isspace
#include <string>
	using std::string;
#include <vector>
	using std::vector;
#include <unordered_set>
	using std::unordered_set;
	using std::unordered_multiset;
#include <unordered_map>
	using std::unordered_map;
	using std::unordered_multimap;
#include <cerrno>
#include <cstring>  // strerror


//======================================================================================================================
//  utils

template< typename Container, typename Value >
auto find( const Container & cont, const Value & val )
{
	return std::find( std::begin(cont), std::end(cont), val );
}

template< typename Container, typename Predicate >
auto findSuch( const Container & cont, const Predicate & pred )
{
	return std::find_if( std::begin(cont), std::end(cont), pred );
}

template< typename Container, typename Value >
bool contains( const Container & cont, const Value & val )
{
	return find( cont, val ) != std::end(cont);
}

template< typename Container, typename Predicate >
bool containsSuch( const Container & cont, const Predicate & pred )
{
	return findSuch( cont, pred ) != std::end(cont);
}

// for enabling templates only for integers or enums
template< typename Type >
struct is_int_or_enum
{
	static constexpr bool value = std::is_integral_v< Type > || std::is_enum_v< Type >;
};
template< typename Type > inline constexpr bool is_int_or_enum_v = is_int_or_enum< Type >::value;

// for universal handling of integers and enums together
template< typename Enum, typename std::enable_if_t< std::is_enum_v< Enum >, int > = 0  >
std::underlying_type_t< Enum > to_underlying( Enum val )
{
	return static_cast< std::underlying_type_t< Enum > >( val );
}

// just to avoid doing these ugly conversions everywhere
template< typename Enum, typename std::enable_if_t< std::is_enum_v< Enum >, int > = 0 >
Enum inc( Enum val )
{
	return Enum( to_underlying( val ) + 1 );
}

template< typename Enum, typename std::enable_if_t< std::is_enum_v< Enum >, int > = 0 >
bool operator<( Enum a, Enum b )
{
	return to_underlying(a) < to_underlying(b);
}

/// Helper object that will indent the output into std::ostream by N levels, where each level is 4 spaces.
/** Intended usage: std::cout << indent(5) << the_rest << std::endl; */
class indent
{
	size_t level;
 public:
	indent( size_t levelOfIndentation ) noexcept : level( levelOfIndentation ) {}
	friend std::ostream & operator<<( std::ostream & os, indent i ) noexcept
	{
		while (i.level --> 0)
			os << "    ";
		return os;
	}
};


//----------------------------------------------------------------------------------------------------------------------
/// Fast statically allocated array list
template< typename Value, size_t maxSize >
class FixedList
{
	size_t count = 0;
	Value array [maxSize];

 public:

	      Value & operator[]( size_t idx )       { return array[ idx ]; }
	const Value & operator[]( size_t idx ) const { return array[ idx ]; }

	      Value * begin()                        { return array; }
	      Value * end()                          { return array + count; }
	const Value * begin() const                  { return array; }
	const Value * end()   const                  { return array + count; }

	size_t size() const                          { return count; }
	bool empty() const                           { return count == 0; }

	void push_back( const Value & val )
	{
		array[ count ] = val;
		count += 1;
	}
	Value * erase( Value * val )
	{
		for (Value * current = val; current + 1 < end(); ++current)
		{
			*current = *(current + 1);
		}
		count -= 1;
		return val;
	}
	void remove( const Value & val )
	{
		auto newEnd = std::remove( begin(), end(), val );
		count -= end() - newEnd;
	}
};


//----------------------------------------------------------------------------------------------------------------------
/// Fast unordered set for index-like elements, using plain array for storage,
/// that remembers how many times a value was inserted and erasing it erases just one occurence.
//
//  A std::unordered_multiset isn't enough because one call to erase() erases all occurences
//  and iteration visits all multiplicated entries as many times as they are there.
//
//  We need the iteration to behave just like it was a standard set and erase() to just decrease the count,
//  in order to be able to restore the set to its previous state, before a batch of items was inserted.
//
//  We could just use simple C array or std::array directly and don't bother with this class,
//  but this way we can at least give it some prettier interface.
template< typename Index, Index endIndex >
class CountedIndexSet
{
 protected:

	// The count has signed so that it remembers when we try to delete an element that wasn't there,
	// and cancels out with trying to re-add it later.
	int array [ size_t(endIndex) ] {};  // initialize all indexes to 0

	class Iterator
	{
		const int * array;
		Index currentIdx;

	 public:

		Iterator( const int * array, Index startingIdx ) : array( array )
		{
			// move the current index to the nearest index that is present
			currentIdx = startingIdx;
			while (currentIdx < endIndex && array[ size_t(currentIdx) ] <= 0)
				currentIdx = inc( currentIdx );
		}

		auto operator*() const { return currentIdx; }
		auto operator->() const { return &currentIdx; }

		Iterator & operator++()
		{
			do
				currentIdx = inc( currentIdx );
			while (currentIdx < endIndex && array[ size_t(currentIdx) ] <= 0);
			return *this;
		}
		Iterator operator++(int)
		{
			auto previous = *this;
			++(*this);
			return previous;
		}

		friend bool operator==( const Iterator & a, const Iterator & b )
		{
			return a.array + a.currentIdx == b.array + b.currentIdx;
		}
		friend bool operator!=( const Iterator & a, const Iterator & b )
		{
			return a.array + a.currentIdx != b.array + b.currentIdx;
		}
	};

 public:

	CountedIndexSet() = default;
	CountedIndexSet( const CountedIndexSet< Index, endIndex > & other ) = default;
	CountedIndexSet< Index, endIndex > & operator=( const CountedIndexSet< Index, endIndex > & other ) = default;

	void insert( Index idx )
	{
		array[ size_t(idx) ] += 1;
	}

	void erase( Index idx )
	{
		array[ size_t(idx) ] -= 1;
	}

	bool contains( Index idx ) const
	{
		return array[ size_t(idx) ] > 0;
	}

	size_t size() const
	{
		// This is not called very often and the size of the array is in decades only, so we can afford to be linear.
		size_t size = 0;
		for (size_t idx = 0; idx < size_t(endIndex); ++idx)
			size += size_t( array[ idx ] > 0 );  // we're using bool as int, true is guaranteed to convert to 1
		return size;
	}

	bool empty() const
	{
		return size() == 0;
	}

	using const_iterator = Iterator;
	const_iterator begin() const { return const_iterator( array, Index(0) ); }
	const_iterator end() const   { return const_iterator( array, endIndex ); }
};


//----------------------------------------------------------------------------------------------------------------------
/// Fast unordered map for index-like elements, using plain array for storage.
//
//  We could just use simple C array or std::array directly and don't bother with this class,
//  but this way we can at least give it some prettier interface.
template< typename Index, Index endIndex, typename Value >
class IndexMap
{
	Value array [ size_t(endIndex) ] {};  // default-initialize everything

 public:

	      Value & operator[]( Index idx )       { return array[ size_t(idx) ]; }
	const Value & operator[]( Index idx ) const { return array[ size_t(idx) ]; }
};


//----------------------------------------------------------------------------------------------------------------------
/// Fast unordered multi-map for index-like elements, using plain array for storage.
//
//  We could just use simple C array or std::array directly and don't bother with this class,
//  but this way we can at least give it some prettier interface.
template< typename Index, Index endIndex, typename Value, size_t maxValues >
class IndexMultimap
{
	using Values = FixedList< Value, maxValues >;
	Values array [ size_t(endIndex) ] {};  // initialize counts of all indexes to 0

 public:

	const Values & operator[]( Index idx ) const { return array[ size_t(idx) ]; }

	void insert( Index idx, const Value & val )
	{
		array[ size_t(idx) ].push_back( val );
	}

	void erase( Index idx, const Value & val )
	{
		array[ size_t(idx) ].remove( val );
	}
};


//======================================================================================================================
//  algorithm

using EngineerList = FixedList< EngineerIdx, numOfEngineers >;
using EngineerSet = CountedIndexSet< EngineerIdx, EngineerIdx::_EndOfEnum >;

/// Finds all engineers that offer modification of specified grade to a specified module.
EngineerSet findEngineersOfferingModification( Modification desiredMod )
{
	EngineerSet selectedEngineers;

	for (EngineerIdx engineerIdx = firstEngineerIdx; engineerIdx <= lastEngineerIdx; engineerIdx = EngineerIdx( engineerIdx + 1 ))
	{
		auto & offeredMods = engineers[ engineerIdx ].modifications;

		if (containsSuch( offeredMods, [ &desiredMod ]( const Modification & offeredMod ) {
			return offeredMod.module == desiredMod.module && offeredMod.grade >= desiredMod.grade;
		}))
		{
			selectedEngineers.insert( EngineerIdx(engineerIdx) );
		}
	}

	return selectedEngineers;
}


//----------------------------------------------------------------------------------------------------------------------

struct DesiredMod : public Modification
{
	bool pinRequired;

	DesiredMod() {}
	DesiredMod( grade_t g, ModuleType m, bool p ) : Modification( g, m ), pinRequired(p) {}
};

static constexpr size_t maxModsPerEngineer = 12;
using EngineerModMultimap = IndexMultimap< EngineerIdx, EngineerIdx::_EndOfEnum, DesiredMod, maxModsPerEngineer >;

/// data related to one modification requested by the user
struct DesiredModContext
{
	DesiredMod mod;  ///< mod specification
	EngineerSet engineers;  ///< engineers offering this mod
};

struct Solution
{
	/// set of engineers required to be unlocked
	/** This must be a counted-set, so that we can easily restore the set to its previous state just by erasing the same
	  * elements we inserted before. */
	EngineerSet requiredEngineers;

	/// which engineer was added for which modification
	/** Key is the engineer, value is the list of modifications for which he was choses by the algorithm.
	  * It could be part of the set above, but then it would be problematic to restore the set to its previous state. */
	EngineerModMultimap relatedModifications;
};

/// intermediate results and support data
struct AlgorithmContext
{
	/// gradually constructed during the recursive combination generation
	Solution currentSolution;

	/// the best solution found so far
	Solution bestSolution;

	/// whether at least one valid solution was found
	/** We need this for verification that the user didn't enter requirements that are impossible to satisfy. */
	bool foundSolution;

	// all of the following is needed to track progress and display how much is remaining
	vector< size_t > originalSetSizes;  // we modify the sets during the algorithm, so these are the sizes before it started
	uint64_t combinationsTotal;  // number of all possible combinations of engineers
	uint64_t combinationsDone;  // number of all engineer combinations that were either tried or skipped
};

// needed to track progress and display how much is remaining
uint64_t getNumOfCombinations( const vector< size_t > & originalSetSizes, size_t recursionLevelsRemaining )
{
	uint64_t numOfCombinations = 1;
	size_t firstSetIdx = originalSetSizes.size() - recursionLevelsRemaining;
	for (size_t setIdx = firstSetIdx; setIdx < originalSetSizes.size(); ++setIdx)
	{
		numOfCombinations *= originalSetSizes[ setIdx ];
	}
	return numOfCombinations;
}

/// This compares to sets of engineers required to be unlocked and decides which one is faster to unlock.
bool isBetter( const Solution & solution1, const Solution & solution2 )
{
	// For simplicity we just compare the number of engineers in the set,
	// but we could account for the difficulty of unlocking each engineer.
	return solution1.requiredEngineers.size() < solution2.requiredEngineers.size();
}

/// Adds all engineers that are required to be unlocked to unlock this engineer, including this engineer himself.
void addEngineerWithAllRequirements( EngineerIdx engineerIdx, EngineerSet & requiredEngineers )
{
	EngineerIdx currentEngineerIdx = engineerIdx;
	while (currentEngineerIdx != EngineerIdx::None)
	{
		requiredEngineers.insert( currentEngineerIdx );
		currentEngineerIdx = engineers[ currentEngineerIdx ].requiredEngineer;
	}
}

/// Removes all engineers that are required to be unlocked to unlock this engineer, including this engineer himself.
void removeEngineerWithAllRequirements( EngineerIdx engineerIdx, EngineerSet & requiredEngineers )
{
	EngineerIdx currentEngineerIdx = engineerIdx;
	while (currentEngineerIdx != EngineerIdx::None)
	{
		requiredEngineers.erase( currentEngineerIdx );
		currentEngineerIdx = engineers[ currentEngineerIdx ].requiredEngineer;
	}
}

/// Removes an engineer from all the sets of engineers offering a mod that is required to be pinned
void removeEngineerFromAllSetsOfPinnedMods( EngineerIdx engineerIdx, DesiredModContext * currentModCtx, DesiredModContext * lastModCtx )
{
	while (currentModCtx <= lastModCtx)
	{
		if (currentModCtx->mod.pinRequired)
			currentModCtx->engineers.erase( engineerIdx );
		++currentModCtx;
	}
}

/// Re-inserts an engineer to all the sets of engineers offering a mod that is required to be pinned
void addEngineerToAllSetsOfPinnedMods( EngineerIdx engineerIdx, DesiredModContext * currentModCtx, DesiredModContext * lastModCtx )
{
	while (currentModCtx <= lastModCtx)
	{
		if (currentModCtx->mod.pinRequired)
			currentModCtx->engineers.insert( engineerIdx );
		++currentModCtx;
	}
}

/// The core of the algorithm, recursive function.
/** \param ctx                 intermediate results for the algorithm
  * \param currentEngineerSet  set of engineers offering the current modification
  * \param lastEngineerSet     set of engineers offering the last modification
  * Executes one level of dynamic cascade of nested for-loops that generates all combinations of engineers from
  * the given engineer sets, and calculates how many other engineers need to be unlocked to get access
  * to these engineers in these sets. */
uint64_t tryAllEngineerCombinations( AlgorithmContext & ctx, DesiredModContext * currentModCtx, DesiredModContext * lastModCtx )
{
	// needed to track progress and display how much is remaining
	uint64_t combinationsDoneLocally = 0;
	uint engineersTried = 0;

	// If all engineers offering this modification are already used for pinning the previous modifications,
	// this loop simply performs no iteration and this combination is not finished and evaluated.
	for (EngineerIdx engineerIdx : currentModCtx->engineers)
	{
		// add all the engineers that are required to be unlocked in order to unlock this one
		addEngineerWithAllRequirements( engineerIdx, ctx.currentSolution.requiredEngineers );
		// and link the main engineer to the desired modification
		ctx.currentSolution.relatedModifications.insert( engineerIdx, currentModCtx->mod );

		if (currentModCtx < lastModCtx)
		{
			// If the current mod is required to be pinned, remove the current engineer from
			// all the sets of engineers offering another mod that is required to be pinned,
			// because pinning more modifications from this engineer will not be possible.
			if (currentModCtx->mod.pinRequired)
				removeEngineerFromAllSetsOfPinnedMods( engineerIdx, currentModCtx + 1, lastModCtx );

			// continue with generating the rest of the combination
			combinationsDoneLocally += tryAllEngineerCombinations( ctx, currentModCtx + 1, lastModCtx );
			engineersTried += 1;  // needed to track progress and display how much is remaining

			// restore the previous state of the sets of engineers offering a mod that is required to be pinned
			if (currentModCtx->mod.pinRequired)
				addEngineerToAllSetsOfPinnedMods( engineerIdx, currentModCtx + 1, lastModCtx );
		}
		else
		{
			// whole combination has been generated, evaluate it
			if (isBetter( ctx.currentSolution, ctx.bestSolution ))
			{
				ctx.bestSolution = ctx.currentSolution;
			}
			ctx.foundSolution = true;
		}

		// restore the previous state of the set of engineers required to be unlocked
		removeEngineerWithAllRequirements( engineerIdx, ctx.currentSolution.requiredEngineers );
		ctx.currentSolution.relatedModifications.erase( engineerIdx, currentModCtx->mod );
	}


	// needed to track progress and display how much is remaining

	const auto printProgress = []( const AlgorithmContext & ctx )
	{
		double progress = double( ctx.combinationsDone ) / double( ctx.combinationsTotal ) * 100;
		cout << "\rcalculating...  progress: " << std::setw(5) << std::fixed << std::setprecision(1) << progress << '%';
		cout.flush();                             // good old printf, really
	};

	size_t recursionLevelsRemaining = lastModCtx - currentModCtx + 1;
	if (recursionLevelsRemaining < 10)
	{
		// No calculation, number of combinations tried will be calculated by the upper level.
		combinationsDoneLocally = 0;
	}
	else if (recursionLevelsRemaining == 10)
	{
		// Ignore value accumulated from lower levels, it's 0 anyway.
		combinationsDoneLocally = getNumOfCombinations( ctx.originalSetSizes, recursionLevelsRemaining );
		ctx.combinationsDone += combinationsDoneLocally;
		printProgress( ctx );
	}
	else // recursionLevelsRemaining > 10
	{
		size_t currentSetIdx = ctx.originalSetSizes.size() - recursionLevelsRemaining;
		if (engineersTried == ctx.originalSetSizes[ currentSetIdx ])
		{
			// All engineers from this set were tried, which means the ctx.combinationsDone incremented by lower levels
			// and combinationsDoneLocally accumulated from the lower levels is correct and progress was already printed,
			// so no action required.
		}
		else
		{
			// Some engineers from the current set were skipped, which means the ctx.combinationsDone and combinationsDoneLocally
			// were only partially incremented and both are missing the same number from the correct value,
			// so revert the global count using the local count and re-calculate it properly.
			ctx.combinationsDone -= combinationsDoneLocally;
			combinationsDoneLocally = getNumOfCombinations( ctx.originalSetSizes, recursionLevelsRemaining );
			ctx.combinationsDone += combinationsDoneLocally;
			// Also update the current progress, since by compensating for the skipped engineers, the number has increased.
			printProgress( ctx );
		}
	}
	return combinationsDoneLocally;
}

/// only a wrapper around the recursive function, performing required initialization
Solution findBestEngineerCombination( vector< DesiredModContext > & desiredModContexts )
{
	AlgorithmContext ctx;

	ctx.foundSolution = false;

	// Initialize the best set to the worst one (with all the engineers)
	// so that it's not evaluated as better than anything else, empty set would always be better than anything else.
	for (EngineerIdx engineerIdx = firstEngineerIdx; engineerIdx <= lastEngineerIdx; engineerIdx = inc( engineerIdx ))
		ctx.bestSolution.requiredEngineers.insert( engineerIdx );

	// needed to track progress and display how much is remaining
	ctx.originalSetSizes.reserve( desiredModContexts.size() );
	for (const auto & modCtx : desiredModContexts)
		ctx.originalSetSizes.push_back( modCtx.engineers.size() );
	ctx.combinationsTotal = getNumOfCombinations( ctx.originalSetSizes, ctx.originalSetSizes.size() );
	ctx.combinationsDone = 0;

	tryAllEngineerCombinations( ctx, &desiredModContexts.front(), &desiredModContexts.back() );

	if (ctx.foundSolution)
		return ctx.bestSolution;
	else
		return {};  // because empty set can never be a valid result, we can use this to indicate failure
}


//----------------------------------------------------------------------------------------------------------------------

/// Sorts the engineers according to their dependancies so that you can unlock them in the resulting order.
EngineerList orderTopologically( const EngineerSet & engineerSet )
{
	EngineerList orderedEngineers;

	const size_t numOfEngineers = engineerSet.size();

	EngineerList engineerList;
	for (EngineerIdx engineerIdx : engineerSet)
		engineerList.push_back( engineerIdx );

	IndexMap< EngineerIdx, EngineerIdx::_EndOfEnum, EngineerIdx > predecessors;
	IndexMap< EngineerIdx, EngineerIdx::_EndOfEnum, EngineerList > successors;
	for (EngineerIdx engineerIdx : engineerList)
	{
		EngineerIdx predecessor = engineers[ engineerIdx ].requiredEngineer;
		predecessors[ engineerIdx ] = predecessor;
		if (predecessor != EngineerIdx::None)
			successors[ predecessor ].push_back( engineerIdx );
	}

	while (orderedEngineers.size() < numOfEngineers)
	{
		// find all engineers with no predecesor
		for (auto iter = engineerList.begin(); iter != engineerList.end();)
		{
			EngineerIdx engineerIdx = *iter;
			if (predecessors[ engineerIdx ] == EngineerIdx::None)
			{
				orderedEngineers.push_back( engineerIdx );

				// remove the engineer from the original list so that he isn't visited and added twice
				iter = engineerList.erase( iter );
				// mark all his successors as having no predecesor now
				for (EngineerIdx successor : successors[ engineerIdx ])
				{
					predecessors[ successor ] = EngineerIdx::None;
				}
			}
			else
			{
				++iter;
			}
		}
	}

	return orderedEngineers;
}

struct UnlockingPath
{
	/// if this is .valid(), an engineer offering this modification couldn't be found
	Modification missingMod;

	/// engineers that need to be unlocked, in the correct unlocking order
	EngineerList orderedEngineers;

	/// which engineer was added for which modification
	/** Key is the engineer, value is the list of modifications for which he was choses by the algorithm. */
	EngineerModMultimap relatedModifications;

	bool valid() const { return !missingMod.valid() && !orderedEngineers.empty(); }
};

/// Finds the shortest path through engineer unlocking that gets you access to desired modifications.
UnlockingPath findShortestEngineerUnlockingPath( const vector< DesiredMod > & desiredModifications )
{
	UnlockingPath unlockingPath;

	// find engineers that offer each desired mod
	vector< DesiredModContext > desiredModContexts;
	desiredModContexts.reserve( desiredModifications.size() );
	for (DesiredMod desiredMod : desiredModifications)
	{
		desiredModContexts.emplace_back();
		desiredModContexts.back().mod = desiredMod;
		desiredModContexts.back().engineers = findEngineersOfferingModification( desiredMod );
		if (desiredModContexts.back().engineers.empty())
		{
			unlockingPath.missingMod = desiredMod;
			return unlockingPath;
		}
	}

	// generate every combination of the engineers, add all their requirements, and choose the best combination
	auto solution = findBestEngineerCombination( desiredModContexts );
	if (solution.requiredEngineers.empty())
	{
		return unlockingPath;
	}

	// order the engineers according to their unlocking requirements
	unlockingPath.orderedEngineers = orderTopologically( solution.requiredEngineers );

	unlockingPath.relatedModifications = solution.relatedModifications;

	return unlockingPath;
}


//----------------------------------------------------------------------------------------------------------------------

/// This returns all additional (originally not wanted) modifications that you will get access to
/// as a side-effect of unlocking the required engineers.
vector< Modification > getAdditionalModifications(
	const vector< DesiredMod > & desiredMods, const EngineerList & unlockedEngineers
)
{
	vector< Modification > additionalMods;

	unordered_map< ModuleType, grade_t > offeredMods;  // table: module -> grade

	// add all modifications of all required engineers
	for (EngineerIdx engineerIdx : unlockedEngineers)
	{
		for (const auto & mod : engineers[ engineerIdx ].modifications)
		{
			offeredMods.insert({ mod.module, mod.grade });
		}
	}

	// now remove all the modifications that the player already wanted
	for (const auto & desiredMod : desiredMods)
	{
		auto modIter = offeredMods.find( desiredMod.module );
		if (modIter != offeredMods.end())
		{
			if (modIter->second <= desiredMod.grade)
			{
				offeredMods.erase( modIter );
			}
		}
	}

	additionalMods.reserve( offeredMods.size() );
	for (auto [module, grade] : offeredMods)
	{
		additionalMods.push_back({ grade, module });
	}

	std::sort( additionalMods.begin(), additionalMods.end(),
		[]( const Modification & a, const Modification & b ) -> bool
		{
			if (a.module != b.module)
				return a.module < b.module;
			else
				return a.grade < b.grade;
		}
	);

	return additionalMods;
}


//======================================================================================================================
//  input/output

const char * moduleToString( ModuleType module )
{
	return ModuleTypeStr[ size_t(module) ];
}
ModuleType moduleFromString( const std::string & moduleStr )
{
	auto iter = find( ModuleTypeStr, moduleStr );
	if (iter != std::end(ModuleTypeStr))
		return ModuleType( iter - std::begin(ModuleTypeStr) );
	else
		return ModuleType::_EndOfEnum;
}

const char * engineerToString( EngineerIdx name )
{
	if (size_t(name) < size_t(EngineerIdx::_EndOfEnum))
		return EngineerNameStr[ size_t(name) ];
	else
		return "<invalid>";
}
EngineerIdx engineerFromString( const std::string & engineerStr )
{
	auto iter = find( EngineerNameStr, engineerStr );
	if (iter != std::end(EngineerNameStr))
		return EngineerIdx( iter - std::begin(EngineerNameStr) );
	else
		return EngineerIdx::_EndOfEnum;
}

vector< DesiredMod > readModifications( istream & in )
{
	vector< DesiredMod > modifications;

	string line;
	while (std::getline( in, line, '\n' ))
	{
		if (line.empty())
		{
			break;
		}

		istringstream iss( line );

		bool pinRequired;
		int c = iss.get();
		if (c == EOF)
		{
			cerr << "input ended unexpectedly" << endl;
			continue;
		}
		else if (c == '>')
		{
			pinRequired = true;
		}
		else if (isspace(c))
		{
			pinRequired = false;
		}
		else
		{
			cerr << "invalid modification format: " << line << " (must be: [>] <grade> <module name>)" << endl;
			continue;
		}

		grade_t modGrade;
		if (!(iss >> modGrade))
		{
			cerr << "invalid modification format: " << line << " (must be: [>] <grade> <module name>)" << endl;
			continue;
		}

		if (modGrade < 1 || modGrade > 5)
		{
			cerr << "invalid modification grade: " << modGrade << " (must be 1 - 5)" << endl;
			continue;
		}

		string modTypeStr;
		std::getline( iss >> std::ws, modTypeStr, '\n' );  // read the rest of the string-stream into a string

		ModuleType modType = moduleFromString( modTypeStr );
		if (modType == ModuleType::_EndOfEnum)
		{
			cerr << "such module does not exist: " << modTypeStr << endl;
			continue;
		}

		modifications.push_back({ modGrade, modType, pinRequired });
	}

	return modifications;
}

ostream & operator<<( ostream & os, const Modification & mod )
{
	os << mod.grade << "  " << moduleToString( mod.module );
	return os;
}

template< typename List >
void printList( const List & list )
{
	for (size_t i = 0; i < list.size(); ++i)
	{
		if (i == 0)
			cout << list[i];
		else
			cout << ", " << list[i];
	}
}

void printEngineerUnlockingPath( const UnlockingPath & unlockingPath, uint indentation = 0 )
{
	for (EngineerIdx engineerIdx : unlockingPath.orderedEngineers)
	{
		cout << indent( indentation ) << std::left << std::setw( 18 ) << engineerToString( engineerIdx );
		// print all of the related modifications

		if (!unlockingPath.relatedModifications[ engineerIdx ].empty())
		{
			cout << '(';
			printList( unlockingPath.relatedModifications[ engineerIdx ] );
			cout << ')';
		}
		cout << '\n';
	}
}

void printModifications( const vector< Modification > & modifications, uint indentation = 0 )
{
	for (const auto & mod : modifications)
	{
		cout << indent( indentation ) << mod << '\n';
	}
}

/// For each engineer in the unlocking path, prints which of his modifications you should pin.
void printDetailedUnlockingPath( const vector< DesiredMod > & desiredMods, const UnlockingPath & unlockingPath )
{
	for (EngineerIdx engineerIdx : unlockingPath.orderedEngineers)
	{
		cout << engineerToString( engineerIdx ) << ":\n";
		for (const auto & offeredMod : engineers[ size_t(engineerIdx) ].modifications)
		{
			const auto & relatedModsOfEngineer = unlockingPath.relatedModifications[ engineerIdx ];
			auto relatedModIter = findSuch( relatedModsOfEngineer,
				[ &offeredMod ]( const DesiredMod & desiredMod )
				{
					return offeredMod.module == desiredMod.module && offeredMod.grade >= desiredMod.grade;
				}
			);
			auto desiredModIter = findSuch( desiredMods,
				[ &offeredMod ]( const DesiredMod & desiredMod )
				{
					return offeredMod.module == desiredMod.module && offeredMod.grade >= desiredMod.grade;
					//    && !desiredMod.pinRequired;
				}
			);

			cout << std::left << std::setw(4);
			if (relatedModIter != relatedModsOfEngineer.end() && relatedModIter->pinRequired)
				cout << '>';
			else if (desiredModIter != desiredMods.end())
				cout << '+';
			else
				cout << ' ';
			cout << offeredMod << '\n';
		}
		cout << '\n';
	}
}


//======================================================================================================================
//  main

struct Args
{
	string fileName;
	bool detailedOutput = false;
	bool invalid = false;
};

Args parseArgs( int argc, char * argv [] )
{
	Args args;

	for (int i = 1; i < argc; ++i)
	{
		if (strcmp( argv[i], "--detailed" ) == 0)
		{
			args.detailedOutput = true;
		}
		else if (strncmp( argv[i], "--", 2 ) == 0)
		{
			cerr << "unknown option: " << argv[i] << endl;
			args.invalid = true;
		}
		else
		{
			if (args.fileName.empty())
			{
				args.fileName = argv[i];
			}
			else
			{
				cerr << "too many arguments" << endl;
				args.invalid = true;
			}
		}
	}

	return args;
}

void waitForEnter()
{
	while (cin.get() != '\n') {}
}

int main( int argc, char * argv [] )
{
	Args args = parseArgs( argc, argv );
	if (args.invalid)
	{
		cout << "usage: " << argv[0] << " [--detailed] <file_name>";
		return 1;
	}

	const bool interactive = args.fileName.empty();

	vector< DesiredMod > desiredMods;

	if (!args.fileName.empty())
	{
		ifstream inputFile;
		inputFile.open( args.fileName );
		if (!inputFile.is_open())
		{
			cerr << "Can't open file " << args.fileName << " (" << strerror(errno) << ")" << endl;
			return 2;
		}

		desiredMods = readModifications( inputFile );
	}
	else
	{
		cout << "Enter the list of modifications:" << endl;
		desiredMods = readModifications( cin );
	}

	if (desiredMods.empty())
	{
		cerr << "No modifications entered." << endl;
		if (interactive) waitForEnter();
		return 3;
	}

	cout << "calculating..."; cout.flush();

	auto unlockingPath = findShortestEngineerUnlockingPath( desiredMods );

	cout << "\n\n";

	if (unlockingPath.missingMod.valid())
	{
		cerr << "There is no engineer that offers modification: " << unlockingPath.missingMod << endl;
		if (interactive) waitForEnter();
		return 3;
	}
	else if (!unlockingPath.valid())
	{
		cerr << "The input requirements couldn't be satisfied,\n"
		     << "there is not enough engineers to cover all your desired modifications." << endl;
		if (interactive) waitForEnter();
		return 4;
	}

	if (args.detailedOutput)
	{
		cout << "Unlocking order (" << unlockingPath.orderedEngineers.size() << " engineers):\n\n";
		printDetailedUnlockingPath( desiredMods, unlockingPath );
		cout << endl;
	}
	else
	{
		cout << "Unlocking order (" << unlockingPath.orderedEngineers.size() << " engineers):\n";
		printEngineerUnlockingPath( unlockingPath, 1 );
		cout << endl;

		auto additionalMods = getAdditionalModifications( desiredMods, unlockingPath.orderedEngineers );
		cout << "Additionally you will get access to:\n";
		printModifications( additionalMods, 1 );
		cout << endl;
	}

	if (interactive) waitForEnter();
	return 0;
}
