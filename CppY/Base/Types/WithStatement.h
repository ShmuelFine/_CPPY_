#pragma once
//
//#define _AS_ ,
//#define WITH(EXPRESSION, TARGET)                \
//exit = type(manager).__exit__                   \
//bool hit_except = false                              \
//                                                \
//try {                                           \
//    TARGET = EXPRESSION;                        \
//    MEM_FUN(TARGET, __enter__));                \
//    
//#define END_WITH\
//} catch(...) {                              \
//    hit_except = true;                               \
//    if (not exit(manager, *sys.exc_info()))         \
//        throw                                       \
//    finally :                                   \
//    if not hit_except :                         \
//        exit(manager, None, None, None)         \