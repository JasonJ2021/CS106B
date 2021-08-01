// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
#include <string>
#include "simpio.h"
using namespace std;


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
string cleanToken(string s)
{
    bool flag = false;
    while(ispunct(s[0])){
        s = s.substr(1);
    }
    while(ispunct(s[s.length() - 1])){
        s = s.substr(0,s.length() - 1);
    }

    for(int i  = 0 ; i < s.length() ; i++){
        if(isalpha(s[i])){
            flag = true;
            if(isupper(s[i])){
                s[i] = toLowerCase(s[i]);
            }
        }
    }
    if(flag == false){
        return "";
    }else{
        return s;

    }
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    Vector<string> temp = stringSplit(text, " ");
    for(string s : temp){
        string ct = cleanToken(s);
        if(ct != ""){
            tokens.add(ct);
        }
    }
    return tokens;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    ifstream in;

    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);

    Vector<string> lines;
    readEntireFile(in, lines);
    int count = 0;
    for(int i = 0 ; i < lines.size();i+=2){
        for(string s : gatherTokens(lines[i+1])){
            if(index.containsKey(s)){
                index[s].add(lines[i]);
            }else{
                Set<string> temp;
                temp.add(lines[i]);
                index.put(s,temp);
            }

        }
        count++;
    }
    return count;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    Vector<string> v = stringSplit(query," ");
    result = index.get(cleanToken(v[0]));
    for(int i = 1 ; i < v.size();i++){
        if(v[i][0] == '+'){
            result *= index.get(cleanToken(v[i]));
        }else if(v[i][0] == '-'){
            result -= index.get(cleanToken(v[i]));
        }else{
            result += index.get(cleanToken(v[i]));
        }
    }
    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior

void searchEngine(string dbfile)
{
    // TODO: your code here
    Map<string, Set<string>> index;
    cout<<"Stand by while building index..."<<endl;
    int nPages = buildIndex(dbfile, index);
    int nTerms = index.size() ;
    cout<<"Indexed "<<nPages<<" pages containing "<<nTerms<<" unique terms"<<endl;
    string input = getLine("Enter query sentence (RETURN/ENTER to quit): ");
    while(input.size()){
        Set<string> set = findQueryMatches(index,input);
        int size = set.size();
        cout<<"Found "<<size<<" matching pages"<<endl;
        cout<<set<<endl;
        input = getLine("Enter query sentence (RETURN/ENTER to quit): ");
    }
    cout<<"All done!"<<endl;

}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on tokens with no punctuation") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
}

PROVIDED_TEST("cleanToken on tokens with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word tokens"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> tokens = gatherTokens("to be or not to be");
    EXPECT_EQUAL(tokens.size(), 4);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> seuss = gatherTokens("One Fish Two Fish Red fish Blue fish!!! 123");
    EXPECT_EQUAL(seuss.size(), 5);
    EXPECT(seuss.contains("fish"));
    EXPECT(!seuss.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 20 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 20);
    EXPECT(index.containsKey("fish"));
    EXPECT(!index.containsKey("@"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 3);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
STUDENT_TEST("cleanToken my case"){
    EXPECT_EQUAL(cleanToken("~APPLE"), "apple");
    EXPECT_EQUAL(cleanToken("~AP!le@"), "ap!le");
    EXPECT_EQUAL(cleanToken(""), "");
}
STUDENT_TEST("buildIndex my case ") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.get("red").size(), 2);
    EXPECT(index.containsKey("fish"));
    EXPECT(!index.containsKey("@"));
}
