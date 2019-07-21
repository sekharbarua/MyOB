#include <iostream>
#include <sstream>
#include <fstream>
#include <functional>
#include "windows.h"
#include "inc\ConsolodiateMarket.h"

void MyTerminate()
{
	std::cout << "No data Found " << endl;
	exit(1);
}


auto countWord = [&](const std::string& s)
{
	std::regex word_regex("(\\S+)");
	auto words_begin = std::sregex_iterator(s.begin(), s.end(), word_regex);
	auto words_end = std::sregex_iterator();

	return std::distance(words_begin, words_end);
};

int main(int argc, char* argv[]) {
	set_terminate(MyTerminate);

	orderbook::Order::Quantity target_size;
	std::string feed_file_path;
	ifstream file_stream;
	bool load_from_file = false;
	
	std::function<void()> setTargetSize = [&]() {
		target_size = std::stoul(argv[1]);
		if (target_size < 1) {
			std::cerr << "TARGET-SIZE should be a positive number";
			abort();
		}
	};

	std::function<void()> setFeedFile = [&]() {
		load_from_file = true;
		feed_file_path = argv[2];
		file_stream.open(feed_file_path);
		if (!file_stream.is_open()) {
			std::cerr << "Market feed file cannot be loaded.";
			abort();
		}
	};

	switch (argc) {
	case 2:
		setTargetSize();
		break;
	case 3:
		setTargetSize();
		setFeedFile();
		break;
	default:
		abort();
	}

	orderbook::BookConsolidator mBookCon;
	mBookCon.setTargetSize(target_size);
	std::string line;

	//if (load_from_file) {
	//	while (getline(file_stream, line))
	//	{
	//		if (countWord(line) > 5)
	//			mBookCon.processOrder(line);
	//		else
	//			mBookCon.processExFeed(line);
	//	}
	//}
	//else {
	//	while (getline(std::cin, line)) {
	//		if (countWord(line) > 5)
	//			mBookCon.processOrder(line);
	//		else
	//			mBookCon.processExFeed(line);;
	//	}
	//}


	int choice;
	try
	{
		do
		{
			system("cls");
			std::cout << "Welcome to Market Book Consoladation\n";
			std::cout << "***************Menu*****************\n";
			std::cout << "* 1. Display Top Level 5 of Book    *" << endl;
			std::cout << "* 2. Process Order                  *" << endl;
			std::cout << "* 3. EXIT                           *" << endl;
			std::cout << "*************************************\n";
			cin >> choice;

			switch (choice)
			{
			case 1:
				mBookCon.printTop5();

				system("PAUSE");
				break;
			case 2:
				if (load_from_file) {
					while (getline(file_stream, line))
					{
						if (!line.empty())
						{
							if (countWord(line) == 5)
								mBookCon.processExFeed(line);
							else
								mBookCon.processOrder(line);
						}
					}
				}
				else {
					while (getline(std::cin, line)) {
						if (!line.empty())
						{
							if (countWord(line) == 5)
								mBookCon.processExFeed(line);
							else
								mBookCon.processOrder(line);
						}
					}
				}
				break;

			case 3:
				std::cout << "Goodbye\n";
				break;
			default: std::cout << "Wrong Choice. Enter again\n";
				break;
			}

		} while (choice != 3);
	}
	catch (std::exception& myexception)
	{
		std::cout << "Exiting" << endl;
	}

	return 0;
}


