/*-----------------------------------------------
* Author: William Watkins
* This program decrypts messages encrypted using
* vigenere ciphers with a key of length 3 to 6 letters
* CS 378 hmwk-4
* Sources: 
* https://www.youtube.com/watch?v=LaWp_Kq0cKs
* http://user.it.uu.se/~elenaf/Teaching/Krypto2003/vigenere.html
* ------------------------------------------------
*/

#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
using namespace std;

// maximum key length
//const int MAXKEYLENGTH = 6;

// filename
const string filename = "vig2.txt";

int find_key_length(string ciphertext) {
	int two = 0, three = 0, four = 0, five = 0, six = 0;
	int distance;
	int len = ciphertext.length();

	//find factors of distances between repeated pairs
	for (int i = 0; i < len; i++) {
		for (int j = i + 1; j < len - 1; j++) {
			if ((string() + ciphertext[i] + ciphertext[i + 1]) == 
				(string() + ciphertext[j] + ciphertext[j + 1])) {

				distance = j - i;
				if (distance % 6 == 0) {
					six++;
				}
				if (distance % 5 == 0) {
					five++;
				}
				if (distance % 4 == 0) {
					four++;
				}
				if (distance % 3 == 0) {
					three++;
				}
			}
		}
	}

	//return the most frequent factors
	int highest_freq = six;
	int highest_int = 6;
	if (five > highest_freq) {
		highest_freq = five;
		highest_int = 5;
	}
	if (four > highest_freq) {
		highest_freq = four;
		highest_int = 4;
	}
	if (three > highest_freq) {
		highest_freq = three;
		highest_int = 3;
	}

	cout << "The key has length " << highest_int << "\n";
	return highest_int;
}

char find_key(string s) {

	//max sum i=0 to 25, qi * pi+s mod 26 where s = {0, 1, ..., 25}

	//english alphabet frequency table
	float p[26] = { .082, .015, .028, .043, .127, .022, .020,
	.061, .070, .002, .008, .040, .024, .067, .075, .019, .001,
	.060, .063, .091, .028, .010, .023, .001, .020, .001 };
	string alphabet = "abcdefghijklmnopqrstuvwxyz";

	//ciphertext frequency table
	float q[26];
	int str_length = s.length();

	//initialize to 0
	for (int i = 0; i < 26; i++){
		q[i] = 0;
	}

	//find frequency
	for (int i = 0; i < str_length; i++){
		for (int j = 0; j < 26; j++){
			if (alphabet[j] == s[i]){
				q[j]++;
			}
		}
	}

	//set probability of occurrence
	for (int i = 0; i < 26; i++){
		q[i] = q[i] / str_length;
	}

	int index;
	float sum, max = 0;
	char key;
	
	//find max sum (formula commented above)
	for (int i = 0; i < 26; i++) {
		sum = 0;
		for (int j = 0; j < 26; j++) {
			sum += p[j] * q[(j + i) % 26];
			if (sum > max) {
				max = sum;
				index = i;
			}
		}
	}

	//return key
	return alphabet[index];
}

void decrypt(string ciphertext, int length) {

	string one = "", two = "", three = "", four = "", five = "", six = "";
	int count = 0;
	char key[6];

	//split cipher text into substrings with each nth letter
	for (int i=0; i<ciphertext.length(); i++){
		if (i % length == 0){
			one.push_back(ciphertext[i]);
		}
	}

	for (int i = 0; i < ciphertext.length(); i++) {
		if (i % length == 1) {
			two.push_back(ciphertext[i]);
		}
	}

	for (int i = 0; i < ciphertext.length(); i++) {
		if (i % length == 2) {
				three.push_back(ciphertext[i]);
		}
	}
	
	if (length >= 4) {
		for (int i = 0; i < ciphertext.length(); i++) {
			if (i % length == 3) {
				four.push_back(ciphertext[i]);
			}
		}
		
	}

	if (length >= 5) {
		for (int i = 0; i < ciphertext.length(); i++) {
			if (i % length == 4) {
				five.push_back(ciphertext[i]);
			}
		}
	}

	if (length >= 6) {
		for (int i = 0; i < ciphertext.length(); i++) {
			if (i % length == 5) {
				six.push_back(ciphertext[i]);
			}
		}
		
	}

	//find key
	key[0] = find_key(one);
	key[1] = find_key(two);
	key[2] = find_key(three);
	if (length >= 4) {
		key[3] = find_key(four);
	}
	if (length >= 5) {
		key[4] = find_key(five);
	}
	if (length >= 6) {
		key[5] = find_key(six);
	}
	
	//display key
	cout << "Key is: ";
	for (int i = 0; i < 6; i++) {
		cout << key[i];
	}
	cout << "\n";

	//display plaintext
	string alphabet = "abcdefghijklmnopqrstuvwxyz";
	for (int i = 0; i < ciphertext.length(); i++) {
		cout << alphabet[(ciphertext[i] - key[i % length] + 26) % 26];
	}
	cout << "\n";
	
}


int main() {
	//string of ciphertext
	string ciphertext;
	
	//open file
	ifstream f;
	
	f.open(filename);
	if (!f) {
		cout << "Unable to open file\n";
		exit(1); // exit with error
	}
	while (f >> ciphertext) {}

	f.close();

	//get frequency
	int key_length = find_key_length(ciphertext);

	
	//divide ciphertext into substrings and find key
	decrypt(ciphertext, key_length);

	return 0;
}
