#include "fsm.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

void read(char& cache, istream& stream) {
    if (stream) {
        stream.get(cache);
    }
    if (!stream) {
        cache = 0;
    }
}

pair<int, Lexem> tick(int state, istream& stream, char& cache) {
    
    switch (state) {
    case 0:
        switch (cache) {
        case '>':
            read(cache, stream);
            return { 0, {"opgt", ""} };
        case '!':
            read(cache, stream);
            if (cache == '=') {
                return { 1, {"opne", ""} };
            }
            else {
                return { 0, {"opnot", ""} };
            }
        case '=':
            read(cache, stream);
            if (cache == '=') {
                return { 1, {"opeq", ""} };
            }
            else {
                return { 0, {"opassign", ""} };
            }
        case '+':
            read(cache, stream);
            if (cache == '+') {
                return { 1, {"opinc", ""} };
            }
            else {
                return { 0, {"opplus", ""} };
            }
        case '(':
            read(cache, stream);
            return { 0, {"lpar", ""} };
        case ')':
            read(cache, stream);
            return { 0, {"rpar", ""} };
        case '{':
            read(cache, stream);
            return { 0, {"lbrace", ""} };
        case '}':
            read(cache, stream);
            return { 0, {"rbrace", ""} };
        case ';':
            read(cache, stream);
            return { 0, {"semicolon", ""} };
        case '.':
            read(cache, stream);
            return { 0, {"colon", ""} };
        case ',':
            read(cache, stream);
            return { 0, {"comma", ""} };
        case '*':
            read(cache, stream);
            return { 0, {"opmul", ""} };
        case '<':
            read(cache, stream);
            if (cache == '=') {
                return { 1, {"ople", ""} };
            }
            else {
                return { 0, {"oplt", ""} };
            }
        case '|':
            read(cache, stream);
            if (cache == '|') {
                return { 1, {"opor", ""} };
            }
            else {
                return { -1, LEX_ERROR };
            }
        case '&':
            read(cache, stream);
            if (cache == '&') {
                return { 1, {"opand", ""} };
            }
            else {
                return { -1, LEX_ERROR };
            }
        case '"':
            read(cache, stream);
            return { 2, LEX_EMPTY };
        case '\'':
            read(cache, stream);
            if (cache == '$') {
                return { -1, LEX_ERROR };
            }
            else {
                string value;
                value += cache;
                read(cache, stream);
                if (cache == '\'') {
                    read(cache, stream);
                    return { 0, { "char(" + value + ")", "" } };
                }
                else {
                    return { -1, LEX_ERROR };
                }
            }
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
        case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
        case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
        {
            string value;
            while (isalpha(cache) || isdigit(cache)) {
                value += cache;
                read(cache, stream);
            }
            if (value == "if" || value == "else" || value == "while" || value == "for" || value == "switch" || value == "int" || value == "char" || value == "double" || value == "string" || value == "float") {
                return { 0, { value, "" } };
            }
            else {
                return { 0, { "id(" + value + ")", "" } };
            }
        }
 
        case '$':
            return { -1, LEX_EOF };
        case ' ':
        case '\t':
        case '\n':
            read(cache, stream);
            return { 0, LEX_EMPTY };
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
        {
            string value;
            while (isdigit(cache)) {
                value += cache;
                read(cache, stream);
            }
            return { 0, { "num(" + value + ")", "" } };
        }
        case '-':
            read(cache, stream);
            if (isdigit(cache)) {
                string value = "-";
                while (isdigit(cache)) {
                    value += cache;
                    read(cache, stream);
                }
                return { 0, { "num(" + value + ")", "" } };
            }
            else {
                return { 0, {"opminus", ""} };
            }
        default:
            return { -1, LEX_ERROR };
        }

    case 1:
        read(cache, stream);
        return { 0, LEX_EMPTY };
    case 2:
    {
        string value;
        while (cache != '"') {
            value += cache;
            read(cache, stream);
            if (cache == '$') {
                return { -1, LEX_ERROR };
            }
        }
        read(cache, stream);
        return { 0, { "str(" + value + ")", "" } };
    }

    default:
        return { -1, LEX_EMPTY };
    }
}
