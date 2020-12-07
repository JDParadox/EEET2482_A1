#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <math.h>

//#include <vector> // TEMPORARY INCLUSION FOR TESTING. ISN'T ACTUALLY USED IN PROGRAM

using namespace std;

double mean(double arr[], int size) { 
	// Calculates the mean of an array
	
	int i;
	double sum = 0, res;

	for (i = 0; i < size; ++i) {
		sum += arr[i];
	}
	res = sum / size;

	return res;
}

double median(double arr[], int size) {
	//Calculates the median of an array
	
	sort(arr, arr + size); // If we're not allowed to sort using the inbuilt algorithm, we'll have to code a sorting algorithm ourselves
	
	if (size % 2 == 0)
		return (arr[size / 2 - 1] + arr[size / 2]) / 2;
	
	return arr[size / 2];
}

double mode(double arr[], int n) {
	//Calculate the mode of an array

	// Sort the array 
	sort(arr, arr + n);

	//finding max frequency  
	int max_count = 1, res = arr[0], count = 1;
	for (int i = 1; i < n; i++) {
		if (arr[i] == arr[i - 1])
			count++;
		else {
			if (count > max_count) {
				max_count = count;
				res = arr[i - 1];
			}
			count = 1;
		}
	}

	// when the last element is most frequent 
	if (count > max_count)
	{
		max_count = count;
		res = arr[n - 1];
	}

	return res;
}

double variance(double arr[], int n) {
	// Calculate the variance of an array

	double res, sum = 0;

	double tempMean = mean(arr, n); // We could reuse the results below, but that would mean these functions wouldn't be standalone

	for (int i = 0; i < n; ++i) {
		sum += pow(arr[i] - tempMean, 2);
	}

	// Here be dragons. I don't actually understand why I need the -1 for the correct results
	res = sum / (n - 1);

	return res;
}

double stdev(double arr[], int n) {
	// Calculate the standard deviation of an array

	double res = 0;

	double var = variance(arr, n); // We could reuse the results below, but that would mean these functions wouldn't be standalone

	res = sqrt(var);

	return res;
}

double mad(double arr[], int n) {
	// Calculate the mean absolute deviation of an array

	double res, sum = 0;
	double tempMean = mean(arr, n);

	for (int i = 0; i < n; i++)
		sum = sum + abs(arr[i] - tempMean);
	res = sum / n;

	return res;
}

bool isValid(string data) {
	// This function checks the input data to see if it is a valid number e.g. int, float, double, blah blah. Does not check for scientific notations;
	// Also helps in skipping the first line of the csv file

	for (unsigned int i = 0; i < data.length(); i++) {
		// This part of the code checks if it contains blank space(s), automatically returns false;
		if (data[i] == ' ') {
			return false;
		}

		// This part of the code checks if it contains a letter. Returns false if it does.
		if ((!(data[i] >= '0' && data[i] <= '9') && data[i] != '-' && data[i] != '+')) { // If char isn't 0 to 9 AND char is not a '-' or '+' return false
			return false;
		}
	}
	
	// If the code gets to here without returning false, it's valid
	return true;
}

