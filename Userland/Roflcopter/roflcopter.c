#include <stdio.h>
#include <stdint.h>

extern int int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);

int main(int argc, char *argv[]) {
	int rightSide = 0;
	uint64_t counter;

	while(1){
		int80(2,0,0,0);
		if(rightSide){
			
			printf("\n\n\n\n\n\n\n\n");
			printf("                                      LOL:ROFL:ROFL\n"    );
			printf("                             _________||_________\n"      );
			printf("                     L      /                 [ O\\\n"    );
			printf("                     O =====                  |_|_\\\n"   );
			printf("                     L     B|O M B                 )\n"   );
			printf("                          F |______,-------¬_____/\n"     );
			printf("                         T //    }-OMGROCKET))\n"         );
			printf("                        W // _______||_||_________/_/\n"  );
			rightSide = 0;
		}
		else{

			printf("\n\n\n\n\n\n\n\n");
			printf("                            ROFL:ROFL:LOL\n"              );
			printf("                             _________||_________\n"      );
			printf("                            /                 [ O\\\n"    );
			printf("                    LOL=====                  |_|_\\\n"   );
			printf("                           B|O M B                 )\n"   );
			printf("                          F |______,-------¬_____/\n"     );
			printf("                         T //    }-OMGROCKET))\n"         );
			printf("                        W // _______||_||_________/_/\n"  );
			rightSide = 1;

		}
		counter = 20000000;
		while(counter)
			counter--;
	}

}
