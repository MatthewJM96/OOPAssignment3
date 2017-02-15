#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cctype>

/// Collection of helper functions to act on strings.
namespace String {
	// Trims a string from the left.
	void ltrim(std::string& s) {
		// Erase from the beginning of the string to wherever the first non-whitespace character is.
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char a) {
			return !std::isspace((int)a);
		}));
	}
	// Trims a string from the right.
	void rtrim(std::string& s) {
		// Erase from the end of the string to wherever the last non-whitespace character is.
		s.erase(std::find_if(s.rbegin(), s.rend(), [](char a) {
			return !std::isspace((int)a);
		}).base(), s.end()); // base returns the string iterator of the found reverse iterator.
	}
	// Trims a string from both ends.
	void trim(std::string& s) {
		ltrim(s);
		rtrim(s);
	}

	int getFirstDigit(std::string s) {
		auto& it = std::find_if(s.begin(), s.end(), [](char a) {
			return std::isdigit((int)a);
		});
		if (it == s.end()) return -1;

		return ((int)*it - 48);
	}

	int getFirstNumber(std::string s) {
		auto& it = std::find_if(s.begin(), s.end(), [](char a) {
			return std::isdigit((int)a);
		});
		if (it == s.end()) return -1;
		
		std::string numberString(&*it);

		do {
			++it;
			if (std::isdigit((int)*it)) {
				numberString.append(&*it);
			} else {
				break;
			}
		} while (true);

		std::stringstream sstream(numberString);
		int number;
		sstream >> number;

		return number;
	}

	std::string getTitle(std::string s) {
		std::stringstream sstream(s);

		std::string temp;
		sstream >> temp;
		sstream >> temp;

		std::getline(sstream, temp);
		String::trim(temp);

		return temp;
	}
}

/// Collection of helper functions to handle user input.
namespace Input {
	// Compares two strings in a case-insensitive manner.
	bool icompare(const std::string& a, const std::string& b) {
		// Simple shortcut, two strings must be the same length to be the same.
		if (a.length() != b.length()) return false;

		// Not-so-efficient algorithm that equates each pair of characters with the same index in the string objects' char arrays one at a time.
		return std::equal(a.begin(), a.end(), b.begin(), [](char a, char b) {
			// Case insensitive so simply force characters to their lower-case form.
			return std::tolower(a) == std::tolower(b);
		});
	}

	// Gets a user choice between two sets of valid responses. Returns 1 where the user response was in the first set and -1 where the response was in the second set.
	int getBetweenTwoStringSetOptions(std::string message, const std::vector<std::string>& firstStringSet, const std::vector<std::string>& secondStringSet) {
		int response;
		std::string line;
		// Grab a line from the cin buffer.
		while (std::getline(std::cin, line)) {
			// Iterate over valid true strings and compare them with the grabbed line in a case-insensitive manner.
			const auto& itTrue = std::find_if(firstStringSet.begin(), firstStringSet.end(), [&line](const std::string& a) {
				return icompare(a, line);
			});
			// Iterate over valid false strings and compare them with the grabbed line in a case-insensitive manner.
			const auto& itFalse = std::find_if(secondStringSet.begin(), secondStringSet.end(), [&line](const std::string& a) {
				return icompare(a, line);
			});

			// If either iterator is not pointing at the end of their respective string arrays, then the grabbed line must have matched one of the valid strings in one of those arrays.
			if (itTrue != firstStringSet.end()) {
				response = 1;
				break;
			} else if (itFalse != secondStringSet.end()) {
				response = -1;
				break;
			}

			std::cout << "Sorry, the value you inputted was not valid." << std::endl;
			std::cout << message;
		}
		// Can only get here via the if block, but returning inside the if block makes VS throw a warning, I don't like warnings.
		return response;
	}

