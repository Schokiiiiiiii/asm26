void foo() {
    __asm__("instruction");
}

void bar() {
    __asm__("instruction 1"
            "instruction 2");
}

void baz() {
    __asm__("instruction 1\n"
            "instruction 2");
}

void fun() {
    __asm__("instruction 1\n\t"
            "instruction 2");
}