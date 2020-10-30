void main() {
	char *stuff[] = {
		"a",
		"b",
		"c"
	};
	int sz = 3;
	for ( int c=0 ; c<sz; c++ ) {
		printf("item %d: %s\n", c, stuff[c]);
	}
}
