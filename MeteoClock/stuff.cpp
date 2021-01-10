#include "stuff.h"

String S(char* s, char* s2 = NULL, char* s3 = NULL, char* s4 = NULL, char* s5 = NULL, char* s6 = NULL, char* s7 = NULL, char* s8 = NULL, char* s9 = NULL, char* s0 = NULL) {
    String res = String(s);
    if (s2 != NULL) {
        res += String(s2);
    } else {
        return res;
    }
    if (s3 != NULL) {
        res += String(s3);
    } else {
        return res;
    }
    if (s4 != NULL) {
        res += String(s4);
    } else {
        return res;
    }
    if (s5 != NULL) {
        res += String(s5);
    } else {
        return res;
    }
    if (s6 != NULL) {
        res += String(s6);
    } else {
        return res;
    }
    if (s7 != NULL) {
        res += String(s7);
    } else {
        return res;
    }
    if (s8 != NULL) {
        res += String(s8);
    } else {
        return res;
    }
    if (s9 != NULL) {
        res += String(s9);
    } else {
        return res;
    }
    if (s0 != NULL) {
        res += String(s0);
    } else {
        return res;
    }
    return res;
}
