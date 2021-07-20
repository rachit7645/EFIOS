#include "KBScanCodeTranslation.h"

namespace QWERTYKeyboard {

    // Table of characters indexed by scancodes
	const char ASCIITable[] = {
         0 ,  0 , '1', '2',
        '3', '4', '5', '6',
        '7', '8', '9', '0',
        '-', '=',  0 ,  0 ,
        'q', 'w', 'e', 'r',
        't', 'y', 'u', 'i',
        'o', 'p', '[', ']',
         0 ,  0 , 'a', 's',
        'd', 'f', 'g', 'h',
        'j', 'k', 'l', ';',
        '\'','`',  0 , '\\',
        'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',',
        '.', '/',  0 , '*',
         0 , ' '
    };

	char Translate(uint8_t scancode, bool upperCase) {
		if (scancode > 58) return 0;

        if (upperCase) return ASCIITable[scancode] - 32;    // Uppercase letters are at an offset of 32 less than lowercase ones 
        else return ASCIITable[scancode];

	}

}