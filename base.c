#include "base.h"
#include "stack.h"

WireArr AllocateWireArr(Stack* stack, int wire_count) {
	WireArr wa = {};
	wa.count = wire_count;
	wa.values = StackPush(stack, sizeof(bool)*wa.count);
	return wa;
}

WireArr Or(Stack* stack, WireArr in) {
	bool outbool = false;
	for (int i=0; i<in.count; i++) {
		outbool = outbool || in.values;
	}
	WireArr out = AllocateWireArr(stack, 1);
	*out.values = outbool;
	return out;
}
WireArr And(Stack* stack, WireArr in) {
	bool outbool = false;
	for (int i=0; i<in.count; i++) {
		outbool = outbool && in.values;
	}
	WireArr out = AllocateWireArr(stack, 1);
	*out.values = outbool;
	return out;
}

WireArr Not(Stack* stack, WireArr in) {
	bool outbool = false;
	for (int i=0; i<in.count; i++) {
		in.values[i] = !in.values[i];
	}
	return in;
}
