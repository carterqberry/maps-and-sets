#include <iostream>
#include <sstream>
#include <fstream>

#include "Set.hpp"
#include "HashMap.hpp"

using namespace std;

enum Command
{
    SET,
    POKEMON_,
    MOVE,
    EFFECTIVE,
    INEFFECTIVE,
    POKEMON,
    MOVES,
    EFFECTIVITIES,
    INEFFECTIVITIES,
    BATTLE,
    ERROR
};

enum Effect
{
    E_INEFFECTIVE,
    E_EFFECTIVE,
    E_SUPER_EFFECTIVE
};

string effectToString(Effect effect) {
    return
        (effect == E_INEFFECTIVE) ? "ineffective" :
        (effect == E_EFFECTIVE) ? "effective" :
        (effect == E_SUPER_EFFECTIVE) ? "super effective" :
        "(invalid effect)";
}

int main(int argc, char *argv[])
{
	// open arg[1] or cin for input
    istream& in = (argc > 1) ? *(new ifstream(argv[1])) : cin;

    // open arg[2] or out for output
    ostream& out = (argc > 2) ? *(new ofstream(argv[2])) : std::cout;

    string line;

    HashMap<string> poke;
    HashMap<string> moves;
    HashMap<Set<string>> eff;
    HashMap<Set<string>> ineff;

    while(getline(in, line))
	{
		if(line == "") continue;

		istringstream in(line);

        string command;

		in >> command;

		Command c =
            (command == "Set:") ? Command::SET :
            (command == "Pokemon:") ? Command::POKEMON_ :
            (command == "Move:") ? Command::MOVE :
            (command == "Effective:") ? Command::EFFECTIVE :
            (command == "Ineffective:") ? Command::INEFFECTIVE :
            (command == "Pokemon") ? Command::POKEMON :
            (command == "Moves") ? Command::MOVES :
            (command == "Effectivities") ? Command::EFFECTIVITIES :
            (command == "Ineffectivities") ? Command::INEFFECTIVITIES :
            (command == "Battle:") ? Command::BATTLE :
            Command::ERROR;

        if(c == Command::POKEMON || c == Command::MOVES || c == Command::EFFECTIVITIES || c == Command::INEFFECTIVITIES)
            out << line << ": ";
        else
            out << line << " ";
        
        string addItem;
        string name;
        string type_;
        Set<string> types;

        Set<string> set;
        
        switch(c)
        {
            case Command::SET:
                while(in >> addItem)
                    set.insert(addItem);
                
                out << endl << "  [" << set << "]";
                break;
            case Command::POKEMON_:
                in >> name >> type_;
                poke[name] = type_;
                break;
            case Command::MOVE:
                in >> name >> type_;
                moves[name] = type_;
                break;
            case Command::EFFECTIVE:
                in >> name;
                types = Set<string>();
                while(in >> type_)
                    types.insert(type_);
                eff[name] = types;
                break;
            case Command::INEFFECTIVE:
                in >> name;
                types = Set<string>();
                while(in >> type_)
                    types.insert(type_);
                ineff[name] = types;
                break;
            case Command::POKEMON:
                out << poke.size() << "/" << poke.max_size() << endl;
                out << poke;
                break;
            case Command::MOVES:
                out << moves.size() << "/" << moves.max_size() << endl;
                out << moves;
                break;
            case Command::EFFECTIVITIES:
                out << eff.size() << "/" << eff.max_size() << endl;
                out << eff;
                break;
            case Command::INEFFECTIVITIES:
                out << ineff.size() << "/" << ineff.max_size() << endl;
                out << ineff;
                break;
            case Command::BATTLE:
            {
                string p1;
                string m1;
                string p2;
                string m2;

                string p1t;
                string m1t;
                string p2t;
                string m2t;

                in >> p1 >> m1 >> p2 >> m2;
                out << endl << "  " << p1 << " (" << m1 << ") vs " << p2 << " (" << m2 << ")";
                p1t = poke[p1];
                p2t = poke[p2];
                m1t = moves[m1];
                m2t = moves[m2];

                Effect m1effect =
                    (eff[m1t].find(p2t))   ? E_SUPER_EFFECTIVE :
                    (ineff[m1t].find(p2t)) ? E_INEFFECTIVE :
                                             E_EFFECTIVE;                         
                out << endl << "  " << p1 << "'s " << m1 << " is " << effectToString(m1effect) << " against " << p2;

                Effect m2effect =
                    (eff[m2t].find(p1t))   ? E_SUPER_EFFECTIVE :
                    (ineff[m2t].find(p1t)) ? E_INEFFECTIVE :
                                             E_EFFECTIVE;
                out << endl << "  " << p2 << "'s " << m2 << " is " << effectToString(m2effect) << " against " << p1;

                if(m1effect != m2effect)
                    out << endl <<  "  In the battle between " << p1 << " and " << p2 << ", ";
                
                if(m1effect > m2effect)
                    out << p1 << " wins!";
                else if(m2effect > m1effect)
                    out << p2 << " wins!";
                else
                    out << endl << "The battle between " << p1 << " and " << p2 << " is a tie.";

                break;
            }
            case Command::ERROR:
            default:
                out << "ERROR";
                break;
        }

		out << endl;
    }
    // Clean up input and output streams
    if(&in != &cin) delete &in;
    if(&out != &std::cout) delete &out;

	return 0;
}