int main(int argc, char* argv[]) {

	if (argc < 2) { cout << "Did you forget to input your filename?"; return 1; }

	string fileName = argv[1]; // Reads filename from cmdline

	ifstream fileStr(fileName, ios::in); // Opens file. Checks if file is opened. Errors otherwise.
	if (!fileStr) {
		cerr << "Error opening file " << fileName << ". Program now exiting";
		return 1;
	}

	int numberOfLines = 0; // Counting the number of lines in the file so we know how much memory-ish to allocate since we can't use vectors. This does mean we have to iterate through the file twice though

	string line;
	while (getline(fileStr, line)) {
		numberOfLines++;
	}
	fileStr.clear(); // Returns to start of stream again so we can actually read it properly again
	fileStr.seekg(0, ios::beg);
	//cout << numberOfLines << endl; // Debug statement

	double* xArrPtr = new double[numberOfLines + 1]; // Creating the x array with the number of lines
	double* yArrPtr = new double[numberOfLines + 1]; // Creating the y array with the number of lines
	// This won't account for invalid lines, so it's always slightly inefficient


	// Could maybe be its own function here.
	string dataLine;
	
	int errorCount = 0;
	int actualArraySize = 0;
	while (getline(fileStr, dataLine)) {
		stringstream dataLineStr(dataLine);

		string x; // temp values to contain x and y
		string y;

		bool lineOk = true;
		getline(dataLineStr, x, ','); // Get X
		if (!isValid(x)) lineOk = false; // Checks if X is okay, else flag the line

		getline(dataLineStr, y, ','); //Get Y
		if (!isValid (y)) lineOk = false; // Checks if Y is okay, else flag the line

			
		if (lineOk) { // If the line is good, put x and y into the array and increase the arraySize counter
			xArrPtr[actualArraySize] = stod(x);
			yArrPtr[actualArraySize] = stod(y);
			actualArraySize++;
		} else {
			errorCount++; // Else increase the error/invalid counter
		}
		
	}

	// Using var actualArraySize as the array length since dynamic array's lengths aren't stored. Is this going to cause issues elsewhere?
	
	// Prints all the numbers
	/*for (int i = 0; i < actualArraySize; i++) {
		cout << xArrPtr[i] << ",";
		cout << yArrPtr[i] << endl;
	}*/

	//cout << "There was " << errorCount << " line(s) with an error"; // DEBUG. If it hasn't thrown an error yet (it passed the stof test), the data should be clean.

	// Do your calculations below here. You've got the two arrays. Should be clean. Use actualArraySize for array size

	fileStr.close(); // Close the file since we've already got the arrays

	cout << "Printing results for file " << fileName << endl;

	// Mean calc and display
	double meanX = mean(xArrPtr, actualArraySize);
	double meanY = mean(yArrPtr, actualArraySize);
	cout << setprecision(10) << "mean_X= " << meanX << " - mean_Y = " << meanY << endl; // Tested against google sheets (imported the csv file, told it to calc the average for a column. Works)

	// Median calc and display
	double medianX = median(xArrPtr, actualArraySize);
	double medianY = median(yArrPtr, actualArraySize);
	cout << setprecision(10) << "median_X= " << medianX << " - median_Y = " << medianY << endl;

	// Mode calc and display
	double modeX = mode(xArrPtr, actualArraySize);
	double modeY = mode(yArrPtr, actualArraySize);
	cout << setprecision(10) << "mode_X= " << modeX << " - mode_Y = " << modeY << endl;

	// Variance calc and display
	double varianceX = variance(xArrPtr, actualArraySize);
	double varianceY = variance(yArrPtr, actualArraySize);
	cout << setprecision(10) << "var_X= " << varianceX << " - var_Y = " << varianceY << endl;

	// Standard deviation calc and display
	double stdevX = stdev(xArrPtr, actualArraySize);
	double stdevY = stdev(yArrPtr, actualArraySize);
	cout << setprecision(10) << "stdev_X= " << stdevX << " - stdev_Y = " << stdevY << endl;

	// Mad deviation calc and display
	double madX = mad(xArrPtr, actualArraySize);
	double madY = mad(yArrPtr, actualArraySize);
	cout << setprecision(10) << "mad_X= " << madX << " - mad_Y = " << madY << endl;

	// ADD YOUR NAMES HERE
	cout << endl;
	cout << "ASSIGNMENT 1GROUP<TT>" << endl;
	cout << "sWWWWWWW,sWWWWWWW@rmit.edu.vn,FirstName,LastName" << endl;
	cout << "sXXXXXXX,sXXXXXXX@rmit.edu.vn,FirstName,LastName" << endl;
	cout << "sYYYYYYY,sYYYYYYY@rmit.edu.vn,FirstName,LastName" << endl;
	cout << "s3694603,3694603@rmit.edu.vn,Khoi,Dang " << endl;

	// Cleanup operations
	delete[] xArrPtr;
	xArrPtr = NULL;
	delete[] yArrPtr;
	yArrPtr = NULL;
	
	return 0;
}