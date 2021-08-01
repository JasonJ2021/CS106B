/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function takes a string s and returns a version of
 * the string that has all non-letter characters removed
 * from it.
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s) {
    for (int i = 0; i < s.length(); i++) {
        if (!isalpha(s[i])) {
            // remove the character at index i
            s = s.substr(0,i) + s.substr(i+1);
            i--;
        }
    }
    return s;
}
char switchToCode(char c){
    switch(c){
        case'A':
        case'a':
        case'E':
        case'e':
        case'I':
        case'i':
        case'O':
        case'o':
        case'U':
        case'u':
        case'H':
        case'h':
        case'W':
        case'w':
        case'Y':
        case'y':
        {
            return '0';
        }
        case'B':
        case'b':
        case'F':
        case'f':
        case'P':
        case'p':
        case'V':
        case'v':
    {
        return '1';
    }
    case'C':
    case'c':
    case'G':
    case'g':
    case'J':
    case'j':
    case'K':
    case'k':
    case'Q':
    case'q':
    case'S':
    case's':
    case'X':
    case'x':
    case'Z':
    case'z':
    {
        return '2';
    }
    case'D':
    case'd':
    case'T':
    case't':
    {
        return '3';
    }
    case'L':
    case'l':
    {
        return '4';
    }
    case'M':
    case'm':
    case'N':
    case'n':
    {
        return '5';
    }
    case'R':
    case'r':
    {
        return '6';
    }
    }
    return 0;
}

string encode(string s){
    for(int i = 0 ; i < s.length();i++){
        s[i] = switchToCode(s[i]);
    }
    return s;
}

string coalesce(string s){
    for(int i = 0 ; i < s.length()-1;i++){
        if(s[i] == s[i+1]){
            s = s.substr(0,i) + s.substr(i+1);\
            i--;
        }
    }
    return s;
}
string RemoveZero(string s){
    for(int i = 0 ; i < s.length();i++){
        if(s[i] == '0'){
            s = s.substr(0,i) + s.substr(i+1);\
            i--;
        }
    }
    return s;
}

string soundex(string s) {
    string Letters = removeNonLetters(s);
    string encoded = encode(Letters);
    string coal = coalesce(encoded);
    char first = toupper(Letters[0]);
    coal[0] = first;
    string nonZero = RemoveZero(coal);
    string result;
    if(nonZero.length() >4){
        result = nonZero.substr(0,4);
    }else{

        result = nonZero + string(4-nonZero.length(),'0');
    }
    return result;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames

    /* TODO: Fill in the remainder of this function. */
    string surname = getLine("Enter a surname (RETURN to quit):");
    while(surname.size()){
        string soundexCode = soundex(surname);
        Vector<string> result;
        for(int i = 0 ; i < databaseNames.size();i++){
            string temp = soundex(databaseNames[i]);
            if(!temp.compare(soundexCode)){
                result.add(databaseNames[i]);
            }
        }
        result.sort();
        cout<<result<<endl;
        surname = getLine("Enter a surname (RETURN to quit):");
    }
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

STUDENT_TEST("RemoveNonLetters , Wrong cases!") {
    string s = "OHar@@@a /123 @ @@";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
}
STUDENT_TEST("Encode Test") {
    string s = "OConner";
    string result = encode(s);
    EXPECT_EQUAL(result, "0205506");
}
STUDENT_TEST("coal Test") {
    string s = "10000100";
    string result = coalesce(s);
    EXPECT_EQUAL(result, "1010");
}


