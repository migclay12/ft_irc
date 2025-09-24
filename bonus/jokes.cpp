#include "jokes.hpp"

namespace {
	std::vector<std::string>	_jokes;
	bool						initialized = false;
}

namespace Jokes {
	void    initialize()
	{
		if (initialized == true)
			return;
		std::srand(std::time(0));

		/* programming */
		_jokes.push_back("Debugging: Removing the needles from the haystack.");
		_jokes.push_back("I'd tell you a joke about NAT but I would have to translate.");
		_jokes.push_back("A programmer puts two glasses on his bedside table before going to sleep.\nA full one, in case he gets thirsty, and an empty one, in case he doesn't.");
		_jokes.push_back("Knock knock.\nWho's there?\nRecursion.\nRecursion who?\nKnock knock.");
		_jokes.push_back("The glass is neither half-full nor half-empty, the glass is twice as big as it needs to be.");
		_jokes.push_back("I have a joke about Stack Overflow, but you would say it's a duplicate.");
		_jokes.push_back("Judge: \"I sentence you to the maximum punishment...\"\nMe (thinking): \"Please be death, please be death...\"\nJudge: \"Learn Java!\"\nMe: \"Damn.\"");
		_jokes.push_back("Saying that Java is nice because it works on every OS is like saying that anal sex is nice because it works on every gender.");
		_jokes.push_back("Your mama's so FAT she can't save files bigger than 4GB.");
		_jokes.push_back("Debugging is like being the detective in a crime movie where you're also the murderer at the same time.");
		_jokes.push_back("Documentation is like sex:\nWhen it's good, it's very good.\nWhen it's bad, it's better than nothing...");
		_jokes.push_back("A SQL statement walks into a bar and sees two tables.\nIt approaches, and asks \"may I join you?\"");
		_jokes.push_back("A guy walks into a bar and asks for 1.4 root beers.\nThe bartender says \"I'll have to charge you extra, that's a root beer float\".\nThe guy says \"In that case, better make it a double.\"");
		_jokes.push_back("Your momma is so fat, you need to switch to NTFS to store a picture of her.");
		_jokes.push_back("\"Honey, go to the store and buy some eggs.\"\n\"OK.\"\n\"Oh and while you're there, get some milk.\"\nHe never returned.");
		_jokes.push_back("Java and C were telling jokes. It was C's turn, so he writes something on the wall, points to it and says \"Do you get the reference?\" But Java didn't.");
		_jokes.push_back("I'd tell you a joke about NAT but I would have to translate.");
		_jokes.push_back("If Bill Gates had a dime for every time Windows crashed ... Oh wait, he does.");
		_jokes.push_back("UDP is better in the COVID era since it avoids unnecessary handshakes.");
		_jokes.push_back("\"We messed up the keming again guys.\"");
		_jokes.push_back("// This line doesn't actually do anything, but the code stops working when I delete it.");
	
		/* random */
		_jokes.push_back("How do you make a hankie dance? Put a little boogie in it.");
		_jokes.push_back("I can't take my dog to the pond anymore because the ducks keep attacking him. That's what I get for buying a pure bread dog.");
		_jokes.push_back("I am terrified of elevators. I'm going to start taking steps to avoid them.");
		_jokes.push_back("I got a reversible jacket for Christmas, I can't wait to see how it turns out.");
		_jokes.push_back("To the guy who invented zero... thanks for nothing.");
		_jokes.push_back("Americans can't switch from pounds to kilograms overnight. That would cause mass confusion.");
		_jokes.push_back("What is worse then finding a worm in your Apple? Finding half a worm in your Apple.");
		_jokes.push_back("How does a penguin build it's house? Igloos it together.");
		_jokes.push_back("Why are giraffes so slow to apologize? Because it takes them a long time to swallow their pride.");
		_jokes.push_back("I used to have a job at a calendar factory but I got the sack because I took a couple of days off.");
		_jokes.push_back("If you walk into a forest and cut down a tree, but the tree doesn't understand why you cut it down, do you think it's stumped?");
		_jokes.push_back("Have you heard about the film \"Constipation\", you probably haven't because it's not out yet.");
		_jokes.push_back("Conjunctivitis.com - now that's a site for sore eyes.");
		_jokes.push_back("A weasel walks into a bar. The bartender says, \"Wow, I've never served a weasel before. What can I get for you?\"\n\"Pop,\" goes the weasel.");

		initialized = true;
	}

	std::vector<std::string>    splitStringByNewline(const std::string& input)
	{
		std::vector<std::string>	result;
		std::istringstream			iss(input);
		std::string					line;
		
		while (std::getline(iss, line))
		{
			result.push_back(line);
		}

		return (result);
	}

	std::vector<std::string>  get_joke_vect()
	{
		std::vector<std::string>	joke_v;

		if (initialized == false || _jokes.empty())
			throw std::runtime_error("Jokes not initialized");

		std::string joke = _jokes[std::rand() % _jokes.size()];
		joke_v = splitStringByNewline(joke);
		joke_v.push_back("---------------------------------------------------");
		return (joke_v);
	}
}
