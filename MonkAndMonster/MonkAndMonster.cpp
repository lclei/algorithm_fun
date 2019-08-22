#include <iostream>
#include <deque>
#include <algorithm>
#include <assert.h>

#define debug_out cout

using namespace std;

typedef enum{
	LOCAL,
	REMOTE,
}BOAT_LOCATION;

typedef enum{
	ONE_MONSTER_GO = 0,
	TWO_MONSTER_GO,
	ONE_MONK_GO,
	TWO_MONK_GO,
	ONR_MONSTER_ONE_MONK_GO,
	ONE_MONSTER_BACK,
	TWO_MONSTER_BACK,
	ONE_MONK_BACK,
	TWO_MONK_BACK,
	ONR_MONSTER_ONE_MONK_BACK,
	INVALID_ACTION_NAME,
}ACTION_NAME;	
	
typedef struct{
	ACTION_NAME act;
	BOAT_LOCATION boat_to;
	int move_monster;
	int move_monk;
}ACTION_EFFECTION;

class ItemState{
friend	ostream& operator<<(ostream&, const ItemState&);
public:
	bool operator==(const ItemState&);
	bool isFinalState();
	bool takeAction(ItemState& next, const ACTION_EFFECTION& action);
	bool isValid();
	int local_monster;
	int local_monk;
	int remote_monster;
	int remote_monk;
	BOAT_LOCATION boat;
};

static ACTION_EFFECTION actEffect[] =
{
	{ONE_MONSTER_GO,  			REMOTE, -1, 0},
	{TWO_MONSTER_GO, 			REMOTE,	-2,	0},
	{ONE_MONK_GO,				REMOTE,	0,	-1},
	{TWO_MONK_GO,				REMOTE,	0,	-2},
	{ONR_MONSTER_ONE_MONK_GO,	REMOTE,	-1,	-1},
	{ONE_MONSTER_BACK,			LOCAL,	1,	0},
	{TWO_MONSTER_BACK,			LOCAL,	2,	0},
	{ONE_MONK_BACK,				LOCAL,	0,	1},
	{TWO_MONK_BACK,				LOCAL,	0,	2},
	{ONR_MONSTER_ONE_MONK_BACK,	LOCAL,	1,	1}
};

void searchState(deque<ItemState> &states);
void searchStateOnOneAction(deque<ItemState> &states, ItemState &current, ACTION_EFFECTION& actEff);
void printResult(const deque<ItemState> &states);
bool isProcessed(deque<ItemState> &states, ItemState& state);

bool ItemState::operator==(const ItemState& state)
{
	if( (local_monk != state.local_monk) || (local_monster != state.local_monster) ||
		(remote_monk != state.remote_monk) || (remote_monster != state.remote_monster) ||
		(boat != state.boat))
		return false;
	else return true;
}

ostream& operator<<(ostream &out, const ItemState& s)
{
	out << s.local_monk << "o  ||  " << s.remote_monk << "o" <<endl;
	out << s.local_monster << "x  ||  " << s.remote_monster << "x" <<endl;
	if(s.boat) out << "    ||boat" << endl;
	else out << "boat||    " << endl;
	out << endl;
	return out;
}
	
bool ItemState::takeAction(ItemState& next, const ACTION_EFFECTION& action)
{
	//debug_out<<boat<<action.boat_to << endl;
	if(boat != action.boat_to)
	{
		next.boat = action.boat_to;
		next.local_monk = local_monk + action.move_monk;
		next.local_monster = local_monster + action.move_monster;
		next.remote_monk = remote_monk - action.move_monk;
		next.remote_monster = remote_monster - action.move_monster;
		if(next.isValid()) return true;
	}
	return false;
}

bool ItemState::isValid()
{
	if( (local_monk<0) || (local_monster<0) || (remote_monk<0) || (remote_monster<0) ||
			((local_monk>0) && (local_monster>local_monk)) || ((remote_monk>0) && (remote_monster>remote_monk)))
	{
		return false;
	}
	else return true;
}

bool ItemState::isFinalState()
{
	return ((local_monk == 0) && (local_monster == 0));
}

bool isProcessed(deque<ItemState> &states, ItemState& state)
{
	auto it = states.end();
	it = find_if(states.begin(), states.end(), [&](ItemState& s){return s==state;});
	return(it!=states.end());
}

void printResult(const deque<ItemState> &states){
	cout << "o stands for monk, and x stands for monster." << endl;
	for(auto state : states)
	{
		cout << state;
	}
}

void searchState(deque<ItemState> &states)
{
	int act;
	ItemState current = states.back();
	if(current.isFinalState())
	{
		printResult(states);
		return;
	}
	for(act = 0; act < INVALID_ACTION_NAME; act++)
	{
		//debug_out << act <<endl;
		searchStateOnOneAction(states, current, actEffect[act]);
	}
}

void searchStateOnOneAction(deque<ItemState> &states, ItemState &current, ACTION_EFFECTION& actEff)
{
	ItemState next;
	bool canTackAction;
	canTackAction = current.takeAction(next, actEff);
	//debug_out << next;
	if(canTackAction && !isProcessed(states, next))
	{
		//debug_out << "valid" << endl;
		// printResult(states);
		states.push_back(next);
		searchState(states);
		states.pop_back();
	}
}

int main(int argc, char *argv[])
{
	ItemState initState;
	deque<ItemState> states;
	initState.local_monk = 3;
	initState.local_monster = 3;
	initState.remote_monk = 0;
	initState.remote_monster = 0;
	initState.boat = LOCAL;
	states.push_back(initState);
	searchState(states);
	return 0;
}
	
	