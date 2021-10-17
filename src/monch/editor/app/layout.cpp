//
// Created by Christopher Boyle on 17/10/2021.
//
#include <iostream>
#include "EditorApp.h"

WindowPoint EditorApp::get_size_of(const Object *object)
{
    int n = count_children(), i=1;
    for (auto *child : *this) {
        if (child == object)
            break;
        i++;
    }
    if (i > n) throw std::runtime_error("cant layout object that is not child?"); // maybe just revert to default?

    WindowPoint sz = get_size();
    sz.x = (sz.x*i)/n;
    return sz;
}

WindowPoint EditorApp::get_position_of(const Object *object)
{
    int n = count_children(), i=0;
    for (auto *child : *this) {
        if (child == object)
            break;
        i++;
    }
    if (i == n) throw std::runtime_error("cant layout object that is not child?"); // maybe just revert to default?

    int w = get_width();
    WindowPoint pos{(w*i)/n, 0};
    return pos;
}