	// Gets an integer from the user within the defined range.
	int getInt(int min = INT_MIN, int max = INT_MAX) {
		// By making this type the next size up from standard integers, can avoid edge cases where values in
		// possibleInteger seem reasonable even though no value was necessarily obtained from the stringstream.
		long long possibleInteger;
		std::string line;
		// Grab a line from the cin buffer.
		while (std::getline(std::cin, line)) {
			possibleInteger = std::numeric_limits<long long>::min();

			// Move string into a stringstream.
			std::stringstream input(line);

			// Grab as much integer as possible from the front of the stringstream's buffer.        
			input >> possibleInteger;
			// Apply range bounds checks to resulting maybe-integer.
			if (possibleInteger >= (long long)min && possibleInteger <= (long long)max) {
				// Make sure the stringstream's buffer is clear; "123abc" does not an integer make.
				std::string emptyTest;
				input >> emptyTest;
				if (emptyTest.empty()) {
					// All's good, break out of while loop and return the integer.
					break;
				}
			}

			std::cout << "Sorry, the value you inputted was not valid." << std::endl;
			std::cout << "Input an integer between " << min << " and " << max << ":\n";
		}
		// Can only get here via the if block, but returning inside the if block makes VS throw a warning, I don't like warnings.
		return possibleInteger;
	}

	enum class SourceType {
		DIRECT,
		FILE
	};

	// Gets a bool from the user.
	SourceType getSourceType() {
		static const std::vector<std::string> validDirects = {
			"direct",
			"d"
		};
		static const std::vector<std::string> validFiles = {
			"file",
			"f"
		};

		return (getBetweenTwoStringSetOptions("Direct, or file? [d/f]:\n", validDirects, validFiles) == 1) ? SourceType::DIRECT : SourceType::FILE;
	}

	enum class SortOrder {
		NONE,
		COURSE_TITLE,
		COURSE_NUMBER
	};

	// Gets a sorting order from the user.
	SortOrder getSortOrder() {
		static const std::vector<std::string> validTitles = {
			"title",
			"t"
		};
		static const std::vector<std::string> validNumbers = {
			"number",
			"n"
		};

		return (getBetweenTwoStringSetOptions("Sort by course title or number? [t/n]:\n", validTitles, validNumbers) == 1) ? SortOrder::COURSE_TITLE : SortOrder::COURSE_NUMBER;
	}

	// Gets a bool from the user.
	bool getBool() {
		static const std::vector<std::string> validTrues = {
			"yes",
			"y",
			"true",
			"1"
		};
		static const std::vector<std::string> validFalses = {
			"no",
			"n",
			"false",
			"0"
		};

		return getBetweenTwoStringSetOptions("Yay, or nay? [y/n]:\n", validTrues, validFalses) == 1;
	}
}

using CourseDatum = std::string;
using CourseData = std::vector<CourseDatum>;
/// Model that holds and acquires the course data.
class CourseDataModel {
public:
	CourseDataModel() {}
	~CourseDataModel() {
		dispose();
	}

	void dispose() {
		// Clear up memory.
		CourseData().swap(m_courses);
	}

	void appendDatum(CourseDatum datum) {
		// Construct a stream of the datum to be tested and added.
		std::stringstream sstream(datum);

		// Output course code to an int.
		int possibleCourseCode = -1;
		sstream >> possibleCourseCode;

		// Output course name to a string.
		std::string possibleCourseName;
		sstream >> possibleCourseName;

		// If either the possible charge has an invalid value, or the empty test string is not empty, fail.
		if (possibleCourseCode < 0 || possibleCourseName.empty()) {
			std::cout << "The course data you provided was invalid and was not added." << std::endl;
			return;
		}

		// Construct course string.
		sstream = std::stringstream();
		sstream << "PHYS " << possibleCourseCode << " " << possibleCourseName;

		// All's well, push the read charge onto the charge array.
		m_courses.push_back(sstream.str());
	}
	void appendDataFromFile(std::string filepath) {
		loadDataFromFile(filepath);
	}

	CourseData getCourseData() {
		return m_courses;
	}
private:
	bool openFile(std::fstream& file, std::string filepath) {
		file.open(filepath, std::ios::in);
		return file.is_open();
	}

