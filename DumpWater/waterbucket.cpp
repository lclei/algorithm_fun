#include <assert.h>
#include <deque>
#include <iostream>
#include <algorithm>

#define BUCKET_COUNT 3
#define debug_out cout
using namespace std;

//  Pour water action struct
typedef struct ACTION{
	int from;
	int to;
	int water;
}ACTION_t;

// An array indicate water and the action
class BucketState{
public:
	int buckets[BUCKET_COUNT];
	ACTION_t curAction;
	bool CanTakeAction(int, int);
	bool operator==(const BucketState&);
	bool IsFinalState();
	void SetBucket(int *bucket_v);
	void SetAction(int from, int to, int water);
	bool DumpWater(int from, int to, BucketState& next);
	bool IsBucketEmpty(int n);
	bool IsBucketFull(int n);
private:
	static int bucket_capicity[BUCKET_COUNT];
};
int BucketState::bucket_capicity[BUCKET_COUNT]  = {8,5,3};

// function declaration

bool IsSameBucket(const BucketState &lhs, const BucketState &rhs);
void PrintResult(const deque<BucketState> &states);
void SearchState(deque<BucketState>& states);
void SearchStateOnAction(deque<BucketState> &states, BucketState& current,int from, int to);

bool BucketState::IsBucketEmpty(int n)
{
	if(buckets[n]==0) return true;
	else return false;
}

bool BucketState::IsBucketFull(int n)
{
	if(buckets[n]>=bucket_capicity[n]) return true;
	else return false;
}


// Judge  if the pour water action is valid
bool BucketState::CanTakeAction(int from, int to)
{
	assert( (from>=0) && (from<BUCKET_COUNT));
	assert( (to>=0) && (to<BUCKET_COUNT));
	
	if( (from!=to) && !IsBucketEmpty(from) && !IsBucketFull(to) )
	{
		return true;
	}
	
	return false;
}

bool BucketState::IsFinalState()
{
	if( (buckets[0]!=4) || (buckets[1]!=4) ) return false;
	return true;
}

void BucketState::SetBucket(int *bucket_v)
{
	for(int i=0;i<BUCKET_COUNT;i++)
	{
		buckets[i] = bucket_v[i];
	}
}

void BucketState::SetAction(int from, int to, int water)
{
	curAction.from = from;
	curAction.to = to;
	curAction.water = water;
}

bool BucketState::DumpWater(int from, int to, BucketState& next)
{
	int dump_water;
	next.SetBucket(buckets);
	dump_water = bucket_capicity[to] - next.buckets[to];
	//debug_out << "bucket" << to << " can have " << dump_water <<endl;
	if(next.buckets[from] >= dump_water)
	{
		next.buckets[from] -= dump_water;
		next.buckets[to] += dump_water;
	}
	else
	{
		next.buckets[to] += buckets[from];
		dump_water = buckets[from];
		next.buckets[from] = 0;
	}
	
	if(dump_water > 0)
	{
		next.SetAction(from,to,dump_water);
		return true;
	}
	else return false;
}

bool BucketState::operator==(const BucketState& rhs)
{
	int i;
	for(i=0;i<BUCKET_COUNT;i++)
	{
		if(buckets[i] != rhs.buckets[i]) return false;
	}
	return true;
}



// Judge if the state is searched already
bool IsProcessedState(deque<BucketState> &states, const BucketState& newState)
{
	auto it = states.end();
	//debug_out << "called" << endl;
	it = find_if(states.begin(), states.end(), 
				[&newState](BucketState &s){return IsSameBucket(s,newState);});
	//debug_out << "processed?: " << (it!=states.end()) << endl;
	return (it!=states.end());
}

bool IsSameBucket(const BucketState &lhs, const BucketState &rhs)
{
	int i;
	//debug_out << "called" << endl;
	for(i=0;i<BUCKET_COUNT;i++)
	{
		if(lhs.buckets[i] != rhs.buckets[i]) return false;
	}
	return true;
}
		
void SearchState(deque<BucketState>& states)
{
	//debug_out << "search" <<endl;
	BucketState current = states.back();
	if(current.IsFinalState())
	{
		PrintResult(states);
		//debug_out << "Get!" << endl;
		return;
	}
	
	for(int j=0; j<BUCKET_COUNT;++j)
	{
		for(int i=0; i<BUCKET_COUNT;++i)
		{
			//debug_out << i << " to " << j << endl;
			SearchStateOnAction(states, current, i, j);
		}
	}
}

void SearchStateOnAction(deque<BucketState> &states, BucketState& current,int from, int to)
{
	if(current.CanTakeAction(from,to))
	{
		BucketState next;
		bool bDump = current.DumpWater(from, to, next);
		//debug_out << "can dump? " << bDump << endl;
		if(bDump && !IsProcessedState(states,next))
		{
			//debug_out << "[" << next.buckets[0] << next.buckets[1] << next.buckets[2] << "] " <<endl;
			states.push_back(next);
			SearchState(states);
			states.pop_back();
		}
	}
}

void PrintResult(const deque<BucketState> &states)
{
	for(auto state : states)
	{
		cout << "(" << state.curAction.from << state.curAction.to << state.curAction.water << ") ";
		cout << "[" << state.buckets[0] << state.buckets[1] << state.buckets[2] << "] ";
	}
	cout << endl;
}

int main(int argc, char *argv[])
{
	BucketState first;
	deque<BucketState> states;
	int first_buckets[BUCKET_COUNT] = {8,0,0};
	first.SetBucket(first_buckets);
	first.SetAction(-1,0,8);
	states.push_back(first);
	SearchState(states);
	return 0;
}