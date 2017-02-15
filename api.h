#pragma once

struct sequence{
	int (*insert)();
	int (*delete)();
	void* (*find)();
	void* who; 
} typedef sequence;


sequence init_hash(size_t);