	void loadDataFromFile(std::string filepath) {
		std::fstream file;

		// If file isn't already open, and failed to open on an attempt, exit the program.
		if (!file.is_open() && !openFile(file, filepath)) {
			std::cout << "Could not open file: " << filepath << "." << std::endl
				<< "Press any key to exit..." << std::endl;
			std::getchar();
			exit(0);
		}

		// Iterate over lines in file and validate them.
		std::string line;
		while (std::getline(file, line)) {
			// Trim whitespace.
			String::trim(line);

			// Put it in a stringstream.
			std::stringstream sstream(line);

			// Output course code to an int.
			int possibleCourseCode = -1;
			sstream >> possibleCourseCode;

			// Output course name to a string.
			std::string possibleCourseName;
			std::getline(sstream, possibleCourseName);
			String::trim(possibleCourseName);

			// If either the possible charge has an invalid value, or the empty test string is not empty, fail.
			if (possibleCourseCode < 0 || possibleCourseName.empty()) {
				std::cout << "File: " << filepath << " has a corrupt data point." << std::endl
					<< "Skipping that data point." << std::endl;
				continue;
			}

			// Construct course string.
			sstream = std::stringstream();
			sstream << "PHYS " << possibleCourseCode << " " << possibleCourseName;

			// All's well, push the read charge onto the charge array.
			m_courses.push_back(sstream.str());
		}
	}
	
	CourseData m_courses;
};

int main() {
	std::cout << "Welcome to the course database!" << std::endl;

	CourseDataModel courseModel;

	bool shouldContinue;
	do {
		std::cout << "Where would you like to get data from (directly or from a file)? [d/f]:" << std::endl;
		Input::SourceType sourceType = Input::getSourceType();

		std::string line;
		if (sourceType == Input::SourceType::DIRECT) {
			std::cout << "Please input the course data you wish to add:" << std::endl;
			std::getline(std::cin, line);
			courseModel.appendDatum(line);
		} else {
			std::cout << "Please input the filepath for the file you wish to load data from:" << std::endl;
			std::getline(std::cin, line);
			courseModel.appendDataFromFile(line);
		}

		std::cout << "Would you like to input any other data? [y/n]" << std::endl;
		shouldContinue = Input::getBool();
	} while (shouldContinue);

	std::cout << std::endl << std::endl;

	CourseData courseData = courseModel.getCourseData();

	// This is gonna be hell, but gotta stick to the spec.
	int year = -1;
	std::cout << "Would you like only courses of a particular year to be displayed? [y/n]:" << std::endl;
	if (Input::getBool()) {
		std::cout << "What year would you like the courses of to be displayed? [1-4]" << std::endl;
		year = Input::getInt(1, 4);
	}
		
	Input::SortOrder sortOrder = Input::SortOrder::NONE;
	std::cout << "Would you like to have the courses displayed sorted? [y/n]:" << std::endl;
	if (Input::getBool()) {
		std::cout << "Would you like to sort the courses by title or by number? [t/n]:" << std::endl;
		sortOrder = Input::getSortOrder();
	}

	CourseData reducedCourses;
	for (auto& it = courseData.begin(); it != courseData.end(); ++it) {
		if (year != -1) {
			int firstCourseNumDigit = String::getFirstDigit(*it);
			if (firstCourseNumDigit != year) continue;
		}

		reducedCourses.push_back(*it);
	}

	if (sortOrder == Input::SortOrder::COURSE_TITLE) {
		std::sort(reducedCourses.begin(), reducedCourses.end(), [](const CourseDatum& a, const CourseDatum& b) {
			CourseDatum aTitle = String::getTitle(a);
			CourseDatum bTitle = String::getTitle(b);

			if (aTitle.length() > bTitle.length()) {
				return std::equal(bTitle.begin(), bTitle.end(), aTitle.begin(), [](char b, char a) {
					return std::toupper((int)a) < std::toupper((int)b);
				});
			} else {
				return std::equal(aTitle.begin(), aTitle.end(), bTitle.begin(), [](char a, char b) {
					return std::toupper((int)a) < std::toupper((int)b);
				});
			}
		});
	} else if (sortOrder == Input::SortOrder::COURSE_NUMBER) {
		std::sort(reducedCourses.begin(), reducedCourses.end(), [](const CourseDatum& a, const CourseDatum& b) {
			return String::getFirstNumber(a) < String::getFirstNumber(b);
		});
	}

	for (auto& it = reducedCourses.begin(); it != reducedCourses.end(); ++it) {
		std::cout << *it << std::endl;
	}

	std::cout << "Press any key to exit..." << std::endl;
	getchar();
	return 0;
}
