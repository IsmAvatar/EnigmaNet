/*
Copyright (C) 2007 39ster
Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>

This file is part of EnigmaNet
EnigmaNet is free software and comes with ABSOLUTELY NO WARRANTY.
See LICENSE for details.
*/

#pragma once
#include "includes.h"
class CList
{
	void**items;
	int buffsize;
public:
	int count;
	CList(int buffsize);
	CList();
	~CList();
	int Add(void*item);
	void Remove(void*item);
	void Remove(int index);
	void Clear();
	void* operator[](int index);
	void* item(int index);
	void* find(void*item);
	void set(int pos, void*item);
};
