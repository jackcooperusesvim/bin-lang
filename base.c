#include "base.h"
#include "stack.h"

Cable Group(Stack* stack, Cable* cables) {
}
Cable AllocateWireArr(Stack* stack, int wire_count) {
	Cable wa = {};
	wa.count = wire_count;
	wa.values = StackPush(stack, sizeof(bool)*wa.count);
	return wa;
}

Cable Or(Stack* stack, Cable in) {
	bool outbool = false;
	for (int i=0; i<in.count; i++) {
		outbool = outbool || in.values;
	}
	Cable out = AllocateWireArr(stack, 1);
	*out.values = outbool;
	return out;
}
Cable And(Stack* stack, Cable in) {
	bool outbool = false;
	for (int i=0; i<in.count; i++) {
		outbool = outbool && in.values;
	}
	Cable out = AllocateWireArr(stack, 1);
	*out.values = outbool;
	return out;
}

Cable Not(Stack* stack, Cable in) {
	bool outbool = false;
	for (int i=0; i<in.count; i++) {
		in.values[i] = !in.values[i];
	}
	return in;
}
